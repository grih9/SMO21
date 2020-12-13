#include "BMS.h"
#include <iostream>
#include <time.h>
#include "properties.h"
#include "automode.h"
#include "QLineSeries"

bool BMS::flagNext = true;
bool BMS::flagEnd = false;

enum class BOS_TYPE {
    DEVICE,
    SOURCE
};

void BMS::printinfo(const Device * devices, const Source * sources, const Buffer * buffer)
{
  QString str("Devices: \n");
  std::string a = std::to_string(1);
  QString tmp = a.c_str();
  std::cout << "Devices:\n";
  for (int i = 0; i < Properties::devicesNum; ++i) {
    a = std::to_string(devices[i].getNum());
    tmp = a.c_str();
    str.append(tmp);
    str.append(". Release time: ");
    a = std::to_string(devices[i].getReleaseTime());
    tmp = a.c_str();
    str.append(tmp);
    str.append(". Status: w - ");
    str.append((devices[i].isWaiting() ? "Yes" : "No"));
    str.append(", a - ");
    str.append((devices[i].availability() ? "Yes" : "No"));
    str.append(". Taken = ");
    a = std::to_string(devices[i].getNumberOfTaken());
    tmp = a.c_str();
    str.append(tmp);
    str.append("\n");
    std::cout << "\t" << devices[i].getNum() << ". Release time: " << devices[i].getReleaseTime() << ". Status: w - "
              << (devices[i].isWaiting() ? "Yes" : "No") << ", a - " << (devices[i].availability() ? "Yes" : "No")
              << ". Taken = " << devices[i].getNumberOfTaken() << std::endl;
  }

  str.append("Sources:\n");
  std::cout << "Sources:\n";
  for (int i = 0; i < Properties::sourcesNum; ++i) {
    str.append("\t");
    a = std::to_string(sources[i].getSourceNum());
    tmp = a.c_str();
    str.append(tmp);
    str.append(". Generated: ");
    a = std::to_string(sources[i].getSourceCount());
    tmp = a.c_str();
    str.append(tmp);
    str.append(", Cancelled: ");
    a = std::to_string(sources[i].getSourceCancelled());
    tmp = a.c_str();
    str.append(tmp);
    str.append(", Next request: ");
    a = std::to_string(sources[i].getNextReqTime());
    tmp = a.c_str();
    str.append(tmp);
    str.append("\n");
    std::cout << "\t" << sources[i].getSourceNum() << ". Generated: " << sources[i].getSourceCount()
              << ", Cancelled: " << sources[i].getSourceCancelled() << ", Next request: "
              << sources[i].getNextReqTime() << "\n";

  }
  str.append("Buffer:\n");
  std::cout << "Buffer:\n";

  buffer->printBufferInfo(str);
  emit stringSend(str);
  std::cout << "\n";
}

bool areDevicesEmpty(Device * devices) {
  bool flag = true;
  for (int i = 0; i < Properties::devicesNum; i++) {
    flag &= devices[i].availability();
  }
  return flag;
}

