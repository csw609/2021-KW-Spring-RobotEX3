/*
 * Filters.h
 *
 * Created: 2021-06-19 오후 2:46:29
 *  Author: csw
 */ 


#ifndef FILTERS_H_
#define FILTERS_H_
// IIR LPF Parameter filter order=2, Fc=5
volatile double b_iir_psd[3] = { 0.0002414,0.0004827,0.0002414 }; //b
volatile double a_iir_psd[3] = { 1.0,-1.9556,0.9565 };//a

volatile double IIR_input_psd[3] = { 0, };
volatile double IIR_psd[3] = { 0, };
	
double IIR_LPF_psd(double data)
{
	double iir_save = 0;
	int j = 0;
	int k = 0;
	IIR_input_psd[0] = data;
	IIR_psd[0] = 0;

	for (j = 0; j < 3; j++) IIR_psd[0] += b_iir_psd[j] * IIR_input_psd[j]; //b process
	for (k = 1; k < 3; k++) IIR_psd[0] -= a_iir_psd[k] * IIR_psd[k]; //a process

	iir_save = IIR_psd[1]; // save
	IIR_psd[1] = IIR_psd[0];
	IIR_psd[2] = iir_save;
	iir_save = IIR_input_psd[1];
	IIR_input_psd[1] = IIR_input_psd[0];
	IIR_input_psd[2] = iir_save;

	return IIR_psd[0];
}

// IIR LPF Parameter filter order=2, Fc=5
volatile double b_iir_hum[3] = { 0.0002414,0.0004827,0.0002414 };
volatile double a_iir_hum[3] = { 1.0,-1.9555782,0.9565437 };
volatile double IIR_input_hum[3] = { 0, };
volatile double IIR_hum[3] = { 0, };
double IIR_LPF_hum(double data)
{
	double iir_save = 0;
	int j = 0;
	int k = 0;
	IIR_input_hum[0] = data;
	IIR_hum[0] = 0;

	for (j = 0; j < 3; j++) IIR_hum[0] += b_iir_hum[j] * IIR_input_hum[j]; //b
	for (k = 1; k < 3; k++) IIR_hum[0] -= a_iir_hum[k] * IIR_hum[k]; //a

	iir_save = IIR_hum[1]; //tmp save
	IIR_hum[1] = IIR_hum[0];
	IIR_hum[2] = iir_save; //
	iir_save = IIR_input_hum[1];
	IIR_input_hum[1] = IIR_input_hum[0];
	IIR_input_hum[2] = iir_save;

	return IIR_hum[0]; //filtered
}
// IIR LPF Parameter filter order=2, Fc=3
volatile double b_iir_GS[3] = { 0.00008766,0.0001753,0.00008766 };
volatile double a_iir_GS[3] = { 1.0,-1.9733,0.9737 };
volatile double IIR_input_GS[3] = { 0, };
volatile double IIR_GS[3] = { 0, };
double IIR_LPF_GS(double data)
{
	double iir_save = 0;
	int j = 0;
	int k = 0;
	IIR_input_GS[0] = data;
	IIR_GS[0] = 0;

	for (j = 0; j < 3; j++) IIR_GS[0] += b_iir_GS[j] * IIR_input_GS[j];
	for (k = 1; k < 3; k++) IIR_GS[0] -= a_iir_GS[k] * IIR_GS[k];

	iir_save = IIR_GS[1];
	IIR_GS[1] = IIR_GS[0];
	IIR_GS[2] = iir_save;
	iir_save = IIR_input_GS[1];
	IIR_input_GS[1] = IIR_input_GS[0];
	IIR_input_GS[2] = iir_save;

	return IIR_GS[0];
}

// IIR LPF Parameter filter order=2, Fc=3
volatile double b_iir_CDS[3] = { 0.00008766,0.0001753,0.00008766 };
volatile double a_iir_CDS[3] = { 1.0,-1.9733,0.9737 };
volatile double IIR_input_CDS[3] = { 0, };
volatile double IIR_CDS[3] = { 0, };
double IIR_LPF_CDS(double data)
{
	double iir_save = 0;
	int j = 0;
	int k = 0;
	IIR_input_CDS[0] = data;
	IIR_CDS[0] = 0;

	for (j = 0; j < 3; j++) IIR_CDS[0] += b_iir_CDS[j] * IIR_input_CDS[j];
	for (k = 1; k < 3; k++) IIR_CDS[0] -= a_iir_CDS[k] * IIR_CDS[k];

	iir_save = IIR_CDS[1];
	IIR_CDS[1] = IIR_CDS[0];
	IIR_CDS[2] = iir_save;
	iir_save = IIR_input_CDS[1];
	IIR_input_CDS[1] = IIR_input_CDS[0];
	IIR_input_CDS[2] = iir_save;

	return IIR_CDS[0];
}

// IIR LPF Parameter filter order=2, Fc=3
volatile double b_iir_LM[3] = { 0.00008766,0.0001753,0.00008766 };
volatile double a_iir_LM[3] = { 1.0,-1.9733,0.9737 };
volatile double IIR_input_LM[3] = { 0, };
volatile double IIR_LM[3] = { 0, };
double IIR_LPF_LM(double data)
{
	double iir_save = 0;
	int j = 0;
	int k = 0;
	IIR_input_LM[0] = data;
	IIR_LM[0] = 0;

	for (j = 0; j < 3; j++) IIR_LM[0] += b_iir_LM[j] * IIR_input_LM[j];
	for (k = 1; k < 3; k++) IIR_LM[0] -= a_iir_LM[k] * IIR_LM[k];

	iir_save = IIR_LM[1];
	IIR_LM[1] = IIR_LM[0];
	IIR_LM[2] = iir_save;
	iir_save = IIR_input_LM[1];
	IIR_input_LM[1] = IIR_input_LM[0];
	IIR_input_LM[2] = iir_save;

	return IIR_LM[0];
}

// IIR LPF Parameter filter order=2, Fc=3
volatile double b_iir_3[3] = { 0.00008766,0.0001753,0.00008766 };
volatile double a_iir_3[3] = { 1.0,-1.9733,0.9737 };
volatile double IIR_input_3[3] = { 0, };
volatile double IIR_3[3] = { 0, };
double IIR_LPF_3(double data)
{
	double iir_save = 0;
	int j = 0;
	int k = 0;
	IIR_input_3[0] = data;
	IIR_3[0] = 0;

	for (j = 0; j < 3; j++) IIR_3[0] += b_iir_3[j] * IIR_input_3[j];
	for (k = 1; k < 3; k++) IIR_3[0] -= a_iir_3[k] * IIR_3[k];

	iir_save = IIR_3[1];
	IIR_3[1] = IIR_3[0];
	IIR_3[2] = iir_save;
	iir_save = IIR_input_3[1];
	IIR_input_3[1] = IIR_input_3[0];
	IIR_input_3[2] = iir_save;

	return IIR_3[0];
}

#endif /* FILTERS_H_ */