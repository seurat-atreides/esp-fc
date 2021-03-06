#ifndef _ESPFC_DEBUG_H_
#define _ESPFC_DEBUG_H_

#include <EspGpio.h>

#if 0
#define PIN_DEBUG(v) EspGpio::digitalWrite(D0, v)
#define PIN_DEBUG_INIT() pinMode(D0, OUTPUT)
#else
#define PIN_DEBUG(v)
#define PIN_DEBUG_INIT()
#endif

#if 0
#define LOG_SERIAL_INIT() Serial.begin(115200)
#define LOG_SERIAL_DEBUG(v) Serial.print(' '); Serial.print(v)
#else
#define LOG_SERIAL_INIT()
#define LOG_SERIAL_DEBUG(v)
#endif

#endif
