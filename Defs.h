#ifndef _DEFS_H
#define _DEFS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <random>
#include <set>
#include <math.h>

using namespace std;

typedef double FP;
typedef vector<int> iVector;
typedef vector<FP> fVector;
typedef vector<iVector> iMatrix;
typedef vector<fVector> fMatrix;

//neurons parameters
const FP a = 1., b = 2., c = 0.2, d = 0.075,
g = 0.2, e = 0.01,

//astrocytes parameters		
c0 = 2., c1 = 0.185,
v1 = 6., v2 = 0.11, v3 = 2.2, v5 = 0.025, v6 = 0.2,
k_1 = 0.5, k_2 = 1., k_3 = 0.1, k_4 = 1.1,
a2 = 0.14, alf = 0.8,
d1 = 0.13, d2 = 1.049, d3 = 0.9434, d5 = 0.082,
tau_ip = 7.143, IP3_st = 0.16, v4 = 0.3,

//threshold
U_T = 2., CaT = 0.15,		

//initial values
U0 = -0.702337, V0 = 1.67619,
IP30 = 0.690169, Ca0 = 0.0697115, h0 = 0.880487;

const int period = 100, duration = 5, duration_test = 10, nImpulses = 5;

enum FrequencyType
{
	FREQ_POISSON = 0,
	FREQ_CONST = 1,
	FREQ_NEVER = 2
};

enum AmplitudeType
{
	AMP_CONST = 0,
	AMP_RAND_UNIFORM = 1
};

inline FP ImpulseAstr(const FP& time, const FP& impulseAmplitude, const FP& impulseDuration, const FP& startTime, const FP& impulsePeriod, const int& impulsesQuantity)
{
	for (int i = 0; i < impulsesQuantity; ++i)
		if (time >= (startTime + i * impulsePeriod) && time < (startTime + i * impulsePeriod + impulseDuration))
			return impulseAmplitude;

	return 0.;
}



#endif