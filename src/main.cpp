#include "main.h"
#include <esp_spp_api.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled
#endif

BluetoothSerial btSerial;
settings Settings;

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
    break;
  }   
}

void setup() {

  #if DEBUG
    Serial.begin(BAUDRATE);
  #endif

  Settings.filter_enable = false;
  Settings.SampleRate = 50000;
  Settings.FirOrder = 64;

  btSerial.register_callback(callback);

  if(!btSerial.begin("ESP"))
  {
    #if DEBUG
        Serial.println("Error initialising bluetooth.");
    #endif
  }
}

void loop() {
  vTaskDelete(NULL);
}
