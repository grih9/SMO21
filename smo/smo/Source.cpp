#include <cstdlib>
#include "Source.h"
#include <cmath>
#include "properties.h"

Source::Source():
    Source(0, 0, 0, 0)
{

}

Source::Source(int num, double lambda):
    Source(num, lambda, 0, (-1.0 / lambda * log(rand() / double(RAND_MAX))))
{
}

Source::Source(int num, double lambda, int count, double nextReqTime):
    lambda_(lambda),
    nextReqTime_(nextReqTime),
    sourceNum_(num),
    sourceCount_(count)
{
}

Request Source::generate(double time)
{
    Request req(nextReqTime_, sourceNum_, ++sourceCount_);
    double r = rand() / double(RAND_MAX);
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