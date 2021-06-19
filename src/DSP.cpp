#include "main.h"
#include "FIR.h"

float coeff[] = {0.0001084f,0.0007729f,0.0009660f,0.0004308f,-0.0006666f,-0.0016312f,-0.0015460f,0.0000000f,
                 0.0022551f,0.0034511f,0.0020203f,-0.0018317f,-0.0055820f,-0.0058130f,-0.0010072f,0.0063925f,
                 0.0107908f,0.0073370f,-0.0035766f,-0.0149768f,-0.0172373f,-0.0054598f,0.0149137f,0.0294065f,
                 0.0235953f,-0.0051087f,-0.0413467f,-0.0572824f,-0.0282764f,0.0501108f,0.1549645f,0.2447228f,
                 0.2800000f,0.2447228f,0.1549645f,0.0501108f,-0.0282764f,-0.0572824f,-0.0413467f,-0.0051087f,
                 0.0235953f,0.0294065f,0.0149137f,-0.0054598f,-0.0172373f,-0.0149768f,-0.0035766f,0.0073370f,
                 0.0107908f,0.0063925f,-0.0010072f,-0.0058130f,-0.0055820f,-0.0018317f,0.0020203f,0.0034511f,
                 0.0022551f,0.0000000f,-0.0015460f,-0.0016312f,-0.0006666f,0.0004308f,0.0009660f,0.0007729f};



FIRFilter stFIR;
float fFIRbuffer[FIR_ORDER];
int16_t *fir_buffer;
Data x;

QueueHandle_t qhRawData;
TaskHandle_t thDSP;


bool InitDSP(void)
{
    FIRFilter_Init(&stFIR, coeff, fFIRbuffer, FIR_ORDER);

    if(NULL == (fir_buffer = (int16_t*)pvPortMalloc(DATA_BUFFER_SIZE*sizeof(int16_t))))
    {
        #if DEBUG
            Serial.println("Error creating temporary buffer.");
        #endif
        return true;
    }
    else
    {
        for(int16_t count = 0; count < DATA_BUFFER_SIZE; count++)
            *(fir_buffer + count) = FIR_EOB;
    }

    if(NULL == (qhRawData = xQueueCreate(DSP_RAW_BUF_SIZE, sizeof(uint16_t))))
    {
        #if DEBUG
            Serial.println("Error creating RawData queue.");
        #endif
        return true;
    }

    if(pdPASS != xTaskCreatePinnedToCore(DSP_loop, "DSP_task", DSP_STACK_SIZE, NULL, 1, &thDSP, DSP_CORE))
    {
        #if DEBUG
           Serial.println("Error creating DSP task.");
        #endif
       return true;
    }
    return false;
}

void DSP_loop(void * param)
{
    uint16_t* temp;
    int i = 0;

    while(true)
    {
        if(pdTRUE == xSemaphoreTake(shDataStream, 0)) 
        {
            temp = GetBuffer();
            for(uint16_t count = 0 ;(count < DATA_BUFFER_SIZE)&&(*(temp + count) != DATA_BUFFER_EOB); count++)
            {
                if(pdTRUE != xQueueSend(qhRawData, (void*)(temp + count), 0))
                {
                    #if DEBUG
                        Serial.println("RawData queue full.");
                    #endif
                    break;
                }  
            }

            if(pdFALSE == xSemaphoreGive(shDataStream))
            {
               #if DEBUG
                 Serial.println("Error giving semaphore.");
               #endif 
               break;
            }


            uint16_t count = 0;

            while((uxQueueMessagesWaiting(qhRawData) >= DSP_SAMPLES)&&(count < DSP_SAMPLES))
            {
                if(pdFALSE == xQueueReceive(qhRawData,(void*)&x.raw[count], 0))
                {
                    #if DEBUG
                        Serial.println("Raw data queue full.");
                    #endif
                    break;
                }
              
                x.filtered[count] = FIRFilter_Update(&stFIR, (float)x.raw[count]);

               // btSerial.println(x.raw[count]);
               // Serial.print(" ");
               // Serial.print((int16_t) (x.filtered[count]));
               // Serial.println();
                count++;
                //Delay(5);
                i++;
                if(i == 4095)
                    i = 0;

                    btSerial.println(i);
            }       
        }
        Delay(100);
    }
}

bool StopDSP(void)
{
    if(thDSP != NULL)
    {
        vTaskDelete(thDSP);
        return false;
    }
    else 
        return true;    
}