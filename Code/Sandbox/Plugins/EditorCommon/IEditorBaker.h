#pragma once

struct IObjectManager;

struct IEditorBaker
{
	virtual void Bake(IObjectManager* pObjectManager) = 0;
};