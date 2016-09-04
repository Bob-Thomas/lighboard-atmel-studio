#include "serial.h"
/*
	These handlers are required for the interrupts.
	On an interrupt they will place the received value into 
	the queue of port where the interrupt originated
	
	These handlers need to be open like this else the NVIC
	won't know how to handle the interrupt and the
	system will hang
*/
void UART_Handler() {
	// Retrieve the status bit to reset the interrupt
	uint32_t status = UART->UART_SR;
	// Add the received byte to the queue of UART
	serial::port0.add_queue(UART->UART_RHR);
}

void USART0_Handler() {
	// Retrieve the status bit to reset the interrupt
	uint32_t status = USART0->US_CSR;
	// Add the received byte to the queue of UART
	serial::port1.add_queue(USART0->US_RHR);
}

void USART1_Handler() {
	// Retrieve the status bit to reset the interrupt
	uint32_t status = USART1->US_CSR;
	// Add the received byte to the queue of UART
	serial::port2.add_queue(USART1->US_RHR);
}

void USART3_Handler() {
	// Retrieve the status bit to reset the interrupt
	uint32_t status = USART3->US_CSR;
	// Add the received byte to the queue of UART
	serial::port3.add_queue(USART3->US_RHR);
}

namespace serial{
	static const USART_info USART0_info = {PIO_PA10A_RXD0, PIO_PA11A_TXD0, PIOA, USART0, ID_USART0, USART0_IRQn};
	static const USART_info USART1_info = {PIO_PA12A_RXD1, PIO_PA11A_TXD0, PIOA, USART1, ID_USART1, USART1_IRQn};
	static const USART_info USART3_info = {PIO_PD5B_RXD3, PIO_PD4B_TXD3, PIOD, USART3, ID_USART3, USART3_IRQn};
		
	_UART port0;
	_USART port1(&USART0_info);
	_USART port2(&USART1_info);
	_USART port3(&USART3_info);
	
	UxART_base::UxART_base(Queue queue) : queue(queue){};
	
	void UxART_base::add_queue(const uint8_t val) {
		queue.enqueue(val);
	}
	
	uint8_t UxART_base::read() {
		return queue.dequeue();
	}
	
	uint8_t UxART_base::available() {
		return queue.get_count();
	}
	
	// Testing
	void UxART_base::put_hex(uint32_t num){
		uint8_t index;
		static const char* hex = "0123456789ABCDEF";
		
		for(index = 7; index != 0; index--) {
			put_char(hex[((num >> (index * 4)) & 0xf)]);
		}
		put_char(hex[(num & 0xf)]);
	};
	
	void UxART_base::put_string(const char* str) {
		for (int i = 0; str[i] != '\0'; i++) {
			put_char(str[i]);
		}
	}
	
	void UxART_base::put_uint32_t(uint32_t num) {
		
		char string[8];
		unsigned char i = 0;

		if (num == 0) {
			put_char('0');
			return;
		}

		while(num != 0) {
			string[i++] = (num % 10) + '0';
			num = num / 10;
		}

		for(i--; i != 0; put_char(string[i--]));
		put_char(string[0]);

	}
	
	UxART_base & UxART_base::operator<< (char c) {
		put_char(c);
		return *this;
	}
	
	UxART_base & UxART_base::operator<< (const char* str) {
		put_string(str);
		return *this;
	}
	
	UxART_base & UxART_base::operator<< (int num) {
		put_uint32_t((uint32_t)num);
		return *this;
	}
	
	_UART::_UART() : UxART_base(Queue()){};
	
	void _UART::start(int BRGR) {
		//Enable clock for the UART
		PMC->PMC_PCER0 = 1 << ID_UART;
		
		// Disable RX and TX PIO lines
		PIOA->PIO_PDR |= PIO_PA8A_URXD;
		PIOA->PIO_PDR |= PIO_PA9A_UTXD;
		//Set pins to use peripheral A
		PIOA->PIO_ABSR &= ~PIO_PA8A_URXD;
		PIOA->PIO_ABSR &= ~PIO_PA9A_UTXD;
		// Enable the pull up on the Rx and Tx pins
		PIOA->PIO_PUER |= PIO_PA8A_URXD;
		PIOA->PIO_PUER |= PIO_PA9A_UTXD;
		
		// Reset receiver & transmitter, on reset both are disabled
		UART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX;
		// Set the baud rate to 115200
		UART->UART_BRGR = BRGR;
		// No Parity
		UART->UART_MR = UART_MR_PAR_NO;
		//Configure receive interrupt
		UART->UART_IER = UART_IER_RXRDY;
		NVIC_EnableIRQ(UART_IRQn);
		// Enable receiver and transmitter
		UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
	}
	
	void _UART::put_char(uint8_t ch) {
		// Check if the transmitter is ready
		while(!(UART->UART_SR & UART_SR_TXRDY)){}
		//if(UART->UART_SR & UART_SR_TXRDY) {
		UART->UART_THR = ch;
		//}
	}
	
	_USART::_USART(const USART_info *info):
		UxART_base(Queue()), info(info)
	{};
		
	void _USART::start(int BRGR) {
		//Enable clock for the UART
		PMC->PMC_PCER0 = 1 << info->USART_id;
		
		// Disable RX and TX PIO lines
		info->PIO_port->PIO_PDR |= info->RX_pin;
		info->PIO_port->PIO_PDR |= info->TX_pin;
		if(info->USART_id == ID_USART3) {
			//Set pins to use peripheral B
			info->PIO_port->PIO_ABSR |= info->RX_pin;
			info->PIO_port->PIO_ABSR |= info->TX_pin;
		} else {
			//Set pins to use peripheral A
			info->PIO_port->PIO_ABSR &= ~info->RX_pin;
			info->PIO_port->PIO_ABSR &= ~info->TX_pin;
		}
		// Enable the pull up on the Rx and Tx pins
		info->PIO_port->PIO_PUER |= info->RX_pin;
		info->PIO_port->PIO_PUER |= info->TX_pin;
		
		// Reset receiver & transmitter, on reset both are disabled
		info->USART_port->US_CR = US_CR_RSTRX | US_CR_RSTTX;
		// Set the baud rate to 115200
		info->USART_port->US_BRGR = BRGR;
		// No Parity
		info->USART_port->US_MR = US_MR_PAR_NO;
		// 1 stop bit
		info->USART_port->US_MR = US_MR_NBSTOP_1_BIT;
		// set 8 data bits
		info->USART_port->US_MR = US_MR_CHRL_8_BIT;
		//Configure receive interrupt
		info->USART_port->US_IER = US_IER_RXRDY;
		NVIC_EnableIRQ(info->USART_IRQn);
		// Enable receiver and transmitter
		info->USART_port->US_CR = US_CR_RXEN | US_CR_TXEN;
	}
	
	void _USART::put_char(uint8_t ch) {
		// Check if the transmitter is ready
		while(!(info->USART_port->US_CSR & US_CSR_TXRDY)){}
		if(info->USART_port->US_CSR & US_CSR_TXRDY) {
			info->USART_port->US_THR = ch;
		}
	}
}