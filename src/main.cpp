#include "main.h"
#include <esp_spp_api.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled
#endif

BluetoothSerial btSerial;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){

  switch(event)
  {
    case ESP_SPP_SRV_OPEN_EVT:
      #if DEBUG
        Serial.println("Client Connected.");
      #endif
      InitSendData();
      //InitSampling();
      //InitDSP(); 
    break;

    case ESP_SPP_WRITE_EVT:
      #if DEBUG
        //Serial.println("Writing bluetooth data.");
      #endif
    break;

    case ESP_SPP_SUCCESS:
      #if DEBUG
        Serial.println("Bluetooth service started.");
      #endif
    break;

    case ESP_SPP_OPEN_EVT:
      Serial.println("Radi nesto");
    break;

    case ESP_SPP_CLOSE_EVT:
      #if DEBUG
        Serial.println("Client disconnected.");
      #endif
      StopSendData();
      //StopSamplingLoop();
      //StopDSP();
    break;
  }   
   // InitSampling();
   // InitDSP();

}

void setup() {

  #if DEBUG
    Serial.begin(BAUDRATE);
  #endif
Serial.begin(BAUDRATE);

  btSerial.register_callback(callback);

  if(!btSerial.begin("ESP"))
    {
      #if DEBUG
          Serial.println("Error initialising bluetooth.");
      #endif
    }

    //InitSampling();
   // InitDSP();
}

void loop() {
  vTaskDelete(NULL);
}
