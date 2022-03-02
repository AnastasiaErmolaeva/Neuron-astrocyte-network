#ifndef _ASTROCYTE_H
#define _ASTROCYTE_H 

#include "Defs.h"
#include "Cell.h"
#include "Neuron.h"
#include "Impulse.h"

class Astrocyte :public Cell
{
private:	//types

	struct connectedNeurons
	{
		int nConnectedNeurons;
		iVector neuroRow;
		iVector neuroCol;
	};

	struct connectedAstrocytes
	{
		int nConnectedAstrocytes;
		iVector astroRow;
		iVector astroCol;
	};

private:	//variables

	vector<vector<Cell*>>	neuroNet;
	vector<vector<Cell*>>	astroNet;

	connectedNeurons		connNeurons;
	connectedAstrocytes		connAstros;

	FP						IP3_old,
							IP3, 
							IP3_temp;
	FP						Ca_old,
							Ca, 
							Ca_temp;
	FP						h_old,
							h, 
							h_temp;

	FP						Ineuro,
							Ineuro1, 
							Ineuro2,
							Ineuro_val,
							timer = 0.,
							timerDur;

	Impulse		impulse;
	FP			impulseAmplitude;
	FP			impulseDuration;
	FP			impulseStartTime;
	FP			impulseFreq;
	FP			impMinAmp;
	FP			impMaxAmp;
	int			impulsesQuantity;

	FP	timeStep;

	int	myRow;
	int myCol;
	int iRow;
	int	iCol;

	int	nAstroRows,
		nAstroCols,
		nNeuroRows,
		nNeuroCols;

public:		//functions

	Astrocyte() {};

	~Astrocyte() {};

	void init(const bool& isInputImpulseTraining, const bool& isInputImpulseTest, const int& myRowin, const int& myColin, const vector<vector<Cell*>>& neuroNetin, const vector<vector<Cell*>>& astroNet);

	void step1(const FP& time);

	void step2(const FP& time);

	void saveState();

	const int getNactiveNeuro(const FP& time, const FP& delay);

	const FP& getIP3() { return IP3; }

	const FP& getCa() { return Ca; }

	const FP& getH() { return h; }

	const FP& getIP3_old() { return IP3_old; }

	const FP& getCa_old() { return Ca_old; }

	const FP& getCa_temp() { return Ca_temp; }

	const FP& getH_old() { return h_old; }

private:	//functions

	void initConnectedNeurons();

	void initConnectedAstrocytes();

	FP calcIneuro(const FP& time);

	FP Jchannel(const FP& IP3old, const FP& Caold, const FP& hold)
	{
		return c1 * v1* (pow((IP3old * Caold * hold), 3.))* ((c0 / c1) - (1. + (1. / c1)) * Caold) / (pow((d1 + IP3old) * (d5 + Caold), 3.));
	}

	FP Jpump(const FP & Caold)
	{
		return v3 * Caold * Caold / (k_3 * k_3 + Caold * Caold);
	}

	FP Jleak(const FP & Caold)
	{
		return c1 * v2 * ((c0 - Caold)/c1 - Caold);
	}

	FP Jin(const FP & IP3old)
	{
		return v5 + (v6 * IP3old * IP3old) / (k_2 * k_2 + IP3old * IP3old);
	}

	FP Jout(const FP & Caold)
	{
		return k_1 * Caold;
	}

	FP Jplc(const FP & Caold)
	{
		return v4 * (Caold + (1. - alf) * k_4) / (Caold + k_4);
	}

public:

	const FP & getU() { return my_inf; cout << "Error"; };

	const FP& getU_old() { return my_inf; cout << "Error"; };

	const FP& getU_temp() { return my_inf; cout << "Error"; };

	const FP& getV() { return my_inf; cout << "Error"; };

	const FP& getV_old() { return my_inf; cout << "Error"; };

	const FP& getV_temp() { return my_inf; cout << "Error"; };

	const int& getNconnections() { return my_int_max; cout << "Error"; };

	const FP& getLastActiveTime() { return my_inf; cout << "Error"; };

};

#endif