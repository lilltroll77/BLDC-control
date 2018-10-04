#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QString>
#include <QFont>
#include <QTime>
#include <stdio.h>
#include <QDebug>
#include <QPushButton>
#include "drv8320s.h"
#include "qcgaugewidget.h"

#define HEADER_SIZE 4
enum DATA_TYPE{FIXED_POINT , FLOATING_POINT};

void writeSerialCommand(char ID , QSerialPort* serial){
    const int len = 4;
    char data[len]={len};
    data[1] = ID;
    serial->write(data , len);
}

void writeSerialUint(char ID , unsigned value , QSerialPort* serial){
    const int len = HEADER_SIZE + sizeof(float);
    char data[len]={len};
    data[1] = ID;
    data[2] = FIXED_POINT;
    reinterpret_cast<unsigned*>(data)[0] = value;
    serial->write(data , len);
}

void writeSerialFloat(char ID , float value , QSerialPort* serial){
    const int len = HEADER_SIZE + sizeof(float);
    char data[len]={len};
    data[1] = ID;
    data[2] = FLOATING_POINT;
    reinterpret_cast<float*>(data)[0] = value;
    serial->write(data , len);
}


void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle("FOC & SVPWM for BLDC with XMOS");
    //QWidget* central_widget=new QWidget(this);
    top_layout = new QGridLayout(this);
    top_box = new QGroupBox(this);


    serial = new QSerialPort("COM15" , this);
    serial->setReadBufferSize(512);
    if(!serial->open(QSerialPort::ReadWrite))
        serial->error();

    fonts.PI.setBold(false);
    fonts.PI.setPointSize(12);
    fonts.ResetButton.setBold(true);
    fonts.FOC.setBold(true);
    fonts.FOC.setPointSize(14);

// ****************** Gauges *******************************

    initCurrentGauge(&I_gauge , this);
    top_layout->addWidget(I_gauge.widget, 3, 3 ,1 ,1 , Qt::AlignBottom);

    initTempGauge(&T_gauge , this);
    top_layout->addWidget(T_gauge.widget, 4, 3 ,1 ,1 , Qt::AlignCenter);


    top_box->setLayout(top_layout);

    setCentralWidget(top_box);
    //this->setContentsMargins(5,10,5,10);
    //this->update();
    //drv8320s->update();
    this->show();

