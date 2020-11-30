#ifndef NEXTWINDOW_H
#define NEXTWINDOW_H

#include <QDialog>

namespace Ui {
  class nextwindow;
}

class nextwindow : public QDialog
{
  Q_OBJECT

public:
  explicit nextwindow(QWidget * parent = nullptr);
  ~nextwindow();
  static int counter;

private slots:
  void on_nextButton_clicked();

private:
  Ui::nextwindow * ui;

};

#endif
