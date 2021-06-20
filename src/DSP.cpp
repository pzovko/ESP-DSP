#include "main.h"
#include "FIR.h"

FIRFilter stFIR;
float *fFIRbuffer;
Data data;

QueueHandle_t qhSend;
TaskHandle_t thDSP;


bool InitDSP(void)
{
    if(Settings.filter_enable)
    {
        if(NULL == (fFIRbuffer = (float*)pvPortMalloc(Settings.FirOrder*sizeof(float))))
        {
            #if DEBUG
                Serial.println("Error creating FIR buffer.");
            #endif
            return true;
        }

        if(NULL == (Settings.FilterCoeff = (float*)pvPortMalloc(Settings.FirOrder*sizeof(float))))
        {
            #if DEBUG
                Serial.println("Error creating FIRCoeff buffer.");
            #endif
            return true;
        }
        FIRFilter_Init(&stFIR, Settings.FilterCoeff, fFIRbuffer, Settings.FirOrder);
    }

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
                if(Settings.filter_enable)
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
        if(Settings.filter_enable)
            vPortFree(fFIRbuffer);
        vQueueDelete(qhSend);
        vTaskDelete(thDSP);
        return false;
    }
    else 
        return true;    
}