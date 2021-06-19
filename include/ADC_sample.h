#define ADC_PIN  15
#define SAMPLE_PRESCALER 16000 // 5kHz
#define QUEUE_LEN        2500

#define DATA_SAMPLES    2048
#define DATA_STACK_SIZE 10000

bool InitSampling(void);
void SamplingLoop(void*);
bool StopSamplingLoop(void);
void ResetBuffer(uint16_t*, uint16_t);
uint16_t* GetBuffer(void);