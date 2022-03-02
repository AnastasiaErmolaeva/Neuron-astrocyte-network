#ifndef _CELL_H
#define _CELL_H

#include "Defs.h"


class Cell
{
public:

	const FP	my_inf = INFINITY;
	const int	my_int_max = INT_MAX;
	bool		isStimul;
	FP			impulseAmplitude;
	FP			impulseDuration;
	FP			impulseStartTime;
	FP			impulseFreq;
	FP			impMinAmp;
	FP			impMaxAmp;
	int			impulsesQuantity;
	ofstream    of;

	virtual void step1(const FP& time) = 0;

	virtual void step2(const FP& time) = 0;

	virtual void init (const bool& isInputImpulseTraining, const bool& isInputImpulseTest, const int& myRow_in, const int& myCol_in, const vector<vector<Cell*>>& neuroNetin, const vector<vector<Cell*>>& astroNet) = 0;

	virtual void saveState() = 0;

	virtual const FP& getU() = 0;

	virtual const FP& getU_old() = 0;

	virtual const FP& getU_temp() = 0;

	virtual const FP& getV() = 0;

	virtual const FP& getV_old() = 0;

	virtual const FP& getV_temp() = 0;

	virtual const int& getNconnections() = 0;

	virtual const FP& getIP3() = 0;

	virtual const FP& getCa() = 0;

	virtual const FP& getH() = 0;

	virtual const FP& getIP3_old() = 0;

	virtual const FP& getCa_old() = 0;

	virtual const FP& getCa_temp() = 0;

	virtual const FP& getH_old() = 0;

	virtual const int getNactiveNeuro(const FP& time, const FP& delay) = 0;

	virtual const FP& getLastActiveTime() = 0;

};

#endif