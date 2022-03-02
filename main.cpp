#include "Neuron.h"
#include "Astrocyte.h"

typedef vector<Cell*> neuronRow;
typedef vector<neuronRow> neuronNet;

typedef vector<Cell*> astrocyteRow;
typedef vector<astrocyteRow> astrocyteNet;

void readParameters(int& nAstroRows, int& nAstroCols, int& nNeuroRows, int& nNeuroCols, FP& timeStepAstro, FP& simDuration)
{
	ifstream in("input/in.txt");
	string line;

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

	do
	{
		getline(in, line);
	} while (line.find("Simulation parameters") == string::npos);
	in.ignore(1000, '=');
	in >> timeStepAstro;
	in.ignore(1000, '=');
	in >> simDuration;
}

void initNet(neuronNet& neuroNet, astrocyteNet& astroNet, const int& nAstroRows, const int& nAstroCols, const int& nNeuroRows, const int& nNeuroCols)
{
	astroNet.resize(nAstroRows, astrocyteRow(nAstroCols));
	neuroNet.resize(nNeuroRows, neuronRow(nNeuroCols));

	for (int i = 0; i < nNeuroRows; ++i)
		for (int j = 0; j < nNeuroCols; ++j)
			neuroNet[i][j] = new Neuron();

	for (int i = 0; i < nAstroRows; ++i)
		for (int j = 0; j < nAstroCols; ++j)
			astroNet[i][j] = new Astrocyte();

	for (int i = 0; i < nAstroRows; ++i)
		for (int j = 0; j < nAstroCols; ++j)
			astroNet[i][j]->init(false, false, i, j, neuroNet, astroNet);

	ifstream imageTraining("input/1 plus 10 percent of salt and pepper.txt");
	ifstream imageTest("input/1 plus 40 percent of salt and pepper.txt");

	for (int i = 0, tempImageTraining, tempImageTest; i < nNeuroRows; ++i)
	{
		for (int j = 0; j < nNeuroCols; ++j)
		{
			imageTraining >> tempImageTraining;
			imageTest >> tempImageTest;

			neuroNet[i][j]->init(tempImageTraining==1, tempImageTest==1, i, j, neuroNet, astroNet);
		}
	}
	imageTraining.close();
	imageTest.close();
}

int main()
{
	FP simDuration, timeAstro, timeNeuro, timeStepAstro, timeStepNeuro;
	int nAstroRows, nAstroCols, nNeuroRows, nNeuroCols;

	readParameters(nAstroRows, nAstroCols, nNeuroRows, nNeuroCols, timeStepNeuro, simDuration);
	timeStepAstro = timeStepNeuro / 1000.;

	long long N = (long long)(ceil(long double(simDuration) / long double(timeStepNeuro))), iStep;

	neuronNet neuroNet;
	astrocyteNet astroNet;

	initNet(neuroNet, astroNet, nAstroRows, nAstroCols, nNeuroRows, nNeuroCols);
	int i, j, ind;
	const int numNeurons = nNeuroCols * nNeuroRows;

	ofstream of_timeNeuro, of_timeAstro;
	vector<ofstream> of_U(nNeuroRows);
	vector<ofstream> of_Ca(nAstroRows);

	fMatrix U(nNeuroRows, fVector(nNeuroCols));
	fMatrix Ca(nAstroRows, fVector(nAstroCols));

	of_timeNeuro.open(("output/timeNeuro.txt"));
	of_timeAstro.open(("output/timeAstro.txt"));

	iVector astroRow(nAstroCols * nAstroRows);
	iVector astroCol(nAstroCols * nAstroRows);
	iVector neuroRow(nNeuroCols * nNeuroRows);
	iVector neuroCol(nNeuroCols * nNeuroRows);

	for (i = 0, ind = -1; i < nAstroRows; ++i)
		for (j = 0; j < nAstroCols; ++j)
		{
			astroRow[++ind] = i;
			astroCol[ind] = j;
		}

	for (i = 0, ind = -1; i < nNeuroRows; ++i)
		for (j = 0; j < nNeuroCols; ++j)
		{
			neuroRow[++ind] = i;
			neuroCol[ind] = j;
		}

	for (i = 0; i < nNeuroRows; ++i)
	{
		of_U[i].open(("output/U_" + to_string(i) + ".txt"));
	}

	for (i = 0; i < nAstroRows; ++i)
	{
		of_Ca[i].open(("output/Ca_" + to_string(i) + ".txt"));
	}

	int nThreads = 8;
	for (iStep = 0ll; iStep < N; iStep += 1ll)
	{
		timeAstro = FP((long double)iStep * (long double)timeStepAstro);
		timeNeuro = FP((long double)iStep * (long double)timeStepNeuro);

#pragma omp parallel num_threads(nThreads)
		{
#pragma omp for 
			for (i = 0; i < nAstroRows * nAstroCols; ++i)
				astroNet[astroRow[i]][astroCol[i]]->step1(timeAstro);

#pragma omp for
			for (i = 0; i < nNeuroRows * nNeuroCols; ++i)
				neuroNet[neuroRow[i]][neuroCol[i]]->step1(timeNeuro);

#pragma omp for 
			for (i = 0; i < nAstroRows * nAstroCols; ++i)
				astroNet[astroRow[i]][astroCol[i]]->step2(timeAstro);

#pragma omp for
			for (i = 0; i < nNeuroRows * nNeuroCols; ++i)
				neuroNet[neuroRow[i]][neuroCol[i]]->step2(timeNeuro);

#pragma omp for
			for (i = 0; i < nAstroRows * nAstroCols; ++i)
				astroNet[astroRow[i]][astroCol[i]]->saveState();

#pragma omp for
			for (i = 0; i < nNeuroRows * nNeuroCols; ++i)
				neuroNet[neuroRow[i]][neuroCol[i]]->saveState();

#pragma omp single
			{
				if (iStep % 100 == 0)
				{
					of_timeNeuro << setprecision(5) << timeNeuro << endl;

					for (i = 0; i < nNeuroRows * nNeuroCols; ++i)
						of_U[neuroRow[i]] << setprecision(5) << neuroNet[neuroRow[i]][neuroCol[i]]->getU() << " ";

					for (i = 0; i < nNeuroRows; ++i)
						of_U[i] << endl;
				}

				if (iStep % 10000 == 0)
				{
					of_timeAstro << setprecision(5) << timeAstro << endl;

					for (i = 0; i < nAstroRows * nAstroCols; ++i)
						of_Ca[astroRow[i]] << setprecision(5) << astroNet[astroRow[i]][astroCol[i]]->getCa() << " ";

					for (i = 0; i < nAstroRows; ++i)
						of_Ca[i] << endl;
				}

				if (iStep % 10000 == 0)
					cout << (N - iStep) / 10000 << endl;
			}
		}
	}

	for (i = 0; i < nNeuroRows; ++i)
		of_U[i].close();
	for (i = 0; i < nAstroRows; ++i)
		of_Ca[i].close();

	of_timeNeuro.close();
	of_timeAstro.close();

	return 0;
}