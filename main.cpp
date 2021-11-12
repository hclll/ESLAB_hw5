/*
 * Copyright (c) 2014-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "BufferedSerial.h"
#include "ThisThread.h"
#include "mbed.h"

// Adjust pin name to your board specification.
// You can use LED1/LED2/LED3/LED4 if any is connected to PWM capable pin,
// or use any PWM capable pin, and see generated signal on logical analyzer.

static BufferedSerial serial_port(USBTX, USBRX);
PwmOut led(PWM_OUT);

int main()
{
    // specify period first
    // led.period(4.0f);      // 4 second period
    // led.write(0.50f);      // 50% duty cycle, relative to period
    //led = 0.5f;          // shorthand for led.write()
    //led.pulsewidth(2);   // alternative to led.write, set duty cycle time in seconds

    serial_port.set_baud(9600);
    serial_port.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );

    // Application buffer to receive the data
    #define MAXIMUM_BUFFER_SIZE 10
    char buf[MAXIMUM_BUFFER_SIZE] = {0};

    printf("Control of LED dimmer by host terminal\n\r");
    printf("Press 'u' = brighter, 'd' = dimmer\n\r");
    int brightness=0;

    while (1) {
        if (serial_port.read(buf, sizeof(buf))) {
            // serial_port.write(buf, c);
           char c = buf[0];
            printf("%c\n", c);
            printf("brightness: %d\n", brightness);
            if((c == 'u') && (brightness < 100)) {
                brightness += 1;
                led = (float)brightness/100.0; }
            if((c == 'd') && (brightness > 0)) { 
                brightness -= 1;
                led = (float)brightness/100.0;
            }
        }
        ThisThread::sleep_for(0.001);
    }

    while (1);
}
