#include "comport.h"


COMport::COMport(QString str)
{
    serial = new QSerialPort(str);
    serial->setReadBufferSize(512);
    if(!serial->open(QSerialPort::ReadWrite))
        serial->error();
}

void COMport::sendSerial(short* data , int len){
    serial->write(reinterpret_cast<const char*>(data) , 2*len);
}
