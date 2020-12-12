#include "automode.h"
#include "ui_automode.h"

autoMode::autoMode(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::autoMode)
{
  ui->setupUi(this);
}


void autoMode::on_backButton_clicked()
{
  emit ret();
  this->close();
}

autoMode::~autoMode()
{
  delete ui;
}
