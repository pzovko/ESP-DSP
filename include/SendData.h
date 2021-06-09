#define DATA_STACK_SIZE 10000

#define DATA_BUFFER_SIZE QUEUE_LEN
#define DATA_BUFFER_EOB  40412

bool InitSendData(void);
void SendData_loop(void*);
bool StopSendData(void);
int32_t PackData(int16_t, int16_t);