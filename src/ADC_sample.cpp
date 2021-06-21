#include "main.h"

TaskHandle_t thSamplingLoop;
QueueHandle_t qhSample;

//BaseType_t  HPTaskWoken = 0;

uint16_t *uiDataBuffer;

//hw_timer_t * timer = NULL;
//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

/*void IRAM_ATTR onTimer()
{ 
  uint16_t uiSample;

  portENTER_CRITICAL_ISR(&timerMux);

    uiSample = analogRead(34);
    if(pdFALSE == xQueueIsQueueFullFromISR(qhSample))
    {
        xQueueSendFromISR(qhSample, (void*)&uiSample, &HPTaskWoken);
    }
    
  portEXIT_CRITICAL_ISR(&timerMux);
}*/

bool InitSampling(void)
{

    if(NULL == (uiDataBuffer = (uint16_t*)pvPortMalloc(NUM_SAMPLES*sizeof(uint16_t))))
    {
        #if DEBUG
            Serial.println("Error creating data buffer.");
        #endif
        return true;
    }

    if(NULL == (qhSample = xQueueCreate(QUEUE_SIZE, sizeof(uint16_t))))
    {
        #if DEBUG
            Serial.println("Error creating qhSample queue.");
        #endif
        return true;
    }
    else
    {
       /* timer = timerBegin(0, (uint16_t)(80000000L/Settings.SampleRate), true);
        timerAttachInterrupt(timer, &onTimer, true);
        timerAlarmWrite(timer, 1, true);
        timerAlarmEnable(timer);*/
        configure_i2s();
        if(pdPASS != xTaskCreatePinnedToCore(SamplingLoop, "Sampling_task", ADC_STACK_SIZE, NULL, 2, &thSamplingLoop, ADC_SAMPLE_CORE))
        {
            #if DEBUG
                Serial.println("Error creating sampling task.");
            #endif
            return true;
        }  
        return false;
    }
}

void SamplingLoop(void * param)
{  
    while(true)
    {   
        if(ADC_Sampling(uiDataBuffer))
        {
            for(int uiBufferCount = 0; uiBufferCount < NUM_SAMPLES; uiBufferCount++)
            {
                *(uiDataBuffer + uiBufferCount) = *(uiDataBuffer + uiBufferCount)& 0xFFF;
                xQueueSend(qhSample, (void*)(uiDataBuffer + uiBufferCount), portMAX_DELAY);
            }
        }
    }
}

bool StopSamplingLoop(void)
{
    if((thSamplingLoop != NULL)&&(qhSample != NULL))
    {
        vPortFree(uiDataBuffer);
        vQueueDelete(qhSample);
        vTaskDelete(thSamplingLoop);
        return false;
    }
    else 
        return true;    
}