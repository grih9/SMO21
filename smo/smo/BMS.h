#ifndef BMS_H
#define BMS_H
#include "Request.h"
#include "QObject"
#include "Device.h"
#include "Source.h"
#include "Buffer.h"

class BMS: public QObject
{
  Q_OBJECT
public:
  static bool flagNext;
  static bool flagEnd;
public slots:
  void doWork();
  void setTrue();
signals:
  void stringSend(const QString &);
  void timeSend(const QString &);
private:
  void printinfo(const Device *, const Source *, const Buffer *);
};

#endif
