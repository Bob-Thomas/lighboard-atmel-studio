/*! \mainpage Due Beeper
 *
 * \section intro_sec Introduction
 *
 * This is an library to set up your arduino due as a synthesizer.
 * It has an own made serial library that is main focus of this.
 * The synthesizer part uses the serial library extensivly to read out midi
 * data to then output sound.
 */



#include "asf.h"
#include "hwlib-due.hpp"
#include "serial.h"
#include "HCSR04.h"
#include "led_strip.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "delay.h"

volatile uint32_t timer;
void ISR() {
	timer++;
}

int main( void ) {
	/* kill the watchdog */
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	/* Initialize the system */
	sysclk_init();
	board_init();
	delay_init(sysclk_get_cpu_hz());
    auto data = due::pin_in_out(due::pins::d3);
    auto clock = due::pin_in_out(due::pins::d4);
	auto trig = due::pin_in_out(due::pins::d5);
	auto echo = due::pin_in_out(due::pins::d6);
	HCSR04 height_sensor(trig, echo);
 	led_strip strip = led_strip(32, data, clock);
	auto led = hwlib::target::pin_out( hwlib::target::pins::led );
	led.set( 0 );
	serial::port0.start(167);
	serial::port0 << "penis";
		while(1) {
			serial::port0 << (int) timer;
			//serial::port0 << (int)height_sensor.get_distance() << "\n";
		}
	
}