/**
 * \file MeshViewSniffer.h
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

class MeshViewSniffer : public MeshView
{
	Q_OBJECT
public:
	MeshViewSniffer(QWidget* parent = 0);

	void setDocument(MeshDocument* doc);

protected slots:
	void addMessage(const Message* pMessage);
	void clear();

private:
	void init();
	static QString stationId(StationId id);
	QTreeWidgetItem* createItem(const Message* pMessage);

	QTreeWidget*	m_messages;
};

#endif /* MESHVIEWMESSAGESLIST_H_ */
