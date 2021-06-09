
#define DSP_STACK_SIZE 10000
#define FIR_EOB        -1
#define DSP_SAMPLES     1024
#define DSP_RAW_BUF_SIZE    8192

bool InitDSP(void);
void DSP_loop(void*);
bool StopDSP(void);
int32_t PackData(int16_t, int16_t);