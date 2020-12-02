#include "Buffer.h"
#include <iostream>

Buffer::Buffer():
    Buffer(0)
{
}

Buffer::Buffer(int capacity):
        capacity_(capacity),
        pushPosition_(0),
        cancelled_(0),
        popPosition_(0),
        volume_(0)
{
    for (int i = 0; i < capacity; i++) {
        buf_[i] = Request(-1, 0, 0);
    }
}

bool Buffer::push(Request request)
{
    if (volume_ != capacity_) {
        while (buf_[pushPosition_].getGenerationTime() != -1) {
            pushPosition_++;
            pushPosition_ = pushPosition_ % capacity_;
        }
        std::cout << "Insert in " << pushPosition_ << std::endl;
        buf_[pushPosition_] = request;
        volume_++;
        pushPosition_ = (pushPosition_ + 1) % capacity_;
        return true;
    } else {
        int minNum = 0;
        double minTime = buf_[0].getGenerationTime();
        for (std::size_t i = 1; i < capacity_; ++i) {
            if (buf_[i].getGenerationTime() < minTime) {
                minNum = i;
                minTime = buf_[i].getGenerationTime();
            }
        }
        std::cout << "Removed request " << buf_[minNum].getRequestNumber()[0] << buf_[minNum].getRequestNumber()[1]
                  << " with generationTime " << minTime << " from " << minNum << std::endl;
        buf_[minNum] = request;
        cancelled_++;
        std::cout << "Insert in " << minNum << std::endl;
        pushPosition_ = (minNum + 1) % capacity_;
        return false;
    }
}

int Buffer::getCancelled() const
{
  return cancelled_;
}
Request Buffer::pop()
{
    if (volume_ != 0) {
        while (buf_[popPosition_].getGenerationTime() == -1) {
            popPosition_++;
            popPosition_ = popPosition_ % capacity_;
        }
        Request tmp = buf_[popPosition_];
        std::cout << "Extract from " << popPosition_ << std::endl;
        buf_[popPosition_] = Request();
        --volume_;
        popPosition_ = (popPosition_ + 1) % capacity_;
        return tmp;
    } else {
        std::cerr << "Buffer is empty\n";
        throw std::range_error("Buffer is empty\n");
    }
}

bool Buffer::isFull() const
{
    return volume_ == capacity_;
}

bool Buffer::isEmpty() const
{
    return volume_ == 0;
}

void Buffer::printBufferInfo() const
{
    std::cout << "\tCapacity = " << capacity_ << ", used = " << volume_ << ", cancelled = " << cancelled_ << std::endl;
    std::cout << "\tPush ptr = " << pushPosition_ << ", pop ptr = " << popPosition_ << std::endl;
    for (int i = 0; i < Properties::bufferCapacity; ++i) {
        std::cout << "\t" << i << ": ";
        double t = buf_[i].getGenerationTime();
        if (t == -1) {
            std::cout << "Empty\n";
        } else {
            std::cout << buf_[i].getRequestNumber()[0] << buf_[i].getRequestNumber()[1] << ", generation time = " << t << std::endl;
        }
    }
}
