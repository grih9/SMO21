#include "nextwindow.h"
#include "ui_nextwindow.h"
#include <iostream>
#include <BMS.h>
#include <mainwindow.h>

int nextwindow::counter = 0;

nextwindow::nextwindow(QWidget * parent) :
  QDialog(parent),
  ui(new Ui::nextwindow)
{
  BMS * programm = new BMS();
  programm->moveToThread(&programmThread);
  ui->setupUi(this);
  connect(this, &nextwindow::next, programm, &BMS::setTrue);
  connect(this, &nextwindow::operate, programm, &BMS::doWork);
  connect(programm, &BMS::string, this, &nextwindow::setMessage);
  programmThread.start();

}

nextwindow::~nextwindow()
{
  programmThread.quit();
  delete ui;
}

void nextwindow::on_nextButton_clicked()
{
  if (counter == 0) {
    ui->nextButton->setText("СЛЕДУЮЩИЙ");
    counter = counter + 1;
    BMS::flagNext = false;
    emit operate("1");
  } else {
    counter = counter + 1;
    BMS::flagNext = true;
    emit next();
  }
}

void nextwindow::setMessage(const QString & tmp)
{
  ui->textBrowser->setText(tmp);
}

void nextwindow::on_retButton_clicked()
{
  emit ret();
  BMS::flagEnd = true;
  counter = 0;
  this->close();
}
