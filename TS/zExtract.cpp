#include "hexbin.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>

using namespace std;
int main(){
	//string bin = "00003090007efe4b5c7ec00a";
	string inName = "phi0_n.dat";
	ifstream phi0_n;
	phi0_n.open(inName);
	string line;
	int k;
	cin >> k;
	for (int i = 0; i < k; ++i){
		getline(phi0_n,line);	
	}
	
	line = hex_to_bin(line,96);
	string zbin = line.substr(41,12);
	int z0 = bin_to_int(zbin);
	if (bin_to_int(zbin) > pow(2,11)){
		z0 -= pow(2,12);
	}
	
	
	float zf = z0/20.0;
	string etabin = line.substr(53,16);
	int eta0 = bin_to_int(etabin);
	if (eta0 > pow(2,15)){
		eta0 -= pow(2,16);
	}
	
	float eta = (1.0e-4)*eta0;
	int phi0 = bin_to_int(line.substr(69, 12));
	float phi;
	if (phi0 > pow(2,11)){
		phi0 -= pow(2,12);
	}
	
	phi = (1.6e-3)*phi0;
	int pT = bin_to_int(line.substr(82,14));
	float pTf = pT/4.0;
	cout << "eta: ";
	cout << eta << endl;
	cout << "phi: ";
	cout << phi << endl;
	cout << "zf: ";
	cout << zf << endl;
	cout << "pT: ";
	cout << pTf << endl;
	phi0_n.close();
	return 0;
}
