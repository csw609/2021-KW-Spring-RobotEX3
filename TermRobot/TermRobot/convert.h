/*
 * convert.h
 *
 * Created: 2021-06-19 오전 11:16:31
 *  Author: csw
 */ 


#ifndef CONVERT_H_
#define CONVERT_H_


unsigned int PSD_dist(int a_adc) //calibration and excel
{
	double amped = 1.0;
	amped = (double)a_adc / amped;
	float V_out = (amped * (5.0/1024.0)); //[V]
	//float val = 0.04 * V_out;
	//return (unsigned int)((1/val)*1000.0);
	float val = 568.69*pow(V_out,-1.11);
	if(val > 300){
		return 300;
	}
	return val;
}

unsigned int humid(int a_adc) { // amp 1.2X
	double V_out = ((double)a_adc * (5.0 / 1024.0)); //Vout [V]
	//cout << "V_out" << V_out << endl; debug on PC
	/*int i;
	for (i = 0; i < 20; i++) {
		if (humidity[i] > V_out) break;
	}

	if (i > 0) {
		double a = (humidity[i] - V_out) / ((double)humidity[i] - (double)humidity[i - 1]);
		return (unsigned int)((double)i * 5.0 + a * 5.0);
	}
	return  5;*/
	double hum = 0.7851*pow(V_out,4)-9.0261*pow(V_out,3) + 40.262*V_out*V_out -47.392*V_out+21.593; //amp 1.2X
	if(V_out > 3.98) return 100; //error correction
	return (unsigned int)hum;
}

unsigned int adc_to_Lux(unsigned int a_adc){ // CDS to LUX
	double V_out = ((double)a_adc * (5.0/1024.0)); //[V]
	double R_cds = 4700.0 * 5.0 / V_out - 4700.0; // [ohm] // 4700 => RLOAD [ohm]
	double tmp = 1.0 - (log10(R_cds) - 4.602) / 0.8; // 0.8 => gamma,   4.602 => log(40000)
	double Lux = pow(10.0,tmp); //[Lux]
	
	return (unsigned int)Lux;
}

unsigned int adc_to_cel_LM35(unsigned int a_adc){// 화재 감지 용도 4배 증폭
	double amped = 4;
	amped = (double)a_adc/amped;
	double V_LM35 = ( amped * (5.0/1024.0)); //[V]
	double V_out = V_LM35; //
	double celcius = 100 * V_out; //[oC]
	
	return (unsigned int) celcius;
}

unsigned int GSBT11_ppm(int a_adc){// 포름알데히드 감지
	double V_out = ((double)a_adc * (5.0/1024.0)); //[V]
	V_out = V_out / 3; // amp 3X
	double tmp = -5.905 + 6.996 * V_out - 1.327 * V_out * V_out;
	double val = pow(10.0, tmp);
	
	return (unsigned int)val;
}

#endif /* CONVERT_H_ */