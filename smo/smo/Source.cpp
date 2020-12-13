#include <cstdlib>
#include "Source.h"
#include <cmath>
#include "properties.h"
#include <iostream>

Source::Source():
    Source(0, 0, 0)
{

}

Source::Source(int num, double lambda):
    Source(num, lambda, (-1.0 / lambda * log(rand() / double(RAND_MAX))))
{
}

Source::Source(int num, double lambda, double nextReqTime):
    lambda_(lambda),
    nextReqTime_(nextReqTime),
    sourceNum_(num),
    sourceCount_(0),
    sourceCancelled_(0),
    tobsl_(0),
    tbp_(0),
    dispbp_(0),
    dispobsl_(0)
{
  if (nextReqTime == INFINITY) {
    std::cerr << "error with generate request time";
  }
}

Request Source::generate(double time)
{
    Request req(nextReqTime_, sourceNum_, ++sourceCount_);
    double r = rand() / double(RAND_MAX);
    if (r == 0) {
      r = 0.01;
    }
    nextReqTime_ += (-1.0/lambda_ * log(r));
    return req;
}

double Source::getNextReqTime() const {
    return nextReqTime_;
}

int Source::getSourceCount() const {
    return sourceCount_;
}

int Source::getSourceNum() const {
    return sourceNum_;
}

int Source::getSourceCancelled() const {
    return sourceCancelled_;
}

double Source::getTimeBp() const {
  return tbp_;
}

double Source::getTimeObsl() const {
  return tobsl_;
}

void Source::setTimeBp(double time) {
  tbp_ += time;
}

void Source::setTimeObsl(double time) {
  tobsl_ += time;
}

double Source::getDispBp() const {
  return dispbp_;
}

double Source::getDispObsl() const {
  return dispobsl_;
}

void Source::setDispBp(double time) {
  dispbp_ += time;
}

void Source::setDispObsl(double time) {
  dispobsl_ += time;
}

void Source::cancell() {
  sourceCancelled_++;
}
