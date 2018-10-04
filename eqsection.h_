#ifndef EQSECTION_H
#define EQSECTION_H

#include <complex>
#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <classes/knob.h>
#include "classes/qcustomplot.h"
#include "defines.h"
#include "calcfilt.h"
#include "UDPcommands.h"
#include "maintab.h"

//enum FilterType{LowShelf , HighSelf , PeakingEQ , Notch , AllPass , HighPass , LowPass , BandPass };



class EQSection : public QWidget
{
    Q_OBJECT
public:
    explicit EQSection(QWidget *parent = 0 , QCustomPlot *new_plot = 0 , Network *udp=0 , Knob *knob_linkedFc_ref=0 , int* fs=0);
    void setSectionID(int newID);
    void setChannelID(int newID);
    void setBoxTitle(const QString &title);
    void updateSettingsAndPlot(bool updatePlot, int new_fs);

    void updateLinked();
    bool getLinked();
    void setLinked(bool , bool);
    double getFc();
    void setFc(double , bool);
    double getQ();
    void setQ(double , bool);
    double getGain();
    void setGain(double , bool);
    filterType_t getFilterType();
    void setFilterType(filterType_t , bool);
    bool getFilterActive();
    void setFilterActive(bool , bool);

    double B[3];
    double A[2];
    std::complex<double> freq[PLOTSIZE];
    QCPItemTracer *eqTracer;

signals:
    void eqchanged();

private slots:
void slot_gainChanged(double gain);
void slot_Q_Changed(double Q);
void slot_fcChanged(double fc);
void slot_linkStatusChanged(bool state);
void slot_filtertypeChanged(int type);
void slot_activeEQChanged(bool state);

private:
 quint16 *port;
 int channelID;
 int sectionID;
 QVBoxLayout *topLayout;
 QVBoxLayout *layout;
 QCustomPlot *plot;
 QUdpSocket  *UDP_Socket;
 QHostAddress *IP_XMOS;
 quint16 *port_XMOS;
 QByteArray datagram;
 Knob *knob_linkedFc;
 Knob *knob_fc;
 Knob *knob_Q;
 Knob *knob_gain;
 QGroupBox *groupBox;
 QComboBox *filterType;
 QCheckBox *link;
 MainTab* main_tab;
 int* fs_ptr;
 int fs=FS;
};



#endif // EQSECTION_H
