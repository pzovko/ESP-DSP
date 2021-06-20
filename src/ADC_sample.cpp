#include "main.h"

TaskHandle_t thSamplingLoop;
QueueHandle_t qhSample;

uint16_t *uiDataBuffer;


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
        configure_i2s();
        if(pdPASS != xTaskCreatePinnedToCore(SamplingLoop, "Sampling_task", DATA_STACK_SIZE, NULL, 1, &thSamplingLoop, ADC_SAMPLE_CORE))
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
        ADC_Sampling(uiDataBuffer);

        for(int uiBufferCount = 0; uiBufferCount < NUM_SAMPLES; uiBufferCount++)
        {
            *(uiDataBuffer + uiBufferCount) = *(uiDataBuffer + uiBufferCount)& 0xFFF;
            xQueueSend(qhSample, (void*)(uiDataBuffer + uiBufferCount), portMAX_DELAY);
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