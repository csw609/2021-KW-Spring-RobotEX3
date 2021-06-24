#define F_CPU 16000000UL

#define sbi(m,p) m |= (1<<p)
#define cbi(m,p) m &= ~(1<<p)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include "MQ135.h"
#include "RBGLED.h"
#include "UART_csw.h"
#include "convert.h"
#include "Filters.h"
#include "dynamixel.h"
#include "LCD_Text.c"
//set
void set_ADC();
void set_GPIO();
void set_Timer3_DC();
void set_Timer2();
void set_Timer1();
void set_UART1();

//ADC
unsigned int get_adc(int id);

//buzzer
void buzzer_ON();
void buzzer_OFF();
int buzzer_cnt = 0;

//servo
void servo_up();
void servo_down();

//DC Fan
void Fan_A_High();
void Fan_A_Low();
void Fan_A_mid();
void Fan_A_Stop();
void Fan_B_High();
void Fan_B_Low();
void Fan_B_mid();
void Fan_B_Stop();

//action 
void action0(unsigned int val,unsigned int val2);
void action2(unsigned int val);
void action4(unsigned int val);
void action5(unsigned int val);
void OpenWindow(unsigned int val,int open);

int cnt = 0;
int lcd_count = 0;
int windowFlag = 0;
unsigned int val[6];
int buz_flag = 0;
ISR(TIMER2_OVF_vect){//제어주기16.384ms
	cli();
	//control part//////////////
	unsigned int a_adc;
	//lcdString(0,0,"start");
	if(lcd_count < 20){
		lcd_count++;
	}
	else{
		lcd_count = 0;
		lcdClear(); //lcd 주기적 청소
	}
	//USART1_Transmit('s');
	//buzzer_ON();
	//set_RGBLED(BLUE);
	//servo_down();
	//PacketTX(1,0x000,0);
	//set_RGBLED(4);
	switch(cnt){
		// sensor processing
		case 0 : // GSBT11
			a_adc = get_adc(0);
			a_adc = ((unsigned int) IIR_LPF_GS((double)a_adc));
			val[0] = GSBT11_ppm(a_adc);
			USART1_Transmit_UINT(val[cnt]);USART1_Transmit(',');//USART1_Transmit('\r'); //줄넘// USART1_Transmit('p'); USART1_Transmit('p'); USART1_Transmit('m');	
			//print on LCD
			lcdNumber(0,0,val[cnt]);
			break;
		case 1: // MQ135
			a_adc = get_adc(1);
			a_adc = ((unsigned int) IIR_LPF_3((double)a_adc));
			//val[1] = getRZero(a_adc);
			val[1] = getPPM(a_adc);
			USART1_Transmit_UINT(val[cnt]);USART1_Transmit(','); //USART1_Transmit('\r');// USART1_Transmit('p'); USART1_Transmit('p'); USART1_Transmit('m');
			//print on LCD
			lcdString(0,2,"CO2");
			lcdNumber(0,6,val[cnt]);lcdString(0,9,"ppm");
			break;
		case 2: //CDS
			a_adc = get_adc(2);
			a_adc = ((unsigned int) IIR_LPF_CDS((double)a_adc));
			val[2] = adc_to_Lux(a_adc);
			USART1_Transmit_UINT(val[cnt]);USART1_Transmit(',');// USART1_Transmit('L'); USART1_Transmit('u'); USART1_Transmit('x');
			//print on LCD
			lcdNumber(0,12,val[cnt]);
			break;
		case 3: //PSD
			a_adc = get_adc(3);
			a_adc = ((unsigned int) IIR_LPF_psd((double)a_adc));
			val[3] = PSD_dist(a_adc);
			USART1_Transmit_UINT(val[cnt]);USART1_Transmit(','); //USART1_Transmit('m'); USART1_Transmit('m');
			//print on LCD
			lcdNumber(1,0,val[cnt]);
			break;
		case 4: //LM35
			a_adc = get_adc(4);
			a_adc = ((unsigned int) IIR_LPF_LM((double)a_adc));
			val[4] = adc_to_cel_LM35(a_adc);
			USART1_Transmit_UINT(val[cnt]);USART1_Transmit(','); //USART1_Transmit('o'); USART1_Transmit('C');
			//print on LCD
			lcdNumber(1,6,val[cnt]);
			break;
		case 5: //HTG3535
			a_adc = get_adc(5);
			a_adc = ((unsigned int) IIR_LPF_hum((double)a_adc));
			val[5] = humid(a_adc);
			USART1_Transmit_UINT(val[cnt]);USART1_Transmit('\r');// USART1_Transmit('%');
			//print on LCD
			lcdNumber(1,12,val[cnt]);
			break;
		case 6: // action
			action0(val[0],val[3]); //window servo
			action2(val[2]); //bright to  motor speed
			action4(val[4]); //fire alarm buzzer
			action5(val[5]); //RGB LED humidity
		default:
			break;
	}
	if(cnt < 7){
		cnt++;
	}
	else{
		cnt = 0;
	}
	if(buz_flag){
		buzzer_ON();
	}
	//TCNT2 = 255-156;
	///////////////////////////
	sei();
}
void servo_up(){
	OCR1A = 40000.0 * (0.50 / 20.0);	//duty ratio 3% at 50Hz
}

