#include <cstdlib>
#include "Source.h"
#include <cmath>
#include "properties.h"

Source::Source():
    Source(0, 0, 0, 0)
{

}

Source::Source(double lambda, int num):
    Source(lambda, num, 0, (-1.0/Properties::lambda * log(rand() / double(RAND_MAX))))
{
}

Source::Source(double lambda, int num, int count, double nextReqTime):
    lambda_(lambda),
    sourceNum_(num),
    sourceCount_(count),
    nextReqTime_(nextReqTime)
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
