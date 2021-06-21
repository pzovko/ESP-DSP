#include <math.h>
#include <Arduino.h>

#define BASE 4
#define PRIME 17
#define KEY_MIN 0
#define KEY_MAX 4

#define CALC_PUBLIC(private_key) (uint32_t)pow(BASE,private_key) % PRIME
#define CALC_SHARED(public_key, private_key) (uint32_t)pow(public_key,private_key) % PRIME

uint32_t genSharedKey(uint32_t,uint32_t);
uint32_t genPrivateKey(void);
uint32_t genPublicKey(uint32_t);
