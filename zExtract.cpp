#include "hexbin.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>

using namespace std;
int main(){
	//string bin = "000000000000000000000000000000000000000000001110000001110100100100100110101111111100000010000000";
	string inName = "phi0_p.dat";
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
	int eta = bin_to_int(line.substr(53,16));
	int phi0 = bin_to_int(line.substr(69, 12));
	float phi;
	if (phi0 > pow(2,11)){
		phi0 -= pow(2,12);
	}
	phi = (1.6e-3)*phi0 + 2.793 - 3.88e-4;
	int pT = bin_to_int(line.substr(82,14));
	float pTf = pT/4.0;
	cout << "eta: ";
	cout << eta << endl;
	cout << "phi: ";
	cout << phi0 << endl;
	cout << "z0: ";
	cout << zf << endl;
	cout << "pT: ";
	cout << pTf << endl;
	phi0_n.close();
	return 0;
}
