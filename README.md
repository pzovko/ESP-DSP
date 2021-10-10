# ESP-DSP
[![Project Status: Active – The project has reached a stable, usable state and is being actively developed.](https://www.repostatus.org/badges/latest/active.svg)](https://www.repostatus.org/#active)

The ESP Digital Signal Processing firmware is an open source project developed mainly for educational purposes. Filters can be designed and send to the device using the ESP-DSP-Desktop application. The raw signal and filtered signal can be compared and filtered frequencies can be seen using an FFT. This project uses I2S and DMA capabilities of the ESP32 board for minimum processor interaction. Communication is done using Bluetooth.

Custom hardware is also developed for test purposes, so it is still in an early stage.

## Things to improve

- Analog to digital converter

  The integrated ESP32 ADC has its many flaws, results are better when some ADC registers are tweaked, but it is far from good. So the development board will have an external ADC for better results.

- Faster transfer rate
  
  Bluetooth has its bandwidth limitations, so when testing filters on high frequency signals the results have some latency. Other communication methods will be added.
  
- Support for IIR filters
 
  Main focus was on FIR filters, but IIR filters will be added too.



