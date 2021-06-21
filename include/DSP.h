
#define DSP_STACK_SIZE 5000

bool InitDSP(void);
void DSP_loop(void*);
bool StopDSP(void);

typedef struct {
    uint16_t raw;
    int16_t filtered;
}Data;
