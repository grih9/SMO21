#ifndef BMS_H
#define BMS_H
#include "Request.h"
#include "QObject"


class BMS: public QObject
{
  Q_OBJECT
public:
  static bool flagNext;

public slots:
  void doWork();
  void setTrue();
};

#endif
