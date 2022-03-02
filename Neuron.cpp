#include "Neuron.h"

void Neuron::init(const bool& isInputImpulseTraining, const bool& isInputImpulseTest, const int& myRow_in, const int& myCol_in, const vector<vector<Cell*>>& neuroNet_in, const vector<vector<Cell*>>& astroNet_in)
{
	ifstream in("input/in.txt");
	string line;
	int freqTypeInt, ampTypeInt;

	myRow = myRow_in;
	myCol = myCol_in;

	in.ignore(1000, '=');
	in >> nAstroRows;
	in.ignore(1000, '=');
	in >> nAstroCols;
	do
	{
		getline(in, line);
	} while (line.find("Neurons net parameters") == string::npos);
	in.ignore(1000, '=');
	in >> nNeuroRows;
	in.ignore(1000, '=');
	in >> nNeuroCols;
	in.ignore(1000, '=');
	in >> impulseAmplitude;
	in.ignore(1000, '=');
	in >> impulseDuration;
	in.ignore(1000, '=');
	in >> impulseStartTime;
	in.ignore(1000, '=');
	in >> impulseFreq;
	in.ignore(1000, '=');
	in >> impulsesQuantity;
	in.ignore(1000, '=');
	in >> freqTypeInt;
	in.ignore(1000, '=');
	in >> ampTypeInt;
	in.ignore(1000, '=');
	in >> impMinAmp;
	in.ignore(1000, '=');
	in >> impMaxAmp;
	int isFixedSeed, impSeed;
	in.ignore(1000, '=');
	in >> isFixedSeed;
	do
	{
		getline(in, line);
	} while (line.find("System parameters") == string::npos);
	in.ignore(1000, '=');
	in >> sigma2;
	in.ignore(1000, '=');
	in >> Iapp_val;
	Iapp_val_Training = isInputImpulseTraining ? Iapp_val : 0.;
	Iapp_val_Test = isInputImpulseTest ? Iapp_val : 0.;

	in.ignore(1000, '=');
	in >> Iastro_val;
	in.ignore(1000, '=');
	in.ignore(1000, '=');
	in.ignore(1000, '=');
	in >> D_u;
	in.ignore(1000, '=');
	in >> D_v;
	do
	{
		getline(in, line);
	} while (line.find("Simulation parameters") == string::npos);
	in.ignore(1000, '=');
	in >> timeStep;

	if (impulsesQuantity == (-1))
		impulsesQuantity = INT_MAX;

	neuroNet = neuroNet_in;
	astroNet = astroNet_in;

	impSeed = (isFixedSeed == 1) ? 1 : myRow * nNeuroCols + myCol + 1;

	initConnectedNeurons();
	initConnectedAstros();

	generator = default_random_engine(impSeed);
	norm_distr = normal_distribution<FP>(0., 1.);

	lastActiveTime = -1000.;
}

void Neuron::initConnectedNeurons()				
{
	connNeurons.neuroCol.clear();
	connNeurons.neuroRow.clear();
	connNeurons.nConnectedNeurons = 0;

	if (myRow > 0)
	{
		connNeurons.neuroCol.push_back(myCol);
		connNeurons.neuroRow.push_back(myRow - 1);
		++connNeurons.nConnectedNeurons;
	}
	if (myRow < nNeuroRows - 1)
	{
		connNeurons.neuroCol.push_back(myCol);
		connNeurons.neuroRow.push_back(myRow + 1);
		++connNeurons.nConnectedNeurons;
	}
	if (myCol > 0)
	{
		connNeurons.neuroCol.push_back(myCol - 1);
		connNeurons.neuroRow.push_back(myRow);
		++connNeurons.nConnectedNeurons;
	}
	if (myCol < nNeuroCols - 1)
	{
		connNeurons.neuroCol.push_back(myCol + 1);
		connNeurons.neuroRow.push_back(myRow);
		++connNeurons.nConnectedNeurons;
	}
}

void Neuron::initConnectedAstros()
{
	iVector astroRows, astroCols;

	if ((myRow % 3) == 0 && myRow > 0 && myRow < nNeuroRows-1)
		astroRows = { myRow / 3 - 1, myRow / 3 };
	else if (myRow == nNeuroRows - 1)
		astroRows = { myRow / 3 - 1 };
	else
		astroRows = { myRow / 3 };

	if ((myCol % 3) == 0 && myCol > 0 && myCol < nNeuroCols - 1)
		astroCols = { myCol / 3 - 1, myCol / 3 };
	else if (myCol == nNeuroCols - 1)
		astroCols = { myCol / 3 - 1 };
	else
		astroCols = { myCol / 3 };

	connAstros.nConnectedAstrocytes = 0;
	for (int iAstroRow = 0; iAstroRow < astroRows.size(); ++iAstroRow)
		for (int iAstroCol = 0; iAstroCol < astroCols.size(); ++iAstroCol)
		{
			++connAstros.nConnectedAstrocytes;
			connAstros.astroCol.push_back(astroCols[iAstroCol]);
			connAstros.astroRow.push_back(astroRows[iAstroRow]);
		}
}

FP Neuron::initIapp(const FP& time)
{
	if ((int(time) < (period * nImpulses) && (int(time) % period) <= duration))
		Iapp = Iapp_val_Training;
	else if ((int(time) > 1000) && (int(time) < (1000 + duration_test)))
		Iapp = Iapp_val_Test;
	else
		Iapp = 0.;

	return Iapp;
}

