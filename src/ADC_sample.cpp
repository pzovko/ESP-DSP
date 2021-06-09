#include "master.h"

SemaphoreHandle_t shDataStream = NULL;

TaskHandle_t thSamplingLoop;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

BaseType_t  HPTaskWoken = 0;

QueueHandle_t qhSample;

uint16_t *uiDataBuffer;


void IRAM_ATTR onTimer()
{ 
  uint16_t uiSample;

 portENTER_CRITICAL_ISR(&timerMux);
   uiSample = analogRead(ADC_PIN);

   if(pdFALSE == xQueueIsQueueFullFromISR(qhSample))
   {
      xQueueSendFromISR(qhSample, (void*)&uiSample, &HPTaskWoken);
   }
 portEXIT_CRITICAL_ISR(&timerMux);
}

bool InitSampling(void)
{
    if(NULL == (uiDataBuffer = (uint16_t*)pvPortMalloc(DATA_BUFFER_SIZE*sizeof(uint16_t))))
    {
        #if DEBUG
            Serial.println("Error creating data buffer.");
        #endif
        return true;
    }
    else
    {
        ResetBuffer(uiDataBuffer, DATA_BUFFER_EOB);
    }
    
    if(NULL == (qhSample = xQueueCreate(QUEUE_LEN, sizeof(uint16_t))))
    {
        #if DEBUG
            Serial.println("Error creating sampling queue.");
        #endif
        return true;
    }
    else
    {
        timer = timerBegin(0, SAMPLE_PRESCALER, true);
        timerAttachInterrupt(timer, &onTimer, true);
        timerAlarmWrite(timer, 1, true);
        timerAlarmEnable(timer);

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
    uint16_t uiBufferCount = 0;

    if(NULL == (shDataStream =xSemaphoreCreateBinary()))
    {
        #if DEBUG
            Serial.println("Error creating semaphore.");
        #endif
    }
    else
    {
        if(pdFALSE == xSemaphoreGive(shDataStream))
        {
            #if DEBUG
                Serial.println("Error giving semaphore.");
            #endif 
        }
    }
    
    while(true)
    {   
        if(pdTRUE == xSemaphoreTake(shDataStream, 0))
        {  
            if(uxQueueMessagesWaiting(qhSample))
            {
                #if CHECK_BUFFER
                    Serial.print(uxQueueMessagesWaiting(qhSample));
                    Serial.print(',');
                #endif

                ResetBuffer(uiDataBuffer, DATA_BUFFER_EOB);
                while(pdTRUE == xQueueReceive(qhSample,(void*)(uiDataBuffer + uiBufferCount), 0))
                {
                    if(uiBufferCount == (DATA_BUFFER_SIZE - 1))
                    {
                        #if DEBUG
                            Serial.println("Data buffer too small.");
                            Serial.println(uiBufferCount);
                        #endif 
                        break;
                    }
                    uiBufferCount++;
                }

                #if CHECK_BUFFER
                    Serial.print(uiBufferCount);
                    Serial.print('\r');
                #endif 

                uiBufferCount = 0;     
            }

            if(pdFALSE == xSemaphoreGive(shDataStream))
            {
                #if DEBUG
                    Serial.println("Error giving semaphore.");
                #endif 
                break;
            }
        }

        Delay(50);  
    }
}

bool StopSamplingLoop(void)
{
    if(thSamplingLoop != NULL)
    {
        vTaskDelete(thSamplingLoop);
        return false;
    }
    else 
        return true;    
}

uint16_t* GetBuffer(void)
{
    return uiDataBuffer;
}

void ResetBuffer(uint16_t* buffer, uint16_t eob)
{
    for(uint16_t count = 0; count <= (DATA_BUFFER_SIZE - 1); count++)
        *(buffer + count) = eob;
}