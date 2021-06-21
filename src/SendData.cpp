#include "main.h"

TaskHandle_t thSendData;
Data dataSend;
BaseType_t err;

bool InitSendData(void)
{
     if(pdPASS !=(err = xTaskCreatePinnedToCore(SendData_loop, "SendData_task", DATA_STACK_SIZE, NULL, 1, &thSendData, DATA_CORE)))
     {
         #if DEBUG
            Serial.print("Error creating SendData task.");     
            Serial.println(err);
         #endif
        return true;
     }
     return false;
}
  
  
void SendData_loop(void * param)
{
    while(true)
    {
        if(qhSend != NULL)
        {
            while(uxQueueMessagesWaiting(qhSend))
            {
                if(pdFALSE == xQueueReceive(qhSend,(void*)&dataSend, 0))
                {
                    #if DEBUG
                        Serial.println("Raw data queue full.");
                    #endif
                    break;
                }

                if(Settings.filter_enable)
                {
                  btSerial.println((PackData(dataSend.raw, dataSend.filtered))^Settings.SharedKey);
                }  
                else
                {
                  btSerial.println((dataSend.raw^Settings.SharedKey));                  
                } 
               vTaskDelay(1); 
            }      
        }  
    }
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

int32_t PackData(int16_t raw, int16_t filtered)
{
    return (int32_t)((raw << 16)|(filtered&65535));
}