/**
 * @file AolMailActor.cpp
 * @date 2024. 8. 27.
 * @author root
 */

#include "AolMailActor.h"

AolMailActor::AolMailActor()
{
}

AolMailActor::~AolMailActor()
{
}

bool AolMailActor::subAction(int actionTag)
{
	LOG(INFO)<<"[AolMailActor::subAction] actionTag: " << actionTag;
	string bodyStr = "";
	vector<string> appPath;
	int seq;

	switch (actionTag)
	{
	case 2: // autoSaveV3
		setRecipComposeItem();
		setSubjectComposeItem();
		bodyStr = getMailContent().getBody();
		setBodyContentComposeItem(bodyStr);
		getMailContent().setBody("");
		insertRedisComposeInfo();
		g_MetaData.setSiteState(eSITE_SEND_BODY);
		g_MetaData.setStrLargeType("W");
		g_MetaData.setStrMediumType("AO");
		g_MetaData.setStrSmallType("T");

		return false;

	case 3: // saveV3
		if (g_MetaData.getStrSubjectPrefix().compare("U ") == 0)
			g_MetaData.setStrSubjectPrefix("W ");

		setRecipComposeItem();
		setSubjectComposeItem();
		bodyStr = getMailContent().getBody();
		setBodyContentComposeItem(bodyStr);

		if (getMailContent().getAttachs().empty()) {
			getMailContent().clearMappingKey();
			g_MetaData.setSiteState(eSITE_SEND_MIDINFO);
			insertRedisComposeInfo();
			g_MetaData.setSiteState(eSITE_SEND_BODY);
			g_MetaData.setStrLargeType("W");
			g_MetaData.setStrMediumType("AO");
			g_MetaData.setStrSmallType("T");
		}
		else {
			if (g_EmassConfig.isUsedHadoopThreePoint())
			{
				g_MetaData.setSiteState(eSITE_SEND_MIDINFO);
				getAppPath(appPath);
				if (!appPath.empty())
					removeAppPath();
				insertRedisComposeInfo();
				removeOptionalFields();
				seq = g_EmassFileName.getSeq() + 1;
				g_EmassFileName.setSeq(seq);
				g_MetaData.setSiteState(eSITE_SEND_ATTACH);
				if (!appPath.empty())
					addAppPath(appPath);
			}
			else
			{
				insertRedisComposeInfo();
			}
		}
		break;
	default:
		break;
	}
	return true;
}