/*
    I_gauge.widget = new QcGaugeWidget(this);
    I_gauge.widget->setFixedSize(200,200);
    T_gauge.widget = new QcGaugeWidget(this);
    T_gauge.widget->setFixedSize(200,200);

    T_gauge.label = T_gauge.widget->addLabel(65);
    T_gauge.label->setText(T_gauge.label_str);

    I_gauge.bkg = I_gauge.widget -> addBackground(97 , I_gauge.DegreeStart , I_gauge.DegreeEnd);
    I_gauge.bkg->clearrColors();
    I_gauge.bkg->addColor(0 , Qt::white);
    I_gauge.bkg_center = I_gauge.widget -> addBackground(5 , 0 , 360);
    I_gauge.bkg_center->clearrColors();
    I_gauge.bkg_center->addColor(0 , Qt::black);

    T_gauge.bkg = T_gauge.widget -> addBackground(97 , 0 , 360);
    T_gauge.bkg->clearrColors();
    T_gauge.bkg->addColor(0 , Qt::white);
    T_gauge.bkg_center = I_gauge.widget -> addBackground(5 , 0 , 360);
    T_gauge.bkg_center->clearrColors();
    T_gauge.bkg_center->addColor(0 , Qt::black);

    I_gauge.label = I_gauge.widget->addLabel(20);
    I_gauge.label->setText(I_gauge.label_str);


    I_gauge.ms_needle_label = I_gauge.widget->addLabel(20);
    I_gauge.ms_needle_label->setAngle(90);
    I_gauge.ms_needle_label->setText("True RMS");

    I_gauge.peak_needle_label = I_gauge.widget->addLabel(35);
    I_gauge.peak_needle_label-> setText("Peak");
    I_gauge.peak_needle_label->setColor(Qt::red);
    I_gauge.peak_needle_label->setAngle(90);


    I_gauge.ms_needle =   I_gauge.widget->addNeedle(85);
    I_gauge.peak_needle = I_gauge.widget->addNeedle(85);
    I_gauge.ms_needle->setColor(Qt::black);
    I_gauge.peak_needle->setColor(Qt::red);
    I_gauge.ms_needle->setNeedle(QcNeedleItem::TriangleNeedle);
    I_gauge.peak_needle->setNeedle(QcNeedleItem::TriangleNeedle);
    I_gauge.peak_needle->setDgereeRange(I_gauge.DegreeStart , I_gauge.DegreeEnd);
    I_gauge.ms_needle->setDgereeRange(I_gauge.DegreeStart , I_gauge.DegreeEnd);
    //I_gauge.ms_needle->setLabel(I_gauge.ms_needle_label);
    //I_gauge.peak_needle->setLabel(I_gauge.peak_needle_label);

    T_gauge.needle = T_gauge.widget->addNeedle(85);
    T_gauge.needle->setColor(Qt::red);

    I_gauge.DegreeItem =    I_gauge.widget->addDegrees(93);
    I_gauge.DegreeItem->    setDgereeRange(I_gauge.DegreeStart , I_gauge.DegreeEnd);
    I_gauge.DegreeItem->    setSubDegree(false);
    I_gauge.DegreeSubItem = I_gauge.widget->addDegrees(95);
    I_gauge.DegreeSubItem-> setDgereeRange(I_gauge.DegreeStart , I_gauge.DegreeEnd);
    I_gauge.DegreeSubItem-> setSubDegree(true);
    T_gauge.DegreeItem =    T_gauge.widget->addDegrees(93);
    T_gauge.DegreeSubItem = T_gauge.widget->addDegrees(95);

    T_gauge.Values = T_gauge.widget->addValues(65);
    T_gauge.Values->setMinValue(T_gauge.MinTemp);
    T_gauge.Values->setMaxValue(T_gauge.MaxTemp);
    I_gauge.Values = I_gauge.widget->addValues(65);
    I_gauge.Values->setDgereeRange(I_gauge.DegreeStart , I_gauge.DegreeEnd);
    I_gauge.Values->setMinValue(0);

    I_gauge.widget->addArc(97)->setDgereeRange(I_gauge.DegreeStart , I_gauge.DegreeEnd);
    T_gauge.widget->addArc(97)->setDgereeRange(0,360);

    for(int i=0; i< 4 ; i++){
        I_gauge.BandColors.append(I_gauge.pair[i]);
        T_gauge.BandColors.append(T_gauge.pair[i]);
    }

    T_gauge.ColorBand = T_gauge.widget->addColorBand(92);
    I_gauge.ColorBand = I_gauge.widget->addColorBand(92);
    I_gauge.ColorBand->setColors(I_gauge.BandColors);
    T_gauge.ColorBand->setColors(T_gauge.BandColors);
    I_gauge.ColorBand->setDgereeRange(I_gauge.DegreeStart , I_gauge.DegreeEnd);



    T_gauge.widget->addGlass(100);
    calc_currentGauge(Imax);
    */
    //I_gauge.widget->update();



    // ****************** DRV8320S *******************************

        DRV8320S* drv8320s = new DRV8320S(this , serial);
        top_layout->addWidget(drv8320s->masterBox , 1 , 1 ,1 ,3);
        struct PI_t T[2];

    Knob* T_set = new Knob(linScale , this);
    Knob* F_set = new Knob(linScale , this);
    Knob* I_set = new Knob(linScale , this);
    QBoxLayout* layout_TF =new QBoxLayout(QBoxLayout::LeftToRight);
    QGroupBox* box_TF = new QGroupBox;


    T_set->setTitle("Torque [%]");
    F_set->setTitle("Flux [%]");
    I_set->setTitle("Active fuse\nMax current [A]");

    T_set->setValue(0);
    F_set->setValue(0);
    I_set->setValue(Imax);

    T_set->setRange(-100 , 100 , 100);
    I_set->setRange(0.5 , Imax , 100);
    F_set->setRange(-100 , 100 , 100);

    F_set->setDecimals(1);
    T_set->setDecimals(1);
    I_set->setDecimals(1);

    T_set->setSingleStep(1);
    F_set->setSingleStep(1);
    I_set->setSingleStep(0.1);


    //F_set->setFixedHeight(250);
    //F_set->setFixedWidth(250);
    //T_set->setFixedHeight(250);
    //T_set->setFixedWidth(250);
