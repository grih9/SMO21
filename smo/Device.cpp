#include <cstdlib>
#include <cmath>
#include <iostream>
#include "Device.h"
#include "properties.h"


Device::Device():
    Device(0)
{
}

Device::Device(int num):
    isAvailable_(true),
    releaseTime_(0),
    num_(num),
    isWaiting_(true)
{
}

double Device::release()
{
    isAvailable_ = true;
    std::cout << "Прибор " << num_ <<" свободен " << std::endl;
    return releaseTime_;
}

double Device::take(double time) {
    isAvailable_ = false;
    isWaiting_ = false;
    double r = rand() / double(RAND_MAX);
    releaseTime_ = time + (-1.0/Properties::lambda * log(r));
    std::cout << "Прибор " << num_ <<" получил заявку " << std::endl;
    std::cout << "Время будущего освобождения " << releaseTime_ << std::endl;
    return releaseTime_;
}

double Device::getReleaseTime() {
    return releaseTime_;
}

bool Device::availability() {
    return isAvailable_;
}

bool Device::isWaiting() {
    return isWaiting_;
}

void Device::wait() {
    isWaiting_ = true;
}
