#ifndef SMO_SOURCE_H
#define SMO_SOURCE_H

#include "Request.h"

class Source {
public:
    Source();
    Source(double, int);
    Source(double, int, int, double);

    double getNextReqTime() const;
    Request generate(double);

private:
    double lambda_;
    int sourceNum_;
    int sourceCount_;
    double nextReqTime_;
};

#endif
