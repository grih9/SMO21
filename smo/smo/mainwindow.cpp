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

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_stepButton_clicked()
{
  nextWindow = new nextwindow();
  nextWindow->show();
  this->close();
}
