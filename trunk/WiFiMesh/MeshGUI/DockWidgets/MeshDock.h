/*
 * AMeshDock.h
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#ifndef AMESHDOCK_H_
#define AMESHDOCK_H_

#include <QtGui>
#include "../CoreWrappers/MeshStation.h"
#include "../Document/MeshDocument.h"

class MeshDock : public QDockWidget
{
public:
	void setDocument(MeshDocument* doc);

protected:
	MeshDock(const QString& title, QWidget* parent = 0);
	MeshDock(QWidget* parent = 0);

	virtual ~MeshDock();

	void refresh() {};
	MeshDocument* document();

private:
	MeshDocument* m_document;
};

#endif /* AMESHDOCK_H_ */
