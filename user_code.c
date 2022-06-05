//PIC32duino code
// - using PIC32MX1xx/2xx
// - free running core timer for ticks() (or tmr2, selected by the user)
// - free running tmr2 for pwm / output compare / input capture
//
// - version history
// - v2.0, 5/13/2021: original port
// - v2.1, 5/26/2021: implemented SystemCoreClockUpdate() that updates SystemCoreClock during initialization
//                    implemented systicks() from TMR2 ISR - optional (in addition to core timer)
// - v2.2, 5/27/2021: implemented i2c/rtcc
// - v2.3, 6/19/2021: added support for output compare (dah!)
// - v2.4, 5/29/2021: output compare routines no longer advance compare registers
// - v2.5, 5/24/2022: added support for C32 compiler
//
//
//hardware configuration
//
//               PIC32MX1/2xx
//              |=====================|
//    Vcc       |                     |
//     |        |                AVdd |>---+--[1k]-->Vcc
//     |        |                     |  [.1u]
//     +-[10K]-<| MCLR           AVss |>---+-------->GND
//              |                     |
//              |                     |
//     +------->| OSCI           Vcap |>--[.1u]-+->GND
//  [Xtal]      |                     |         |
//     +-------<| OSCO           Vss  |>--------+
//              |                     |
//     +------->| SOSCI          RPB0 |>---------->Uart2TX
// [32,768Hz]   |                     |
//     +-------<| SOSCO           RB7 |>---------->LED
//              |                     |
//              |                     |
//              |                     |
//              |                     |
//              |                     |
//              |                     |
//              |                     |
//              |                     |
//              |                     |
//              |=====================|
//
//
//

#include "pic32duino.h"					//we use pic32duino
//#include "dhry.h"						//we use drystone for benchmark

//hardware configuration
#define LED			PB7					//led pin
#define LED_DLY		(F_CPU / 2)			//duration of delay
//end hardware configuration

//global defines

//global variables

//user defined set up code
void setup(void) {
	pinMode(LED, OUTPUT);				//led as output pin

	//initialize the uart
	//uart1Init(UART_BR9600);				//initialize uart1
	uart2Init(UART_BR9600);				//initialize uart2

	ei();
}

//user defined main loop
void loop(void) {
	static uint32_t tick0=0;
	static uint32_t tmp0;


#if 1
	//blinky - non-blocking execution
	//if enough time has elapsed
	if (ticks() - tick0 > LED_DLY) {	//if enough time has passed
		tick0 += LED_DLY;				//advance to the next match point
		pinFlip(LED);					//digitalWrite(LED, !digitalRead(LED));	//flip led, 96 - 100 ticks

		//benchmarking
		tmp0=ticks();
		//something to measure
		//dhrystone();					//dhrystone benchmark
		tmp0=ticks() - tmp0;

		//display information
		u2Print("F_CPU=                 ", F_CPU);
		u2Print("ticks=                 ", ticks());
		u2Print("tmp0 =                 ", tmp0);
		//u2Print("DevID=                 ", PIC32ID->DEVID);
		//u2Print("TMR2 =                 ", TMR2);
		//u2Print("OC1R =                 ", OC1R);
		//u2Print("lcnt =                 ", led_cnt);
		//u2Print("F_SYS=                 ", SystemCoreClock);
		//u2Print("F_PHB=                 ", F_PHB);
		//u2Print("ticks=                 ", tmp1=ticks());
		//u2Print("ticks=                 ", tmp1-tmp0);
		//u2Print("RTCT =                 ", RTCCGetSec());
		//u2Print("RTCCK=                 ", RTCCONbits.RTCCLKON);
		//u2Print("RTCHS=                 ", RTCCONbits.HALFSEC);
		u2Println();
	}
#else
	//blinky - blocking
	tmp0=ticks();												//timestamp the blinky
	digitalWrite(LED, HIGH); delay(LED_DLY * 1000 / F_CPU);		//set led pin high
	digitalWrite(LED,  LOW); delay(LED_DLY * 1000 / F_CPU);		//set led pin low
	tmp0=ticks() - tmp0;										//measure timing
	//display information
	u2Print("tmp0 =                 ", tmp0);
	u2Println();
#endif
}
