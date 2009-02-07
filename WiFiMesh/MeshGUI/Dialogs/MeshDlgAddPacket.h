/**
 * \file MeshDlgAddPacket.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 07/02/2009
 * \author Denis Itskovich
 */


#ifndef MESHDLGADDPACKET_H_
#define MESHDLGADDPACKET_H_

#include <QtGui>
#include "../CoreWrappers/MeshCore.h"

class MeshDlgAddPacket : public QDialog
{
public:
	MeshDlgAddPacket(const QList<StationId>& destList, StationId src, QWidget* parent = 0);

	double time() const;
	StationId destination() const;
	unsigned long dataSize() const;

private:
	void init();

	QPushButton*	m_buttonAdd;
	QPushButton*	m_buttonCancel;

	QComboBox*		m_comboDestination;
	QDoubleSpinBox*	m_spinTime;
	QSpinBox*		m_spinSize;
};

#endif /* MESHDLGADDPACKET_H_ */
