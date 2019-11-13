#include <iostream>
#include <math.h>
using namespace std;
int main(int argc, char *argv[]) {
	/// This undos the splitting of a byte and shifting them back into one.
	/*
	int numToTest = 225;
	int firstValR = (numToTest & 0x0f) << 4;
	int secondValR = (numToTest & 0xf0) >> 4;
	int test = (firstValR >> 4);
	int test2 = (secondValR << 4);
	int total = test2 | test;

	printf("Testing number %02x \nsplitting two bytes and flipping: %02x %02x \nthen reverse bytes back: %02x %02x \nThe answer %02x",numToTest, firstValR, secondValR, test, test2, total);
	*/
	
	/// undo color delta
	/*
	uint16_t duration = 0x5a;
	uint8_t colorStart = 250;
	uint8_t colorTarget = 50;
	uint8_t divider = duration / 16;
	uint8_t result = (colorTarget - colorStart) / divider;
	
	printf("start %02x, target %02x, delta result %02x\n", colorStart, colorTarget ,result);
	*/
	uint16_t dur = 0x5a;
	uint8_t colorStart = 75;
	uint8_t colorTest = 0x23;
	uint8_t divider = dur / 16;
	uint8_t calc = colorStart + (divider * colorTest);
	printf("%02x", calc);
	
	
	/*
	uint8_t durFirst = 0x5a;
	uint8_t durSecond = 0x01;
	
	uint16_t dur = (durSecond << 8) | durFirst;
	printf("%02x", dur);
	*/
}