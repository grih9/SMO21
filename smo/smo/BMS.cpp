#include "BMS.h"
#include <iostream>
#include <time.h>
#include "properties.h"

bool BMS::flagNext = true;
bool BMS::flagEnd = false;

enum class BOS_TYPE {
    DEVICE,
    SOURCE
};

void BMS::printinfo(const Device * devices, const Source * sources, const Buffer * buffer)
{
  QString str("Devices: ");
  std::string a = std::to_string(1);
  QString tmp = a.c_str();
  std::cout << "Devices:\n";
  for (int i = 0; i < Properties::devicesNum; ++i) {
    str.append("\t");
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
    str.append(", Next request: ");
    a = std::to_string(sources[i].getNextReqTime());
    tmp = a.c_str();
    str.append(tmp);
    str.append("\n");
    std::cout << "\t" << sources[i].getSourceNum() << ". Generated: " << sources[i].getSourceCount() << ", Next request: "
              << sources[i].getNextReqTime() << "\n";

  }
  str.append("Buffer:\n");
  std::cout << "Buffer:\n";
  emit string(str);
  buffer->printBufferInfo();
  std::cout << "\n";
}

void BMS::doWork() {
  flagEnd = false;
  srand(time(0));
  rand();

  Device devices[7] = { Device(1, Properties::lambdaDevices), Device(2, Properties::lambdaDevices), Device(3, Properties::lambdaDevices) };
  Source sources[7] = { Source(1, Properties::lambdaSources), Source(2, Properties::lambdaSources), Source(3, Properties::lambdaSources) };
  Buffer buffer(Properties::bufferCapacity);

  double time = 0;
  QString str("Time: ");
  std::string a = std::to_string(time);
  QString tmp = a.c_str();
  str.append(tmp);
  emit string(str);
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
          std::cout << "Release of the device " << devices[tmpNum].getNum() << std::endl;
          printinfo(devices, sources, &buffer);
          std::cout << "Buffer is empty. Device is waiting\n";
          devices[tmpNum].wait();
          printinfo(devices, sources, &buffer);
        } else {
          time = devices[tmpNum].release();
          std::cout << "Time: " << time << std::endl;
          std::cout << "Release of the device " << devices[tmpNum].getNum() << std::endl;
          printinfo(devices, sources, &buffer);
          std::cout << "Placing a request from buffer on the device " << devices[tmpNum].getNum() << std::endl;
          Request r = buffer.pop();
          std::cout << "Request " << r.getRequestNumber()[0] << r.getRequestNumber()[1]
                    << ". Generation time " << r.getGenerationTime() << std::endl;
          devices[tmpNum].take(time);
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
          std::cout << "Generating request. ";
          std::cout << "Request " << r.getRequestNumber()[0] << r.getRequestNumber()[1]
                    << ". Generation time " << r.getGenerationTime() << std::endl;
          std::cout << "Placing a request on the free device" << std::endl;
          devices[devNum].take(time);
          printinfo(devices, sources, &buffer);
        } else {
          Request r = sources[tmpNum].generate(time);
          std::cout << "Time: " << time << " \n";
          std::cout << "Generating request. ";
          std::cout << "Request " << r.getRequestNumber()[0] << r.getRequestNumber()[1]
                    << ". Generation time " << r.getGenerationTime() << std::endl;
          std::cout << "No free devices. Placing a request in the buffer " << std::endl;
          buffer.push(r);
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

