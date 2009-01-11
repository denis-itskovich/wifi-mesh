/**
 * \file DockSettings.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 11/01/2009
 * \author Denis Itskovich
 */

#ifndef DOCKSETTINGS_H_
#define DOCKSETTINGS_H_

#include "DockFrame.h"
#include <QtGui>
#include <QtCore>

class DockSettings : public DockFrame
{
	Q_OBJECT
public:
	DockSettings(MeshDocument* doc = 0, QWidget* parent = 0);
	virtual ~DockSettings();

	virtual void setDocument(MeshDocument* doc);

signals:
	void updateDataRate(int dataRate);

private slots:
	void setDataRate(int dataRate);

private:
	void init();
	int dataRate() const;

	QDoubleSpinBox*	m_spinCoverage;
	QDoubleSpinBox*	m_spinRouteTTL;
	QSpinBox*		m_spinDataRate;
	QComboBox*		m_comboDataUnits;
};

#endif /* DOCKSETTINGS_H_ */