void servo_down(){
	OCR1A = 40000.0 * (1.0 / 20.0); //duty ratio 10% at 50Hz
}
int main(void)
{
    set_GPIO();
	init_RGBLED();
	lcdInit();
	lcdClear();
    
    set_ADC();
	
	set_Timer1();
    
	
	set_Timer3_DC();
	sbi(PORTA,0);cbi(PORTA,1); sbi(PORTA,2);cbi(PORTA,3);
	
	PortInit(); // PORT 초기설정 dynamixel
	UARTInit(); // UART 초기설정 dynamixel
	set_UART1();
	//PacketTX(1,0x3FF,0);
	
	set_Timer2();
	//servo_up();
    sei();//Global Interrupt set
	//servo_up();
	//_delay_ms(300);
	//servo_down();
	//_delay_ms(300);
	//servo_up();
	
	//test board interrupt debug
	//DDRD  = 0x00; // switch0, switch1을 사용하기 위해 PD0와 PD1을 Input 설정
	// Switch setting
	//EIMSK = (1<<INT0) | (1<<INT1);	 // INT0와 INT1 활성화
	//EICRA = (1<<ISC01) | (1<<ISC11);
    while (1)
    {
    }
}

void set_GPIO(){
	cbi(DDRF,0); // ADC _ GSBT11(유기성 휘발 물질)
	cbi(DDRF,1); // ADC _ MQ135(이산화탄소)
	cbi(DDRF,2); // ADC _ CDS(밝기)
	cbi(DDRF,3); // ADC _ PSD(거리)
	cbi(DDRF,4); // ADC _ LM35(온도)
	cbi(DDRF,5); // ADC _ HGT3535(습도)
	
	
	sbi(LEDDDR,REDPORT); //RGBLED R
	//sbi(LEDDDR,GREENPORT); //RGBLED G
	//sbi(LEDDDR,BLUEPORT); //RGBLED B
	
	sbi(DDRB,5); //Servo
	
	sbi(DDRA,0); //DC fan direction
	sbi(DDRA,1); //DC fan direction
	sbi(DDRA,2); //DC fan direction
	sbi(DDRA,3); //DC fan direction
	
	sbi(DDRA,5); //buzzer
	sbi(PORTA,5);
	
	sbi(DDRE,3); //DC fan PWM
	sbi(DDRE,4); //DC fan PWM
}

void set_ADC(){
	ADMUX = 0X00; // => REF set, ADLAR => 0 , MUX =>
	ADCSRA = 0x87; // ADC Enable,
}

