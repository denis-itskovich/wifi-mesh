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

#define COPYRIGHT_STRING 	"<p>%1, <b>version %2</b></p>" 		\
							"<p><b>\xa9 2008 Denis Itskovich</b></p>"				    \
							"<p>Application is distributed under <a href=\"http://www.gnu.org/licenses/gpl-2.0.html#TOC1\">GPL v2 license</a></p>" \
							"<p>Source code can be found at <a href=\"http://wifi-mesh.googlecode.com/svn/trunk/WiFiMesh\">SVN repository</a></p>"


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

	QPushButton* buttonOK = new QPushButton(tr("Close"));
	QBoxLayout* vlayout = new QVBoxLayout(this);
	QGridLayout* gridLayout = new QGridLayout();

	gridLayout->setSpacing(10);
	gridLayout->addWidget(meshLogo, 0, 0);
	gridLayout->addWidget(copyright, 0, 1);

	vlayout->addItem(gridLayout);
	vlayout->addWidget(buttonOK);

	setLayout(vlayout);

	connect(buttonOK, SIGNAL(clicked()), this, SLOT(close()));
}
