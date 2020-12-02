#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "properties.h"
#include <QMessageBox>

settingsWindow::settingsWindow(QWidget *parent) :
  QGroupBox(parent),
  ui(new Ui::settingsWindow)
{
  ui->setupUi(this);
  ui->acceptButton->setEnabled(false);
  ui->devicesNum->setValue(Properties::devicesNum);
  ui->sourcesNum->setValue(Properties::sourcesNum);
  ui->lambdaDevice->setValue(Properties::lambdaDevices);
  ui->lambdaSource->setValue(Properties::lambdaSources);
  ui->bufferSize->setValue(Properties::bufferCapacity);
  ui->genNum->setValue(Properties::requestsNum);
  for (int i = 0; i < 6; i++) {
    buttonVisionFlag[i] = false;
  }
  checkbutton();
}

settingsWindow::~settingsWindow()
{
  delete ui;
}


void settingsWindow::on_acceptButton_clicked()
{
  Properties::bufferCapacity = ui->bufferSize->value();
  Properties::devicesNum = ui->devicesNum->value();
  Properties::sourcesNum = ui->sourcesNum->value();
  Properties::lambdaDevices = ui->lambdaDevice->value();
  Properties::lambdaSources = ui->lambdaSource->value();
  Properties::requestsNum = ui->genNum->value();
  for (int i = 0; i < 6; i++) {
    buttonVisionFlag[i] = false;
  }
  checkbutton();

  QMessageBox msgBox;
  msgBox.setWindowTitle("Изменение настроек");
  msgBox.setText("Новые настройки успешно установлены");
  msgBox.exec();
}


void settingsWindow::on_backButton_clicked()
{
  checkbutton();
  emit ret();
  this->close();
}

void settingsWindow::checkbutton()
{
  if (!buttonVisionFlag[0] &&
      !buttonVisionFlag[1] &&
      !buttonVisionFlag[2] &&
      !buttonVisionFlag[3] &&
      !buttonVisionFlag[4] &&
      !buttonVisionFlag[5]) {
    ui->acceptButton->setEnabled(false);
  } else {
    ui->acceptButton->setEnabled(true);
  }
}

void settingsWindow::on_lambdaDevice_valueChanged(double arg1)
{
  if (arg1 != Properties::lambdaDevices) {
    buttonVisionFlag[3] = true;
  } else {
    buttonVisionFlag[3] = false;
  }
  checkbutton();
}

void settingsWindow::on_devicesNum_valueChanged(int arg1)
{
  if (arg1 != Properties::devicesNum) {
    buttonVisionFlag[2] = true;
  } else {
    buttonVisionFlag[2] = false;
  }
  checkbutton();
}

void settingsWindow::on_sourcesNum_valueChanged(int arg1)
{
  if (arg1 != Properties::sourcesNum) {
    buttonVisionFlag[0] = true;
  } else {
    buttonVisionFlag[0] = false;
  }
  checkbutton();
}

void settingsWindow::on_lambdaSource_valueChanged(double arg1)
{
  if (arg1 != Properties::lambdaSources) {
    buttonVisionFlag[1] = true;
  } else {
    buttonVisionFlag[1] = false;
  }
  checkbutton();
}

void settingsWindow::on_bufferSize_valueChanged(int arg1)
{
  if (arg1 != Properties::bufferCapacity) {
    buttonVisionFlag[4] = true;
  } else {
    buttonVisionFlag[4] = false;
  }
  checkbutton();
}

void settingsWindow::on_genNum_valueChanged(int arg1)
{
  if (arg1 != Properties::requestsNum) {
    buttonVisionFlag[5] = true;
  } else {
    buttonVisionFlag[5] = false;
  }
  checkbutton();
}

void settingsWindow::on_resetButton_clicked()
{
  ui->devicesNum->setValue(Properties::devicesNum);
  ui->sourcesNum->setValue(Properties::sourcesNum);
  ui->lambdaDevice->setValue(Properties::lambdaDevices);
  ui->lambdaSource->setValue(Properties::lambdaSources);
  ui->bufferSize->setValue(Properties::bufferCapacity);
  ui->genNum->setValue(Properties::requestsNum);
  for (int i = 0; i < 6; i++) {
    buttonVisionFlag[i] = false;
  }
  checkbutton();
}
