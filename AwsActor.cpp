/**
 * @file AwsActor.cpp
 * @date 2025. 12. 15
 * @author root
 */

#include "AwsActor.h"
#include "CMimetic.h"

AwsActor::AwsActor()
{
}

AwsActor::~AwsActor()
{
}

bool AwsActor::subAction(int actionTag)
{
	switch (actionTag)
	{
	case 1:
	case 3:
	{
		if (getMailContent().StringGetter("chk").compare("?uploads") == 0 || getMailContent().StringGetter("chk").compare("?encryption=") == 0) {
			LOG(INFO) << "[NOLOG] AWS Cli S3 Cotent Not Use";
			return false;
		}
		if (!getMailContent().StringGetter("tmpattachfile").empty()) {
			if (getMailContent().getMappingKey().empty() || !getMailContent().StringGetter("attach_begin_seq").empty()) {
				string tmpattachfile = getMailContent().StringGetter("tmpattachfile");
				vector<string> vecDecoded;
				if (getMailContent().StringGetter("content_encoding").compare("aws-chunked") == 0) {
					vecDecoded.push_back(decodeAwsChunked(tmpattachfile));
				}
				else {
					vecDecoded.push_back(tmpattachfile);
				}
				makeAttachWithoutDecodeByVector(vecDecoded, getMailContent().VectorGetter("filename"));
				if (!getMailContent().StringGetter("attach_begin_seq").empty()) {
					g_MetaData.setSiteState(eSITE_SEND_ATTACH);
					string attach_begin_seq = getMailContent().StringGetter("attach_begin_seq");
					getMailContent().changeAttachInfo(0, D_TREE_PATH_ATT_BSEQ, attach_begin_seq);
				}
			}
		}

		if (getMailContent().getAttachs().empty() && getMailContent().getMappingKey().empty()) {
			LOG(INFO) << "[NOLOG] AWS Cli S3 Cotent NULL";
			return false;
		}


	}
	break;

	case 2:
	{
		string tmpbody = getMailContent().StringGetter("tmpbody");
		if (!tmpbody.empty()) {
			ByteArray buffer;
			buffer.assign((const byte*)tmpbody.data(), tmpbody.size());

			CMimetic mimeParser;
			mimeParser.setDefaultCharset("UTF-8", "UTF-8");

			if (!mimeParser.begin(buffer)) {
				LOG(INFO) << "Mime parsing fail";
				return false;
			}
			string from = mimeParser.get_header_from();
			string subject = mimeParser.get_header_subject();
			vector<string> vecTo = mimeParser.get_header_to();
			vector<string> vecCc = mimeParser.get_header_cc();
			vector<string> vecBcc = mimeParser.get_header_bcc();

			getMailContent().setFrom(from);
			getMailContent().setSubject(subject);
			for (unsigned int i = 0; i < vecTo.size(); i++) {
				getMailContent().addTo(vecTo.at(i));
			}
			for (unsigned int j = 0; j < vecCc.size(); j++) {
				getMailContent().addCc(vecCc.at(j));
			}
			for (unsigned int k = 0; k < vecBcc.size(); k++) {
				getMailContent().addBcc(vecBcc.at(k));
			}

			string bodyText;
			string bodyHtml;
			vector<string> vecAttachfile;
			vector<string> vecFilename;
			while (mimeParser.setNextPart()) {
				string contentType = mimeParser.get_common_contenttype();
				//LOG(INFO) << "hhbaik" << mimeParser.get_part_content(true) << "\n\n";

				if (contentType.find("text/plain") != string::npos) {
					// 텍스트 본문
					mimeParser.setInCharset(mimeParser.get_common_charset());
					bodyText += (mimeParser.get_part_content(true) + "\n\n");  // charset 변환 포함

				}
				else if (contentType.find("text/html") != string::npos) {
					// HTML 본문
					mimeParser.setInCharset(mimeParser.get_common_charset());
					bodyHtml = mimeParser.get_part_content(true);

				}
				else if (contentType.find("application/") != string::npos || contentType.find("image/") != string::npos) {
					// 첨부파일
					string attachName = mimeParser.get_part_attachname();
					byte* attachData = mimeParser.get_part_attachdata(true);  // Base64 디코딩
					size_t attachLen = mimeParser.getAttachLen();
					if (attachLen > 0 && attachData != NULL) {
						vecAttachfile.push_back(string((const char*)attachData, attachLen));
						if (!attachName.empty()) {
							vecFilename.push_back(attachName);
						}
						else {
							vecFilename.push_back("");
						}
					}
					mimeParser.freeAttach();  // 메모리 해제
				}
			}
			getMailContent().setBody(bodyHtml+"\n\n\n"+bodyText);
			makeAttachWithoutDecodeByVector(vecAttachfile, vecFilename);
		}
		else {
			LOG(INFO) << "[NOLOG] AWS SES Content Null";
			return false;
		}

	}
	break;

	default:
		break;
	}

	return true;
}

