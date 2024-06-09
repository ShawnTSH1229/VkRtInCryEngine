// Copyright 2013-2021 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

class CLensFlareItem;
class CLensFlareElement;

struct ILensFlareChangeItemListener
{
	virtual ~ILensFlareChangeItemListener() {}
	virtual void OnLensFlareChangeItem(CLensFlareItem* pLensFlareItem) = 0;
	virtual void OnLensFlareDeleteItem(CLensFlareItem* pLensFlareItem) = 0;
};

struct ILensFlareChangeElementListener
{
	virtual ~ILensFlareChangeElementListener() {}
	virtual void OnLensFlareChangeElement(CLensFlareElement* pLensFlareElement) = 0;
};
