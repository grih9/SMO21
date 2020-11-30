#include "mainwindow.h"
#include <QStyleFactory>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent):
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->startButton->setStyle(QStyleFactory::create("Fusion"));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_startButton_clicked()
{
  nextWindow = new nextwindow();
  nextWindow->show();
  this->close();
}
