// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include <Schematyc/GamePlatformEnv.h>

#include <GamePlatformHelper.h>
#include <Types/RemoteStorage.h>

#include <CryCore/StaticInstanceList.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CrySchematyc/Env/Elements/EnvModule.h>
#include <CrySchematyc/Utils/SharedString.h>

#include <CryGamePlatform/Interface/IPlatformRemoteStorage.h>

namespace Cry::GamePlatform::UserGeneratedContent
{

bool SetTitle(const CUserGeneratedContent& content, const Schematyc::CSharedString& title)
{
	bool success = false;

	if (IUserGeneratedContent* pContent = content.GetPtr())
	{
		success = pContent->SetTitle(title.c_str());
	}

	return success;
}

bool SetDescription(const CUserGeneratedContent& content, const Schematyc::CSharedString& description)
{
	bool success = false;

	if (IUserGeneratedContent* pContent = content.GetPtr())
	{
		success = pContent->SetDescription(description.c_str());
	}

	return success;
}

bool SetVisibility(const CUserGeneratedContent& content, IUserGeneratedContent::EVisibility visibility)
{
	bool success = false;

	if (IUserGeneratedContent* pContent = content.GetPtr())
	{
		success = pContent->SetVisibility(visibility);
	}

	return success;
}

bool SetTags(const CUserGeneratedContent& content, Schematyc::CAnyArrayPtr tagsArray)
{
	bool success = false;

	if (IUserGeneratedContent* pContent = content.GetPtr())
	{
		if (tagsArray && tagsArray->GetSize())
		{
			DynArray<const char*> tags;
			for (int i = 0; i < tagsArray->GetSize(); i++)
			{
				const Schematyc::CSharedString& element = Schematyc::DynamicCast<Schematyc::CSharedString>((*tagsArray)[i]);
				tags.push_back(element.c_str());
			}

			success = pContent->SetTags(tags.data(), tags.size());
		}
	}

	return success;
}

bool SetContent(const CUserGeneratedContent& content, const Schematyc::CSharedString& contentFolderPath)
{
	bool success = false;

	if (IUserGeneratedContent* pContent = content.GetPtr())
	{
		success = pContent->SetContent(contentFolderPath.c_str());
	}

	return success;
}

bool SetPreview(const CUserGeneratedContent& content, const Schematyc::CSharedString& previewFilePath)
{
	bool success = false;

	if (IUserGeneratedContent* pContent = content.GetPtr())
	{
		success = pContent->SetPreview(previewFilePath.c_str());
	}

	return success;
}

void CommitChanges(const CUserGeneratedContent& content, const Schematyc::CSharedString& changeNote)
{
	if (IUserGeneratedContent* pContent = content.GetPtr())
	{
		pContent->CommitChanges(changeNote.c_str());
	}
}

void Create(IUserGeneratedContent::EType type)
{
	if (auto pService = Helper::GetMainService())
	{
		if (auto pRemoteStorage = pService->GetRemoteStorage())
		{
			if (auto pUGCMgr = pRemoteStorage->GetUserGeneratedContentManager())
			{
				pUGCMgr->Create(pService->GetApplicationIdentifier(), type);
			}
		}
	}
}

void RegisterElements(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope serviceScope = registrar.Scope(g_gamePlatformServiceGUID);

	auto scope = serviceScope.Register(SCHEMATYC_MAKE_ENV_MODULE("{267C0513-D89C-4F34-88F5-6982AE50BCED}"_cry_guid, "UserGeneratedContent"));
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetTitle, "{336D6053-AD9F-4DEA-949A-15AC7BBA388E}"_cry_guid, "SetTitle");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'cont', "Content");
		pFunction->BindInput(2, 'titl', "Title");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetDescription, "{805842B0-2D5E-463E-A9DD-0955F95E75A1}"_cry_guid, "SetDescription");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'cont', "Content");
		pFunction->BindInput(2, 'desc', "Description");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetVisibility, "{577E99BE-7B20-4393-AB62-FEEB68586FA1}"_cry_guid, "SetVisibility");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'cont', "Content");
		pFunction->BindInput(2, 'visb', "Visibility");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetTags, "{4A451FA6-C904-46A9-8EB3-107DC8FD21BA}"_cry_guid, "SetTags");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'cont', "Content");
		pFunction->BindInputArrayType<Schematyc::CSharedString>(2, 'tags', "Array [String]");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetContent, "{B33F1EAE-A643-40E8-A205-25AD7185B921}"_cry_guid, "SetContentFolder");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'cont', "Content");
		pFunction->BindInput(2, 'cfld', "Content Folder");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&SetPreview, "{34F54FE4-DFD6-45B2-AE83-46BCF2F9CF8C}"_cry_guid, "SetPreviewFolder");
		pFunction->BindOutput(0, 'succ', "Success");
		pFunction->BindInput(1, 'cont', "Content");
		pFunction->BindInput(2, 'pfld', "Preview Folder");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CommitChanges, "{B9C1F034-2BDF-4479-90FD-0358D3570311}"_cry_guid, "CommitChanges");
		pFunction->BindInput(1, 'cont', "Content");
		pFunction->BindInput(2, 'chgn', "Change Note");
		scope.Register(pFunction);
	}
	{
		auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&Create, "{3ED13796-E976-4969-AB9D-FD6779D72956}"_cry_guid, "Create");
		pFunction->BindInput(1, 'type', "Content Type");
		scope.Register(pFunction);
	}
}

} // Cry::GamePlatform::UserGeneratedContent
