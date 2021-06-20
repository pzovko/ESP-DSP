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
Data data;

QueueHandle_t qhSend;
TaskHandle_t thDSP;


bool InitDSP(void)
{
    FIRFilter_Init(&stFIR, coeff, fFIRbuffer, FIR_ORDER);

    if(NULL == (qhSend = xQueueCreate(QUEUE_SIZE, sizeof(Data))))
    {
        #if DEBUG
            Serial.println("Error creating qhSend queue.");
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
    while(true)
    {
        if(qhSample != NULL)
        {
            while(uxQueueMessagesWaiting(qhSample))
            {
                if(pdFALSE == xQueueReceive(qhSample,(void*)&data.raw, 0))
                {
                    #if DEBUG
                        Serial.println("Raw data queue full.");
                    #endif
                    break;
                }

                data.filtered = (uint16_t)FIRFilter_Update(&stFIR, (float)data.raw);
                xQueueSend(qhSend, (void*)&data, portMAX_DELAY);
            }  
        }
    }
}

bool StopDSP(void)
{
    if((thDSP != NULL)&& (qhSend != NULL))
    {
        vQueueDelete(qhSend);
        vTaskDelete(thDSP);
        return false;
    }
    else 
        return true;    
}