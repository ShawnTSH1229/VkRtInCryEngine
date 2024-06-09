// Copyright 2010-2022 Crytek GmbH / Crytek Group. All rights reserved.

#include "StdAfx.h"
#include "AboutDialog.h"
#include "IEditorImpl.h"

#include <EditorStyleHelper.h>

#include <QLayout>
#include <QLabel>
#include <QPushButton>

inline QLabel* CreateRichLabel(const QString& objectName = "", bool shouldWrap = false, bool allowLinks = false)
{
	QLabel* pLabel = new QLabel();
	pLabel->setObjectName(objectName);

	pLabel->setTextFormat(Qt::RichText);
	pLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

	pLabel->setWordWrap(shouldWrap);
	pLabel->setOpenExternalLinks(allowLinks);

	return pLabel;
}

CAboutDialog::CAboutDialog()
	: CEditorDialog("About", nullptr, false)
{
	setModal(true);
	SetResizable(false);

	// Visible Elements
	QLabel* pLogo = CreateRichLabel("About-Logo");
	QLabel* pCopyright = CreateRichLabel("About-Copyright");
	QLabel* pTrademarks = CreateRichLabel("About-Trademarks");
	QLabel* pThirdParty = CreateRichLabel("About-ThirdParty");
	QLabel* pAttributionLeft = CreateRichLabel("About-AttributionLeft");
	QLabel* pAttributionMid = CreateRichLabel("About-AttributionMiddle");
	QLabel* pAttributionRight = CreateRichLabel("About-AttributionRight", false, true);
	QLabel* pFbxAttribution = CreateRichLabel("About-FbxAttribution", true);
	QLabel* pThirdPartyDocs = CreateRichLabel("About-ThirdPartyDocs", false, true);
	QLabel* pVersion = CreateRichLabel("About-Version");

	QWidget* pCloseButtonFrame = new QWidget();
	pCloseButtonFrame->setObjectName("About-CloseButtonFrame");
	QPushButton* pCloseButton = new QPushButton(tr("&Close"), pCloseButtonFrame);
	pCloseButton->setObjectName("About-CloseButton");

	// Element Contents
	QPixmap* pLogoImage = new QPixmap(":/about.png");
	pLogo->setPixmap(*pLogoImage);

	const QString qtlink("https://doc.qt.io/qt-5/lgpl.html");
	const QString lgpl21link("https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html");
	const QString docslink("https://docs.cryengine.com/display/RN/Third+Party+SDKs+in+5.7");

	pCopyright->setText(
		"CRYENGINE&reg; Sandbox&trade; Editor<br />"
		"Copyright&copy; 2022, Crytek GmbH"
	);

	pTrademarks->setText(
		"Crytek, Crytek logo, CRYENGINE, Sandbox are trademarks of Crytek."
	);

	pThirdParty->setText(
		"Some portions of this software make use of the following third party libraries:"
	);

	pAttributionLeft->setText(
		"<p style=\"line-height:18px\"><span>Qt<br / >"
		"PySide2<br />"
		"libsndfile<br />"
		"mpg123<br />"
		"qpOASES<br /></span></p>"
	);

	pAttributionMid->setText(
		"<p style=\"line-height:18px\"><span> - <br />"
		" - <br />"
		" - <br />"
		" - <br />"
		" - <br /></span></p>"
	);

	pAttributionRight->setText(QString(
		"<p style=\"line-height:18px\"><span>Copyright&copy; The Qt Company Ltd. under the <a style=\"color: %1;\" href=\"%2\">LGPL v3 License</a>.<br />"
		"Copyright&copy; The Qt Company Ltd. under the <a style=\"color: %1;\" href=\"%2\">LGPL v3 License</a>.<br />"
		"Copyright&copy; Erik de Castro Lopo. under the <a style=\"color: %1;\" href=\"%3\">LGPL v2.1 License</a>.<br />"
		"Copyright&copy; 1995-2013 by Michael Hipp and others, free software under the terms of the <a style=\"color: %1;\" href=\"%3\">LGPL v2.1 License</a>.<br />"
		"Copyright&copy; 2007-2014 by Hans Joachim Ferreau, Andreas Potschka, Christian Kirches et al.<br />"
		"All rights reserved. under the <a style=\"color: %1;\" href=\"%3\">LGPL v2.1 License</a>.</span></p>"
		)
		.arg(GetStyleHelper()->selectedIconTint().name())
		.arg(qtlink)
		.arg(lgpl21link)
	);

	pFbxAttribution->setText(
		"This software contains Autodesk&reg; FBX&reg; code developed by Autodesk, Inc. Copyright 2014 Autodesk, Inc. All rights, "
		"reserved. Such code is provided \"as is\" and Autodesk, Inc. disclaims any and all warranties, whether express or implied, "
		"including without limitation the implied warranties of merchantability, fitness for a particular purpose or non-infringement "
		"of third party rights. In no event shall Autodesk, Inc. be liable for any direct, indirect, incidental, special, exemplary, "
		"or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or "
		"profits; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or "
		"tort (including negligence or otherwise) arising in any way out of such code."
	);

	pThirdPartyDocs->setText(QString(
		"For more information regarding Third Party software included and/or used in CRYENGINE then <br /> please visit the <a style=\"color: %1;\" href=\"%2\">Third Party SDK Documentation</a>."
		)
		.arg(GetStyleHelper()->selectedIconTint().name())
		.arg(docslink)
	);

	Version version = GetIEditorImpl()->GetFileVersion();
	pVersion->setText(QStringLiteral("Version %1.%2.%3 - Build %4").arg(version[3]).arg(version[2]).arg(version[1]).arg(version[0]));

	// Actions
	connect(pCloseButton, &QPushButton::clicked, [this]()
	{
		close();
	});

	// Widget Layouts
	QHBoxLayout* pLogoLayout = new QHBoxLayout();
	pLogoLayout->setSpacing(0);
	pLogoLayout->setMargin(0);
	pLogoLayout->addSpacing(40);
	pLogoLayout->addWidget(pLogo, 1, Qt::AlignCenter);
	pLogoLayout->addSpacing(40);

	QHBoxLayout* pAttributionLayout = new QHBoxLayout();
	pAttributionLayout->setSpacing(0);
	pAttributionLayout->setMargin(0);
	pAttributionLayout->addWidget(pAttributionLeft, 0, Qt::AlignLeft);
	pAttributionLayout->addSpacing(5);
	pAttributionLayout->addWidget(pAttributionMid, 0, Qt::AlignLeft);
	pAttributionLayout->addSpacing(5);
	pAttributionLayout->addWidget(pAttributionRight, 1, Qt::AlignLeft);

	QHBoxLayout* pBottomLayout = new QHBoxLayout();
	pBottomLayout->setSpacing(0);
	pBottomLayout->setMargin(0);
	pBottomLayout->addWidget(pVersion, 1, Qt::AlignLeft);
	pBottomLayout->addWidget(pCloseButton, 1, Qt::AlignRight);

	QVBoxLayout* pContentLayout = new QVBoxLayout();
	pContentLayout->setSpacing(0);
	pContentLayout->setMargin(0);
	pContentLayout->addWidget(pCopyright);
	pContentLayout->addSpacing(20);
	pContentLayout->addWidget(pTrademarks);
	pContentLayout->addSpacing(20);
	pContentLayout->addWidget(pThirdParty);
	pContentLayout->addSpacing(10);
	pContentLayout->addLayout(pAttributionLayout);
	pContentLayout->addSpacing(20);
	pContentLayout->addWidget(pFbxAttribution);
	pContentLayout->addSpacing(20);
	pContentLayout->addWidget(pThirdPartyDocs);
	pContentLayout->addSpacing(69);
	pContentLayout->addLayout(pBottomLayout);

	QHBoxLayout* pInnerLayout = new QHBoxLayout();
	pInnerLayout->setSpacing(0);
	pInnerLayout->setMargin(0);
	pInnerLayout->addSpacing(80);
	pInnerLayout->addLayout(pContentLayout);
	pInnerLayout->addSpacing(80);

	QVBoxLayout* pOuterLayout = new QVBoxLayout();
	pOuterLayout->setSpacing(0);
	pOuterLayout->setMargin(0);
	pOuterLayout->addSpacing(40);
	pOuterLayout->addLayout(pLogoLayout);
	pOuterLayout->addSpacing(40);
	pOuterLayout->addLayout(pInnerLayout);
	pOuterLayout->addSpacing(40);

	setLayout(pOuterLayout);
}

CAboutDialog::~CAboutDialog()
{
	close();
}
