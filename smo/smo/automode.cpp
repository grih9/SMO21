#include "automode.h"
#include "ui_automode.h"
#include <QLogValueAxis>
#include <QLineSeries>
#include <QValueAxis>
#include <QChart>
#include <QChartView>
#include <math.h>

static int randomBetween(int low, int high, int seed)
{
    qsrand(seed); // Установка базового числа для отсчёта рандома в qrand
    return (qrand() % ((high + 1) - low) + low);
}

using namespace QtCharts;

autoMode::autoMode(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::autoMode)
{
  ui->setupUi(this);

  // Инициализирующее число для установки базы рандома в qrand
  int seed = 0;

  for (int i = 0; i < 3; ++i) {
    // Создаём представление графика
    QChartView *chartView = new QChartView(this);
    // Добавляем его в горизонтальный Layout
    ui->horizontalLayout->addWidget(chartView);

    // Создаём случайную синусоиду
    seed = randomBetween(0, 100, seed);
    QLineSeries *series = new QLineSeries();
    int k = 0;
    while (k <= 100) {
      *series << QPointF(k, sin((seed+ k)*0.1));
      ++k;
    }

    // Создаём график и добавляем в него синусоиду
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Graphic");

    // Добавим всплывающую подсказку для графика
    chart->setToolTip(QString("График №%1\n"
                                "Количество отсчётов %2").arg(i + 1).arg(k));

    // Настройка осей графика
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("x, м");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(33);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("t, мс");
    axisY->setLabelFormat("%i");
    axisY->setTickCount(12);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Устанавливаем график в представление
    chartView->setChart(chart);
  }
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
