/*********************************************************************************
WiFi Mesh Simulator PRO
Copyright (C) 2009 Denis Itskovich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*********************************************************************************/

#include "MeshDlgAbout.h"

#define COPYRIGHT_STRING 	"<p align=\"center\"><b>%1</b>, version <b>%2</b></p>" \
							"<p align=\"center\">\xa9 Copyright 2008 <a href=\"mailto:denis.itskovich@gmail.com\">Denis Itskovich</a></p><p> </p>" \
							"<p>Application is distributed under <a href=\"http://www.gnu.org/licenses/gpl-2.0.html#TOC1\">GPL v2 license</a></p>" \
							"<p>Source code can be found at <a href=\"http://wifi-mesh.googlecode.com/svn/trunk/WiFiMesh\">SVN repository</a></p>" \
							"<p>Please visit <a href=\"http://wifi-mesh.googlecode.com\">project homepage</a> for updates</p>"


MeshDlgAbout::MeshDlgAbout(QWidget *parent)
    : QDialog(parent)
{
	init();
}

void MeshDlgAbout::init()
{
	setModal(true);
	setWindowTitle(tr("About %1").arg(qApp->applicationName()));
	setWindowIcon(QIcon(":/about.png"));

	QLabel* copyright = new QLabel(tr(COPYRIGHT_STRING).arg(qApp->applicationName()).arg(qApp->applicationVersion()), this);
	QLabel* meshLogo = new QLabel(this);
	copyright->setOpenExternalLinks(true);
	meshLogo->setAlignment(Qt::AlignTop);

	meshLogo->setPixmap(QPixmap(":/station.png"));

	QPushButton* buttonOK = new QPushButton(tr("OK"));
	QBoxLayout* vlayout = new QVBoxLayout;
	QBoxLayout* logoLayout = new QHBoxLayout;

	logoLayout->setSpacing(10);
	logoLayout->addWidget(meshLogo, 0);
	logoLayout->addWidget(copyright, 99);

	vlayout->addItem(logoLayout);
	vlayout->addWidget(buttonOK, 0, Qt::AlignCenter);

	setLayout(vlayout);
	setMaximumSize(minimumSize());

	connect(buttonOK, SIGNAL(clicked()), this, SLOT(close()));
}
