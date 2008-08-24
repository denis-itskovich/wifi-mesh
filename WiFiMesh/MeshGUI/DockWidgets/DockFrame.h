/*
 * .h
 *
 *  Created on: 21/08/2008
 *      Author: denis
 */

#ifndef MESHFRAME_H_
#define MESHFRAME_H_

#include <QtGui>
#include "../CoreWrappers/MeshStation.h"
#include "../Document/MeshDocument.h"

class DockFrame : public QFrame
{
public:
	void setDocument(MeshDocument* doc);

protected:
	DockFrame(QWidget* parent = 0);
	virtual ~DockFrame();

	void refresh() {};
	MeshDocument* document();

private:
	void init();

	MeshDocument* 	m_document;
};

#endif /* MESHFRAME_H_ */
