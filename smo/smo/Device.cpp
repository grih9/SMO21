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
    workingTime_(0),
    lambda_(lambda),
    numberOfTaken_(0),
    releaseTime_(0),
    num_(num),
    isAvailable_(true),
    isWaiting_(true)
{
}

double Device::release()
{
    isAvailable_ = true;
    return releaseTime_;
}

double Device::take(double time, QString & str) {
  isAvailable_ = false;
  isWaiting_ = false;
  numberOfTaken_++;
  double r = rand() / double(RAND_MAX);
  if (r == 0) {
    r = 0.01;
  }
  releaseTime_ = time + (-1.0/lambda_ * log(r));
  //std::cout << "Device " << num_ << " took a request " << std::endl;
  str.append("Device ");
  std::string a = std::to_string(num_);
  QString tmp = a.c_str();
  str.append(tmp);
  str.append(" took a request ");
  str.append("\n");
  //std::cout << "Future release time is " << releaseTime_ << std::endl;
  str.append("Future release time is ");
  a = std::to_string(releaseTime_);
  tmp = a.c_str();
  str.append(tmp);
  workingTime_ += releaseTime_ - time;
  str.append("\n");
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

int Device::getNumberOfTaken() const {
  return numberOfTaken_;
}

double Device::getWorkingTime() const {
  return workingTime_;
}
