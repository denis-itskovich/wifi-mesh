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
	void updateSize(Size size);

private slots:
	void setDataRate(double dataRate);
	void setUnits(int units);
	void setHeight(int height);
	void setWidth(int width);

private:
	void init();
	int dataRate();

	QDoubleSpinBox*	m_spinCoverage;
	QDoubleSpinBox*	m_spinRouteTTL;
	QDoubleSpinBox*	m_spinDataRate;
	QDoubleSpinBox*	m_spinDuration;
	QComboBox*		m_comboDataUnits;
	QSpinBox*		m_spinHeight;
	QSpinBox*		m_spinWidth;
	int				m_dataRate;
};

#endif /* DOCKSETTINGS_H_ */
