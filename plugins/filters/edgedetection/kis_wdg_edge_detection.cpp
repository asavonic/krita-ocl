/*
 * Copyright (c) 2017 Wolthera van Hövell tot Westerflier <griffinvalley@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "kis_wdg_edge_detection.h"

#include <filter/kis_filter_configuration.h>
#include <QComboBox>
#include <klocalizedstring.h>

KisWdgEdgeDetection::KisWdgEdgeDetection(QWidget *parent) :
    KisConfigWidget(parent),
    ui(new Ui_WidgetEdgeDetection)
{
    ui->setupUi(this);

    m_types << "prewitt"<< "sobol"<< "simple";
    m_types_translatable << i18n("Prewitt") << i18n("Sobol") << i18n("Simple");
    m_output << "pythagorean" << "xGrowth" << "xFall" << "yGrowth" << "yFall" << "radian";
    m_output_translatable << i18n("All sides")
                          << i18n("Top Edge")
                          << i18n("Bottom Edge")
                          << i18n("Right Edge")
                          << i18n("Left Edge")
                          << i18n("Direction in Radians");

    ui->cmbType->addItems(m_types_translatable);
    ui->cmbOutput->addItems(m_output_translatable);

    ui->btnAspect->setKeepAspectRatio(false);
    ui->sldHorizontalRadius->setRange(1.0, 100.0, 2);
    ui->sldHorizontalRadius->setPrefix(i18n("Horizontal Radius:"));
    connect(ui->sldHorizontalRadius, SIGNAL(valueChanged(qreal)), this, SLOT(horizontalRadiusChanged(qreal)));

    ui->sldVerticalRadius->setRange(1.0, 100.0, 2);
    ui->sldVerticalRadius->setPrefix(i18n("Verical Radius:"));
    connect(ui->sldVerticalRadius, SIGNAL(valueChanged(qreal)), this, SLOT(verticalRadiusChanged(qreal)));


    connect(ui->btnAspect, SIGNAL(keepAspectRatioChanged(bool)), this, SLOT(aspectLockChanged(bool)));
    connect(ui->cmbType, SIGNAL(currentIndexChanged(int)), this, SIGNAL(sigConfigurationItemChanged()));
    connect(ui->cmbOutput, SIGNAL(currentIndexChanged(int)), this, SIGNAL(sigConfigurationItemChanged()));
    connect(ui->sldHorizontalRadius, SIGNAL(valueChanged(qreal)), this, SIGNAL(sigConfigurationItemChanged()));
    connect(ui->sldVerticalRadius, SIGNAL(valueChanged(qreal)), this, SIGNAL(sigConfigurationItemChanged()));
    connect(ui->chkTransparent, SIGNAL(clicked()), this, SIGNAL(sigConfigurationItemChanged()));
}

KisWdgEdgeDetection::~KisWdgEdgeDetection()
{
    delete ui;
}

KisPropertiesConfigurationSP KisWdgEdgeDetection::configuration() const
{
    KisFilterConfigurationSP config = new KisFilterConfiguration("edge detection", 1);
    config->setProperty("horizRadius", ui->sldHorizontalRadius->value());
    config->setProperty("vertRadius", ui->sldVerticalRadius->value());
    config->setProperty("type", m_types.at(ui->cmbType->currentIndex()));
    config->setProperty("output", m_output.at(ui->cmbOutput->currentIndex()));
    config->setProperty("lockAspect", ui->btnAspect->keepAspectRatio());
    config->setProperty("transparency", ui->chkTransparent->isChecked());

    return config;
}

void KisWdgEdgeDetection::setConfiguration(const KisPropertiesConfigurationSP config)
{
    ui->sldHorizontalRadius->setValue(config->getFloat("horizRadius", 1.0));
    ui->sldVerticalRadius->setValue(config->getFloat("vertRadius", 1.0));
    int index = 0;
    if (m_types.contains(config->getString("type", "prewitt"))){
        index = m_types.indexOf(config->getString("type", "prewitt"));
    }
    ui->cmbType->setCurrentIndex(index);
    index = 0;
    if (m_output.contains(config->getString("output", "pythagorean"))){
        index = m_output.indexOf(config->getString("output", "pythagorean"));
    }
    ui->cmbOutput->setCurrentIndex(index);
    ui->chkTransparent->setChecked(config->getBool("transparency", false));
    ui->btnAspect->setKeepAspectRatio(config->getBool("lockAspect", false));

}

void KisWdgEdgeDetection::horizontalRadiusChanged(qreal r)
{
    ui->sldHorizontalRadius->blockSignals(true);
    ui->sldHorizontalRadius->setValue(r);
    ui->sldHorizontalRadius->blockSignals(false);

    if (ui->btnAspect->keepAspectRatio()) {
        ui->sldVerticalRadius->blockSignals(true);
        ui->sldVerticalRadius->setValue(r);
        ui->sldVerticalRadius->blockSignals(false);
    }
}

void KisWdgEdgeDetection::verticalRadiusChanged(qreal r)
{
    ui->sldVerticalRadius->blockSignals(true);
    ui->sldVerticalRadius->setValue(r);
    ui->sldVerticalRadius->blockSignals(false);

    if (ui->btnAspect->keepAspectRatio()) {
        ui->sldHorizontalRadius->blockSignals(true);
        ui->sldHorizontalRadius->setValue(r);
        ui->sldHorizontalRadius->blockSignals(false);
    }
}

void KisWdgEdgeDetection::aspectLockChanged(bool v)
{
    if (v) {
        ui->sldVerticalRadius->setValue( ui->sldHorizontalRadius->value() );
    }
}
