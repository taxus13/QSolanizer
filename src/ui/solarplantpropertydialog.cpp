#include "solarplantpropertydialog.h"
#include "ui_solarplantpropertydialog.h"

SolarPlantPropertyDialog::SolarPlantPropertyDialog(SolarPlantProperties spp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolarPlantPropertyDialog)
{
    ui->setupUi(this);
    this->ui->sArea->setValue(spp.getArea());
    this->ui->sLatitude->setValue(spp.getLatitude());
    this->ui->sEfficiency->setValue(spp.getEfficiency());
    this->ui->sBeta->setValue(spp.getBeta());
    this->ui->sGamma->setValue(spp.getGamma());
    this->ui->sPeakPower->setValue(spp.getPeakPower());
}

SolarPlantPropertyDialog::~SolarPlantPropertyDialog()
{
    delete ui;
}

SolarPlantProperties SolarPlantPropertyDialog::getSolarPlantProperties()
{
    double area = this->ui->sArea->value();
    double latitude = this->ui->sLatitude->value();
    double efficiency = this->ui->sEfficiency->value();
    double beta = this->ui->sBeta->value();
    double gamma = this->ui->sGamma->value();

    double peakPower = this->ui->sPeakPower->value();

    return SolarPlantProperties(latitude, beta, gamma, area, efficiency, peakPower);
}
