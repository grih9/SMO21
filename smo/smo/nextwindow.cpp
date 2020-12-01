#include "nextwindow.h"
#include "ui_nextwindow.h"
#include <iostream>
#include <BMS.h>

int nextwindow::counter = 0;

nextwindow::nextwindow(QWidget * parent) :
  QDialog(parent),
  ui(new Ui::nextwindow)
{
  BMS * programm = new BMS();
  programm->moveToThread(&programmThread);
  std::string a = std::to_string(counter);
  QString tmp = a.c_str();
  ui->setupUi(this);
  ui->textBrowser->setText(tmp);
  connect(this, &nextwindow::next, programm, &BMS::setTrue);
  connect(this, &nextwindow::operate, programm, &BMS::doWork);
  programmThread.start();
  emit operate("1");
}

nextwindow::~nextwindow()
{
  programmThread.quit();
  delete ui;
}

void nextwindow::on_nextButton_clicked()
{
  counter = counter + 1;
  BMS::flagNext = true;
  std::string a = std::to_string(counter);
  QString tmp = a.c_str();
  ui->textBrowser->setText(tmp);
  emit next();
}
