// Copyright 2012-2021 Crytek GmbH / Crytek Group. All rights reserved.
//
//  Crytek Engine Source File.
// Copyright 2012-2021 Crytek GmbH / Crytek Group. All rights reserved.
// -------------------------------------------------------------------------
//  File name:   AlembicConverter.h
//  Created:     20/7/2012 by Axel Gneiting
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IConverter.h"

struct ICryXML;

class AlembicConverter : public IConverter
{
public:
	AlembicConverter(ICryXML *pXMLParser, IPakSystem* pPakSystem);
	virtual ~AlembicConverter();

	// IConverter methods.
	virtual void Release();
	virtual void DeInit() {}
	virtual ICompiler *CreateCompiler();
	virtual bool SupportsMultithreading() const;
	virtual const char *GetExt(int index) const;

private:
	int m_refCount;
	ICryXML *m_pXMLParser;
	IPakSystem *m_pPakSystem;
};