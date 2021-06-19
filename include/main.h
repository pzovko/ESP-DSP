#include <BluetoothSerial.h> 
#include <Arduino.h>
#include <FreeRTOS.h>

#include "DSP.h"
#include "SendData.h"
#include "ADC_sample.h"

#define Delay(t) vTaskDelay(t/portTICK_PERIOD_MS)

#define LED_PIN      2

#define ADC_SAMPLE_CORE 1
#define DSP_CORE        1
#define DATA_CORE       0

#define BAUDRATE     115200
#define DEBUG        1
#define CHECK_BUFFER 0

extern SemaphoreHandle_t shDataStream;
extern BluetoothSerial btSerial;