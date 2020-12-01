/*#ifndef SMO_BMS2_H
#define SMO_BMS2_H

#include "Request.h"
#include "QThread"

class BMS2: public QThread
{
  Q_OBJECT
public:
  explicit BMS2(QObject * parent = nullptr);
  virtual ~BMS2();
  void run() override;
  static bool flagNext;

private:
  static Request getFromBuffer();
  static void requestService();

};

#endif
*/
