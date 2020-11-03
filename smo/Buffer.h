#ifndef SMO_BUFFER_H
#define SMO_BUFFER_H
#include "Request.h"

class Buffer {
public:
    Buffer();
    Buffer(int);

    bool push(Request request);
    Request pop();
    bool isFull() const;
    bool isEmpty() const;

private:
    int capacity_;
    int pushPosition_;
    int popPosition_;
    int volume_;
    Request buf_[];
};

#endif
