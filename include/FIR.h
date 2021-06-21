typedef struct {
    float out;
	float *coeff;
	float *buf;
	uint16_t order;
	uint16_t putIndex;
} FIRFilter ;

void FIRFilter_Init(FIRFilter*, float*, float*,uint16_t);

float FIRFilter_Update(FIRFilter*, float );
