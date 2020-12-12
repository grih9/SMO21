#ifndef AUTOMODE_H
#define AUTOMODE_H

#include <QFrame>

namespace Ui {
class autoMode;
}

class autoMode : public QFrame
{
  Q_OBJECT

public:
  explicit autoMode(QWidget *parent = nullptr);
  ~autoMode();

private slots:
  void on_backButton_clicked();

signals:
  void ret();

private:
  Ui::autoMode *ui;
};

#endif // AUTOMODE_H
