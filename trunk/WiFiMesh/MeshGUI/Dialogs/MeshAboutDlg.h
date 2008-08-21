#ifndef MESHABOUTDLG_H
#define MESHABOUTDLG_H

#include <QtGui>

class MeshAboutDlg : public QDialog
{
    Q_OBJECT

public:
    MeshAboutDlg(QWidget *parent = 0);
    ~MeshAboutDlg();

private:
	void init();
};

#endif // MESHABOUTDLG_H
