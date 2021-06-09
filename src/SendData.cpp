#include "master.h"

TaskHandle_t thSendData;
//uint16_t* buffer;

bool InitSendData(void)
{
   /* if(NULL == (buffer = (uint16_t*)pvPortMalloc(DATA_BUFFER_SIZE*sizeof(uint16_t))))
    {
        #if DEBUG
            Serial.println("Error creating temporary buffer.");
        #endif
        return true;
    }
    else
    {
        ResetBuffer(buffer);
    }
    */
     if(pdPASS != xTaskCreatePinnedToCore(SendData_loop, "SendData_task", DATA_STACK_SIZE, NULL, 1, &thSendData, DATA_CORE))
     {
         #if DEBUG
            Serial.println("Error creating SendData task.");     
         #endif
        return true;
     }
     return false;
}
  
  
void SendData_loop(void * param)
{
  /*  uint16_t* temp;

    while(true)
    {
        temp = GetBuffer();
        if((pdTRUE == xSemaphoreTake(shDataStream, 0))&&(*(temp + 0) != DATA_BUFFER_EOB))
        {
            
            for(uint16_t count = 0 ;(count < DATA_BUFFER_SIZE)&&(*(temp + count) != DATA_BUFFER_EOB); count++)
                *(buffer + count) = *(temp + count);

            if(pdFALSE == xSemaphoreGive(shDataStream))
            {
               #if DEBUG
                 Serial.println("Error giving semaphore.");
               #endif 
               break;
            }

            for(uint16_t count = 0 ;(count < DATA_BUFFER_SIZE)&&(*(buffer + count) != DATA_BUFFER_EOB); count++)
            { 
                btSerial.println(*(buffer + count));             
            }
            ResetBuffer(buffer);
        }
        vTaskDelay(500/portTICK_PERIOD_MS);
    }*/
}

bool StopSendData(void)
{
    if(thSendData != NULL)
    {
        vTaskDelete(thSendData);
        return false;
    }
    else 
        return true;    
}

