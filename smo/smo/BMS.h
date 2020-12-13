#ifndef BMS_H
#define BMS_H
#include "Request.h"
#include "QObject"
#include "Device.h"
#include "Source.h"
#include "Buffer.h"
#include <QLineSeries>

class BMS: public QObject
{
  Q_OBJECT
public:
  static bool flagNext;
  static bool flagEnd;
public slots:
  void stepByStep();
  void autoMode();
  void setTrue();

signals:
  void stringSend(const QString &);
  void timeSend(const QString &);
  void potk(int, QtCharts::QLineSeries *);
  void usageKoef(int, QtCharts::QLineSeries *, QtCharts::QLineSeries *);
  void tbp(int, QtCharts::QLineSeries *, QtCharts::QLineSeries *);
  void tobsl(int, QtCharts::QLineSeries *, QtCharts::QLineSeries *);

private:
  void printinfo(const Device *, const Source *, const Buffer *);
};

#endif
