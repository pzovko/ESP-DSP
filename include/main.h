#include <BluetoothSerial.h> 
#include <Arduino.h>
#include <FreeRTOS.h>
#include <driver/i2s.h>
#include <driver/adc.h>
#include <soc/syscon_reg.h>

#include "DMA.h"
#include "DSP.h"
#include "SendData.h"
#include "ADC_sample.h"


#define Delay(t) vTaskDelay(t/portTICK_PERIOD_MS)

#define ADC_SAMPLE_CORE 1
#define DSP_CORE        1
#define DATA_CORE       1

#define QUEUE_SIZE 10000

#define BAUDRATE     115200
#define DEBUG        1
#define CHECK_BUFFER 0

typedef struct _settings{
    bool filter_enable;
    uint16_t SampleRate;
    uint16_t FirOrder;
    float* FilterCoeff;
}settings;

extern settings Settings;

extern BluetoothSerial btSerial;
extern QueueHandle_t qhSample;
extern QueueHandle_t qhSend;

