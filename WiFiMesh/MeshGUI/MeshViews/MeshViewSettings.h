/**
 * \file MeshViewSettings.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 11/01/2009
 * \author Denis Itskovich
 */

#ifndef DOCKSETTINGS_H_
#define DOCKSETTINGS_H_

#include "MeshView.h"

class MeshViewSettings : public MeshView
{
	Q_OBJECT
public:
	MeshViewSettings(QWidget* parent = 0);
	virtual ~MeshViewSettings();

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
