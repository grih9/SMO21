#include <QApplication>
#include <iostream>
#include <time.h>
#include "properties.h"
#include "Device.h"
#include "Source.h"
#include "Buffer.h"
#include "dialog.h"
#include "mainwindow.h"

//FIXME: ВЫВОД ИНФОРМАЦИИ ПРО ВЫТЕСНЕНИЕ ИЗ БУФЕРА, ПОИНТЕР ПРИ ВЫТЕСНЕНИИ ИЗ БУФЕРА, ((ЭКСПОНЕНЦИАЛЬНОЕ РАСПРЕДЕЛЕНИЕ)), ВЫВОД ИНФОРМАЦИИ ПОСЛЕ ОСВОБОЖДЕНИЯ ПРИБОРА
enum class BOS_TYPE {
    DEVICE,
    SOURCE
};

void printinfo(const Device * devices, const Source * sources, const Buffer * buffer)
{
    std::cout << "Devices:\n";
    for (int i = 0; i < Properties::devicesNum; ++i) {
        std::cout << "\t" << devices[i].getNum() << ". Release time: " << devices[i].getReleaseTime() << ". Status: w - "
                  << (devices[i].isWaiting() ? "Yes" : "No") << ", a - " << (devices[i].availability() ? "Yes" : "No")
                  << ". Taken = " << devices[i].getNumberOfTaken() << std::endl;
    }

    std::cout << "Sources:\n";
    for (int i = 0; i < Properties::sourcesNum; ++i) {
        std::cout << "\t" << sources[i].getSourceNum() << ". Generated: " << sources[i].getSourceCount() << ", Next request: "
                  << sources[i].getNextReqTime() << "\n";

    }
    std::cout << "Buffer:\n";
    buffer->printBufferInfo();
    std::cout << "\n";
}

int main(int argc, char * argv[])
{
  QApplication app(argc, argv);

  MainWindow window;
  window.show();
  return app.exec();

}
