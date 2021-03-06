#ifndef SMO_DEVICE_H
#define SMO_DEVICE_H

#include <QString>

class Device {
public:
    Device();
    Device(int, double);

    double take(double, QString &);
    double release();
    void wait();
    double getReleaseTime() const;
    bool availability() const;
    bool isWaiting() const;
    int getNum() const;
    int getNumberOfTaken() const;
    double getWorkingTime() const;

private:
    double workingTime_;
    double lambda_;
    int numberOfTaken_;
    double releaseTime_;
    int num_;
    bool isAvailable_;
    bool isWaiting_;
};


#endif
