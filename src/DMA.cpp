#include "main.h"

                   // dump samples at this interval
int32_t  last_millis = -DUMP_INTERVAL;
uint32_t collected_samples = 0;
uint32_t last_sample_count = 0;


void configure_i2s() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX | I2S_MODE_ADC_BUILT_IN),  // I2S receive mode with ADC
    .sample_rate = 5000,                                                        // sample rate
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,                                 // 16 bit I2S
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,                                 // only the right channel
    .communication_format = I2S_COMM_FORMAT_I2S,                                  // I2S format
    .intr_alloc_flags = 0,                                                        // none
    .dma_buf_count = 8,                                                           // number of DMA buffers
    .dma_buf_len = NUM_SAMPLES,                                                   // number of samples
    .use_apll = 0,                                                                // no Audio PLL
  };
  // ADC channel 0 with 11db (divide by input 3.6)
  adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_11db);
  // 12 bit ADC
  adc1_config_width(ADC_WIDTH_12Bit);
  // install I2S 0 driver, using event queue
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  // ADC should use ADC_CHANNEL
  i2s_set_adc_mode(ADC_UNIT_1, ADC_CHANNEL);
  // The raw ADC data is written in DMA in inverted form. This add aninversion:
  SET_PERI_REG_MASK(SYSCON_SARADC_CTRL2_REG, SYSCON_SARADC_SAR1_INV);
  // enable I2S ADC
  i2s_adc_enable(I2S_NUM_0);
  Serial.println("I2S started");
}


//void setup() {
 // Serial.begin(115200);
 // configure_i2s();
//}

void ADC_Sampling(){
  uint16_t i2s_read_buff[NUM_SAMPLES];
  int num = i2s_read_bytes(I2S_NUM_0, (char*)i2s_read_buff,  NUM_SAMPLES * sizeof(uint16_t), portMAX_DELAY);
  collected_samples += num / sizeof(uint16_t);
  if( (millis() - last_millis) >= DUMP_INTERVAL )
  {
    last_millis += DUMP_INTERVAL;
    last_sample_count = collected_samples;
    for (int i=0;i<NUM_SAMPLES;i++) {
   
     // json += String(i2s_read_buff[i] & 0xFFF);

    }
    
  }
}

//void loop() {
//  ADC_Sampling();
//}