void BMS::autoMode() {
  QtCharts::QLineSeries * pOtk = new QtCharts::QLineSeries();
  QtCharts::QLineSeries * usageKoef1 = new QtCharts::QLineSeries();
  QtCharts::QLineSeries * usageKoefavg = new QtCharts::QLineSeries();
  QtCharts::QLineSeries * tbpavg = new QtCharts::QLineSeries();
  QtCharts::QLineSeries * tobslavg = new QtCharts::QLineSeries();
  QtCharts::QLineSeries * dispbpavg = new QtCharts::QLineSeries();
  QtCharts::QLineSeries * dispobslavg = new QtCharts::QLineSeries();
  int stmp = Properties::sourcesNum;
  for (int sNum = 1; sNum <= 7; sNum++) {
    Properties::sourcesNum = sNum;
    srand(time(0));
    rand();

    Device devices[7];
    for (int i = 0; i < Properties::devicesNum; i++) {
      devices[i] = Device(i + 1, Properties::lambdaDevices);
    }
    Source sources[7];
    for (int i = 0; i < Properties::sourcesNum; i++) {
      sources[i] = Source(i + 1, Properties::lambdaSources);
    }

    Buffer buffer(Properties::bufferCapacity);
    double time = 0;
    int counter = 0;

    while ((counter < Properties::requestsNum) || !buffer.isEmpty() || !areDevicesEmpty(devices)) {
      double minTime = sources[0].getNextReqTime();
      BOS_TYPE bosType = BOS_TYPE::SOURCE;
      int tmpNum = 0;
      if (counter >= Properties::requestsNum) {

        minTime = devices[0].getReleaseTime() + 100;
        bosType = BOS_TYPE::DEVICE;
        tmpNum = 0;
      }

      for (int i = 0; i < Properties::devicesNum; ++i) {
        if (devices[i].getReleaseTime() < minTime && !devices[i].isWaiting() && !devices[i].availability()) {
          minTime = devices[i].getReleaseTime();
          bosType = BOS_TYPE::DEVICE;
          tmpNum = i;
        }
      }       // device's release

      if (counter < Properties::requestsNum) {
        for (int i = 0; i < Properties::sourcesNum; ++i) {
          if (sources[i].getNextReqTime() < minTime) {
            minTime = sources[i].getNextReqTime();
            bosType = BOS_TYPE::SOURCE;
            tmpNum = i;
          }
        }   // request generation
      }
      switch (bosType) {
        case BOS_TYPE::DEVICE: {
          if (buffer.isEmpty()) {
            time = devices[tmpNum].release();
            devices[tmpNum].wait();
          } else {
            time = devices[tmpNum].release();
            QString str2;
            Request req = buffer.pop(str2);
            sources[req.getRequestNumber()[0] - 1].setTimeBp(time - req.getGenerationTime());
            sources[req.getRequestNumber()[0] - 1]
                .setDispBp(pow((time - req.getGenerationTime() - sources[req.getRequestNumber()[0] - 1]
                .getTimeBp()) / (sources[req.getRequestNumber()[0] - 1].getSourceCount()- sources[req.getRequestNumber()[0] - 1].getSourceCancelled()), 2));
            double relTime = devices[tmpNum].take(time, str2);
            sources[req.getRequestNumber()[0] - 1].setTimeObsl(relTime - time);
            sources[req.getRequestNumber()[0] - 1]
                .setDispObsl(pow((time - req.getGenerationTime() - sources[req.getRequestNumber()[0] - 1]
                .getTimeObsl()) / (sources[req.getRequestNumber()[0] - 1].getSourceCount() - sources[req.getRequestNumber()[0] - 1].getSourceCancelled()), 2));

          }
          break;
        } case BOS_TYPE::SOURCE: {
          int devNum = -1;
          for (int i = 0; i < Properties::devicesNum; ++i) {
            if (devices[i].isWaiting()) {
              devNum = i;
              break;
            }
          }
          time = sources[tmpNum].getNextReqTime();
          counter++;
          if (devNum != -1) {
            sources[tmpNum].generate(time);
            QString str;
            double relTime = devices[devNum].take(time, str);
            sources[tmpNum].setTimeObsl(relTime - time);
            sources[tmpNum]
                .setDispObsl(pow((relTime - time - sources[tmpNum].getTimeObsl()) / (sources[tmpNum].getSourceCount() - sources[tmpNum].getSourceCancelled()), 2));

          } else {
            Request r = sources[tmpNum].generate(time);
            QString str;
            int cancelled = buffer.push(r, str);
            if (cancelled != 0) {
              sources[cancelled - 1].cancell();
            }
          }
          break;
        }
      }
    }
    double workingavg = 0;
    for (int i = 0; i < Properties::devicesNum; i++) {
      std::cout << "Device "<< devices[i].getNum() <<  ":\ntaken =  " << devices[i].getNumberOfTaken() << std::endl;
      std::cout << "working time = " << devices[i].getWorkingTime() << ", koef preb = " << double(devices[i].getWorkingTime()) / double(time) * 100 << "%" << std::endl;
      workingavg += double(devices[i].getWorkingTime()) / double(time) * 100;
    }
    *usageKoef1 << QPointF(Properties::sourcesNum, double(devices[0].getWorkingTime()) / double(time) * 100);
    workingavg /= Properties::devicesNum;
    *usageKoefavg << QPointF(Properties::sourcesNum, workingavg);
    std::cout << std::endl;
    double potkavg = 0;
    double tobslavgt = 0;
    double tbpavgt = 0;
    double dispobslavgt = 0;
    double dispbpavgt = 0;
    for (int i = 0; i < Properties::sourcesNum; i++) {
      double potk = (double(sources[i].getSourceCancelled())/double(sources[i].getSourceCount()) * 100);
      potkavg += potk;
      std::cout << "Source "<< sources[i].getSourceNum() <<  ":\ncreated = " << sources[i].getSourceCount()
                << ", cancelled = " << sources[i].getSourceCancelled() << ", potk = " << potk << "%" << std::endl;
      std::cout << "time preb = " << sources[i].getTimeObsl() + sources[i].getTimeBp() << ", time obsl = "
                << sources[i].getTimeObsl() << ", time bp = " << sources[i].getTimeBp() << std::endl;
      std::cout << "time preb avg = " << (sources[i].getTimeObsl() + sources[i].getTimeBp()) / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", time obsl avg = " << sources[i].getTimeObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", time bp avg = " << sources[i].getTimeBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled()) << std::endl;
      tobslavgt += sources[i].getTimeObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      tbpavgt += sources[i].getTimeBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      std::cout << "disp obsl = " << sources[i].getDispObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", disp bp = " << sources[i].getDispBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled()) << std::endl;
      dispobslavgt += sources[i].getDispObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      dispbpavgt += sources[i].getDispBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
    }
    potkavg /= Properties::sourcesNum;
    tobslavgt /= Properties::sourcesNum;
    tbpavgt /= Properties::sourcesNum;
    dispobslavgt /= Properties::sourcesNum;
    dispbpavgt /= Properties::sourcesNum;
    *pOtk << QPointF(Properties::sourcesNum, potkavg);
    *tobslavg << QPointF(Properties::sourcesNum, tobslavgt);
    *tbpavg << QPointF(Properties::sourcesNum, tbpavgt);
    *dispobslavg << QPointF(Properties::sourcesNum, dispobslavgt);
    *dispbpavg << QPointF(Properties::sourcesNum, dispbpavgt);
  }
  emit potk(0, pOtk);
  emit usageKoef(0, usageKoef1, usageKoefavg);
  emit tbp(0, tbpavg, dispbpavg);
  emit tobsl(0, tobslavg, dispobslavg);

  Properties::sourcesNum = stmp;
  int dtmp = Properties::devicesNum;
  pOtk = new QtCharts::QLineSeries();
  usageKoef1 = new QtCharts::QLineSeries();
  usageKoefavg = new QtCharts::QLineSeries();
  tbpavg = new QtCharts::QLineSeries();
  tobslavg = new QtCharts::QLineSeries();
  dispbpavg = new QtCharts::QLineSeries();
  dispobslavg = new QtCharts::QLineSeries();
  for (int dNum = 1; dNum <= 7; dNum++) {
    Properties::devicesNum = dNum;
    srand(time(0));
    rand();

    Device devices[7];
    for (int i = 0; i < Properties::devicesNum; i++) {
      devices[i] = Device(i + 1, Properties::lambdaDevices);
    }
    Source sources[7];
    for (int i = 0; i < Properties::sourcesNum; i++) {
      sources[i] = Source(i + 1, Properties::lambdaSources);
    }

    Buffer buffer(Properties::bufferCapacity);
    double time = 0;
    int counter = 0;

    while ((counter < Properties::requestsNum) || !buffer.isEmpty() || !areDevicesEmpty(devices)) {
      double minTime = sources[0].getNextReqTime();
      BOS_TYPE bosType = BOS_TYPE::SOURCE;
      int tmpNum = 0;
      if (counter >= Properties::requestsNum) {

        minTime = devices[0].getReleaseTime() + 100;
        bosType = BOS_TYPE::DEVICE;
        tmpNum = 0;
      }

      for (int i = 0; i < Properties::devicesNum; ++i) {
        if (devices[i].getReleaseTime() < minTime && !devices[i].isWaiting() && !devices[i].availability()) {
          minTime = devices[i].getReleaseTime();
          bosType = BOS_TYPE::DEVICE;
          tmpNum = i;
        }
      }       // device's release

      if (counter < Properties::requestsNum) {
        for (int i = 0; i < Properties::sourcesNum; ++i) {
          if (sources[i].getNextReqTime() < minTime) {
            minTime = sources[i].getNextReqTime();
            bosType = BOS_TYPE::SOURCE;
            tmpNum = i;
          }
        }   // request generation
      }
      switch (bosType) {
        case BOS_TYPE::DEVICE: {
          if (buffer.isEmpty()) {
            time = devices[tmpNum].release();
            devices[tmpNum].wait();
          } else {
            time = devices[tmpNum].release();
            QString str2;
            Request req = buffer.pop(str2);
            sources[req.getRequestNumber()[0] - 1].setTimeBp(time - req.getGenerationTime());
            sources[req.getRequestNumber()[0] - 1]
                .setDispBp(pow((time - req.getGenerationTime() - sources[req.getRequestNumber()[0] - 1]
                .getTimeBp()) / (sources[req.getRequestNumber()[0] - 1].getSourceCount()- sources[req.getRequestNumber()[0] - 1].getSourceCancelled()), 2));
            double relTime = devices[tmpNum].take(time, str2);
            sources[req.getRequestNumber()[0] - 1].setTimeObsl(relTime - time);
            sources[req.getRequestNumber()[0] - 1]
                .setDispObsl(pow((time - req.getGenerationTime() - sources[req.getRequestNumber()[0] - 1]
                .getTimeObsl()) / (sources[req.getRequestNumber()[0] - 1].getSourceCount() - sources[req.getRequestNumber()[0] - 1].getSourceCancelled()), 2));

          }
          break;
        } case BOS_TYPE::SOURCE: {
          int devNum = -1;
          for (int i = 0; i < Properties::devicesNum; ++i) {
            if (devices[i].isWaiting()) {
              devNum = i;
              break;
            }
          }
          time = sources[tmpNum].getNextReqTime();
          counter++;
          if (devNum != -1) {
            sources[tmpNum].generate(time);
            QString str;
            double relTime = devices[devNum].take(time, str);
            sources[tmpNum].setTimeObsl(relTime - time);
            sources[tmpNum]
                .setDispObsl(pow((relTime - time - sources[tmpNum].getTimeObsl()) / (sources[tmpNum].getSourceCount() - sources[tmpNum].getSourceCancelled()), 2));

          } else {
            Request r = sources[tmpNum].generate(time);
            QString str;
            int cancelled = buffer.push(r, str);
            if (cancelled != 0) {
              sources[cancelled - 1].cancell();
            }
          }
          break;
        }
      }
    }
    double workingavg = 0;
    for (int i = 0; i < Properties::devicesNum; i++) {
      std::cout << "Device "<< devices[i].getNum() <<  ":\ntaken =  " << devices[i].getNumberOfTaken() << std::endl;
      std::cout << "working time = " << devices[i].getWorkingTime() << ", koef preb = " << double(devices[i].getWorkingTime()) / double(time) * 100 << "%" << std::endl;
      workingavg += double(devices[i].getWorkingTime()) / double(time) * 100;
    }
    *usageKoef1 << QPointF(Properties::devicesNum, double(devices[0].getWorkingTime()) / double(time) * 100);
    workingavg /= Properties::devicesNum;
    *usageKoefavg << QPointF(Properties::devicesNum, workingavg);
    std::cout << std::endl;
    double potkavg = 0;
    double tobslavgt = 0;
    double tbpavgt = 0;
    double dispobslavgt = 0;
    double dispbpavgt = 0;
    for (int i = 0; i < Properties::sourcesNum; i++) {
      double potk = (double(sources[i].getSourceCancelled())/double(sources[i].getSourceCount()) * 100);
      potkavg += potk;
      std::cout << "Source "<< sources[i].getSourceNum() <<  ":\ncreated = " << sources[i].getSourceCount()
                << ", cancelled = " << sources[i].getSourceCancelled() << ", potk = " << potk << "%" << std::endl;
      std::cout << "time preb = " << sources[i].getTimeObsl() + sources[i].getTimeBp() << ", time obsl = "
                << sources[i].getTimeObsl() << ", time bp = " << sources[i].getTimeBp() << std::endl;
      std::cout << "time preb avg = " << (sources[i].getTimeObsl() + sources[i].getTimeBp()) / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", time obsl avg = " << sources[i].getTimeObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", time bp avg = " << sources[i].getTimeBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled()) << std::endl;
      tobslavgt += sources[i].getTimeObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      tbpavgt += sources[i].getTimeBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      std::cout << "disp obsl = " << sources[i].getDispObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", disp bp = " << sources[i].getDispBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled()) << std::endl;
      dispobslavgt += sources[i].getDispObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      dispbpavgt += sources[i].getDispBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
    }
    potkavg /= Properties::sourcesNum;
    tobslavgt /= Properties::sourcesNum;
    tbpavgt /= Properties::sourcesNum;
    dispobslavgt /= Properties::sourcesNum;
    dispbpavgt /= Properties::sourcesNum;
    *pOtk << QPointF(Properties::devicesNum, potkavg);
    *tobslavg << QPointF(Properties::devicesNum, tobslavgt);
    *tbpavg << QPointF(Properties::devicesNum, tbpavgt);
    *dispobslavg << QPointF(Properties::devicesNum, dispobslavgt);
    *dispbpavg << QPointF(Properties::devicesNum, dispbpavgt);
  }
  emit potk(1, pOtk);
  emit usageKoef(1, usageKoef1, usageKoefavg);
  emit tbp(1, tbpavg, dispbpavg);
  emit tobsl(1, tobslavg, dispobslavg);

  Properties::devicesNum = dtmp;
  int bufTmp = Properties::bufferCapacity;

  pOtk = new QtCharts::QLineSeries();
  usageKoef1 = new QtCharts::QLineSeries();
  usageKoefavg = new QtCharts::QLineSeries();
  tbpavg = new QtCharts::QLineSeries();
  tobslavg = new QtCharts::QLineSeries();
  dispbpavg = new QtCharts::QLineSeries();
  dispobslavg = new QtCharts::QLineSeries();
  for (int bSize = 1; bSize <= 10; bSize++) {
    Properties::bufferCapacity = bSize;
    srand(time(0));
    rand();

    Device devices[7];
    for (int i = 0; i < Properties::devicesNum; i++) {
      devices[i] = Device(i + 1, Properties::lambdaDevices);
    }
    Source sources[7];
    for (int i = 0; i < Properties::sourcesNum; i++) {
      sources[i] = Source(i + 1, Properties::lambdaSources);
    }

    Buffer buffer(Properties::bufferCapacity);
    double time = 0;
    int counter = 0;

    while ((counter < Properties::requestsNum) || !buffer.isEmpty() || !areDevicesEmpty(devices)) {
      double minTime = sources[0].getNextReqTime();
      BOS_TYPE bosType = BOS_TYPE::SOURCE;
      int tmpNum = 0;
      if (counter >= Properties::requestsNum) {

        minTime = devices[0].getReleaseTime() + 100;
        bosType = BOS_TYPE::DEVICE;
        tmpNum = 0;
      }

      for (int i = 0; i < Properties::devicesNum; ++i) {
        if (devices[i].getReleaseTime() < minTime && !devices[i].isWaiting() && !devices[i].availability()) {
          minTime = devices[i].getReleaseTime();
          bosType = BOS_TYPE::DEVICE;
          tmpNum = i;
        }
      }       // device's release

      if (counter < Properties::requestsNum) {
        for (int i = 0; i < Properties::sourcesNum; ++i) {
          if (sources[i].getNextReqTime() < minTime) {
            minTime = sources[i].getNextReqTime();
            bosType = BOS_TYPE::SOURCE;
            tmpNum = i;
          }
        }   // request generation
      }
      switch (bosType) {
        case BOS_TYPE::DEVICE: {
          if (buffer.isEmpty()) {
            time = devices[tmpNum].release();
            devices[tmpNum].wait();
          } else {
            time = devices[tmpNum].release();
            QString str2;
            Request req = buffer.pop(str2);
            sources[req.getRequestNumber()[0] - 1].setTimeBp(time - req.getGenerationTime());
            sources[req.getRequestNumber()[0] - 1]
                .setDispBp(pow((time - req.getGenerationTime() - sources[req.getRequestNumber()[0] - 1]
                .getTimeBp()) / (sources[req.getRequestNumber()[0] - 1].getSourceCount()- sources[req.getRequestNumber()[0] - 1].getSourceCancelled()), 2));
            double relTime = devices[tmpNum].take(time, str2);
            sources[req.getRequestNumber()[0] - 1].setTimeObsl(relTime - time);
            sources[req.getRequestNumber()[0] - 1]
                .setDispObsl(pow((time - req.getGenerationTime() - sources[req.getRequestNumber()[0] - 1]
                .getTimeObsl()) / (sources[req.getRequestNumber()[0] - 1].getSourceCount() - sources[req.getRequestNumber()[0] - 1].getSourceCancelled()), 2));

          }
          break;
        } case BOS_TYPE::SOURCE: {
          int devNum = -1;
          for (int i = 0; i < Properties::devicesNum; ++i) {
            if (devices[i].isWaiting()) {
              devNum = i;
              break;
            }
          }
          time = sources[tmpNum].getNextReqTime();
          counter++;
          if (devNum != -1) {
            sources[tmpNum].generate(time);
            QString str;
            double relTime = devices[devNum].take(time, str);
            sources[tmpNum].setTimeObsl(relTime - time);
            sources[tmpNum]
                .setDispObsl(pow((relTime - time - sources[tmpNum].getTimeObsl()) / (sources[tmpNum].getSourceCount() - sources[tmpNum].getSourceCancelled()), 2));

          } else {
            Request r = sources[tmpNum].generate(time);
            QString str;
            int cancelled = buffer.push(r, str);
            if (cancelled != 0) {
              sources[cancelled - 1].cancell();
            }
          }
          break;
        }
      }
    }
    double workingavg = 0;
    for (int i = 0; i < Properties::devicesNum; i++) {
      std::cout << "Device "<< devices[i].getNum() <<  ":\ntaken =  " << devices[i].getNumberOfTaken() << std::endl;
      std::cout << "working time = " << devices[i].getWorkingTime() << ", koef preb = " << double(devices[i].getWorkingTime()) / double(time) * 100 << "%" << std::endl;
      workingavg += double(devices[i].getWorkingTime()) / double(time) * 100;
    }
    *usageKoef1 << QPointF(Properties::bufferCapacity, double(devices[0].getWorkingTime()) / double(time) * 100);
    workingavg /= Properties::devicesNum;
    *usageKoefavg << QPointF(Properties::bufferCapacity, workingavg);
    std::cout << std::endl;
    double potkavg = 0;
    double tobslavgt = 0;
    double tbpavgt = 0;
    double dispobslavgt = 0;
    double dispbpavgt = 0;
    for (int i = 0; i < Properties::sourcesNum; i++) {
      double potk = (double(sources[i].getSourceCancelled())/double(sources[i].getSourceCount()) * 100);
      potkavg += potk;
      std::cout << "Source "<< sources[i].getSourceNum() <<  ":\ncreated = " << sources[i].getSourceCount()
                << ", cancelled = " << sources[i].getSourceCancelled() << ", potk = " << potk << "%" << std::endl;
      std::cout << "time preb = " << sources[i].getTimeObsl() + sources[i].getTimeBp() << ", time obsl = "
                << sources[i].getTimeObsl() << ", time bp = " << sources[i].getTimeBp() << std::endl;
      std::cout << "time preb avg = " << (sources[i].getTimeObsl() + sources[i].getTimeBp()) / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", time obsl avg = " << sources[i].getTimeObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", time bp avg = " << sources[i].getTimeBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled()) << std::endl;
      tobslavgt += sources[i].getTimeObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      tbpavgt += sources[i].getTimeBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      std::cout << "disp obsl = " << sources[i].getDispObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", disp bp = " << sources[i].getDispBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled()) << std::endl;
      dispobslavgt += sources[i].getDispObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      dispbpavgt += sources[i].getDispBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
    }
    potkavg /= Properties::sourcesNum;
    tobslavgt /= Properties::sourcesNum;
    tbpavgt /= Properties::sourcesNum;
    dispobslavgt /= Properties::sourcesNum;
    dispbpavgt /= Properties::sourcesNum;
    *pOtk << QPointF(Properties::bufferCapacity, potkavg);
    *tobslavg << QPointF(Properties::bufferCapacity, tobslavgt);
    *tbpavg << QPointF(Properties::bufferCapacity, tbpavgt);
    *dispobslavg << QPointF(Properties::bufferCapacity, dispobslavgt);
    *dispbpavg << QPointF(Properties::bufferCapacity, dispbpavgt);
  }

  Properties::bufferCapacity = bufTmp;
  emit potk(2, pOtk);
  emit usageKoef(2, usageKoef1, usageKoefavg);
  emit tbp(2, tbpavg, dispbpavg);
  emit tobsl(2, tobslavg, dispobslavg);

  double lambdaS = Properties::lambdaSources;

  pOtk = new QtCharts::QLineSeries();
  usageKoef1 = new QtCharts::QLineSeries();
  usageKoefavg = new QtCharts::QLineSeries();
  tbpavg = new QtCharts::QLineSeries();
  tobslavg = new QtCharts::QLineSeries();
  dispbpavg = new QtCharts::QLineSeries();
  dispobslavg = new QtCharts::QLineSeries();
  for (double ls = 0.1; ls <= 1.5; ls = ls + 0.2) {
    Properties::lambdaSources = ls;
    srand(time(0));
    rand();

    Device devices[7];
    for (int i = 0; i < Properties::devicesNum; i++) {
      devices[i] = Device(i + 1, Properties::lambdaDevices);
    }
    Source sources[7];
    for (int i = 0; i < Properties::sourcesNum; i++) {
      sources[i] = Source(i + 1, Properties::lambdaSources);
    }

    Buffer buffer(Properties::bufferCapacity);
    double time = 0;
    int counter = 0;

    while ((counter < Properties::requestsNum) || !buffer.isEmpty() || !areDevicesEmpty(devices)) {
      double minTime = sources[0].getNextReqTime();
      BOS_TYPE bosType = BOS_TYPE::SOURCE;
      int tmpNum = 0;
      if (counter >= Properties::requestsNum) {

        minTime = devices[0].getReleaseTime() + 100;
        bosType = BOS_TYPE::DEVICE;
        tmpNum = 0;
      }

      for (int i = 0; i < Properties::devicesNum; ++i) {
        if (devices[i].getReleaseTime() < minTime && !devices[i].isWaiting() && !devices[i].availability()) {
          minTime = devices[i].getReleaseTime();
          bosType = BOS_TYPE::DEVICE;
          tmpNum = i;
        }
      }       // device's release

      if (counter < Properties::requestsNum) {
        for (int i = 0; i < Properties::sourcesNum; ++i) {
          if (sources[i].getNextReqTime() < minTime) {
            minTime = sources[i].getNextReqTime();
            bosType = BOS_TYPE::SOURCE;
            tmpNum = i;
          }
        }   // request generation
      }
      switch (bosType) {
        case BOS_TYPE::DEVICE: {
          if (buffer.isEmpty()) {
            time = devices[tmpNum].release();
            devices[tmpNum].wait();
          } else {
            time = devices[tmpNum].release();
            QString str2;
            Request req = buffer.pop(str2);
            sources[req.getRequestNumber()[0] - 1].setTimeBp(time - req.getGenerationTime());
            sources[req.getRequestNumber()[0] - 1]
                .setDispBp(pow((time - req.getGenerationTime() - sources[req.getRequestNumber()[0] - 1]
                .getTimeBp()) / (sources[req.getRequestNumber()[0] - 1].getSourceCount()- sources[req.getRequestNumber()[0] - 1].getSourceCancelled()), 2));
            double relTime = devices[tmpNum].take(time, str2);
            sources[req.getRequestNumber()[0] - 1].setTimeObsl(relTime - time);
            sources[req.getRequestNumber()[0] - 1]
                .setDispObsl(pow((time - req.getGenerationTime() - sources[req.getRequestNumber()[0] - 1]
                .getTimeObsl()) / (sources[req.getRequestNumber()[0] - 1].getSourceCount() - sources[req.getRequestNumber()[0] - 1].getSourceCancelled()), 2));

          }
          break;
        } case BOS_TYPE::SOURCE: {
          int devNum = -1;
          for (int i = 0; i < Properties::devicesNum; ++i) {
            if (devices[i].isWaiting()) {
              devNum = i;
              break;
            }
          }
          time = sources[tmpNum].getNextReqTime();
          counter++;
          if (devNum != -1) {
            sources[tmpNum].generate(time);
            QString str;
            double relTime = devices[devNum].take(time, str);
            sources[tmpNum].setTimeObsl(relTime - time);
            sources[tmpNum]
                .setDispObsl(pow((relTime - time - sources[tmpNum].getTimeObsl()) / (sources[tmpNum].getSourceCount() - sources[tmpNum].getSourceCancelled()), 2));

          } else {
            Request r = sources[tmpNum].generate(time);
            QString str;
            int cancelled = buffer.push(r, str);
            if (cancelled != 0) {
              sources[cancelled - 1].cancell();
            }
          }
          break;
        }
      }
    }
    double workingavg = 0;
    for (int i = 0; i < Properties::devicesNum; i++) {
      std::cout << "Device "<< devices[i].getNum() <<  ":\ntaken =  " << devices[i].getNumberOfTaken() << std::endl;
      std::cout << "working time = " << devices[i].getWorkingTime() << ", koef preb = " << double(devices[i].getWorkingTime()) / double(time) * 100 << "%" << std::endl;
      workingavg += double(devices[i].getWorkingTime()) / double(time) * 100;
    }
    *usageKoef1 << QPointF(Properties::lambdaSources, double(devices[0].getWorkingTime()) / double(time) * 100);
    workingavg /= Properties::devicesNum;
    *usageKoefavg << QPointF(Properties::lambdaSources, workingavg);
    std::cout << std::endl;
    double potkavg = 0;
    double tobslavgt = 0;
    double tbpavgt = 0;
    double dispobslavgt = 0;
    double dispbpavgt = 0;
    for (int i = 0; i < Properties::sourcesNum; i++) {
      double potk = (double(sources[i].getSourceCancelled())/double(sources[i].getSourceCount()) * 100);
      potkavg += potk;
      std::cout << "Source "<< sources[i].getSourceNum() <<  ":\ncreated = " << sources[i].getSourceCount()
                << ", cancelled = " << sources[i].getSourceCancelled() << ", potk = " << potk << "%" << std::endl;
      std::cout << "time preb = " << sources[i].getTimeObsl() + sources[i].getTimeBp() << ", time obsl = "
                << sources[i].getTimeObsl() << ", time bp = " << sources[i].getTimeBp() << std::endl;
      std::cout << "time preb avg = " << (sources[i].getTimeObsl() + sources[i].getTimeBp()) / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", time obsl avg = " << sources[i].getTimeObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", time bp avg = " << sources[i].getTimeBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled()) << std::endl;
      tobslavgt += sources[i].getTimeObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      tbpavgt += sources[i].getTimeBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      std::cout << "disp obsl = " << sources[i].getDispObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", disp bp = " << sources[i].getDispBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled()) << std::endl;
      dispobslavgt += sources[i].getDispObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      dispbpavgt += sources[i].getDispBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
    }
    potkavg /= Properties::sourcesNum;
    tobslavgt /= Properties::sourcesNum;
    tbpavgt /= Properties::sourcesNum;
    dispobslavgt /= Properties::sourcesNum;
    dispbpavgt /= Properties::sourcesNum;
    *pOtk << QPointF(Properties::lambdaSources, potkavg);
    *tobslavg << QPointF(Properties::lambdaSources, tobslavgt);
    *tbpavg << QPointF(Properties::lambdaSources, tbpavgt);
    *dispobslavg << QPointF(Properties::lambdaSources, dispobslavgt);
    *dispbpavg << QPointF(Properties::lambdaSources, dispbpavgt);
  }

  Properties::lambdaSources = lambdaS;
  emit potk(3, pOtk);
  emit usageKoef(3, usageKoef1, usageKoefavg);
  emit tbp(3, tbpavg, dispbpavg);
  emit tobsl(3, tobslavg, dispobslavg);

  double lambdaD = Properties::lambdaDevices;

  pOtk = new QtCharts::QLineSeries();
  usageKoef1 = new QtCharts::QLineSeries();
  usageKoefavg = new QtCharts::QLineSeries();
  tbpavg = new QtCharts::QLineSeries();
  tobslavg = new QtCharts::QLineSeries();
  dispbpavg = new QtCharts::QLineSeries();
  dispobslavg = new QtCharts::QLineSeries();
  for (double ld = 0.1; ld <= 1.5; ld = ld + 0.2) {
    Properties::lambdaDevices = ld;
    srand(time(0));
    rand();

    Device devices[7];
    for (int i = 0; i < Properties::devicesNum; i++) {
      devices[i] = Device(i + 1, Properties::lambdaDevices);
    }
    Source sources[7];
    for (int i = 0; i < Properties::sourcesNum; i++) {
      sources[i] = Source(i + 1, Properties::lambdaSources);
    }

    Buffer buffer(Properties::bufferCapacity);
    double time = 0;
    int counter = 0;

    while ((counter < Properties::requestsNum) || !buffer.isEmpty() || !areDevicesEmpty(devices)) {
      double minTime = sources[0].getNextReqTime();
      BOS_TYPE bosType = BOS_TYPE::SOURCE;
      int tmpNum = 0;
      if (counter >= Properties::requestsNum) {

        minTime = devices[0].getReleaseTime() + 100;
        bosType = BOS_TYPE::DEVICE;
        tmpNum = 0;
      }

      for (int i = 0; i < Properties::devicesNum; ++i) {
        if (devices[i].getReleaseTime() < minTime && !devices[i].isWaiting() && !devices[i].availability()) {
          minTime = devices[i].getReleaseTime();
          bosType = BOS_TYPE::DEVICE;
          tmpNum = i;
        }
      }       // device's release

      if (counter < Properties::requestsNum) {
        for (int i = 0; i < Properties::sourcesNum; ++i) {
          if (sources[i].getNextReqTime() < minTime) {
            minTime = sources[i].getNextReqTime();
            bosType = BOS_TYPE::SOURCE;
            tmpNum = i;
          }
        }   // request generation
      }
      switch (bosType) {
        case BOS_TYPE::DEVICE: {
          if (buffer.isEmpty()) {
            time = devices[tmpNum].release();
            devices[tmpNum].wait();
          } else {
            time = devices[tmpNum].release();
            QString str2;
            Request req = buffer.pop(str2);
            sources[req.getRequestNumber()[0] - 1].setTimeBp(time - req.getGenerationTime());
            sources[req.getRequestNumber()[0] - 1]
                .setDispBp(pow((time - req.getGenerationTime() - sources[req.getRequestNumber()[0] - 1]
                .getTimeBp()) / (sources[req.getRequestNumber()[0] - 1].getSourceCount()- sources[req.getRequestNumber()[0] - 1].getSourceCancelled()), 2));
            double relTime = devices[tmpNum].take(time, str2);
            sources[req.getRequestNumber()[0] - 1].setTimeObsl(relTime - time);
            sources[req.getRequestNumber()[0] - 1]
                .setDispObsl(pow((time - req.getGenerationTime() - sources[req.getRequestNumber()[0] - 1]
                .getTimeObsl()) / (sources[req.getRequestNumber()[0] - 1].getSourceCount() - sources[req.getRequestNumber()[0] - 1].getSourceCancelled()), 2));

          }
          break;
        } case BOS_TYPE::SOURCE: {
          int devNum = -1;
          for (int i = 0; i < Properties::devicesNum; ++i) {
            if (devices[i].isWaiting()) {
              devNum = i;
              break;
            }
          }
          time = sources[tmpNum].getNextReqTime();
          counter++;
          if (devNum != -1) {
            sources[tmpNum].generate(time);
            QString str;
            double relTime = devices[devNum].take(time, str);
            sources[tmpNum].setTimeObsl(relTime - time);
            sources[tmpNum]
                .setDispObsl(pow((relTime - time - sources[tmpNum].getTimeObsl()) / (sources[tmpNum].getSourceCount() - sources[tmpNum].getSourceCancelled()), 2));

          } else {
            Request r = sources[tmpNum].generate(time);
            QString str;
            int cancelled = buffer.push(r, str);
            if (cancelled != 0) {
              sources[cancelled - 1].cancell();
            }
          }
          break;
        }
      }
    }
    double workingavg = 0;
    for (int i = 0; i < Properties::devicesNum; i++) {
      std::cout << "Device "<< devices[i].getNum() <<  ":\ntaken =  " << devices[i].getNumberOfTaken() << std::endl;
      std::cout << "working time = " << devices[i].getWorkingTime() << ", koef preb = " << double(devices[i].getWorkingTime()) / double(time) * 100 << "%" << std::endl;
      workingavg += double(devices[i].getWorkingTime()) / double(time) * 100;
    }
    *usageKoef1 << QPointF(Properties::lambdaDevices, double(devices[0].getWorkingTime()) / double(time) * 100);
    workingavg /= Properties::devicesNum;
    *usageKoefavg << QPointF(Properties::lambdaDevices, workingavg);
    std::cout << std::endl;
    double potkavg = 0;
    double tobslavgt = 0;
    double tbpavgt = 0;
    double dispobslavgt = 0;
    double dispbpavgt = 0;
    for (int i = 0; i < Properties::sourcesNum; i++) {
      double potk = (double(sources[i].getSourceCancelled())/double(sources[i].getSourceCount()) * 100);
      potkavg += potk;
      std::cout << "Source "<< sources[i].getSourceNum() <<  ":\ncreated = " << sources[i].getSourceCount()
                << ", cancelled = " << sources[i].getSourceCancelled() << ", potk = " << potk << "%" << std::endl;
      std::cout << "time preb = " << sources[i].getTimeObsl() + sources[i].getTimeBp() << ", time obsl = "
                << sources[i].getTimeObsl() << ", time bp = " << sources[i].getTimeBp() << std::endl;
      std::cout << "time preb avg = " << (sources[i].getTimeObsl() + sources[i].getTimeBp()) / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", time obsl avg = " << sources[i].getTimeObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", time bp avg = " << sources[i].getTimeBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled()) << std::endl;
      tobslavgt += sources[i].getTimeObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      tbpavgt += sources[i].getTimeBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      std::cout << "disp obsl = " << sources[i].getDispObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled())
                << ", disp bp = " << sources[i].getDispBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled()) << std::endl;
      dispobslavgt += sources[i].getDispObsl() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
      dispbpavgt += sources[i].getDispBp() / (sources[i].getSourceCount() - sources[i].getSourceCancelled());
    }
    potkavg /= Properties::sourcesNum;
    tobslavgt /= Properties::sourcesNum;
    tbpavgt /= Properties::sourcesNum;
    dispobslavgt /= Properties::sourcesNum;
    dispbpavgt /= Properties::sourcesNum;
    *pOtk << QPointF(Properties::lambdaDevices, potkavg);
    *tobslavg << QPointF(Properties::lambdaDevices, tobslavgt);
    *tbpavg << QPointF(Properties::lambdaDevices, tbpavgt);
    *dispobslavg << QPointF(Properties::lambdaDevices, dispobslavgt);
    *dispbpavg << QPointF(Properties::lambdaDevices, dispbpavgt);
  }

  Properties::lambdaDevices = lambdaD;
  emit potk(4, pOtk);
  emit usageKoef(4, usageKoef1, usageKoefavg);
  emit tbp(4, tbpavg, dispbpavg);
  emit tobsl(4, tobslavg, dispobslavg);
}

