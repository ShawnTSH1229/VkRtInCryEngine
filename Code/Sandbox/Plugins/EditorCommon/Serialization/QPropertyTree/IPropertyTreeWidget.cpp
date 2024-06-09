// Copyright 2017-2021 Crytek GmbH / Crytek Group. All rights reserved.
#include "StdAfx.h"
#include "IPropertyTreeWidget.h"

PROPERTY_TREE_API CPropertyTreeWidgetFactory& GetPropertyTreeWidgetFactory()
{
	static CPropertyTreeWidgetFactory sFactory;
	return sFactory;
}
