#ifndef SMO_BMS2_H
#define SMO_BMS2_H

#include "Request.h"

class BMS2 {
public:
    static void work();

private:
    static Request getFromBuffer();
    static void requestService();

};

#endif