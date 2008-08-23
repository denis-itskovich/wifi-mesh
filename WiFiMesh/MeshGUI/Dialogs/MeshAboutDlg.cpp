#include "MeshAboutDlg.h"

#define COPYRIGHT_STRING 	"<p>%1, <b>version %2</b></p>" 		\
							"<p><b>(C) 2008 Denis Itskovich</b></p>"				\
							"<p>Application is distributed 'as is'</p>"				\
							"<p>Source code can be found at <a href=\"http://wifi-mesh.googlecode.com/svn/trunk/\">SVN repository</a></p>"


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
	setWindowTitle(tr("About %1").arg(qApp->applicationName()));

	QLabel* copyright = new QLabel(tr(COPYRIGHT_STRING).arg(qApp->applicationName()).arg(qApp->applicationVersion()), this);
	QLabel* meshLogo = new QLabel(this);
	copyright->setOpenExternalLinks(true);
	meshLogo->setAlignment(Qt::AlignTop);

	meshLogo->setPixmap(QPixmap(":/wifi.png"));

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
