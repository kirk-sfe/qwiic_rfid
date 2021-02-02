



#pragma once 

#include "pico/stdlib.h"


#define SPARKFUN_RFID_DEFAULT_ADDR         0x7D
#define SPARKFUN_RFID_ALTERNATE_ADDR       0x7C

// max events in the devices buffer

#define kMaxEventsAvailable 20

// define our RFID data type -  6 char string AND float of time since tag read by device

#define kSparkFunRFIDTagLen   18


typedef struct sparkFunRFIDEvent_ {
  char tag[kSparkFunRFIDTagLen+1]; 
  float time; 
} SparkFunRFIDEvent;


class Qwiic_Rfid{ 

public:

    // logistic/standard interface
    Qwiic_Rfid(uint8_t deviceAddress = SPARKFUN_RFID_DEFAULT_ADDR);
    bool begin();
    bool isConnected(void);
    bool changeAddress(uint8_t newAddress);

    // get the last event Ref and Pointer editions
    bool getLastEvent(SparkFunRFIDEvent& theEvent){return getLastEvent(&theEvent);}
    bool getLastEvent(SparkFunRFIDEvent* pEvent){ return getLastEvents(pEvent, 1) > 0;};    

    // return number of events retrieved. Caller provides memory. 
    // 
    // Up to kMaxEventsAvailable can be retrieved.
    // 
    // Returns the number of events read, or -1 on error  
    int getLastEvents(SparkFunRFIDEvent *pEvents, int nEvents);


private:

    uint8_t _deviceAddress;

};