unsigned int get_adc(int id)
{
	ADMUX =  0x40 | id;
	ADCSRA |= (1<<ADSC);
	
	while( !(ADCSRA & (1<<ADIF)) );//while ADC converting complete
	ADCSRA &= ~(1<<ADIF); // ADIF=> 0 AD interrupt flag set
	return ADC;
}

void set_Timer2(){
	//Timer2
	TCCR2	= (1<<WGM20)|(1<<WGM21)|(1<<COM21)|(0<<COM20)|(1<<CS22)|(0<<CS21)|(1<<CS20);
	// Fast PWM mode / Clear OC2 on compare match, set OC2 at BOTTOM / 분주비 1024
	TIMSK	= (1<<TOIE2);	// Timer/Counter2 Overflow Interrupt 활성화
	//TCNT2	= 255-156;  // = 99 // 정확하게 나누어 떨어지진 않음 (원래는 98.5번이 정확함)
}

void set_Timer1()
{
	// TIMER1 Setting
	TCCR1A	= (1<<COM1A1)|(1<<COM1B1)|(1<<COM1C1)|(1<<WGM11)|(0<<WGM10);
	// Fast PWM (TOP=ICR1) / Clear OC1A on compare match 사용
	TCCR1B  = (1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<< CS11)|(0<<CS10); // 분주비 8
	//TCCR1C	= 0x00; // Force Output Compare A, B, C를 다 0으로 설정
	
	//servo
	ICR1	= 40000; // TOP값을 40000으로 설정해 서브모터의 주기를 20ms로 맞춰줌
	//top		= ICR1;
	OCR1A = 40000.0 * (1.5/20.0); // = 1000, 서보모터의 첫시작 위치는 90도 (1.5ms)
	
	//RGB LED GB
	OCR1B = 30000;
	OCR1C = 30000;
}

void set_Timer3_DC(){
	// TIMER3 Setting
	sbi(TCCR3A,COM3A1); sbi(TCCR3A,COM3B1); sbi(TCCR3A,WGM31);cbi(TCCR3A,WGM30);
	// Fast PWM (TOP=ICR3) / Clear OC3A, OC3B on compare match 사용
	sbi(TCCR3B,WGM33); sbi(TCCR3B,WGM32); cbi(TCCR3B,CS32); sbi(TCCR3B,CS31); sbi(TCCR3B,CS30);// 분주비 64
	
	//servo
	ICR3	= 2499; // TOP값을 2500, PWM 100HZ
	//top		= ICR1;
	OCR3A = 1; // Motor speed init 0;
	OCR3B = 1; // Motor speed init 0;
	
	
}
int fan_OCR = 600;
void Fan_A_High(){
	OCR3A = fan_OCR*4;
}

void Fan_B_High(){
	OCR3B = fan_OCR*4;
}

void Fan_A_mid(){
	OCR3A = fan_OCR*2;
}

void Fan_B_mid(){
	OCR3B = fan_OCR*2;
}

void Fan_A_Low(){
	OCR3A = fan_OCR*1;
}

void Fan_B_Low(){
	OCR3B = fan_OCR*1;
}

void Fan_A_Stop(){
	OCR3A = 1;
}

void Fan_B_Stop(){
	OCR3B = 1;
}

void buzzer_ON(){
	switch(buzzer_cnt){
		case 0 :
		cbi(PORTA,PINA5); //buzzer on
		break;
		case 10 :
		sbi(PORTA,PINA5); //buzzer off
		break;
		case 15 :
		buzzer_cnt = -1;
		break;
		default:
		break;
	}
	buzzer_cnt++; //제어주기의 7배
}

void buzzer_OFF(){
	sbi(PORTA,PINA5); // buzzer off
}




