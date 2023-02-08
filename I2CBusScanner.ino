/*
 * This program was inspired by a Python I2C scanner used with the Raspberry Pi.
 * I have tried to make it as convenient as possible, incorporating features from
 * both the inspiration program and from traditional Arduino scanners.
 * @copyright   Copyright © 2022 Adam Howell
 * @licence     The MIT License (MIT)
 */
#include "Wire.h"

// Globals are used for the convenience of configuration changes.
unsigned long loopDelay = 5000;					// The maximum value of 4,294,967,295 allows for a delay of about 49.7 days.
unsigned long lastLoop = 0;						// Holds the time when the most recent loop completed.
unsigned long loopCount = 0;						// The maximum value of 4,294,967,295 allows for a delay of about 49.7 days.
const char * sketchName = "I2CBusScanner";	// The name of this sketch.

#define OVERRIDE_WIRE							// Commend out this line to use the default SCL and SDA GPIOs.
#ifdef OVERRIDE_WIRE
const byte sdaGPIO = 4;							// Use this to set the SDA GPIO if your board uses a non-standard GPIOs for the I2C bus.
const byte sclGPIO = 5;							// Use this to set the SCL GPIO if your board uses a non-standard GPIOs for the I2C bus.
#endif

// Characters used in the display.
const char foundChar = '#';
const char errorChar = 'E';
const char emptyChar = '-';
const char unscannedChar = ' ';


void setup()
{
	// A short delay to provide time to turn on the serial monitor.
	delay( 1000 );
	Serial.begin( 115200 );
	if( !Serial )
		delay( 1000 );
	Serial.printf( "\nRunning setup() in %s.\n", sketchName );
	Serial.println( __FILE__ );

#ifdef OVERRIDE_WIRE
	// Override the default I2C GPIOs.
	Wire.begin( sdaGPIO, sclGPIO );
#else
	// Use default I2C GPIOs.
	 Wire.begin();
#endif

	Serial.println( "Setup complete." );
}


void loop()
{
	unsigned long time = millis();
	if( ( lastLoop == 0 ) || ( time - lastLoop ) > loopDelay )
	{
		byte error;
		byte address;
		byte deviceCount = 0;
		byte errorCount = 0;
		byte foundArray[111];
		byte arrayIndex = 0;

		loopCount++;
		Serial.printf( "\n%s will scan the I2C bus for device addresses in the range of 0x08 to 0x77.\n", sketchName );

#ifdef OVERRIDE_WIRE
		Serial.printf( "Using %d for the SDA GPIO.\n", sdaGPIO );
		Serial.printf( "Using %d for the SCL GPIO.\n", sclGPIO );
#else
		Serial.println( "Using the default SDA and SCL GPIOs." );
#endif

		Serial.printf( "Addresses with a device will be represented by '%c'.\n", foundChar );
		Serial.printf( "Addresses without a device will be represented by '%c'.\n", emptyChar );
		Serial.printf( "Addresses which return an error will be represented by '%c'.\n", errorChar );

		// 0x00 - 0x07 and 0x78 - 0x7F are reserved I2C addresses.
		// Source: https://learn.adafruit.com/i2c-addresses/the-list
		// Because of these reservations, scanning will begin at 0x08 and end at 0x77.
		Serial.println( "    0123456789ABCDEF" );
		Serial.print( "0x0 " );
		for( int i = 0x00; i < 0x08; i++ )
			Serial.print( unscannedChar );

		for( address = 0x08; address < 0x78; address++ )
		{
			// Print a carriage return and the row.
			if( address % 0x10 == 0 )
			{
				Serial.println( "" );
				Serial.print( "0x" );
				// Get the 2nd-least significant hex number.
				int mostSignificantFigure = address / 0x10;
				Serial.print( mostSignificantFigure );
				Serial.print( " " );
			}

			Wire.beginTransmission( address );
			error = Wire.endTransmission();
			// If a device is found.
			if( error == 0 )
			{
				Serial.print( foundChar );
				foundArray[arrayIndex] = address;
				arrayIndex++;
				deviceCount++;
			}
			// If there is an error at the tested address.
			else if( error != 2 )
			{
				Serial.print( errorChar );
				errorCount++;
			}
			else
				Serial.print( emptyChar );
		}
		Serial.println( "\n" );

		if( deviceCount == 0 )
			Serial.println( "No I2C devices found." );
		else if( deviceCount > 0 )
		{
			Serial.print( deviceCount );
			Serial.print( " device" );
			if( deviceCount > 1 )
				Serial.print( "s" );
			Serial.println( " found." );
			for( int i = 0; i < arrayIndex; i++ )
				Serial.printf( "Address: 0x%X\n", foundArray[i] );
			Serial.println( "" );
		}

		if( errorCount > 0 )
			Serial.printf( "%d scanning errors encountered!\n", errorCount );

		Serial.printf( "Scan # %lu complete.\n", loopCount );

		Serial.printf( "Pausing for %lu seconds.\n\n", loopDelay / 1000 );
		lastLoop = millis();
	}
}
