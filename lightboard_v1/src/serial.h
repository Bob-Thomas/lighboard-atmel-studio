/** 
 *  @namespace serial
 *  
 *  @brief Namespace that holds the instanced serial ports
 *
 *  @section DESCRIPTION
 *  
 * This namespace is created to avoid any conflicts that might occur.
 * The serial ports that get created here will be flagged as extern and thus globally usable.
 * It also holds all the classes needed to create the serial ports.
 *
**/


#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "asf.h"
#include "queue.h"

namespace serial {
	
	/** 
	 *  @struct USART_info
	 *  
	 *  @brief Holds all the info to create an _USART class.
	 *
	 *  @section DESCRIPTION
	 *  
	 * This namespace is created to avoid any conflicts that might occur.
	 * The serial ports that get created here will be flagged as extern and thus globally usable.
	 * It also holds all the classes needed to create the serial ports.
	 *
	**/
	struct USART_info {
		uint32_t RX_pin;
		uint32_t TX_pin;
		Pio* PIO_port;
		Usart* USART_port;
		uint8_t USART_id;
		IRQn_Type USART_IRQn;
	};
	
	/** 
	 *  @class UxART_base
	 *  
	 *  @brief Holds most of the functions for the _UART and _USART classes.
	 *
	 *  @section DESCRIPTION
	 *  
	 * This class holds most of the functions used by it's children.
	 * There is no abstraction to the hardware in this class, the abstraction will
	 * be created in it's child classes _UART and _USART.
	 * The functions defined in this class are usable in all children of this class.
	 *
	**/
	class UxART_base {
		private:
			Queue queue;
		protected:
		public:
			/**
			 * Creates an UxART_base class with an instanced empty queue.
			 * @param queue an empty queue specific to that port.
			 * @return UxART_base
			 **/
			UxART_base(Queue queue);
			/**
			 * Converts an uint32_t value to a hex and prints it in chars
			 * over the serial port.
			 * @param num an uint32_t to be print out in hex format.
			 * @return void
			 **/
			void put_hex(uint32_t num);
			/**
			 * Prints out an string char by char over the serial port.
			 * @param str a string to be print out.
			 * @return void
			 **/
			void put_string(const char* str);
			/**
			 * @bug will only print unsigned ints, probably breaks at -ints. 
			 *
			 * This converts the int into a string to be then print out
			 * char by char over the serial port.
			 * @param num a uint32_t to be print out.
			 * @return void
			 **/
			void put_uint32_t(uint32_t num);
			/**
			 * Used by the interrupt handler to push values into the queue.
			 * @param val a uint8_t to be added to the queue.
			 * @return void
			 **/
			void add_queue(const uint8_t val);
			/**
			 * Retrieves the first value in the queue.
			 * @return uint8_t
			 **/
			uint8_t read();
			/**
			 * Returns how many items there currently are in the queue.
			 * @return uint8_t
			 **/
			uint8_t available();
			/**
			 * Virtual function to be overwritten for hardware specific implementation
			 * @param ch is the char that will be send over the specific TX pin.
			 * @return void
			 **/
			virtual void put_char(uint8_t ch);
			
			/**
			 * Calls put_char() to print out a char value over the serial port.
			 * @param c is the char passed over to put_char()
			 * @return UxART_base
			 **/
			UxART_base & operator<< (char c);
			/**
			 * Calls put_string() to print out a string over the serial port.
			 * @param str is the string passed over to put_strint()
			 * @return UxART_base
			 **/
			UxART_base & operator<< (const char* str);
			/**
			 * Calls put_uint32_t() to print out a uint32_t over the serial port.
			 * @param num is the integer passed over to put_uint32_t()
			 * @return UxART_base
			 **/
			UxART_base & operator<< (int num);
	};
	
	/** 
	 *  @class _UART
	 *  
	 *  @brief API for the UART chip on the processor. 
	 *
	 *  @section DESCRIPTION
	 *  
	 * This class holds the abstraction to the hardware of the UART chip.
	 *
	**/
	class _UART : public UxART_base {
		private:
		protected:
		public:
			_UART();
			/**
			 * Setup all the pins and then setup the UART chip.
			 * It will also enable interrupts to the receive pin.
			 * The handler will place the received value into the queue
			 * @param BRGR this determents at what BAUD the chip will run at. 
			 * @return void
			 **/
			void start(int BRGR);
			
			/**
			 * Hardware specific to push a value into the UART Transmit Holding Register
			 * @param ch is the char that will be send over the TX pin
			 * @return void
			 **/
			void put_char(uint8_t ch);
	};
	
	/** 
	 *  @class _USART
	 *  
	 *  @brief API for the USART chips on the processor. 
	 *
	 *  @section DESCRIPTION
	 *  
	 * This namespace is created to avoid any conflicts that might occur.
	 * The serial ports that get created here will be flagged as extern and thus globally usable.
	 * It also holds all the classes needed to create the serial ports.
	 *
	**/
	class _USART : public UxART_base {
		private:
			const USART_info *info;
		protected:
		public:
			/**
			 * Will create the _USART instance with the specified setup info.
			 * @param info an USART_info struct holding all the info for the specific USART chip
			 * @return _USART
			**/
			_USART(const USART_info *info);
			/**
			 * Setup all the pins and then setup the specified USART chip.
			 * It will also enable interrupts to the receive pin.
			 * The handler will place the received value into the queue
			 * @param BRGR this determents at what BAUD the chip will run at. 
			 * @return void
			 **/
			
			void start(int BRGR);
			
			/**
			 * Hardware specific to push a value into the USARTx Transmit Holding Register
			 * @param ch is the char that will be send over the TX pin
			 * @return void
			 **/
			void put_char(uint8_t ch);
	};
	
	
	extern _UART port0; ///< Instanced _UART, corresponds to the serial connection via USB And the RX0/TX0 pins on the board.
	extern _USART port1; ///< Instanced _USART, corresponds to RX1/TX1 pins on the board.
	extern _USART port2; ///< Instanced _USART, corresponds to RX2/TX2 pins on the board.
	extern _USART port3; ///< Instanced _USART, corresponds to RX3/TX3 pins on the board.
}

#endif //__SERIAL_H__
