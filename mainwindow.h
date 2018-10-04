#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include "qcgaugewidget.h"
#include <QMessageBox>
#include "knob.h"
#include "drv8320s.h"

enum COMMAND{LINK_DOWN , LINK_UP , COM_CURRENT , COM_STOP, COM_DRV, COM_DRV_ERROR , COM_DRV_RESET , COM_NEWTEMP , COM_FUSE_BLOWN , COM_FUSE_RESET , COM_FUSE_CURRENT , COM_VDS , COM_ODT , COM_TDRIVE , COM_IDRIVE_P_HS , COM_IDRIVE_N_HS , COM_IDRIVE_P_LS , COM_IDRIVE_N_LS , COM_SET_TORQUE , COM_SET_FLUX , COM_PI_FUSE , COM_PI_TORQUE_FREQ , COM_PI_TORQUE_GAIN , COM_PI_FLUX_FREQ , COM_PI_FLUX_GAIN};
void writeSerialCommand(char ID , QSerialPort* serial);
void writeSerialUint(char ID , unsigned value , QSerialPort* serial);
void writeSerialFloat(char ID , float value , QSerialPort* serial);

struct I_t{
    float peak;
    float ms; //Mean square
};

struct PI_t{
  Knob* freq;
  Knob* gain;
  QBoxLayout* layout;
  QGroupBox* box;
};

struct I_gauge_t{
    QcGaugeWidget* widget;
    QcBackgroundItem *bkg;
    QcBackgroundItem *bkg_center;
    QcNeedleItem* peak_needle;
    QcLabelItem* peak_needle_label;
    QcNeedleItem* ms_needle;
    QcLabelItem* ms_needle_label;
    QcValuesItem* Values;
    QcDegreesItem* DegreeSubItem;
    QcDegreesItem* DegreeItem;
    const int DegreeStart = 0;
    const int DegreeEnd =   180;
    QPair<QColor,float> pair[4]={{Qt::green , 15},{Qt::darkGreen , 65},{Qt::yellow , 80},{Qt::red , 100}};
    QcColorBand* ColorBand;
    QcLabelItem* label;
    const QString label_str = "CoilCurrent [A]";
};


struct Temp_gauge_t{
    QcGaugeWidget* widget;
    QcBackgroundItem *bkg;
    QcBackgroundItem *bkg_center;
    QcNeedleItem* needle;
    QcValuesItem* Values;
    QcDegreesItem* DegreeSubItem;
    QcDegreesItem* DegreeItem;
    QPair<QColor,float> pair[4]={{Qt::green , 15},{Qt::darkGreen , 75},{Qt::yellow , 80},{Qt::red , 100}};
    QList<QPair<QColor,float> > BandColors;
    QcColorBand* ColorBand;
    QcLabelItem* label;
    const QString label_str = "Temp. [C°]";
    const float MinTemp = 20;
    const float MaxTemp = 120;

};


struct fonts_t{
    QFont PI;
    QFont ResetButton;
    QFont FOC;
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void setTemp(float temp);
    void setCurrent(struct I_t* I);

    ~MainWindow();
public slots:
    void Tset_changed(double value);
    void Iset_changed(double value);
    void Fset_changed(double value);
    void Tfreq_changed(double value);
    void Tgain_changed(double value);
    void Ffreq_changed(double value);
    void Fgain_changed(double value);
    void fuse_reset(bool state);


private slots:
    void calc_currentGauge(float I);

private:

    void initCurrentGauge(struct I_gauge_t* gauge , QWidget* parent);
    void initTempGauge(struct Temp_gauge_t* gauge , QWidget* parent);
    Ui::MainWindow *ui;
    QGridLayout* top_layout;
    QGroupBox* top_box;
    DRV8320S* drv8320s;
    const float MaxCurrent = 30;
    QSerialPort* serial;
    QPushButton* button_reset;
    float Imax=25;
    const int error_bits[2]={10,8};
    struct fonts_t fonts;
    struct I_gauge_t    I_gauge; // Current gauge
    struct Temp_gauge_t T_gauge; // Temperature gauge

};

#endif // MAINWINDOW_H
