
#ifndef SMO_REQUEST_H
#define SMO_REQUEST_H


class Request {
public:
    Request();
    Request(double, int, int);

    double getGenerationTime() const;
    const int * getRequestNumber() const;

private:
    double generationTime_;
    int requestNumber_[2];
};

#endif
