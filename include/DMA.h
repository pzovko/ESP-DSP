#define ADC_CHANNEL   ADC1_CHANNEL_6 // GPIO34
#define NUM_SAMPLES   1024                       // number of samples
#define DUMP_INTERVAL 50  

size_t ADC_Sampling(uint16_t*);
void configure_i2s();