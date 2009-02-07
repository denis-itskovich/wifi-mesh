#ifndef MESHABOUTDLG_H
#define MESHABOUTDLG_H

#include <QtGui>

class MeshDlgAbout : public QDialog
{
    Q_OBJECT

public:
    MeshDlgAbout(QWidget *parent = 0);

private:
	void init();
};

#endif // MESHABOUTDLG_H
