#include "mainwindow.h"

void MainWindow::initTempGauge(struct Temp_gauge_t* gauge , QWidget* parent){
//widget
    gauge->widget = new QcGaugeWidget(parent);
    gauge->widget->setFixedSize(200,200);

//background
    gauge->bkg = gauge->widget -> addBackground(97 , 0 , 360);
    gauge->bkg->clearrColors();
    gauge->bkg->addColor(0 , Qt::white);
    gauge->bkg_center = gauge->widget -> addBackground(5 , 0 , 360);
    gauge->bkg_center->clearrColors();
    gauge->bkg_center->addColor(0 , Qt::black);

//labels
    gauge->label = gauge->widget->addLabel(65);
    gauge->label->setText(gauge->label_str);

//needle
    gauge->needle = gauge->widget->addNeedle(85);
    gauge->needle->setColor(Qt::red);

// Degree
    gauge->DegreeItem =    gauge->widget->addDegrees(93);
    gauge->DegreeSubItem = gauge->widget->addDegrees(95);

//Values
    gauge->Values = gauge->widget->addValues(65);
    gauge->Values->setMinValue(gauge->MinTemp);
    gauge->Values->setMaxValue(gauge->MaxTemp);
    gauge->widget->addArc(97)->setDgereeRange(0,360);

//Colorband
    gauge->ColorBand = gauge->widget->addColorBand(92);
    QList<QPair<QColor,float> > BandColors;
    for(int i=0; i< 4 ; i++){
      BandColors.append(gauge->pair[i]);
    }
    gauge->ColorBand->setColors(gauge->BandColors);

}
