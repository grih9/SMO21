#include "automode.h"
#include "ui_automode.h"
#include <QLogValueAxis>
#include <QLineSeries>
#include <QValueAxis>
#include <QChart>
#include <QChartView>
#include <math.h>
#include "BMS.h"

using namespace QtCharts;

autoMode::autoMode(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::autoMode)
{
  ui->setupUi(this);


  BMS * programm = new BMS();
  connect(programm, &BMS::potk, this, &autoMode::addPotk);
  connect(programm, &BMS::usageKoef, this, &autoMode::addUsageKoef);
  connect(programm, &BMS::tbp, this, &autoMode::addtbp);
  connect(programm, &BMS::tobsl, this, &autoMode::addtobsl);
  QString str("requestNumber = ");
  std::string a = std::to_string(Properties::requestsNum);
  QString t = a.c_str();
  str.append(t);
  str.append(";   sourcesNum = ");
  a = std::to_string(Properties::sourcesNum);
  t = a.c_str();
  str.append(t);
  str.append(";   devicesNum = ");
  a = std::to_string(Properties::devicesNum);
  t = a.c_str();
  str.append(t);
  str.append(";   bufferCapacity = ");
  a = std::to_string(Properties::bufferCapacity);
  t = a.c_str();
  str.append(t);
  str.append(";   lambdaSources = ");
  a = std::to_string(Properties::lambdaSources);
  t = a.c_str();
  str.append(t);
  str.append(";   lambdaDevices = ");
  a = std::to_string(Properties::lambdaDevices);
  t = a.c_str();
  str.append(t);
  ui->infoLabel->setText(str);
  programm->autoMode();

}

void autoMode::addPotk(int i, QtCharts::QLineSeries * lineSeries)
{
  QChartView *chartView = new QChartView(this);
  ui->pOtk->addWidget(chartView);


  QChart *chart = new QChart();
  chart->addSeries(lineSeries);
  chart->legend()->hide();
  chart->setTitle("");

  QValueAxis *axisX = new QValueAxis();
  if (i == 0) {
    axisX->setTitleText("sources");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(7);
  } else if (i == 1) {
    axisX->setTitleText("devices");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(7);
  } else if (i == 2) {
    axisX->setTitleText("bufferCap");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(10);
  } else if (i == 3) {
    axisX->setTitleText("lambdaSource");
    axisX->setLabelFormat("%g");
    axisX->setTickCount(3);
  } else if (i == 4) {
    axisX->setTitleText("lambdaDevices");
    axisX->setLabelFormat("%g");
    axisX->setTickCount(3);
  }

  chart->addAxis(axisX, Qt::AlignBottom);
  lineSeries->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis();

  axisY->setTitleText("potk, %");
  axisY->setLabelFormat("%g");
  axisY->setTickCount(5);

  chart->addAxis(axisY, Qt::AlignLeft);
  lineSeries->attachAxis(axisY);

  chartView->setChart(chart);
}

void autoMode::addUsageKoef(int i, QtCharts::QLineSeries * lineSeries1, QtCharts::QLineSeries * lineSeriesavg)
{
  QChartView *chartView = new QChartView(this);
  ui->usageKoef->addWidget(chartView);

  QChart *chart = new QChart();
  chart->addSeries(lineSeries1);
  chart->addSeries(lineSeriesavg);
  chart->legend()->hide();
  chart->setTitle("");

  QValueAxis *axisX = new QValueAxis();
  if (i == 0) {
    axisX->setTitleText("sources");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(7);
  } else if (i == 1) {
    axisX->setTitleText("devices");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(7);
  } else if (i == 2) {
    axisX->setTitleText("bufferCap");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(10);
  } else if (i == 3) {
    axisX->setTitleText("lambdaSource");
    axisX->setLabelFormat("%g");
    axisX->setTickCount(3);
  } else if (i == 4) {
    axisX->setTitleText("lambdaDevices");
    axisX->setLabelFormat("%g");
    axisX->setTickCount(3);
  }

  chart->addAxis(axisX, Qt::AlignBottom);
  lineSeries1->attachAxis(axisX);
  lineSeriesavg->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis();

  axisY->setTitleText("usagekoef, %");
  axisY->setLabelFormat("%g");
  axisY->setTickCount(5);

  chart->addAxis(axisY, Qt::AlignLeft);
  lineSeries1->attachAxis(axisY);
  lineSeriesavg->attachAxis(axisY);

  chartView->setChart(chart);
}


void autoMode::addtbp(int i, QtCharts::QLineSeries * lineSeries1, QtCharts::QLineSeries * lineSeries2)
{
  QChartView *chartView = new QChartView(this);
  ui->tbpdisp->addWidget(chartView);

  QChart *chart = new QChart();
  chart->addSeries(lineSeries1);
  chart->addSeries(lineSeries2);
  chart->legend()->hide();
  chart->setTitle("");

  QValueAxis *axisX = new QValueAxis();
  if (i == 0) {
    axisX->setTitleText("sources");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(7);
  } else if (i == 1) {
    axisX->setTitleText("devices");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(7);
  } else if (i == 2) {
    axisX->setTitleText("bufferCap");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(10);
  } else if (i == 3) {
    axisX->setTitleText("lambdaSource");
    axisX->setLabelFormat("%g");
    axisX->setTickCount(3);
  } else if (i == 4) {
    axisX->setTitleText("lambdaDevices");
    axisX->setLabelFormat("%g");
    axisX->setTickCount(3);
  }

  chart->addAxis(axisX, Qt::AlignBottom);
  lineSeries1->attachAxis(axisX);
  lineSeries2->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis();

  axisY->setTitleText("tbp/dbp");
  axisY->setLabelFormat("%g");
  axisY->setTickCount(4);

  chart->addAxis(axisY, Qt::AlignLeft);
  lineSeries1->attachAxis(axisY);
  lineSeries2->attachAxis(axisY);

  chartView->setChart(chart);
}

void autoMode::addtobsl(int i, QtCharts::QLineSeries * lineSeries1, QtCharts::QLineSeries * lineSeries2)
{
  QChartView *chartView = new QChartView(this);
  ui->tobsldisp->addWidget(chartView);

  QChart *chart = new QChart();
  chart->addSeries(lineSeries1);
  chart->addSeries(lineSeries2);
  chart->legend()->hide();
  chart->setTitle("");

  QValueAxis *axisX = new QValueAxis();
  if (i == 0) {
    axisX->setTitleText("sources");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(7);
  } else if (i == 1) {
    axisX->setTitleText("devices");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(7);
  } else if (i == 2) {
    axisX->setTitleText("bufferCap");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(10);
  } else if (i == 3) {
    axisX->setTitleText("lambdaSource");
    axisX->setLabelFormat("%g");
    axisX->setTickCount(3);
  } else if (i == 4) {
    axisX->setTitleText("lambdaDevices");
    axisX->setLabelFormat("%g");
    axisX->setTickCount(3);
  }

  chart->addAxis(axisX, Qt::AlignBottom);
  lineSeries1->attachAxis(axisX);
  lineSeries2->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis();

  axisY->setTitleText("tobsl/dobsl");
  axisY->setLabelFormat("%g");
  axisY->setTickCount(4);

  chart->addAxis(axisY, Qt::AlignLeft);
  lineSeries1->attachAxis(axisY);
  lineSeries2->attachAxis(axisY);

  chartView->setChart(chart);
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
