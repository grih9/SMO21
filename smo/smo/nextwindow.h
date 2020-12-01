#ifndef NEXTWINDOW_H
#define NEXTWINDOW_H

#include <QDialog>
#include <QThread>

namespace Ui {
  class nextwindow;
}

class nextwindow : public QDialog
{
  Q_OBJECT
  QThread programmThread;
public:
  explicit nextwindow(QWidget * parent = nullptr);
  ~nextwindow();
  static int counter;

private slots:
  void on_nextButton_clicked();
signals:
  void next();
  void operate(const QString &);

private:
  Ui::nextwindow * ui;

};

#endif
