#pragma once

#include <IEditorBaker.h>


class CEditorBaker : public IEditorBaker
{
public:
	CEditorBaker();
	~CEditorBaker();

	virtual void Bake(IObjectManager* pObjectManager)override;
};