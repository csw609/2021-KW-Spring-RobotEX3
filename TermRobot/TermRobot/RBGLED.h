/*
 * RBGLED.h
 *
 * Created: 2021-06-19 오전 11:13:34
 *  Author: csw
 */ 


#ifndef RBGLED_H_
#define RBGLED_H_

#define sbi(m,p) m |= (1<<p)
#define cbi(m,p) m &= ~(1<<p)

#define WHITE 0
#define RED   1
#define GREEN 2
#define BLUE  3
#define PURPLE 4
#define LEDPORT PORTA
#define LEDDDR DDRA
#define REDPORT 4
//#define GREENPORT 5
//#define BLUEPORT 6

void set_RGBLED(int id){
	switch(id){
		case 0 ://WHITE
			cbi(LEDPORT,REDPORT); OCR1B = 1; OCR1C = 1;//cbi(LEDPORT,GREENPORT);cbi(LEDPORT,BLUEPORT);
		break;
		case 1 ://RED
			cbi(LEDPORT,REDPORT);OCR1B = 40000; OCR1C = 40000;//sbi(LEDPORT,GREENPORT);sbi(LEDPORT,BLUEPORT);
		break;
		case 2 ://GREEN
			sbi(LEDPORT,REDPORT);OCR1B = 1; OCR1C = 39999;//cbi(LEDPORT,GREENPORT);sbi(LEDPORT,BLUEPORT);
		break;
		case 3 ://BLUE
			sbi(LEDPORT,REDPORT);OCR1B = 40000; OCR1C = 1;//sbi(LEDPORT,GREENPORT);cbi(LEDPORT,BLUEPORT);
		break;
		case 4 ://PURPLE
			cbi(LEDPORT,REDPORT);OCR1B = 30000; OCR1C = 30000;//sbi(LEDPORT,GREENPORT);sbi(LEDPORT,BLUEPORT);
		break;
		case 5 :
			sbi(LEDPORT,REDPORT);//sbi(LEDPORT,GREENPORT);sbi(LEDPORT,BLUEPORT);
		break;
		case 6 :
			sbi(LEDPORT,REDPORT);//sbi(LEDPORT,GREENPORT);sbi(LEDPORT,BLUEPORT);
		break;
		case 7 :
			sbi(LEDPORT,REDPORT);//sbi(LEDPORT,GREENPORT);sbi(LEDPORT,BLUEPORT);
		break;
		case 8 :
			sbi(LEDPORT,REDPORT);//sbi(LEDPORT,GREENPORT);sbi(LEDPORT,BLUEPORT);
		break;
		default:
		break;
	}
}

void init_RGBLED(){
	sbi(LEDPORT,REDPORT);
	sbi(DDRB,PINB6);
	sbi(DDRB,PINB7);
	//sbi(LEDPORT,GREENPORT);
	//sbi(LEDPORT,BLUEPORT);
}

#endif /* RBGLED_H_ */