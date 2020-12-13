#ifndef AUTOMODE_H
#define AUTOMODE_H

#include <QFrame>
#include <QLineSeries>

namespace Ui {
class autoMode;
}

class autoMode : public QFrame
{
  Q_OBJECT

public:
  explicit autoMode(QWidget *parent = nullptr);
  ~autoMode();

private slots:
  void on_backButton_clicked();
  void addPotk(int, QtCharts::QLineSeries *);
  void addUsageKoef(int, QtCharts::QLineSeries *, QtCharts::QLineSeries *);
  void addtbp(int, QtCharts::QLineSeries *, QtCharts::QLineSeries *);
  void addtobsl(int, QtCharts::QLineSeries *, QtCharts::QLineSeries *);

signals:
  void ret();

private:
  Ui::autoMode *ui;
};

#endif // AUTOMODE_H
