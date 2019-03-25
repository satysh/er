#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
using namespace std;

int main(int argc, char const *argv[])
{
	if (argc == 1)
	{
		cerr << "Missing input files." << endl;
		return -1;
	}
	for (int i = 1; i < argc; i++)
	{
		ifstream fin(argv[i]);
		if (!fin.is_open())
		{
			cerr << "Can't open file: " << argv[i] << endl;
			fin.clear();
			fin.close();
			continue;
		}
			
		int n = 0;
		while (!fin.eof())
			if (fin.get() == '\n')
				n++;

		
		fin.clear();
		fin.seekg(0, ios::beg);
		double summ = 0.;
		for (int j = 0; j < n; j++)
		{
			double d;
			fin >> d;
			summ += d;
		}
		cerr.precision(7);
		double stdDevSumm = 0.;
		double mean = summ / n;
		cerr << argv[i] << ": " << n << ", mean = " << mean << endl;
		fin.clear();
		fin.seekg(0, ios::beg);
		for (int j = 0; j < n; j++)
		{
			double d;
			fin >> d;
			cerr << "d = " << d << ". ";
			stdDevSumm += (d - mean)*(d - mean);
			cerr << "stdDevSumm: " <<  (d - mean) << "^2 = " << stdDevSumm << endl;
		}
		fin.close();
		double stdDev = sqrt(stdDevSumm/(n-1));
		cout << stdDev << endl;
	
		int events;
		cerr << "Set events number: ";
		cin >> events;
		cerr << "Set number of Events: " << events << endl;
		double sx = stdDev/sqrt(events);
		cerr << "StdDev: " << sx << endl;
		cerr << "Set number of events to calculate stdDev: ";
		cin >> events;
		cerr << "StdDev is " << sx*sqrt(events) << endl;
	}

	return 0;
}