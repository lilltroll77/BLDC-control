#include "mainwindow.h"

void MainWindow::initCurrentGauge(struct I_gauge_t* gauge , QWidget* parent){
// Widget
    gauge->widget = new QcGaugeWidget(parent);
    gauge->widget->setFixedSize(200,200);
// Background
    gauge->bkg = gauge->widget -> addBackground(97 , gauge->DegreeStart , gauge->DegreeEnd);
    gauge->bkg->clearrColors();
    gauge->bkg->addColor(0 , Qt::white);
    gauge->bkg_center = gauge->widget -> addBackground(5 , 0 , 360);
    gauge->bkg_center->clearrColors();
    gauge->bkg_center->addColor(0 , Qt::black);
// Labels
    gauge->label = gauge->widget->addLabel(20);
    gauge->label->setText(gauge->label_str);
    gauge->ms_needle_label = gauge->widget->addLabel(20);
    gauge->ms_needle_label->setAngle(90);
    gauge->ms_needle_label->setText("True RMS");

//Needles
    gauge->peak_needle_label = gauge->widget->addLabel(35);
    gauge->peak_needle_label-> setText("Peak");
    gauge->peak_needle_label->setColor(Qt::red);
    gauge->peak_needle_label->setAngle(90);

    gauge->ms_needle =   gauge->widget->addNeedle(85);
    gauge->peak_needle = gauge->widget->addNeedle(85);
    gauge->ms_needle->setColor(Qt::black);
    gauge->peak_needle->setColor(Qt::red);
    gauge->ms_needle->setNeedle(QcNeedleItem::TriangleNeedle);
    gauge->peak_needle->setNeedle(QcNeedleItem::TriangleNeedle);
    gauge->peak_needle->setDgereeRange(gauge->DegreeStart , gauge->DegreeEnd);
    gauge->ms_needle->setDgereeRange(gauge->DegreeStart , gauge->DegreeEnd);

// Degree Items
    gauge->DegreeItem =    gauge->widget->addDegrees(93);
    gauge->DegreeItem->    setDgereeRange(gauge->DegreeStart , gauge->DegreeEnd);
    gauge->DegreeItem->    setSubDegree(false);
    gauge->DegreeSubItem = gauge->widget->addDegrees(95);
    gauge->DegreeSubItem-> setDgereeRange(gauge->DegreeStart , gauge->DegreeEnd);
    gauge->DegreeSubItem-> setSubDegree(true);

//Values
    gauge->Values = gauge->widget->addValues(65);
    gauge->Values->setDgereeRange(gauge->DegreeStart , gauge->DegreeEnd);
    gauge->Values->setMinValue(0);

// Arc
    gauge->widget->addArc(97)->setDgereeRange(gauge->DegreeStart , gauge->DegreeEnd);

// Color Band
    gauge->ColorBand = gauge->widget->addColorBand(92);
    QList<QPair<QColor,float> > BandColors;
    for(int i=0; i< 4 ; i++){
        BandColors.append(gauge->pair[i]);
     }
    gauge->ColorBand->setColors(BandColors);
    gauge->ColorBand->setDgereeRange(gauge->DegreeStart , gauge->DegreeEnd);

}
