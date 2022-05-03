# ESP32Weather
This is an Arduino-compatible sketch which will scan the I2C bus for devices, report their addresses, and attempt to report any errors encountered.
Because of address reservations, scanning will only attempt addresses from 0x08 to 0x77.
The SCL and SDA GPIOs can be set by changing the values of sdaGPIO and sclGPIO.

##Hardware

The only required hardware is an I2C device and 

##Libraries

The only required library is Wire.h.
