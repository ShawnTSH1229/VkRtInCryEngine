// Copyright 2015-2021 Crytek GmbH / Crytek Group. All rights reserved.
// Wrapper for QViewport usable from Qt Creator's UI Designer
#include <QViewPort.h>

class QViewportWrapper : public QViewport
{
public:
	QViewportWrapper(QWidget* pParent)
		: QViewport(gEnv, pParent)
	{
		SetOrbitMode(true);
	}
};
