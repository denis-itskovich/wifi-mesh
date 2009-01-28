/**
 * \file MeshViewMessagesList.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 27/01/2009
 * \author Denis Itskovich
 */


#ifndef MESHVIEWMESSAGESLIST_H_
#define MESHVIEWMESSAGESLIST_H_

#include "MeshView.h"

class MeshViewMessagesList : public MeshView
{
	Q_OBJECT
public:
	MeshViewMessagesList(QWidget* parent = 0);

	void setDocument(MeshDocument* doc);

protected slots:
	void addMessage(const Message* pMessage);
	void clear();

private:
	void init();
	QTreeWidgetItem* createItem(const Message* pMessage);

	QTreeWidget*	m_messages;
};

#endif /* MESHVIEWMESSAGESLIST_H_ */
