#include "main.h"
#include <esp_spp_api.h>
#include <stdio.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled
#endif

BluetoothSerial btSerial;
settings Settings;
void GetSettings();


void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){

  switch(event)
  {
    case ESP_SPP_SRV_OPEN_EVT:
      #if DEBUG
        Serial.println("Client Connected.");
      #endif

      InitSampling();
      InitDSP(); 
      InitSendData();

    break;

    case ESP_SPP_WRITE_EVT:
    break;
    case ESP_SPP_UNINIT_EVT:
    break;
    case ESP_SPP_DISCOVERY_COMP_EVT:
    break;
    case ESP_SPP_START_EVT:
    break;
    case ESP_SPP_CL_INIT_EVT:
    break;
    case ESP_SPP_DATA_IND_EVT:
    break;
    case ESP_SPP_CONG_EVT:
    break;
    case ESP_SPP_SRV_STOP_EVT:
    break;

    case ESP_SPP_SUCCESS:
      #if DEBUG
        Serial.println("Bluetooth service started.");
      #endif
    break;

    case ESP_SPP_OPEN_EVT:
    break;

    case ESP_SPP_CLOSE_EVT:
      #if DEBUG
        Serial.println("Client disconnected.");
      #endif

      StopSendData();
      StopSamplingLoop();
      StopDSP();
      ESP.restart();

    break;
  }   
}

void setup() {

  #if DEBUG
    Serial.begin(BAUDRATE);
  #endif

  Settings.filter_enable = false;
  Settings.SampleRate = 10000;
  Settings.FirOrder = 64;

  if(!btSerial.begin("ESP"))
  {
    #if DEBUG
        Serial.println("Error initialising bluetooth.");
    #endif
  }

  GetSettings();
}

void loop() {
vTaskDelete(NULL);
}

void GetSettings()
{
  bool loop = true;
  int16_t iterate = 0;
  int i;
  int j = 0;
  int coeffLoaded = 0;
  char c[256] = {'\0'};
  
  while(loop)
  {
    if(btSerial.available() > 0)
    {
      i = btSerial.read();

      if(i == 44)
        i = 46;

      c[j] = i;

      if(i == 10)
      {
        c[j] = '\0';
        j = 0;

        switch(iterate)
        {
          case 0:
            sscanf(c, "%d", (int*)&Settings.SampleRate);
            iterate++;
            #if DEBUG
              Serial.print("Sample rate:");
              Serial.println(Settings.SampleRate);
            #endif
          break;

          case 1:
            int x;
            sscanf(c, "%d", &x);
            Settings.filter_enable = (x ? true : false);

            if(Settings.filter_enable)
              iterate++;
            else
              loop = false;

            #if DEBUG
              Serial.print("Filter enable:");
              Serial.println(Settings.filter_enable);
            #endif
          break;

          case 2:
            sscanf(c, "%d", (int*)&Settings.FirOrder);

            if(NULL == (Settings.FilterCoeff = (float*)pvPortMalloc(Settings.FirOrder * sizeof(float))))
            {
              #if DEBUG
                Serial.println("Error creating filter buffer.");
              #endif
              loop = false;
              ESP.restart();
            }
            
            iterate++;
            #if DEBUG
              Serial.print("FIR order:");
              Serial.println(Settings.FirOrder);
            #endif 
          break;

          case 3:
            Settings.FilterCoeff[coeffLoaded] = 0;
            sscanf(c, "%f", &Settings.FilterCoeff[coeffLoaded]);

            #if DEBUG
              Serial.println(Settings.FilterCoeff[coeffLoaded], 7);
            #endif
            coeffLoaded++;
            if(coeffLoaded == (Settings.FirOrder))
               loop = false;
          break;
        }
        memset(c, '\0', 256);
      }
      else
      {
        j++;
      }     
    }
    Delay(1);
  }

  btSerial.end();
  btSerial.register_callback(callback);

  if(!btSerial.begin("ESP"))
  {
    #if DEBUG
        Serial.println("Error initialising bluetooth.");
    #endif
  }
}