#ifndef SMO_DEVICE_H
#define SMO_DEVICE_H

class Device {
public:
    Device();
    Device(int);

    double take(double);
    double release();
    void wait();
    double getReleaseTime();
    bool availability();
    bool isWaiting();

private:
    bool isAvailable_;
    bool isWaiting_;
    double releaseTime_;
    int num_;
};


#endif
