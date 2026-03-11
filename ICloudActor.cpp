/*
 * ICloudActor.cpp
 *
 *  Created on: 2020. 12. 24.
 *      Author: PC
 */

#include "ICloudActor.h"

ICloudActor::ICloudActor()
{
}

ICloudActor::~ICloudActor()
{
}

bool ICloudActor::subAction(int actionTag)
{
	string subject;
	switch(actionTag)
	{
	//메일 본문 send(/wm/message\?)
	case 1:
		// 수신메일 본문 cmd=get
		if(getMailContent().StringGetter("cmd").compare("get") == 0)
		{
			subject = getMailContent().StringGetter("subject");
			subject.erase(subject.find_last_not_of(" \n\r\t")+1);
			subject = subject.substr(1,subject.size());
			getMailContent().setSubject(subject);
			getMailContent().setMappingkey(subject);
			setComposeKey(subject);
			g_MetaData.setStrSmallType("R");
		}
		// 수신메일 자동저장 cmd=saveDraft
		else if(getMailContent().StringGetter("cmd").compare("saveDraft") == 0)
		{
			vector<string> vmappingkey1, vmappingkey2;
			vmappingkey1.clear();
			vmappingkey2.clear();
			string composekey = "";
			g_MetaData.setSiteState(eSITE_SEND_MIDINFO);
			if(getMailContent().StringGetter("tmpcomposekey").empty())
				return false;
			composekey = getMailContent().StringGetter("tmpcomposekey");
			composekey = composekey.substr(composekey.find(":")+1);
			setComposeKey(composekey);
			if(getMailContent().getSubject().empty())
				getMailContent().setSubject("ICLOUD MAIL EMPTY SUBJECT");
			setSubjectComposeItem();
			setBodyContentComposeItem(getMailContent().getBody());
			setRecipComposeItem();
			vmappingkey1 = getMailContent().VectorGetter("tmpmappingkey1");
			vmappingkey2 = getMailContent().VectorGetter("tmpmappingkey2");
			if(vmappingkey1.size() == vmappingkey2.size())
			{
				for(unsigned int i = 0; i < vmappingkey1.size(); i++)
				{
					// 일반 첨부
					if(vmappingkey1.at(i).find("cachedpart") != string::npos)
						getMailContent().addMappingKey("mappingkey", vmappingkey1.at(i));
					// 메일드롭(대용량 포함 첨부)
					else
						getMailContent().addMappingKey("mappingkey", vmappingkey2.at(i)+":"+getMailContent().StringGetter("id"));
				}
			}
			else
			{
				for(unsigned int i = 0; i < vmappingkey1.size(); i++)
				{
					if(vmappingkey1.at(i).find("cachedpart") != string::npos)
						getMailContent().addMappingKey("mappingkey", vmappingkey1.at(i));
				}
			}
			setMappingkeyComposeItem();
			insertRedisComposeInfo();
			return false;
		}
		break;
	//대용량 첨부 로깅 send(/wm/cloudkit?)
	case 2:
		setMappingkeyComposeItem();
		insertRedisComposeInfo();
		return false;
		break;
	//대용량 첨부 URL send(/[\S]*/singleFileUpload\?)
	case 3:
		{
			vector<string> veckey = getMailContent().VectorGetter("tmpmappingkey");
			for(vector<string>::iterator iter = veckey.begin(); iter != veckey.end(); iter++)
				getMailContent().addMappingKey("mappingkey", *iter+":"+getMailContent().StringGetter("id"));
		}
		break;
	//일반 파일 첨부 URL send(/wm/messagepart\?)
	case 10:
		break;
	// 수신 메일 첨부 다운로드
	case 11:
		break;
	//수신 메일 midinfo
	case 12:
		setMappingkeyComposeItem();
		insertRedisComposeInfo();
		return false;
	case 13:
		{
			if(getMailContent().getAttachs().empty()){
				LOG(INFO)<<"[NOLOG] Icloud Mail Attach Empty";
				return false;
			}
			else{
				g_MetaData.setStrLargeType("W");
				g_MetaData.setStrMediumType("IC");
				g_MetaData.setStrSmallType("S");
			}
		}
		break;

	}

	return true;
}



