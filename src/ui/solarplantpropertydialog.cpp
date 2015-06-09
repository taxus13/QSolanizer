#include "solarplantpropertydialog.h"
#include "ui_solarplantpropertydialog.h"

SolarPlantPropertyDialog::SolarPlantPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolarPlantPropertyDialog)
{
    ui->setupUi(this);
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
