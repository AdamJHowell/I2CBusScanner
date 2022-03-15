#include "Wire.h"

int loopDelay = 5000;
const String sketchName = "I2CBusScanner";
const int sdaGPIO = 33;
const int sclGPIO = 32;
// Characters used in the display.
const char foundChar = '#';
const char errorChar = 'E';
const char emptyChar = '-';
const char unscannedChar = ' ';


void setup()
{
	delay( 1000 );
	Serial.begin( 115200 );
	while ( !Serial )
		delay( 100 );
	Serial.println( "" );
	Serial.print( "Running setup() in " );
	Serial.print( sketchName );
	Serial.println( ".ino!" );
	Serial.println( __FILE__ );

	// Use default I2C GPIOs.
//	Wire.begin();
	// Override the default I2C GPIOs.
	Wire.begin( sdaGPIO, sclGPIO );

	Serial.println( "Setup complete!" );
}


void loop()
{
	byte error;
	byte address;
	int nDevices = 0;
	int nErrors = 0;
	byte foundArray[111];
	int arrayIndex = 0;

	Serial.println( "" );
	Serial.print( sketchName );
	Serial.println( " will scan the I2C bus for device addresses in the range from 0x08 to 0x77." );
	Serial.print( "Using " );
	Serial.print( sdaGPIO );
	Serial.println( " for the SDA GPIO." );
	Serial.print( "Using " );
	Serial.print( sclGPIO );
	Serial.println( " for the SCL GPIO." );
	Serial.print( "Addresses with a device will be represented by \"" );
	Serial.print( foundChar );
	Serial.println( "\"." );
	Serial.print( "Addresses without a device will be represented by \"" );
	Serial.print( emptyChar );
	Serial.println( "\"." );
	Serial.print( "Addresses which return an error will be represented by \"" );
	Serial.print( errorChar );
	Serial.println( "\"." );

	// 0x00 - 0x07 and 0x78 - 0x7F are reserved I2C addresses.
	// Source: https://learn.adafruit.com/i2c-addresses/the-list
	// Because of the reservations, we will scan from 0x08 to 0x77.
	Serial.println( "    0123456789ABCDEF" );
	Serial.print( "0x0 " );
	for( int i = 0x00; i < 0x08; i++ )
		Serial.print( unscannedChar );

	for( address = 0x08; address < 0x78; address++ )
	{
		// Print a carriage return and the row
		if( address % 0x10 == 0 )
		{
			Serial.println( "" );
			Serial.print( "0x" );
			// Get the 2nd-least significant hex number.
			int addressDecade = address / 0x10;
			Serial.print( addressDecade );
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
			nDevices++;
		}
		// If there is an error at the tested address.
		else if( error != 2 )
		{
			Serial.print( errorChar );
			nErrors++;
		}
		else
			Serial.print( emptyChar );
	}
	Serial.println( "\n" );

	if( nDevices == 0 )
	{
		Serial.println( "No I2C devices found!" );
	}
	else if( nDevices > 0 )
	{
		Serial.print( nDevices );
		Serial.print( " device" );
		if( nDevices > 1 )
			Serial.print( "s" );
		Serial.println( " found." );
		for( int i = 0; i < arrayIndex; i++ )
		{
			Serial.print( "Address: 0x" );
			Serial.println( foundArray[i], HEX );
		}
		Serial.println( "" );
	}
	else
	{
		Serial.println( "An unknown error cause the device counter variable to become negative!" );
	}
	if( nErrors > 0 )
	{
		Serial.print( nErrors );
		Serial.println( " scanning errors encountered." );
	}
	Serial.print( "Pausing for " );
	Serial.print( loopDelay / 1000 );
	Serial.println( " seconds.\n" );
	delay( loopDelay );
}
