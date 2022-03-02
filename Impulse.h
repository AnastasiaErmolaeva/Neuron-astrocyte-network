#ifndef _IMP_H
#define _IMP_H

#include "Defs.h"

class Impulse
{
private:

	FP impulseAmplitude;
	FP impulseDuration;
	FP firstImpStartTime;
	fVector startTimeVec;
	FP startTime;
	int impInd;
	FP timeStep;
	FP impulsesQuantity;
	FP impulsePeriod;
	FP impulseFreq;
	FP minAmpl;
	FP maxAmpl;
	FP nPoiss;

	default_random_engine generator;
	exponential_distribution<double> distribution;


	FP seed;

	bool isExist;

	FrequencyType freqType;
	AmplitudeType ampType;

public:

	Impulse() {};

	~Impulse() {};

	void init(const FP & amplitudeIn, const FP & durationIn, const FP & startTimeIn, const FP& impulseFreqIn, const int& impulsesQuantityIn, const FrequencyType& freqTypeIn, const AmplitudeType& ampTypeIn, const FP& minAmpIn, const FP& maxAmpIn, const FP& timeStepIn, const FP& simDuration, const int& seeddd);

	FP createImpulse(const FP& time, const bool& offAllowed);

};

#endif