// Copyright 2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <GamePlatformHelper.h>
#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CryGamePlatform/Interface/IPlatformLobby.h>
#include <CryGamePlatform/Interface/IPlatformMatchmaking.h>

namespace Cry::GamePlatform
{

inline void ReflectType(Schematyc::CTypeDesc<LobbyIdentifier>& typeDesc)
{
	typeDesc.SetGUID("{5B5D942B-7CFA-4B2D-BA49-5734C4DFB8A2}"_cry_guid);
	typeDesc.SetLabel("Lobby");
	typeDesc.SetDescription("An identifier for a platform-specific lobby");
}

inline void ReflectType(Schematyc::CTypeDesc<IUserLobby::EVisibility>& typeDesc)
{
	typeDesc.SetGUID("{937B76C7-43FB-4B97-8178-705DADB987EF}"_cry_guid);
	typeDesc.SetLabel("LobbyVisibility");
	typeDesc.SetDescription("Determines the visibility of the lobby to other users");
	typeDesc.SetDefaultValue(IUserLobby::EVisibility::Public);
	typeDesc.AddConstant(IUserLobby::EVisibility::Public, "public", "Public");
	typeDesc.AddConstant(IUserLobby::EVisibility::Private, "private", "Private");
	typeDesc.AddConstant(IUserLobby::EVisibility::FriendsOnly, "friends", "FriendsOnly");
	typeDesc.AddConstant(IUserLobby::EVisibility::Invisible, "invis", "Invisible");
}

inline void ReflectType(Schematyc::CTypeDesc<IUserLobby::EComparison>& typeDesc)
{
	typeDesc.SetGUID("{23044D2D-D1D1-42AB-A04D-41FF4FFC9E32}"_cry_guid);
	typeDesc.SetLabel("ComparisonType");
	typeDesc.SetDescription("Allows for comparing lobbies based on certain filters");
	typeDesc.SetDefaultValue(IUserLobby::EComparison::Equal);
	typeDesc.AddConstant(IUserLobby::EComparison::Equal, "eq", "Equal");
	typeDesc.AddConstant(IUserLobby::EComparison::EqualToOrGreaterThan, "eqgtr", "EqualToOrGreater");
	typeDesc.AddConstant(IUserLobby::EComparison::EqualToOrLess, "eqlt", "EqualToOrLess");
	typeDesc.AddConstant(IUserLobby::EComparison::NotEqual, "neq", "NotEqual");
	typeDesc.AddConstant(IUserLobby::EComparison::GreaterThan, "gtr", "GreaterThan");
	typeDesc.AddConstant(IUserLobby::EComparison::LessThan, "lt", "LessThan");
}

} // Cry::GamePlatform