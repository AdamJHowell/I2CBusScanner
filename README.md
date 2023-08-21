# I2CBusScanner
This is an Arduino-compatible sketch which will scan the I2C bus for devices, report their addresses, and attempt to report any errors encountered.
Because of address reservations, scanning will only attempt addresses from 0x08 to 0x77.
The SCL and SDA GPIOs can be set by changing the values of sdaGPIO and sclGPIO.

##Hardware

The only required hardware is an I2C device and 

##Libraries

The only required library is Wire.h.


## Sample output:
```
Using the default SDA and SCL GPIOs.
Addresses with a device will be represented by '#'.
Addresses without a device will be represented by '-'.
Addresses which return an error will be represented by 'E'.
    0123456789ABCDEF
0x0         --------
0x1 ----------------
0x2 ----------------
0x3 ------------#---
0x4 ----------------
0x5 ----------------
0x6 ----------------
0x7 --------

1 device found.
Address: 0x3C

Scan # 17 complete.
Pausing for 5 seconds.
```
