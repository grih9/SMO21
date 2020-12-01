#include "BMS.h"
#include <iostream>
#include <time.h>
#include "properties.h"
#include "Device.h"
#include "Source.h"
#include "Buffer.h"
#include "dialog.h"

bool BMS::flagNext = true;

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

void BMS::doWork() {
  srand(time(0));
  rand();

  Device devices[Properties::devicesNum] = { Device(1, Properties::lambda), Device(2, Properties::lambda), Device(3, Properties::lambda) };
  Source sources[Properties::sourcesNum] = { Source(1, Properties::lambda), Source(2, Properties::lambda), Source(3, Properties::lambda) };
  Buffer buffer(Properties::bufferCapacity);

  double time = 0;
  std::cout << "Time: " << time << std::endl;
  printinfo(devices, sources, &buffer);

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
    while (flagNext == false)
    { }
  }
}

void BMS::setTrue()
{
  flagNext = true;
}

