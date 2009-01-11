/*
 * .h
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#ifndef MESHFRAME_H_
#define MESHFRAME_H_

#include <QtGui>
#include "../Document/MeshDocument.h"

class DockFrame : public QWidget
{
	Q_OBJECT
public:
	virtual void setDocument(MeshDocument* document);

protected:
	DockFrame(MeshDocument* doc = 0, QWidget* parent = 0);
	virtual ~DockFrame();

	MeshDocument* document();

protected slots:
	virtual void update() {}

private:
	void init();

	MeshDocument* 	m_document;
};

#endif /* MESHFRAME_H_ */
