#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "nextwindow.h"
#include "automode.h"
#include "settingswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
  class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget * parent = nullptr);
  ~MainWindow();


private slots:
  void on_stepButton_clicked();
  void on_settingsButton_clicked();
  void on_autoButton_clicked();
private:
  Ui::MainWindow * ui;
  settingsWindow * settings;
  nextwindow * nextWindow;
  autoMode * automode;
};

#endif
