#include "Buffer.h"
#include <iostream>

Buffer::Buffer():
    Buffer(0)
{
}

Buffer::Buffer(int capacity):
        capacity_(capacity),
        pushPosition_(0),
        popPosition_(0),
        volume_(0)
{
    for (int i = 0; i < capacity; i++) {
        buf_[i] = Request();
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
        buf_[minNum] = request;
        std::cout << "Removed request with generationTime " << minTime << " from " << minNum << std::endl;
        std::cout << "Insert in " << minNum;
        pushPosition_ = (minNum + 1) % capacity_;
        return false;
    }
}

Request Buffer::pop()
{
    if (volume_ != 0) {
        while (buf_[popPosition_].getGenerationTime() == -1) {
            popPosition_++;
            popPosition_ = popPosition_ % capacity_;
        }
        Request tmp = buf_[popPosition_];
        buf_[popPosition_] = Request();
        --volume_;
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
