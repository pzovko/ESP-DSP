#include "main.h"

i2s_config_t i2s_config;
QueueHandle_t adc_i2s_event_queue;
i2s_event_t adc_i2s_evt;

void configure_i2s() {
  
    i2s_config = 
    {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),  // I2S receive mode with ADC
    .sample_rate = Settings.SampleRate,                                             // sample rate
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,                                 // 16 bit I2S
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,//I2S_CHANNEL_FMT_ONLY_LEFT,                                 // only the left channel I2S_CHANNEL_FMT_ALL_LEFT
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),   // I2S format
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, //ESP_INTR_FLAG_LEVEL1,                                                        // none
    .dma_buf_count = 8,                                                           // number of DMA buffers
    .dma_buf_len = NUM_SAMPLES,                                                   // number of samples
    .use_apll = 0, 
    .tx_desc_auto_clear = true //ADD                                                               // no Audio PLL
  };


adc1_config_width(ADC_WIDTH_11Bit);
adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_11db);
adc_gpio_init(ADC_UNIT_1, ADC_CHANNEL_0) ;

i2s_driver_install(I2S_NUM_0, &i2s_config,  1000, &adc_i2s_event_queue) ;
i2s_set_adc_mode(ADC_UNIT_1, ADC_CHANNEL);


vTaskDelay(1000);
i2s_adc_enable(I2S_NUM_0);
  /*adc1_config_width(ADC_WIDTH_11Bit);
  adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_11db);
  adc_gpio_init(ADC_UNIT_1, ADC_CHANNEL_0) ;*/
//	i2s_driver_install(I2S_NUM_0, &i2s_config,  0, NULL);
//	i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_6);
  //i2s_adc_enable(I2S_NUM_0);
  #if DEBUG
    Serial.println("I2S started");
  #endif
}

size_t ADC_Sampling(uint16_t* uiBuffer){
 size_t bytesRead = 0;

 if(xQueueReceive(adc_i2s_event_queue, (void * )&adc_i2s_evt, (portTickType)portMAX_DELAY))
 {
   if(adc_i2s_evt.type == I2S_EVENT_RX_DONE)
   {
      i2s_adc_disable(I2S_NUM_0);
      i2s_read(I2S_NUM_0, uiBuffer, NUM_SAMPLES *sizeof(uint16_t), &bytesRead, portMAX_DELAY);
      i2s_adc_enable(I2S_NUM_0);
   }
 }

/*i2s_adc_enable(I2S_NUM_0);
  i2s_read(I2S_NUM_0, uiBuffer, NUM_SAMPLES *sizeof(uint16_t), &bytesRead, portMAX_DELAY);
i2s_adc_disable(I2S_NUM_0);*/

 return bytesRead;
}

void reset_i2s()
{
  i2s_adc_disable(I2S_NUM_0);
  i2s_driver_uninstall(I2S_NUM_0);
}
