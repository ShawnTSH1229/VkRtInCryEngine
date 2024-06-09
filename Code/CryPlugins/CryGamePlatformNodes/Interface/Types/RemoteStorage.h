// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Types/Platform.h>

#include <CrySchematyc/Utils/SharedString.h>
#include <CryGamePlatform/Interface/IPlatformRemoteStorage.h>
#include <CryGamePlatform/Interface/IPlatformUserGeneratedContent.h>

namespace Cry::GamePlatform
{

class CCachedLocal
{

public:

	CCachedLocal()
		: m_name()
		, m_platform(EGamePlatform::None)
		, m_pRemoteFile(nullptr)
	{}

	explicit CCachedLocal(const Schematyc::CSharedString& name, EGamePlatform platform, const std::shared_ptr<IRemoteFile>& pRemoteFile)
		: m_name(name)
		, m_platform(platform)
		, m_pRemoteFile(pRemoteFile)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<CCachedLocal>& typeDesc)
	{
		typeDesc.SetGUID("{CB822A6D-2BFC-4372-9D9B-06A3B8B85406}"_cry_guid);
		typeDesc.SetLabel("RemoteFile");
		typeDesc.AddMember(&CCachedLocal::m_name, 'name', "name", "Name", nullptr, "");
		typeDesc.AddMember(&CCachedLocal::m_platform, 'pfrm', "platform", "Platform", nullptr, EGamePlatform::Steam);
	}

	const Schematyc::CSharedString& GetName() const
	{
		return m_name;
	}

	EGamePlatform GetPlatform() const
	{
		return m_platform;
	}

	IRemoteFile* GetPtr() const
	{
		return m_pRemoteFile.get();
	}

	bool operator==(const CCachedLocal& other) const
	{
		return m_name == other.m_name;
	}

	bool operator!=(const CCachedLocal& other) const
	{
		return !(m_name == other.m_name);
	}

	bool operator<(const CCachedLocal& rhs) const
	{
		return strcmp(m_name.c_str(), rhs.m_name.c_str()) < 0;
	}

	friend bool operator<(const char* name, const CCachedLocal& rhs)
	{
		return strcmp(name, rhs.m_name.c_str()) < 0;
	}

	friend bool operator<(const CCachedLocal& lhs, const char* name)
	{
		return strcmp(lhs.m_name.c_str(), name) < 0;
	}

private:

	Schematyc::CSharedString m_name;
	EGamePlatform            m_platform;
	std::shared_ptr<IRemoteFile> m_pRemoteFile;
};

class CCachedShared
{

public:

	CCachedShared()
		: m_identifier(0)
		, m_platform(EGamePlatform::None)
	{}

	explicit CCachedShared(ISharedRemoteFile::Identifier identifier, EGamePlatform platform, const std::shared_ptr<ISharedRemoteFile>& pSharedFile)
		: m_identifier(identifier)
		, m_platform(platform)
		, m_pSharedFile(pSharedFile)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<CCachedShared>& typeDesc)
	{
		typeDesc.SetGUID("{45EF17DC-1B49-4318-B465-04DCDCA4CE23}"_cry_guid);
		typeDesc.SetLabel("SharedFile");
		typeDesc.AddMember(&CCachedShared::m_identifier, 'sfid', "sharedfileid", "Shared Id", nullptr, 0);
		typeDesc.AddMember(&CCachedShared::m_platform, 'pfrm', "platform", "Platform", nullptr, EGamePlatform::Steam);
	}

	const ISharedRemoteFile::Identifier& GetSharedFileId() const
	{
		return m_identifier;
	}

	EGamePlatform GetPlatform() const
	{
		return m_platform;
	}

	ISharedRemoteFile* GetPtr() const
	{
		return m_pSharedFile.get();
	}

	bool operator==(const CCachedShared& other) const
	{
		return m_identifier == other.m_identifier;
	}

	bool operator!=(const CCachedShared& other) const
	{
		return !(m_identifier == other.m_identifier);
	}

	bool operator<(const CCachedShared& rhs) const
	{
		return m_identifier < rhs.m_identifier;
	}

	friend bool operator<(const ISharedRemoteFile::Identifier& id, const CCachedShared& rhs)
	{
		return id < rhs.GetSharedFileId();
	}

	friend bool operator<(const CCachedShared& lhs, const ISharedRemoteFile::Identifier& id)
	{
		return lhs.GetSharedFileId() < id;
	}

