#include <iostream>
#include <time.h>
#include "properties.h"
#include "Device.h"
#include "Source.h"
#include "Buffer.h"

enum class BOS_TYPE {
    DEVICE,
    SOURCE
};

int main()
{
    srand(time(0));
    rand();
    Device devices[Properties::devicesNum];
    Source sources[Properties::sourcesNum];
    Buffer buffer(Properties::bufferCapacity);

    devices[0] = Device(1);
    devices[1] = Device(2);
    devices[2] = Device(3);
    sources[0] = Source(Properties::lambda, 1);
    sources[1] = Source(Properties::lambda, 2);
    sources[2] = Source(Properties::lambda, 3);

    double time = 0;

    double minTime = sources[0].getNextReqTime();
    BOS_TYPE bosType = BOS_TYPE::SOURCE;
    int tmpNum = 0;

    std::cout << time;

    for (int i = 0; i < Properties::devicesNum; ++i) {
        if (devices[i].getReleaseTime() < minTime && !devices[i].isWaiting()) {
            minTime = devices[i].getReleaseTime();
            bosType = BOS_TYPE::DEVICE;
            tmpNum = i;
        }
    }       // освобождение прибора

    for (int i = 0; i < Properties::sourcesNum; ++i) {
        if (sources[i].getNextReqTime() < minTime) {
            minTime = sources[i].getNextReqTime();
            bosType = BOS_TYPE::SOURCE;
            tmpNum = i;
        }
    }   // генерация заявки

    switch (bosType) {
        case BOS_TYPE::DEVICE: {
            if (buffer.isEmpty()) {
                std::cout << "Освобождение прибора " << tmpNum <<", но пустой буфер. Простой прибора\n";
                time = devices[tmpNum].release();
                devices[tmpNum].wait();
                std::cout << time << std::endl;
            } else {
                std::cout << "Освобождение прибора" << tmpNum << std::endl;
                time = devices[tmpNum].release();
                std::cout << time << std::endl;
                std::cout << "Постановка заявка на обслуживание на прибор" << tmpNum << std::endl;
                Request r = buffer.pop();
                std::cout << "Запрос " << r.getRequestNumber()[0] << r.getRequestNumber()[1]
                          <<  ". Время поступления" << r.getGenerationTime() << std::endl;
                devices[tmpNum].take(time);
            }
        }
        case BOS_TYPE::SOURCE: {
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
                std::cout << "Постановка заявка на обслуживание на прибор" << devNum << std::endl;
                std::cout << "Запрос " << r.getRequestNumber()[0] << r.getRequestNumber()[1]
                          <<  ". Время поступления" << r.getGenerationTime() << std::endl;
                devices[tmpNum].take(time);
            }

        }

    }

    return 0;
}
