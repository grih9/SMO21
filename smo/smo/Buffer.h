#ifndef SMO_BUFFER_H
#define SMO_BUFFER_H

#include "Request.h"
#include "QString"
#include "properties.h"

class Buffer {
public:
  Buffer();
  Buffer(int);

  bool push(Request request);
  Request pop();
  bool isFull() const;
  bool isEmpty() const;
  void printBufferInfo(QString & str) const;
  int getCancelled() const;

private:
  Request buf_[10];
  int capacity_;
  int cancelled_;
  int pushPosition_;
  int popPosition_;
  int volume_;
};

#endif
