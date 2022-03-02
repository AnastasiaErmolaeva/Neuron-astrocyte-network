#ifndef _NEURON_H
#define _NEURON_H

#include "Defs.h"
#include "Cell.h"
#include "Astrocyte.h"
#include "Impulse.h"


class Neuron :public Cell
{
private:	//types

	enum NeuroType
	{
		NEURO_TYPE_PRE = 0,
		NEURO_TYPE_POST = 1
	};

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
		vector<NeuroType> neuroType;
	};

private:	//variables

	vector<vector<Cell*>>	neuroNet;
	vector<vector<Cell*>>	astroNet;

	FP						U_old,
							U, 
							U_temp;
	FP						V_old,
							V, 
							V_temp;
	FP						U_aver,
							V_aver,
							U_temp_aver,
							V_temp_aver;

	FP						timeStep;

	FP						sigma2,
							Iapp,
							Iapp1, 
							Iapp2,
							Iapp_val,
							Iapp_val_Training,
							Iapp_val_Test,
							Iastro_old, 
							Iastro_temp,
							Iastro_val,
							D_u,
							D_v,
							Ca_in,
							U_in;

	int						nAstroRows,
							nAstroCols,
							nNeuroRows,
							nNeuroCols;

	int						myRow;
	int						myCol;
	int						iRow;
	int						iCol;

	FP						lastActiveTime;

	Impulse					impulse;

	connectedAstrocytes		connAstros;
	connectedNeurons        connNeurons;

	normal_distribution<FP> norm_distr;
	default_random_engine generator;

public:		//functions

	Neuron() :U(U0), V(V0), U_old(U0), V_old(V0), U_temp(U0), V_temp(V0) {};

	~Neuron() {};

	Neuron(const Neuron& neuro) {};

	void init(const bool& isInputImpulseTraining, const bool& isInputImpulseTest, const int& myRow_in, const int& myCol_in, const vector<vector<Cell*>>& neuroNetin, const vector<vector<Cell*>>& astroNet);

	void step1(const FP& time);

	void step2(const FP& time);

	void saveState();

	const FP& getU() { return U; }

	const FP& getU_old() { return U_old; }

	const FP& getU_temp() { return U_temp; }

	const FP& getV() { return V; }

	const FP& getV_old() { return V_old; }

	const FP& getV_temp() { return V_temp; }

	const FP& getLastActiveTime() { return lastActiveTime; }

	const int& getNconnections() { return connNeurons.nConnectedNeurons; }

private:	//functions

	void initConnectedNeurons();

	void initConnectedAstros();

	FP initIapp(const FP& time);

	void calcIastro_old(const FP& time);

	void calcIastro_temp(const FP& time);

	void calcU_aver();

	void calcV_aver();

	void calcU_temp_aver();

	void calcV_temp_aver();

	FP Vm_noise(FP V);

	FP F_U(const FP& Uold);


public:

	void init(const FP& timeStepin, const int& myRowin, const int& myColin, const int& nRowsAstr, const int& nColsAstr, const vector<vector<Cell*>>& neuroNetin, const vector<vector<Cell*>>& astroNet, const FP& d_IP3_in, const FP& d_Ca_in) {};

	const FP& getIP3() { return my_inf; cout << "Error"; }

	const FP& getCa() { return my_inf; cout << "Error"; }

	const FP& getH() { return my_inf; cout << "Error"; }

	const FP& getIP3_old() { return my_inf; cout << "Error"; }

	const FP& getCa_old() { return my_inf; cout << "Error"; }

	const FP& getCa_temp() { return my_inf; cout << "Error"; }

	const FP& getH_old() { return my_inf; cout << "Error"; }

	const int getNactiveNeuro(const FP& time, const FP& delay) { return my_int_max; cout << "Error"; }
};

#endif