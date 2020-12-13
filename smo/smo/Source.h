#ifndef SMO_SOURCE_H
#define SMO_SOURCE_H

#include "Request.h"

class Source {
public:
    Source();
    Source(int, double);
    Source(int, double, int, double, int);

    double getNextReqTime() const;
    int getSourceNum() const;
    int getSourceCount() const;
    int getSourceCancelled() const;
    Request generate(double);
    void cancell();

private:
    double lambda_;
    double nextReqTime_;
    int sourceNum_;
    int sourceCount_;
    int sourceCancelled_;
};

#endif
