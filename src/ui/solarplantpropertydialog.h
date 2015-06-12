#ifndef SOLARPLANTPROPERTYDIALOG_H
#define SOLARPLANTPROPERTYDIALOG_H

#include <QDialog>
#include "datastructure/solarplantproperties.h"

namespace Ui {
class SolarPlantPropertyDialog;
}

class SolarPlantPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SolarPlantPropertyDialog(SolarPlantProperties spp, QWidget *parent = 0);
    ~SolarPlantPropertyDialog();

    SolarPlantProperties getSolarPlantProperties();

private:
    Ui::SolarPlantPropertyDialog *ui;
};

#endif // SOLARPLANTPROPERTYDIALOG_H
