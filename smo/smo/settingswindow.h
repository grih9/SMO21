#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QGroupBox>

namespace Ui {
class settingsWindow;
}

class settingsWindow : public QGroupBox
{
  Q_OBJECT

public:
  explicit settingsWindow(QWidget *parent = nullptr);
  ~settingsWindow();

private slots:
  void on_acceptButton_clicked();
  void on_backButton_clicked();
  void on_lambdaDevice_valueChanged(double arg1);
  void on_devicesNum_valueChanged(int arg1);
  void on_sourcesNum_valueChanged(int arg1);
  void on_lambdaSource_valueChanged(double arg1);
  void on_bufferSize_valueChanged(int arg1);
  void on_genNum_valueChanged(int arg1);

  void on_resetButton_clicked();

signals:
  void ret();
private:
  void checkbutton();

  Ui::settingsWindow *ui;
  bool buttonVisionFlag[6];
};

#endif
