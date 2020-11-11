#include <cstdlib>
#include <cmath>
#include <iostream>
#include "Device.h"
#include "properties.h"


Device::Device():
    Device(0, 1)
{
}

Device::Device(int num, double lambda):
    lambda_(lambda),
    numberOfTaken_(0),
    isAvailable_(true),
    releaseTime_(0),
    num_(num),
    isWaiting_(true)
{
}

double Device::release()
{
    isAvailable_ = true;
    return releaseTime_;
}

double Device::take(double time) {
    isAvailable_ = false;
    isWaiting_ = false;
    numberOfTaken_++;
    releaseTime_ = time + exp(lambda_);
    std::cout << "Device " << num_ <<" took a request " << std::endl;
    std::cout << "Future release time is " << releaseTime_ << std::endl;
    return releaseTime_;
}

double Device::getReleaseTime() const {
    return releaseTime_;
}

bool Device::availability() const {
    return isAvailable_;
}

bool Device::isWaiting() const {
    return isWaiting_;
}

void Device::wait() {
    isWaiting_ = true;
}

int Device::getNum() const {
    return num_;
}

double Device::getNumberOfTaken() const {
    return numberOfTaken_;
}
