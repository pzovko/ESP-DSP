#define DATA_STACK_SIZE 2500

bool InitSendData(void);
void SendData_loop(void*);
bool StopSendData(void);
int32_t PackData(int16_t, int16_t);