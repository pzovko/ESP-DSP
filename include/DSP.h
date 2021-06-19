
#define DSP_STACK_SIZE 10000
#define FIR_EOB        -1
#define DSP_SAMPLES     1024
#define DSP_RAW_BUF_SIZE    8192
#define FIR_ORDER   64

bool InitDSP(void);
void DSP_loop(void*);
bool StopDSP(void);

typedef struct {
    uint16_t raw[DSP_SAMPLES];
    float filtered[DSP_SAMPLES];
    uint16_t fft[DSP_SAMPLES];
}Data;
