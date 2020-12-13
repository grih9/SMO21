#include "nextwindow.h"
#include "ui_nextwindow.h"
#include <iostream>
#include <BMS.h>
#include <QScrollBar>
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
  connect(programm, &BMS::stringSend, this, &nextwindow::setMessage);
  connect(programm, &BMS::timeSend, this, &nextwindow::printTime);
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
  ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
  QString text = ui->textBrowser->toPlainText();
  if (!text.isEmpty()) {
    text += "\n";
  }
  ui->textBrowser->setText(text + tmp);
  ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
}

void nextwindow::printTime(const QString & tmp)
{
  ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
  std::string a = std::to_string(counter);
  QString count = a.c_str();
  count.append(")------------------------------------------------------------\n");
  QString text = ui->textBrowser->toPlainText();
  if (!text.isEmpty()) {
    text += "\n";
  }
  ui->textBrowser->setText(text + count + tmp);
  ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
}

void nextwindow::on_retButton_clicked()
{
  emit ret();
  BMS::flagEnd = true;
  counter = 0;
  this->close();
}