string AwsActor::decodeAwsChunked(const string& input)
{
	std::string result;
	size_t pos = 0;

	while (pos < input.length())
	{
		// 청크 크기 라인 찾기 (\r\n 또는 \n)
		size_t lineEnd = input.find('\n', pos);
		if (lineEnd == string::npos)
			break;

		// 청크 크기 파싱 (hex)
		string sizeLine = input.substr(pos, lineEnd - pos);
		// \r 제거
		if (!sizeLine.empty() && sizeLine[sizeLine.length() - 1] == '\r')
			sizeLine.erase(sizeLine.length() - 1);

		// 세미콜론 이후 시그니처 부분 제거 (aws-chunked 확장)
		size_t semicolon = sizeLine.find(';');
		if (semicolon != string::npos)
			sizeLine = sizeLine.substr(0, semicolon);

		long chunkSize = strtol(sizeLine.c_str(), NULL, 16);

		if (chunkSize <= 0)
			break;

		// 청크 데이터 추출
		size_t dataStart = lineEnd + 1;
		if (dataStart + chunkSize <= input.length())
		{
			result.append(input, dataStart, chunkSize);
		}

		// 다음 청크로 이동 (데이터 + \r\n)
		pos = dataStart + chunkSize;
		if (pos < input.length() && input[pos] == '\r')
			pos++;
		if (pos < input.length() && input[pos] == '\n')
			pos++;
	}

	return result;
}

void AwsActor::makeAttachWithoutDecodeByVector(vector<string>& vecContent, const vector<string>& vecFilename) {
	vector<string> vecAttach;
	CAttachInfo att;
	string Reqbody;
	string content;

	try
	{
		for (int i = 0; i < vecContent.size(); i++)
		{
			if (!vecContent.at(i).empty())
			{
				content = vecContent.at(i);
				att.appfpath = g_EmassFileName.getAttachPath(i);
				if (g_HttpSave.saveAttach(content, att.appfpath, att) == true)
				{
					FileStream* fs = CFileStreamFactory::create();
					size_f fsize = fs->fileSize(att.appfpath);
					stringstream strstream;
					strstream << fsize;
					att.fsize = strstream.str();
					try {
						string filename = vecFilename.at(i);
						if (vecFilename.size() > i && !vecFilename.at(i).empty()) {
							att.pcfname = filename;
						}
						else {
							att.extension = g_FileDetector.DetectFile(att.appfpath);
							att.pcfname = "noname" + att.extension;
						}
					}
					catch (const std::out_of_range& e) {
						VLOG(DINFO) << "vecFilename index out of range (" << i << "). Assigning 'noname'.";
						att.extension = g_FileDetector.DetectFile(att.appfpath);
						att.pcfname = "noname" + att.extension;
					}
					getMailContent().addAttach(att);
					VLOG(DINFO) << "save attach successfully";
					content.clear();
				}
			}
		}
	}
	catch (exception& e)
	{
		VLOG(DINFO) << "save attach Fail";
	}
}
