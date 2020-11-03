#include "Request.h"

Request::Request():
    Request(-1, 0,0 )
{
}

Request::Request(double generationTime, int sourceNum, int sourceCount):
    generationTime_(generationTime),
    requestNumber_{ sourceNum, sourceCount }
{
}

double Request::getGenerationTime() const
{
    return generationTime_;
}

const int * Request::getRequestNumber() const {
    return requestNumber_;
}
