#ifndef SMO_BMS1_H
#define SMO_BMS1_H

#include "Request.h"

class BMS1 {
public:
    static void work();
private:
    static Request createRequest();
    static Request addToBuffer();
    static Request delFromBuffer();
};


#endif