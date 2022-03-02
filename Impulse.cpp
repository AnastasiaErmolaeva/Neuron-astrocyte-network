#include "Impulse.h"



FP Impulse::createImpulse(const FP& time, const bool& changeAllowed)
{
	switch (freqType)
	{
	case FREQ_POISSON:
		switch (isExist)
		{
		case false:
			if (changeAllowed && time >= firstImpStartTime && impInd < (nPoiss - 1))
			{
				isExist = true;
				startTime = startTimeVec[++impInd];
				switch (ampType)
				{
				case AMP_CONST:
					break;
				case AMP_RAND_UNIFORM:
					impulseAmplitude = (maxAmpl - minAmpl)*(FP(rand()) / FP(RAND_MAX)) + minAmpl;
					break;
				default:
					cout << "ERROR!" << endl;
					break;
				}

				return 0.;
			}
			return 0.;
			break;
		case true:
			if (time >= (startTime) && ((time - startTime) <= impulseDuration))
			{
				return impulseAmplitude;
			}
			else if ((time - startTime) > impulseDuration)
			{
				if (changeAllowed)
					isExist = false;
				return 0.;
			}
			else
			{
				return 0.;
			}
			break;
		}
		break;

	case FREQ_CONST:
		switch (isExist)
		{
		case false:
			if (changeAllowed && time >= firstImpStartTime)
			{
				for (int i = 0; i < impulsesQuantity; ++i)
					if (time >= (firstImpStartTime + i * impulsePeriod) && time < (firstImpStartTime + i * impulsePeriod + impulseDuration))
					{
						isExist = true;
						switch (ampType)
						{
						case AMP_CONST:
							break;
						case AMP_RAND_UNIFORM:
							impulseAmplitude = (maxAmpl - minAmpl)*(FP(rand()) / FP(RAND_MAX)) + minAmpl;
							break;
						default:
							cout << "ERROR!" << endl;
							break;
						}
						startTime = time;
						return impulseAmplitude;
					}
			}
			return 0.;
			break;
		case true:
			if (time >= (startTime) && time < (startTime + impulseDuration))
			{
				return impulseAmplitude;
			}
			else
			{
				if (changeAllowed)
					isExist = false;
				return 0.;
			}
			break;
		}
		break;

	case FREQ_NEVER:
		return 0.;
		break;

	default:
		cout << "ERROR!" << endl;
		break;
	}

	return INFINITY;
}

void Impulse::init(const FP & amplitudeIn, const FP & durationIn, const FP & startTimeIn, const FP& impulseFreqIn, const int& impulsesQuantityIn, const FrequencyType& freqTypeIn, const AmplitudeType& ampTypeIn, const FP& minAmpIn, const FP& maxAmpIn, const FP& timeStepIn, const FP& simDuration, const int& seeddd)
{
	impulseAmplitude = amplitudeIn;
	impulseDuration = durationIn;
	firstImpStartTime = startTimeIn;
	impulseFreq = impulseFreqIn;
	impulsePeriod = 1. / impulseFreq;
	impulsesQuantity = impulsesQuantityIn;
	freqType = freqTypeIn;
	ampType = ampTypeIn;
	minAmpl = minAmpIn;
	maxAmpl = maxAmpIn;
	timeStep = timeStepIn;
	isExist = false;
	if (freqType == FREQ_POISSON)
	{
		generator = default_random_engine(seeddd);		
		distribution = exponential_distribution<FP>(1. / (impulsePeriod - impulseDuration));

		nPoiss = 0;
		FP newStartTime;
		while (nPoiss != int(round(simDuration*impulseFreq)))
		{
			nPoiss = 0;
			startTimeVec.clear();
			newStartTime = firstImpStartTime;
			do
			{
				++nPoiss;
				seed = distribution(generator) + impulseDuration;
				newStartTime += seed;
				startTimeVec.push_back(newStartTime);
			} while (newStartTime + impulseDuration < simDuration);

		}
		impInd = -1;

	}
}

