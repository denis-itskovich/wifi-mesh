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

class MeshView : public QWidget
{
	Q_OBJECT
public:
	virtual void setDocument(MeshDocument* document);

protected:
	MeshView(QWidget* parent = 0);
	virtual ~MeshView();

	MeshDocument* document() const;

protected slots:
	virtual void update() {}

private:
	void init();

	MeshDocument* 	m_document;
};

#endif /* MESHFRAME_H_ */
