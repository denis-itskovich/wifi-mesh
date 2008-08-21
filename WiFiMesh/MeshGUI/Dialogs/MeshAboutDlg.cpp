#include "MeshAboutDlg.h"

#define COPYRIGHT_STRING 	"<p>WiFi Mesh Simulator, <b>version 0.0.1</b></p>" 	\
							"<p><b>(C) 2008 Denis Itskovich</b></p><p></p>"
#define QT_TEXT				"<p>This application was written using</p>"			\
							"<p><i>Qt4 open source edition</i></p>"


MeshAboutDlg::MeshAboutDlg(QWidget *parent)
    : QDialog(parent)
{
	init();
}

MeshAboutDlg::~MeshAboutDlg()
{

}

void MeshAboutDlg::init()
{
	setModal(true);

	QLabel* copyright = new QLabel(tr(COPYRIGHT_STRING), this);
	QLabel* qtText = new QLabel(tr(QT_TEXT), this);

	QLabel* meshLogo = new QLabel(this);
	QLabel* qtLogo = new QLabel(this);

	meshLogo->setPixmap(QPixmap(":/wifi.png"));
	qtLogo->setPixmap(QPixmap(":/qt.png"));

	QPushButton* buttonOK = new QPushButton(tr("Close"));
	QBoxLayout* vlayout = new QVBoxLayout(this);
	QGridLayout* gridLayout = new QGridLayout();

	gridLayout->setSpacing(10);
	gridLayout->addWidget(meshLogo, 0, 0);
	gridLayout->addWidget(copyright, 0, 1);
	gridLayout->addWidget(qtLogo, 1, 0);
	gridLayout->addWidget(qtText, 1, 1);

	vlayout->addItem(gridLayout);
	vlayout->addWidget(buttonOK);

	this->setLayout(vlayout);

	connect(buttonOK, SIGNAL(clicked()), this, SLOT(close()));
}
