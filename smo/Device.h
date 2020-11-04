#ifndef SMO_DEVICE_H
#define SMO_DEVICE_H

class Device {
public:
    Device();
    Device(int);

    double take(double);
    double release();
    void wait();
    double getReleaseTime() const;
    bool availability() const;
    bool isWaiting() const;
    int getNum() const;

private:
    double releaseTime_;
    int num_;
    bool isAvailable_;
    bool isWaiting_;
};


#endif
