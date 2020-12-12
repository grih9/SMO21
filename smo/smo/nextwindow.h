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
  void on_retButton_clicked();
  void setMessage(const QString &);
  void printTime(const QString &);
signals:
  void next();
  void ret();
  void operate(const QString &);


private:
  Ui::nextwindow * ui;

};

#endif