/*
    T_set->setKnobColor("rgb(127, 255, 127)");
    F_set->setKnobColor("rgb(63, 127, 63)");
    I_set->setKnobColor("rgb(255, 31, 31)");

    button_reset = new QPushButton("FUSE",this);
    button_reset->setFont(fonts.ResetButton);
    button_reset->setStyleSheet("color: rgb(31, 127, 31)");
    button_reset->setFixedWidth(60);
    layout_TF->addWidget(T_set);
    layout_TF->addWidget(F_set);
    layout_TF->addWidget(I_set);
    layout_TF->addWidget(button_reset);

    box_TF->setLayout(layout_TF);
    box_TF->setTitle("Set point");
    box_TF->setFont(fonts.PI);
    top_layout->addWidget(box_TF, 2 , 1 ,1 ,3);

    connect(T_set , SIGNAL(valueChanged(double)) , this ,   SLOT(Tset_changed(double)));
    connect(F_set , SIGNAL(valueChanged(double)) , this ,   SLOT(Fset_changed(double)));
    connect(I_set , SIGNAL(valueChanged(double)) , this ,   SLOT(Iset_changed(double)));
    connect(button_reset , SIGNAL(clicked(bool) ), this , SLOT(fuse_reset(bool)));

    for(int i=0; i<2 ; i++){
        T[i].freq = new Knob(logScale , this);
        T[i].gain = new Knob(linScale , this);
        T[i].layout =  new QBoxLayout(QBoxLayout::LeftToRight , this);
        T[i].box = new(QGroupBox);

        T[i].freq->setRange(2 , 5000 , 100);
        T[i].freq->setDecimals(1);
        T[i].freq->setTitle("Frequency [Hz]");
        T[i].freq->setValue(300);

        //T[i].freq->setSingleStep(0.1);

        T[i].gain->setRange(-60 , 0 , 60);
        T[i].gain->setTitle("Gain [dB]");
        T[i].gain->setSingleStep(0.5);
        T[i].gain->setDecimals(1);
        T[i].gain->setValue(-60);

        T[i].gain->setKnobColor(QString("rgb(255, 127, %1)").arg(128*i));
        T[i].freq->setKnobColor(QString("rgb(%1, 127, 255)").arg(128*i));

       /* T[i].freq->setFixedHeight(175);
        T[i].freq->setFixedWidth(175);
        T[i].gain->setFixedHeight(175);
        T[i].gain->setFixedWidth(175);

        T[i].layout->addWidget(T[i].freq , Qt::AlignCenter);
        T[i].layout->addWidget(T[i].gain , Qt::AlignCenter);
        T[i].box->setLayout(T[i].layout);
        T[i].box->setFont(fonts.PI);
        top_layout->addWidget(T[i].box ,3+i,1 ,1 , 2);


    }
    connect(T[0].freq , SIGNAL(valueChanged(double)) , this ,   SLOT(Tfreq_changed(double)));
    connect(T[1].freq , SIGNAL(valueChanged(double)) , this ,   SLOT(Ffreq_changed(double)));
    connect(T[0].gain , SIGNAL(valueChanged(double)) , this ,   SLOT(Tgain_changed(double)));
    connect(T[1].gain , SIGNAL(valueChanged(double)) , this ,   SLOT(Fgain_changed(double)));


 T[0].box->setTitle("PI Torque control");
 T[0].box->setFixedWidth(340);
 T[1].box->setTitle("PI Flux control");
 T[1].box->setFixedWidth(340);
top_layout->setSpacing(3);




QGroupBox* box=new QGroupBox(this);
box->setLayout(top_layout);
box->setTitle("FOC");



box->setFont(fonts.FOC);
box->setFixedWidth(600);
box->setFixedHeight(950);
//box->set ContentsMargins(5,5,5,5);
 setCentralWidget(box);
 this->setContentsMargins(5,10,5,10);
 this->update();
 drv8320s->update();
 this->show();

 char serial_data[64];

 writeSerialCommand(LINK_UP, serial );
 qDebug()<<"Link UP sent";
struct I_t* I;*/
 /*while(1){
     delay(1);
     while(serial->bytesAvailable()>= HEADER_SIZE){
         serial->read(serial_data , HEADER_SIZE);
         int len = serial_data[0] - HEADER_SIZE;
         qDebug() <<len;
         int command = serial_data[1];
         float* float_vec;
         if(len>0){
             while( serial->bytesAvailable() < len)
                 delay(1);
             serial->read(serial_data , len);
             float_vec = reinterpret_cast<float*>(serial_data);
             I = reinterpret_cast<struct I_t*>(serial_data);


         }
         switch(command){
         case(COM_DRV):{
             qDebug()<<"DRV reg:";
             for(int i=0; i<=5; i++)
                 qDebug()<< i<<": " << reinterpret_cast<unsigned short*>(serial_data)[i];
             drv8320s->set_GateDriveNegHiSide(  reinterpret_cast<unsigned short*>(serial_data)[3]    & 0xF);
             drv8320s->set_GateDrivePosHiSide( (reinterpret_cast<unsigned short*>(serial_data)[3]>>4)& 0xF);
             drv8320s->set_GateDriveNegLoSide(  reinterpret_cast<unsigned short*>(serial_data)[4]    & 0xF);
             drv8320s->set_GateDrivePosLoSide( (reinterpret_cast<unsigned short*>(serial_data)[4]>>4)& 0xF);
             drv8320s->set_TDrive(             (reinterpret_cast<unsigned short*>(serial_data)[4]>>8)& 0x3);
             drv8320s->set_VDS_LVL(             reinterpret_cast<unsigned short*>(serial_data)[5]    & 0xF);
             unsigned short reg_val;

             for(int reg=0; reg<2 ; reg++){
                 reg_val=reinterpret_cast<unsigned short*>(serial_data)[reg];
                 for(int bit=0; bit < error_bits[reg] ; bit++)
                     drv8320s->set_status(reg , bit, (reg_val>>bit)==1);
             }
             break;
         }
         case(COM_DRV_ERROR):{
             unsigned short reg_val;
             for(int reg=0; reg<2 ; reg++){
                 reg_val=reinterpret_cast<unsigned short*>(serial_data)[reg];
                 for(int bit=0; bit<error_bits[reg] ; bit++)
                     drv8320s->set_status(reg , bit, (reg_val>>bit)==1);
             }
             break;
         }
         case(COM_STOP):{
             qDebug()<<"STOPPED!";
             break;
         }
         case(COM_NEWTEMP):{
             setTemp(float_vec[0]);
             qDebug()<< "Temp=" << float_vec[0]<< " C" << reinterpret_cast<unsigned*>(serial_data)[0];
             break;
         }
         case(COM_FUSE_BLOWN):{
             qDebug()<<"FUSE BLOWN";
             button_reset->setText("RESET");
             button_reset->setStyleSheet("color: rgb(255, 31, 31)");
             fonts.ResetButton.setBold(true);
             button_reset->setFont(fonts.ResetButton);
             break;
         }
         case(COM_FUSE_CURRENT):{
             bool blocked = I_set->signalsBlocked();
             I_set->blockSignals(true);
             I_set->setValue(static_cast<double>(float_vec[0]));
             I_set->blockSignals(blocked);
             break;
         }
         case(COM_CURRENT):{
             setCurrent(I);
             break;
         }
         default:
             qDebug()<<"Unknown COM command";
             break;
         }
     }
 }
*/

}

