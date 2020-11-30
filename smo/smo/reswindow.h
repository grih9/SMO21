#ifndef RESWINDOW_H
#define RESWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE

class ResWindow : public QDialog {
  Q_OBJECT;
public:
  ResWindow(QDialog * parent = nullptr);
  ResWindow();
  static int counter;

private slots:
  void on_NextButton_clicked();
private:
};

#endif

