#ifndef SMO_BUFFER_H
#define SMO_BUFFER_H
#include "Request.h"
#include "properties.h"

class Buffer {
public:
    Buffer();
    Buffer(int);

    bool push(Request request);
    Request pop();
    bool isFull() const;
    bool isEmpty() const;
    void printBufferInfo() const;

private:
    Request buf_[Properties::bufferCapacity];
    int capacity_;
    int pushPosition_;
    int popPosition_;
    int volume_;
};

#endif
