#include "nextwindow.h"
#include "ui_nextwindow.h"
#include <iostream>

int nextwindow::counter = 0;

nextwindow::nextwindow(QWidget * parent) :
  QDialog(parent),
  ui(new Ui::nextwindow)
{
  ui->setupUi(this);
}

nextwindow::~nextwindow()
{
  delete ui;
}

void nextwindow::on_nextButton_clicked()
{
    std::cout << nextwindow::counter++ << std::endl;
}