void Neuron::calcIastro_old(const FP& time)
{
	Ca_in = astroNet[connAstros.astroRow[0]][connAstros.astroCol[0]]->getCa_old();
	int k = astroNet[connAstros.astroRow[0]][connAstros.astroCol[0]]->getNactiveNeuro(time / 1000., 40. / 1000.);

	if ((Ca_in > CaT) && (k >= 8))			
	{
		Iastro_old = Iastro_val;
	}
	else
		Iastro_old = 0.;
}

void Neuron::calcIastro_temp(const FP& time)
{
	Ca_in = astroNet[connAstros.astroRow[0]][connAstros.astroCol[0]]->getCa_temp();
	int k = astroNet[connAstros.astroRow[0]][connAstros.astroCol[0]]->getNactiveNeuro(time / 1000., 40. / 1000.);

	if ((Ca_in > CaT) && (k >= 8))		
		Iastro_temp = Iastro_val;
	else
		Iastro_temp = 0.;
}

void Neuron::step1(const FP& time)
{
	calcU_aver();
	calcV_aver();
	Iapp1 = initIapp(time);
	calcIastro_old(time);

	U_temp = U_old + ((1. / e) * (F_U(U_old) - V_old) + D_u * (U_aver - U_old) + Iapp1 + Iastro_old) * timeStep;
	V_temp = V_old + (c * U_old + d + D_v * (V_aver - V_old) + V_old * sigma2 * 0.5) * timeStep + Vm_noise(V_old);
}

void Neuron::step2(const FP& time)
{
	calcU_temp_aver();
	calcV_temp_aver();
	Iapp2 = initIapp(time + timeStep);
	calcIastro_temp(time + timeStep);

	U = U_temp + ((1. / e) * (F_U(U_old) - V_old) + D_u * (U_aver - U_old) + Iapp1 + Iastro_old  + (1. / e) * (F_U(U_temp) - V_temp) + D_u * (U_temp_aver - U_temp) + Iapp2 + Iastro_temp) * timeStep * 0.5;
	V = V_temp + (c * U_old + d + D_v * (V_aver - V_old) + V_old * sigma2 * 0.5 + c * U_temp + d + D_v * (V_temp_aver - V_temp) + V_temp * sigma2 * 0.5) * timeStep * 0.5 + (Vm_noise(V_old) + Vm_noise(V_temp)) * 0.5;
	
	if (U > U_T)
	{
		lastActiveTime = time;
	}
}

void Neuron::saveState()
{
	U_old = U;
	V_old = V;
}

void Neuron::calcU_aver()
{
	U_aver = 0.;

	if (connNeurons.nConnectedNeurons > 0)
	{
		for (int iNeuro = 0, iPrevRow, iPrevCol; iNeuro < connNeurons.nConnectedNeurons; ++iNeuro)
		{
			iPrevRow = connNeurons.neuroRow[iNeuro];
			iPrevCol = connNeurons.neuroCol[iNeuro];
			U_aver += neuroNet[iPrevRow][iPrevCol]->getU_old();
		}

		U_aver /= FP(connNeurons.nConnectedNeurons);
	}
}

void Neuron::calcV_aver()
{
	V_aver = 0.;

	if (connNeurons.nConnectedNeurons > 0)
	{
		for (int iNeuro = 0, iPrevRow, iPrevCol; iNeuro < connNeurons.nConnectedNeurons; ++iNeuro)
		{
			iPrevRow = connNeurons.neuroRow[iNeuro];
			iPrevCol = connNeurons.neuroCol[iNeuro];
			V_aver += neuroNet[iPrevRow][iPrevCol]->getV_old();
		}

		V_aver /= FP(connNeurons.nConnectedNeurons);
	}
}

void Neuron::calcU_temp_aver()
{
	U_temp_aver = 0.;

	if (connNeurons.nConnectedNeurons > 0)
	{
		for (int iNeuro = 0, iPrevRow, iPrevCol; iNeuro < connNeurons.nConnectedNeurons; ++iNeuro)
		{
			iPrevRow = connNeurons.neuroRow[iNeuro];
			iPrevCol = connNeurons.neuroCol[iNeuro];
			U_temp_aver += neuroNet[iPrevRow][iPrevCol]->getU_temp();
		}

		U_temp_aver /= FP(connNeurons.nConnectedNeurons);
	}
}

void Neuron::calcV_temp_aver()
{
	V_temp_aver = 0.;

	if (connNeurons.nConnectedNeurons > 0)
	{
		for (int iNeuro = 0, iPrevRow, iPrevCol; iNeuro < connNeurons.nConnectedNeurons; ++iNeuro)
		{
			iPrevRow = connNeurons.neuroRow[iNeuro];
			iPrevCol = connNeurons.neuroCol[iNeuro];
			V_temp_aver += neuroNet[iPrevRow][iPrevCol]->getV_temp();
		}

		V_temp_aver /= FP(connNeurons.nConnectedNeurons);
	}
}

FP Neuron::Vm_noise(FP Vin)
{
	return Vin * sqrt(sigma2 * timeStep) * norm_distr(generator);
}

FP Neuron::F_U(const FP& Uin)
{
	if (Uin <= -0.5)
		return (-1. - Uin + b);
	else if ((Uin > -0.5) && (Uin < ((1. / g) - 0.5)))
		return (g * Uin + b + 0.5 * (g - 1.));
	else if (Uin >= ((1. / g) - 0.5))
		return (1. - a * Uin + b - 0.5 + a * ((1. / g) - 0.5));
}