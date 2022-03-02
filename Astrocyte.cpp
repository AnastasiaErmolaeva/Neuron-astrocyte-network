#include "Astrocyte.h"

void Astrocyte::init(const bool& isInputImpulseTraining, const bool& isInputImpulseTest, const int& myRowin, const int& myColin, const vector<vector<Cell*>>& neuroNet_in, const vector<vector<Cell*>>& astroNet_in)
{
	ifstream in("input/in.txt");
	string line;
	int freqTypeInt, ampTypeInt;

	in.ignore(1000, '=');
	in >> nAstroRows;
	in.ignore(1000, '=');
	in >> nAstroCols;
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
	} while (line.find("Neurons net parameters") == string::npos);
	in.ignore(1000, '=');
	in >> nNeuroRows;
	in.ignore(1000, '=');
	in >> nNeuroCols;
	do
	{
		getline(in, line);
	} while (line.find("System parameters") == string::npos);
	in.ignore(1000, '=');
	in.ignore(1000, '=');
	in.ignore(1000, '=');
	in.ignore(1000, '=');
	in >> Ineuro_val;
	in.ignore(1000, '=');
	in >> timerDur;
	do
	{
		getline(in, line);
	} while (line.find("Simulation parameters") == string::npos);
	in.ignore(1000, '=');
	in >> timeStep;
	timeStep /= 1000.;

	FP simDur;
	in.ignore(1000, '=');
	in >> simDur;
	if (impulsesQuantity == (-1))
		impulsesQuantity = INT_MAX;

	neuroNet = neuroNet_in;
	astroNet = astroNet_in;

	myRow = myRowin;
	myCol = myColin;

	initConnectedAstrocytes();
	initConnectedNeurons();

	IP3 = IP30;
	Ca = Ca0;
	h = h0;
	IP3_old = IP30;
	Ca_old = Ca0;
	h_old = h0;
	impSeed = (isFixedSeed == 1) ? 1 : nAstroCols * myRow + myCol + 1;

}

void Astrocyte::initConnectedAstrocytes()			
{
	connAstros.astroCol.clear();
	connAstros.astroRow.clear();
	connAstros.nConnectedAstrocytes = 0;
	if (myRow > 0)
	{
		connAstros.astroCol.push_back(myCol);
		connAstros.astroRow.push_back(myRow - 1);
		++connAstros.nConnectedAstrocytes;
	}
	if (myRow < nAstroRows - 1)
	{
		connAstros.astroCol.push_back(myCol);
		connAstros.astroRow.push_back(myRow + 1);
		++connAstros.nConnectedAstrocytes;
	}
	if (myCol > 0)
	{
		connAstros.astroCol.push_back(myCol - 1);
		connAstros.astroRow.push_back(myRow);
		++connAstros.nConnectedAstrocytes;
	}
	if (myCol < nAstroCols - 1)
	{
		connAstros.astroCol.push_back(myCol + 1);
		connAstros.astroRow.push_back(myRow);
		++connAstros.nConnectedAstrocytes;
	}
}

void Astrocyte::initConnectedNeurons()
{
	connNeurons.nConnectedNeurons = 0;

	int astroInd = myRow * nAstroCols + myCol;
	connNeurons.nConnectedNeurons = 0;
	connNeurons.neuroCol.resize(16);
	connNeurons.neuroRow.resize(16);

	for (int iNeuroRow = myRow * 3; iNeuroRow <= myRow * 3 + 3; ++iNeuroRow)
		for (int iNeuroCol = myCol * 3; iNeuroCol <= myCol * 3 + 3; ++iNeuroCol)
		{
			connNeurons.neuroRow[connNeurons.nConnectedNeurons] = iNeuroRow;
			connNeurons.neuroCol[connNeurons.nConnectedNeurons] = iNeuroCol;
			++connNeurons.nConnectedNeurons;
		}
}

FP Astrocyte::calcIneuro(const FP& time)
{
	if (getNactiveNeuro(time, 0.) >= connNeurons.nConnectedNeurons / 2)
	{
		Ineuro = Ineuro_val;
		timer = timerDur;
	}
	else if (timer >= 0.)
	{
		if (timer < timeStep)
			Ineuro = 0.;
		else
			Ineuro = Ineuro_val;

		timer -= timeStep;
	}
	else
	{
		Ineuro = 0.;
	}

	return Ineuro;
}

void Astrocyte::step1(const FP& time)
{
	Ineuro1 = calcIneuro(time);

	IP3_temp = IP3_old + ((IP3_st - IP3_old) / tau_ip + Jplc(Ca_old) + Ineuro1) * timeStep;
	Ca_temp = Ca_old + (Jchannel(IP3_old, Ca_old, h_old) - Jpump(Ca_old) + Jleak(Ca_old) + Jin(IP3_old) - Jout(Ca_old)) * timeStep;
	h_temp = h_old + (a2 * (d2 * ((IP3_old + d1) / (IP3_old + d3)) * (1. - h_old) - h_old * Ca_old)) * timeStep;
}

void Astrocyte::step2(const FP& time)
{
	Ineuro2 = calcIneuro(time + timeStep);

	IP3 = IP3_temp + ((IP3_st - IP3_old) / tau_ip + Jplc(Ca_old) + Ineuro1 + (IP3_st - IP3_temp) / tau_ip + Jplc(Ca_temp) + Ineuro2) * timeStep * 0.5;
	Ca = Ca_temp + (Jchannel(IP3_old, Ca_old, h_old) - Jpump(Ca_old) + Jleak(Ca_old) + Jin(IP3_old) - Jout(Ca_old) + Jchannel(IP3_temp, Ca_temp, h_temp) - Jpump(Ca_temp) + Jleak(Ca_temp) + Jin(IP3_temp) - Jout(Ca_temp)) * timeStep * 0.5;
	h = h_temp + (a2 * (d2 * ((IP3_old + d1) / (IP3_old + d3)) * (1. - h_old) - h_old * Ca_old) + a2 * (d2 * ((IP3_temp + d1) / (IP3_temp + d3)) * (1. - h_temp) - h_temp * Ca_temp)) * timeStep * 0.5;
}

void Astrocyte::saveState()
{
	IP3_old = IP3;
	Ca_old = Ca;
	h_old = h;
}

const int Astrocyte::getNactiveNeuro(const FP& time, const FP& delay)
{
	int k = 0;
	for (int i = 0; i < connNeurons.nConnectedNeurons; ++i)
	{
		if ((time - neuroNet[connNeurons.neuroRow[i]][connNeurons.neuroCol[i]]->getLastActiveTime()/1000.) < (0.005 + delay) && (time - neuroNet[connNeurons.neuroRow[i]][connNeurons.neuroCol[i]]->getLastActiveTime() / 1000.) >= delay)
			++k;
	}
	return k;
}