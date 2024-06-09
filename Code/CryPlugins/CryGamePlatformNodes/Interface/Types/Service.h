// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Types/Platform.h>
#include <CryGamePlatform/Interface/IPlatformService.h>

namespace Cry::GamePlatform
{

inline void ReflectType(Schematyc::CTypeDesc<IService::EProductType>& typeDesc)
{
	typeDesc.SetGUID("{37FE92C8-900C-4C2B-A0A3-C9ED1C1274A7}"_cry_guid);
	typeDesc.SetLabel("ProductType");
	typeDesc.SetDescription("Type of product");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(IService::EProductType::Consumable);
	typeDesc.AddConstant(IService::EProductType::Consumable, "consumable", "Consumable");
	typeDesc.AddConstant(IService::EProductType::DLC, "dlc", "DLC");
}

inline void ReflectType(Schematyc::CTypeDesc<IService::SStoreItem>& typeDesc)
{
	typeDesc.SetGUID("{07E59297-8B2C-47AB-8B5F-EBCDBFCEA7ED}"_cry_guid);
	typeDesc.SetLabel("StoreItem");
	typeDesc.SetDescription("A reference to a store item");
}

inline void ReflectType(Schematyc::CTypeDesc<IService::SStoreItemDetails>& typeDesc)
{
	typeDesc.SetGUID("{3073C3B8-85F0-4318-9926-0FB933917C9F}"_cry_guid);
	typeDesc.SetLabel("StoreItemDetails");
	typeDesc.SetDescription("A reference to a store item's details");
}

inline void ReflectType(Schematyc::CTypeDesc<IService::SInventoryItem>& typeDesc)
{
	typeDesc.SetGUID("{444DA681-F78E-400F-A695-1CFC95383A1E}"_cry_guid);
	typeDesc.SetLabel("InventoryItem");
	typeDesc.SetDescription("Inventory Item");
}

inline bool operator==(const IService::SInventoryItem& lhs, const IService::SInventoryItem& rhs)
{
	return lhs.id == rhs.id && lhs.productId == rhs.productId;
}

inline bool Serialize(Serialization::IArchive& ar, IService::SInventoryItem& inventoryItem, const char* szName, const char* szLabel)
{
	bool success = false;

	success = ar(inventoryItem.id, "id");
	success &= ar(inventoryItem.productId, "productid");
	success &= ar(inventoryItem.quantity, "quantity");

	return success;
}

inline void ReflectType(Schematyc::CTypeDesc<IService::EPermission>& typeDesc)
{
	typeDesc.SetGUID("{BF512FA2-89E6-4DB3-9025-08230C50E54E}"_cry_guid);
	typeDesc.SetLabel("Permission");
	typeDesc.SetDescription("Type of permission provided to the local user, used with HasPermission");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(IService::EPermission::Communication);
	typeDesc.AddConstant(IService::EPermission::Communication, "communication", "Communication");
	typeDesc.AddConstant(IService::EPermission::Multiplayer, "multiplayer", "Multiplayer");
	typeDesc.AddConstant(IService::EPermission::ViewProfiles, "viewprofiles", "ViewProfiles");
	typeDesc.AddConstant(IService::EPermission::WebBrowser, "webbrowser", "WebBrowser");
	typeDesc.AddConstant(IService::EPermission::UserGeneratedContent, "usergeneratedcontent", "UserGeneratedContent");
	typeDesc.AddConstant(IService::EPermission::MultiplayerSubscription, "multiplayersubscription", "MultiplayerSubscription");
	typeDesc.AddConstant(IService::EPermission::CrossNetworkPlay, "crossnetworkplay", "CrossNetworkPlay");
}

inline void ReflectType(Schematyc::CTypeDesc<IService::EPrivacyPermission>& typeDesc)
{
	typeDesc.SetGUID("{BD24B879-51EE-4FD4-A5A4-A78D53A3C972}"_cry_guid);
	typeDesc.SetLabel("PrivacyPermission");
	typeDesc.SetDescription("Type of privacy permission provided to the local user against another user, used with HasPrivacyPermission");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(IService::EPrivacyPermission::VoiceCommunication);
	typeDesc.AddConstant(IService::EPrivacyPermission::VoiceCommunication, "voicecommunication", "VoiceCommunication");
	typeDesc.AddConstant(IService::EPrivacyPermission::TextCommunication, "textcommunication", "TextCommunication");
	typeDesc.AddConstant(IService::EPrivacyPermission::PlayMultiplayer, "playmultiplayer", "PlayMultiplayer");
}

inline void ReflectType(Schematyc::CTypeDesc<IService::ERelationship>& typeDesc)
{
	typeDesc.SetGUID("{7E8BB0A4-834F-4F7F-A2B4-9156039B4A84}"_cry_guid);
	typeDesc.SetLabel("Relationship");
	typeDesc.SetDescription("Relationship with another account");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(IService::ERelationship::Default);
	typeDesc.AddConstant(IService::ERelationship::Default, "default", "Default");
	typeDesc.AddConstant(IService::ERelationship::Friend, "friend", "Friend");
}

inline void ReflectType(Schematyc::CTypeDesc<IService::EPermissionResult>& typeDesc)
{
	typeDesc.SetGUID("{5224A9F0-EF68-4352-878A-85E6BB09F7B6}"_cry_guid);
	typeDesc.SetLabel("PermissionResult");
	typeDesc.SetDescription("The result of a permission query on a platform");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(IService::EPermissionResult::NotApplicable);
	typeDesc.AddConstant(IService::EPermissionResult::NotApplicable, "notapplicable", "Not Applicable");
	typeDesc.AddConstant(IService::EPermissionResult::Denied, "denied", "Denied");
	typeDesc.AddConstant(IService::EPermissionResult::Granted, "granted", "Granted");
	typeDesc.AddConstant(IService::EPermissionResult::Pending, "pending", "Pending");
}

inline void ReflectType(Schematyc::CTypeDesc<IService::ECatalogSortOrder>& typeDesc)
{
	typeDesc.SetGUID("{26184C23-3028-4FFF-BFCE-A6F0EE045B58}"_cry_guid);
	typeDesc.SetLabel("CatalogSortOrder");
	typeDesc.SetDescription("The sorting order used to query the platform product catalog with");
	typeDesc.SetFlags(Schematyc::ETypeFlags::Switchable);
	typeDesc.SetDefaultValue(IService::ECatalogSortOrder::AllPurchaseCountDaily);
	typeDesc.AddConstant(IService::ECatalogSortOrder::AllPurchaseCountDaily, "allpurchasecountdaily", "AllPurchaseCountDaily");
	typeDesc.AddConstant(IService::ECatalogSortOrder::DigitalReleaseDate, "digitalreleasedate", "DigitalReleaseDate");
	typeDesc.AddConstant(IService::ECatalogSortOrder::PaidPurchaseCountAllTime, "paidpurchasecountalltime", "PaidPurchaseCountAllTime");
	typeDesc.AddConstant(IService::ECatalogSortOrder::PaidPurchaseCountDaily, "paidpurchasecountdaily", "PaidPurchaseCountDaily");
	typeDesc.AddConstant(IService::ECatalogSortOrder::PhysicalReleaseDate, "physicalreleasedate", "PhysicalReleaseDate");
	typeDesc.AddConstant(IService::ECatalogSortOrder::UserRating, "userrating", "UserRating");
}

} // Cry::GamePlatform