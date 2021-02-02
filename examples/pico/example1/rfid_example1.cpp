/**
 * Copyright (c) 2020 SparkFun Electronics
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "qwiic_rfid.h"

Qwiic_Rfid rfidReader;

float valid_delay = 1.0;

int main() {

	const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
	gpio_put(LED_PIN, 1);


     // setup stdio. Output device is controlled in CMakeLists.txt file
    stdio_init_all();

	printf("Starting RFID Test\n\n");

    // Init the system 

    if(!rfidReader.begin()){
    	printf("Error starting the RFID Reader. Aboarting\n");
  		return 0;
    }

    SparkFunRFIDEvent rfidEvent;

    while (1) {
        gpio_put(LED_PIN, 1);
        sleep_ms(250);

        printf("LOOP \n");

        // Get the last event - if there was one
        if(rfidReader.getLastEvent(rfidEvent)){
            // okay, we got a valid tag.

            // Within our Valid time
            if(rfidEvent.time < valid_delay)
                printf("VALID TAG:"); 
            else
                printf("Expired TAG:");

            printf(" [%s]\n", rfidEvent.tag); 

        }   

    	gpio_put(LED_PIN, 0);
        sleep_ms(2000);

    }

    return 0;
}
