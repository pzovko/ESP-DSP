#include "master.h"

BluetoothSerial btSerial;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT)
  {
    #if DEBUG
      Serial.println("Client Connected");
    #endif
  }
}

void setup() {

  #if DEBUG
    Serial.begin(BAUDRATE);
  #endif


  btSerial.register_callback(callback);

  if(!btSerial.begin("ESP_BT"))
    {
      #if DEBUG
          Serial.println("Error initialising bluetooth.");
      #endif
    }

    InitSampling();
    InitDSP();
    //InitSendData();
}

void loop() {
  vTaskDelete(NULL);
}
