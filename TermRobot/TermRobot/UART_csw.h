/*
 * UART_csw.h
 *
 * Created: 2021-06-19 오전 11:14:49
 *  Author: csw
 */ 


#ifndef UART_CSW_H_
#define UART_CSW_H_
void set_UART1(){
	//USART 1 => datasize 8, parity none , stop 1
	cbi(UCSR1A,UDRE1);//UCSR1A = 0x00;
	sbi(UCSR1B,RXEN1); sbi(UCSR1B,TXEN1); //UCSR1B = 0x18; // receive transmit enable // => RX complete Interrupt disable 0001 1000
	sbi(UCSR1C,UCSZ11),sbi(UCSR1C,UCSZ10);//UCSR1C = 0x06; // 8bit
	UBRR1L = 103; // BAUDRATE 9600bps
}

unsigned char USART1_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR1A & (1<<RXC)) );
	
	/* Get and return received data from buffer */
	return UDR1;
}

void USART1_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE)) ); // wait while empty
	
	/* Put data into buffer, sends the data */
	UDR1 = data;
}

void USART1_Transmit_INT(int data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE)) ); // wait while empty
	///////EXAMPLE - START ////////////////////////
	if( data < 0){
		USART1_Transmit('-');
		data = (-1)*data;
	}
	unsigned char a_char_data;
	
	a_char_data = data / 10000;
	USART1_Transmit(a_char_data + 48);
	
	data %= 10000;
	a_char_data = data / 1000;
	USART1_Transmit(a_char_data + 48);
	
	data %= 1000;
	a_char_data = data / 100;
	USART1_Transmit(a_char_data + 48);
	
	data %= 100;
	a_char_data = data / 10;
	USART1_Transmit(a_char_data + 48);
	
	data %= 10;
	a_char_data = data;
	USART1_Transmit(a_char_data + 48);
}
void USART1_Transmit_UINT(unsigned int data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR1A & (1<<UDRE)) ); // wait while empty
	///////EXAMPLE - START ////////////////////////
	unsigned char a_char_data;
	
	a_char_data = data / 10000;
	USART1_Transmit(a_char_data + 48);
	
	data %= 10000;
	a_char_data = data / 1000;
	USART1_Transmit(a_char_data + 48);
	
	data %= 1000;
	a_char_data = data / 100;
	USART1_Transmit(a_char_data + 48);
	
	data %= 100;
	a_char_data = data / 10;
	USART1_Transmit(a_char_data + 48);
	
	data %= 10;
	a_char_data = data;
	USART1_Transmit(a_char_data + 48);
}




#endif /* UART_CSW_H_ */