void show(){
	unsigned int a_adc;
	
	a_adc = get_adc(0);
	a_adc = IIR_LPF_psd(a_adc); //필터
	USART1_Transmit_UINT(GSBT11_ppm(a_adc)); //수치화
	USART1_Transmit('p');USART1_Transmit('p');USART1_Transmit('m'); // 단위
	USART1_Transmit('\r'); //줄넘김.
	
	a_adc = get_adc(1);
	a_adc = IIR_LPF_3(a_adc);
	USART1_Transmit_UINT((unsigned int)getPPM(a_adc)); //MQ135 to co2 concentration(ppm) uart
	USART1_Transmit('p');USART1_Transmit('p');USART1_Transmit('m');
	USART1_Transmit('\r'); //줄넘김.
	
	a_adc = get_adc(2);
	a_adc = IIR_LPF_CDS(a_adc);
	USART1_Transmit_UINT(adc_to_Lux(a_adc)); // CDS to LUX
	USART1_Transmit('L');USART1_Transmit('U');USART1_Transmit('X');
	USART1_Transmit('\r'); //줄넘김.
	
	a_adc = get_adc(3);
	a_adc = IIR_LPF_psd(a_adc);
	USART1_Transmit_UINT(PSD_dist(a_adc)); //PSD to dist(mm) uart
	USART1_Transmit('m');USART1_Transmit('m');
	USART1_Transmit('\r'); //줄넘김
	
	a_adc = get_adc(4);
	a_adc = IIR_LPF_hum(a_adc);
	USART1_Transmit_UINT(humid(a_adc)); //HTG3535 to relative humidity(%) uart
	USART1_Transmit('%');
	USART1_Transmit('\r'); //줄넘김
	
	a_adc = get_adc(5);
	a_adc = IIR_LPF_LM(a_adc);
	USART1_Transmit_UINT(adc_to_cel_LM35(a_adc)); // LM35 to celcius
	USART1_Transmit('o');USART1_Transmit('C');
	USART1_Transmit('\r'); //줄넘김.
}

void action0(unsigned int val, unsigned int val2){ //유해가스 성분 일정량 이상 검출시 서보 모터로 창문 자동개방  팬 회전
	if(val >= 2 && val < 4){
		lcdNumber(1,15,'x');
		Fan_A_High();
		Fan_B_High();
	}
	else if(val > 4){
		Fan_A_High();
		Fan_B_High();
		if(windowFlag == 0){
			OpenWindow(val2,1);// warning open window by servo
			windowFlag = 1;
		}
	}
	else{
		windowFlag = 0;
		Fan_A_Stop();
		Fan_B_Stop();
	}
}

void action2(unsigned int val){	// 밝기 값이 일정 수준 이하이면 밤이라 인식하고 모터 속도 감소 => 소음 제거
	if(val < 50){
		if(OCR3A > ICR3 * 0.5 || OCR3B > ICR3 * 0.5){
			Fan_B_Low();
			Fan_A_Low();
		}
	}
}


void WindowMove(unsigned int val, int open) {
	unsigned int dist;
	if(open == 1){ //open
		dist = (150-val); // * 90 / 50; //최대 폐쇄 길이 100mm 최대 개방 길이 150mm , 피니언 반지름 32.05mm => 둘레 =>  50mm 렉 이동을 위해서는 약 90도 회전 필요
		if(dist < 15){
			dyna150();
		}
		else if(dist > 15){
			dyna110();
		}
	}
	else {//close
		dist = (val - 100);
		if(dist < 15){
			
			dyna170();
		}
		else if(dist > 15){
			dyna200();
		}
	}
	
}

void OpenWindow(unsigned int val,int open){ //창문 조절 딜레이 주의
	servo_down();
	_delay_ms(1000);
	dyna150();
	_delay_ms(1000);
	servo_up();
	_delay_ms(1000);
	WindowMove(val,open);
	_delay_ms(1000);
	servo_down();
}

void action4(unsigned int val){ //화재 경보 
	if(val > 100){
		buzzer_ON();
	}
	else{
		buzzer_OFF();
	}
}

void action5(unsigned int val){//불쾌지수 LED표현
	if(val < 55){
		set_RGBLED(BLUE);
	}
	else if(val < 75){
		set_RGBLED(GREEN);
	}
	else{
		set_RGBLED(PURPLE);
	}
}