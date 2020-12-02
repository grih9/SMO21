#include "mainwindow.h"
#include <QStyleFactory>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent):
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->stepButton->setStyle(QStyleFactory::create("Fusion"));
  ui->autoButton->setStyle(QStyleFactory::create("Fusion"));
  ui->settingsButton->setStyle(QStyleFactory::create("Fusion"));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_stepButton_clicked()
{
  nextWindow = new nextwindow();
  connect(nextWindow, &nextwindow::ret, this, &MainWindow::show);
  nextWindow->show();
  this->hide();
}

void MainWindow::on_autoButton_clicked()
{
  nextWindow = new nextwindow();
  connect(nextWindow, &nextwindow::ret, this, &MainWindow::show);
  nextWindow->show();
  this->hide();
}

void MainWindow::on_settingsButton_clicked()
{
  settings = new settingsWindow();
  connect(settings, &settingsWindow::ret, this, &MainWindow::show);
  settings->show();
  this->hide();
}
