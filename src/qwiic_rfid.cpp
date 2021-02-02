/*
  This library is for the SparkFun Qwiic RFID Card Reader. It provides functions to retrieve
  RFID Card IDs that have been scanned by the Qwiic RFID Reader and their times. 
  By: Elias Santistevan
  Date: July 2019
  License: This code is public domain but you buy me a beer if you use this and 
  we meet someday (Beerware license).

  Feel like supporting our work? Buy one from SparkFun Electronics!
  https://www.sparkfun.com/products/15191
 */

#include <stdio.h>
#include <string.h>
#include "qwiic_i2c.h"
#include "qwiic_rfid.h"

#define kSparkFunRFIDChangeAddrReg 0xC7

#define kDeviceTransactionSize 10
#define kSparkFunRFIDTimeOffset   6

const char kEmptyTag[]="000000";

// Qwiic Driver/I2C connection

Qwiic_I2C qwiic;
//--------------------------------------------------------------------------------

Qwiic_Rfid::Qwiic_Rfid(uint8_t deviceAddress){

    _deviceAddress = deviceAddress;

}
bool Qwiic_Rfid::begin(){

    // init I2C.

    if(!qwiic.init()){
        printf("Error: I2C subsystem failed to initialize.");
        return false;
    }


    return isConnected();
}

//--------------------------------------------------------------------------------
bool Qwiic_Rfid::isConnected(void){

    return qwiic.pingDevice(_deviceAddress);
}
//--------------------------------------------------------------------------------
// This function changes the I2C address of the Qwiic RFID. The address
// is written to the memory location in EEPROM that determines its address.
bool Qwiic_Rfid::changeAddress(uint8_t newAddress) {

    bool rc = qwiic.writeRegister(_deviceAddress, kSparkFunRFIDChangeAddrReg, newAddress);

    // Success?
    if(rc)
        _deviceAddress = newAddress;

    return rc;
 
}

//--------------------------------------------------------------------------------
// get 
int Qwiic_Rfid::getLastEvents(SparkFunRFIDEvent *pEventBuffer, int nBuffer){

    // The transaction with the device involves a block of 10 bytes.
    // defined above by kDeviceTransactionSize
    uint8_t uiBuffer[kDeviceTransactionSize]; 

    // clear out our incoming buffer
    memset(pEventBuffer, '\0', sizeof(SparkFunRFIDEvent)*nBuffer);

    // loop over the desired number of events - make sure we're not a max 
    // device buffer size
    int nRequested = nBuffer < kMaxEventsAvailable ? nBuffer : kMaxEventsAvailable;

    int nRetrieved=0;

    int32_t evTime;
    int rc;

    for(int i; i < nRequested; i++){

        rc = qwiic.readRegisterRegion(_deviceAddress, 0, uiBuffer, sizeof(uiBuffer));
        if(rc == QWIIC_ERROR_GENERIC){

            // Error reciving data from the device 
            printf("[RFID] Error recieving tag data\n");
            return -1;
        }

        // process the results - the first 6 bytes are the tag number = char = number (0-255)
        // Each bytes numeric value is determined, then packed to form the overall tag id. 
        // This works, but wouldn't a uint64 be easier? Maybe not supported on the device side...
        snprintf(pEventBuffer[i].tag, kSparkFunRFIDTagLen, "%u%u%u%u%u%u", 
            uiBuffer[0], uiBuffer[1], uiBuffer[2], uiBuffer[3], uiBuffer[4], uiBuffer[5]);

        // if the tag an "Empty tag" don't process time - leave at 0.
        if(!strncmp(pEventBuffer[i].tag, kEmptyTag, kSparkFunRFIDTagLen))
            continue;

        // The last 4 bytes of the transfer buffer represent an int - cast to int pointer,
        // deref and assign. Then move to float and adjust for seconds. Breakup for readability...
        // NOTE: The float should of been sent as LSB->MSB -- would of made this line easlier.
         pEventBuffer[i].time =   (float)(  uiBuffer[kSparkFunRFIDTimeOffset]   << 24 |
                                            uiBuffer[kSparkFunRFIDTimeOffset+1] << 16 |
                                            uiBuffer[kSparkFunRFIDTimeOffset+2] <<  8 |
                                            uiBuffer[kSparkFunRFIDTimeOffset+3]) / 1000.0;

        nRetrieved++;  // incr our "got a valid event counter"

    }

    return nRetrieved;
}



