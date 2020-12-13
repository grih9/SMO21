#ifndef SMO_SOURCE_H
#define SMO_SOURCE_H

#include "Request.h"

class Source {
public:
    Source();
    Source(int, double);
    Source(int, double, double);

    double getNextReqTime() const;
    int getSourceNum() const;
    int getSourceCount() const;
    int getSourceCancelled() const;
    double getTimeObsl() const;
    void setTimeObsl(double);
    double getDispObsl() const;
    void setDispObsl(double);
    double getTimeBp() const;
    void setTimeBp(double);
    double getDispBp() const;
    void setDispBp(double);
    Request generate(double);
    void cancell();

private:
    double lambda_;
    double nextReqTime_;
    int sourceNum_;
    int sourceCount_;
    int sourceCancelled_;
    double tobsl_;
    double tbp_;
    double dispbp_;
    double dispobsl_;
};

#endif