private:

	ISharedRemoteFile::Identifier       m_identifier;
	EGamePlatform                       m_platform;
	std::shared_ptr<ISharedRemoteFile>  m_pSharedFile;

};

class CUserGeneratedContent
{

public:

	CUserGeneratedContent()
		: m_pUserGeneratedContent(nullptr)
	{}

	explicit CUserGeneratedContent(IUserGeneratedContent* pUserGeneratedContent)
		: m_pUserGeneratedContent(pUserGeneratedContent)
	{}

	static inline void ReflectType(Schematyc::CTypeDesc<CUserGeneratedContent>& typeDesc)
	{
		typeDesc.SetGUID("{403BC21C-9300-4614-BC58-07A42F236B3D}"_cry_guid);
		typeDesc.SetLabel("UserGeneratedContent");
	}

	IUserGeneratedContent* GetPtr() const
	{
		return m_pUserGeneratedContent;
	}

	bool operator==(const CUserGeneratedContent& other) const
	{
		if (m_pUserGeneratedContent == nullptr || other.m_pUserGeneratedContent == nullptr)
			return false;

		return m_pUserGeneratedContent->GetIdentifier() == other.m_pUserGeneratedContent->GetIdentifier();
	}

private:

	IUserGeneratedContent* m_pUserGeneratedContent;

};

static inline void ReflectType(Schematyc::CTypeDesc<IUserGeneratedContent::EType>& typeDesc)
{
	typeDesc.SetGUID("{0125F774-DF19-496B-B244-3E40F31F667A}"_cry_guid);
	typeDesc.SetLabel("ContentType");
	typeDesc.SetDefaultValue(IUserGeneratedContent::EType::Community);
	typeDesc.AddConstant(IUserGeneratedContent::EType::Community, "community", "Community");
	typeDesc.AddConstant(IUserGeneratedContent::EType::Microtransaction, "microtransaction", "Microtransaction");
	typeDesc.AddConstant(IUserGeneratedContent::EType::Collection, "collection", "Collection");
	typeDesc.AddConstant(IUserGeneratedContent::EType::Art, "art", "Art");
	typeDesc.AddConstant(IUserGeneratedContent::EType::Video, "video", "Video");
	typeDesc.AddConstant(IUserGeneratedContent::EType::Screenshot, "screenshot", "Screenshot");
	typeDesc.AddConstant(IUserGeneratedContent::EType::Game, "game", "Game");
	typeDesc.AddConstant(IUserGeneratedContent::EType::Software, "software", "Software");
	typeDesc.AddConstant(IUserGeneratedContent::EType::Concept, "concept", "Concept");
	typeDesc.AddConstant(IUserGeneratedContent::EType::WebGuide, "webguide", "Web Guide");
	typeDesc.AddConstant(IUserGeneratedContent::EType::IntegratedGuide, "integratedguide", "Integrated Guide");
	typeDesc.AddConstant(IUserGeneratedContent::EType::Merch, "merch", "Merch");
	typeDesc.AddConstant(IUserGeneratedContent::EType::ControllerBinding, "controllerBinding", "Controller Binding");
	typeDesc.AddConstant(IUserGeneratedContent::EType::SteamworksAccessInvite, "steamworksaccessinvite", "Steamworks Access Invite");
	typeDesc.AddConstant(IUserGeneratedContent::EType::SteamVideo, "steamvideo", "Steam Video");
}

static inline void ReflectType(Schematyc::CTypeDesc<IUserGeneratedContent::EVisibility>& typeDesc)
{
	typeDesc.SetGUID("{904168CD-1E2F-429F-81A8-8363B1D29365}"_cry_guid);
	typeDesc.SetLabel("ContentVisibility");
	typeDesc.SetDefaultValue(IUserGeneratedContent::EVisibility::Public);
	typeDesc.AddConstant(IUserGeneratedContent::EVisibility::Public, "public", "Public");
	typeDesc.AddConstant(IUserGeneratedContent::EVisibility::FriendsOnly, "friendsonly", "Friends Only");
	typeDesc.AddConstant(IUserGeneratedContent::EVisibility::Private, "private", "Private");
}

} // Cry::GamePlatform