void BMS::stepByStep() {
  flagEnd = false;
  srand(time(0));
  rand();
  Device devices[7];
  for (int i = 0; i < Properties::devicesNum; i++) {
    devices[i] = Device(i + 1, Properties::lambdaDevices);
  }
  Source sources[7];
  for (int i = 0; i < Properties::sourcesNum; i++) {
    sources[i] = Source(i + 1, Properties::lambdaSources);
  }
  Buffer buffer(Properties::bufferCapacity);

  double time = 0;
  QString str("Time: ");
  std::string a = std::to_string(time);
  QString tmp = a.c_str();
  str.append(tmp);
  str.append("\n");
  emit timeSend(str);
  std::cout << "Time: " << time << std::endl;
  printinfo(devices, sources, &buffer);

  while (flagNext == false) {
    if (flagEnd == true) {
      break;
    }
  }
  if (flagEnd == true) {
    flagNext = false;
  }
  while (BMS::flagNext == true) {
    double minTime = sources[0].getNextReqTime();
    BOS_TYPE bosType = BOS_TYPE::SOURCE;
    int tmpNum = 0;

    for (int i = 0; i < Properties::devicesNum; ++i) {
      if (devices[i].getReleaseTime() < minTime && !devices[i].isWaiting()) {
        minTime = devices[i].getReleaseTime();
        bosType = BOS_TYPE::DEVICE;
        tmpNum = i;
      }
    }       // device's release

    for (int i = 0; i < Properties::sourcesNum; ++i) {
      if (sources[i].getNextReqTime() < minTime) {
        minTime = sources[i].getNextReqTime();
        bosType = BOS_TYPE::SOURCE;
        tmpNum = i;
      }
    }   // request generation
    switch (bosType) {
      case BOS_TYPE::DEVICE: {
        if (buffer.isEmpty()) {
          time = devices[tmpNum].release();
          std::cout << "Time: " << time << std::endl;
          QString str("Time: ");
          std::string a = std::to_string(time);
          QString tmp = a.c_str();
          str.append(tmp);
          str.append("\n");
          std::cout << "Release of the device " << devices[tmpNum].getNum() << std::endl;
          str.append("Release of the device ");
          a = std::to_string(devices[tmpNum].getNum());
          tmp = a.c_str();
          str.append(tmp);
          str.append("\n");
          emit timeSend(str);
          printinfo(devices, sources, &buffer);
          std::cout << "Buffer is empty. Device is waiting\n";
          QString str2("Buffer is empty. Device is waiting\n");
          emit stringSend(str2);
          devices[tmpNum].wait();
          printinfo(devices, sources, &buffer);
        } else {
          time = devices[tmpNum].release();
          std::cout << "Time: " << time << std::endl;
          QString str("Time: ");
          std::string a = std::to_string(time);
          QString tmp = a.c_str();
          str.append(tmp);
          str.append("\n");
          std::cout << "Release of the device " << devices[tmpNum].getNum() << std::endl;
          str.append("Release of the device ");
          a = std::to_string(devices[tmpNum].getNum());
          tmp = a.c_str();
          str.append(tmp);
          str.append("\n");
          emit timeSend(str);
          printinfo(devices, sources, &buffer);
          QString str2("Placing a request from buffer on the device ");
          a = std::to_string(devices[tmpNum].getNum());
          tmp = a.c_str();
          str2.append(tmp);
          str2.append("\n");
          std::cout << "Placing a request from buffer on the device " << devices[tmpNum].getNum() << std::endl;
          Request r = buffer.pop(str2);
          str2.append("Request ");
          a = std::to_string(r.getRequestNumber()[0]);
          tmp = a.c_str();
          str2.append(tmp);
          a = std::to_string(r.getRequestNumber()[1]);
          tmp = a.c_str();
          str2.append(tmp);
          str2.append(". Generation time ");
          a = std::to_string(r.getGenerationTime());
          tmp = a.c_str();
          str2.append(tmp);
          str2.append("\n");
          std::cout << "Request " << r.getRequestNumber()[0] << r.getRequestNumber()[1]
                    << ". Generation time " << r.getGenerationTime() << std::endl;
          devices[tmpNum].take(time, str2);
          stringSend(str2);
          printinfo(devices, sources, &buffer);
        }
        break;
      } case BOS_TYPE::SOURCE: {
        int devNum = -1;
        for (int i = 0; i < Properties::devicesNum; ++i) {
          if (devices[i].isWaiting()) {
            devNum = i;
            break;
          }
        }
        time = sources[tmpNum].getNextReqTime();
        if (devNum != -1) {
          Request r = sources[tmpNum].generate(time);
          std::cout << "Time: " << time << " \n";
          QString str("Time: ");
          std::string a = std::to_string(time);
          QString tmp = a.c_str();
          str.append(tmp);
          str.append("\n");
          std::cout << "Generating request. ";
          str.append("Generating request. ");
          std::cout << "Request " << r.getRequestNumber()[0] << r.getRequestNumber()[1]
                    << ". Generation time " << r.getGenerationTime() << std::endl;
          str.append("Request ");
          a = std::to_string(r.getRequestNumber()[0]);
          tmp = a.c_str();
          str.append(tmp);
          a = std::to_string(r.getRequestNumber()[1]);
          tmp = a.c_str();
          str.append(tmp);
          str.append(". Generation time ");
          a = std::to_string(r.getGenerationTime());
          tmp = a.c_str();
          str.append(tmp);
          str.append("\n");
          std::cout << "Placing a request on the free device" << std::endl;
          str.append("Placing a request on the free device");
          str.append("\n");
          devices[devNum].take(time, str);
          emit timeSend(str);
          printinfo(devices, sources, &buffer);
        } else {
          Request r = sources[tmpNum].generate(time);
          std::cout << "Time: " << time << " \n";
          QString str("Time: ");
          std::string a = std::to_string(time);
          QString tmp = a.c_str();
          str.append(tmp);
          str.append("\n");
          std::cout << "Generating request. ";
          str.append("Generating request. ");
          std::cout << "Request " << r.getRequestNumber()[0] << r.getRequestNumber()[1]
                    << ". Generation time " << r.getGenerationTime() << std::endl;
          str.append("Request ");
          a = std::to_string(r.getRequestNumber()[0]);
          tmp = a.c_str();
          str.append(tmp);
          a = std::to_string(r.getRequestNumber()[1]);
          tmp = a.c_str();
          str.append(tmp);
          str.append(". Generation time ");
          a = std::to_string(r.getGenerationTime());
          tmp = a.c_str();
          str.append(tmp);
          str.append("\n");
          std::cout << "No free devices. Placing a request in the buffer " << std::endl;
          str.append("No free devices. Placing a request in the buffer \n");
          int cancelled = buffer.push(r, str);
          sources[cancelled - 1].cancell();
          emit timeSend(str);
          printinfo(devices, sources, &buffer);
        }
        break;
      }
    }
    flagNext = false;
    while (flagNext == false) {
      if (flagEnd == true) {
        break;
      }
    }
    if (flagEnd == true) {
      break;
    }
  }
}

void BMS::setTrue()
{
  flagNext = true;
}