void MainWindow::calc_currentGauge(float I){
    Imax = I;
    I *=1.25f;
    I_gauge.Values->setMaxValue(I);
    if(I>10){
        I_gauge.Values->setStep(5);
        I_gauge.DegreeSubItem->setStep(100/I);
        I_gauge.DegreeItem->setStep(500/I);
    }else if(I<1){
        I_gauge.Values->setStep(0.1f);
        I_gauge.DegreeSubItem->setStep(2/I);
        I_gauge.DegreeItem->setStep(10/I);
   }else if(I<3){
        I_gauge.Values->setStep(0.5f);
        I_gauge.DegreeSubItem->setStep(10/I);
        I_gauge.DegreeItem->setStep(50/I);
    }else{
        I_gauge.Values->setStep(1);
        I_gauge.DegreeSubItem->setStep(20/I);
        I_gauge.DegreeItem->setStep(100/I);
    }
}

 //SLOTS
void MainWindow::Tset_changed(double value){
    writeSerialFloat(COM_SET_TORQUE , value , serial);
};

void MainWindow::Iset_changed(double value){
    calc_currentGauge((float)value);
    writeSerialFloat(COM_FUSE_CURRENT , value , serial);
};
 void MainWindow::Fset_changed(double value){
    writeSerialFloat(COM_SET_FLUX , value , serial);
 };
 void MainWindow::Tfreq_changed(double value){
    writeSerialFloat(COM_PI_TORQUE_FREQ , value , serial);
 };
 void MainWindow::Tgain_changed(double value){
    writeSerialFloat(COM_PI_TORQUE_GAIN , value , serial);
 };
 void MainWindow::Ffreq_changed(double value){
    writeSerialFloat(COM_PI_FLUX_FREQ , value , serial);
 };
 void MainWindow::Fgain_changed(double value){
    writeSerialFloat(COM_PI_FLUX_GAIN , value , serial);
 };

 void MainWindow::setTemp(float temp){
     T_gauge.needle->setCurrentValue((temp-T_gauge.MinTemp) * (100/(T_gauge.MaxTemp-T_gauge.MinTemp)));
 }

 void MainWindow::setCurrent(struct I_t *I){
     I_gauge.peak_needle->setCurrentValue(I->peak * (100/(1.25f*Imax)));
     I_gauge.ms_needle->setCurrentValue(sqrtf(I->ms) * (100/(1.25f*Imax)));
 }

 void MainWindow::fuse_reset(bool state){
     button_reset->setText("FUSE");
     fonts.ResetButton.setBold(false);
     button_reset->setFont(fonts.ResetButton);
     button_reset->setStyleSheet("color: rgb(31, 127, 31)");
     button_reset->update();
     writeSerialCommand(COM_FUSE_RESET , serial);
 }

MainWindow::~MainWindow()
{
    char serial_data[4];
    serial_data[0]=2;
    serial_data[1]=-2;
    serial->write(serial_data , 4);
    serial->close();
    delete ui;
}

