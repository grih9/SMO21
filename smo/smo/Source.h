#ifndef SMO_SOURCE_H
#define SMO_SOURCE_H

#include "Request.h"

class Source {
public:
    Source();
    Source(int, double);
    Source(int, double, int, double);

    double getNextReqTime() const;
    int getSourceNum() const;
    int getSourceCount() const;
    Request generate(double);

private:
    double lambda_;
    double nextReqTime_;
    int sourceNum_;
    int sourceCount_;
};

#endif
