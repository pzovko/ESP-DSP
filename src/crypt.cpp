#include "main.h"

uint32_t genSharedKey(uint32_t uiPublicKey, uint32_t uiPrivateKey)
{
    return CALC_SHARED(uiPublicKey, uiPrivateKey);
}

uint32_t genPrivateKey(void)
{
    return (esp_random()%(4+1));
}

uint32_t genPublicKey(uint32_t uiPrivateKey)
{
    return CALC_PUBLIC(uiPrivateKey);
}
