#include "main.h"

TaskHandle_t thSendData;

bool InitSendData(void)
{

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
    int i = 0;
    bool  toggle = false;
    //vTaskDelay(500/portTICK_PERIOD_MS);
    while(true)
    {
        
       // btSerial.println(analogRead(15));
       
       // Serial.println(i);
        i++;
        if(i == 600)
        {
            toggle = !toggle;
             
            i = 0;
        }

        if(toggle)
        {
            btSerial.println(PackData(2000, 0));
        }  
        else
        {
            btSerial.println(PackData(0, 2000));
        }
            

       // Delay(10);

         
       // Delay(20);
       //Delay(1);
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