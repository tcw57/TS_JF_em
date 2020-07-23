#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "hexbin.h"
#include <vector>

using namespace std;

//compare simulation output to emulation
//UPDATED JAN 4, 2019 (to use vectors instead of arrays)
// Version 4: will compare one event at a time 
// 	(simulation output is writing 0s at end of event now)

int main(int argc, char ** argv){
	ifstream simfile;
	simfile.open("sim_outL2.txt");
//gracefully catch file-not-found error
	if(simfile.fail()) {
                cout << "Error: Could not open sim_outL2.txt." << endl;
                return 0;
        }
        ifstream emfile;
        string filename = "int_em_out.txt";
        emfile.open(filename.c_str());
        if(emfile.fail()) {
                cout << "Error: Could not open int_em_out.txt." << endl;
                return 0;
        }
	ofstream outfile;
	outfile.open("matches.txt");
	string data, data2;
	int i;
	int tot_matched = 0;
	int tot_almost = 0;
//total number of unmatched em, sim clusters
	int tot_unem = 0;
	int tot_unsim = 0;
      //Now FOR EACH EVENT, add all emulation and simulation clusters to separate arrays
        int nevent = -1;
NEW_EVENT:
	nevent++;
	vector<string> simclusts(0);
	vector<string> emclusts(0);
	vector<int> emused(0);
	vector<int> simused(0);
	i = 0;
	while(!emfile.eof()){
		getline(emfile, data);
		if(data == "") break;
              //only one event at a time now!
		if(data == "00000000") break;
		emclusts.push_back(data);
		emused.push_back(0);	
	}
	int nem = emclusts.size();
      //Add all simulation clusters to an array if not 0
	int j = 0;
	while(!simfile.eof()){
		getline(simfile, data);
		if(data == "") break;
	      //One event at a time only!
		if(data == "00000000") break;
	    //If a cluster contains an X, there is a problem somewhere.
		if(data.find('x') != string::npos || data.find('X') != string::npos){
			cout << "Error! Cluster " << data << " found in simulation." << endl;
			continue;
		}
		simclusts.push_back(data);
		simused.push_back(0);
	}
	int nsim = simclusts.size();
	outfile << "**********EVENT " << nevent << "*************" << endl;
	outfile << "Emulation clusters \t\t\t\t\t\tSimulation clusters\n";
	int matched = 0;
	int almost = 0;
	int emintntrx = 0;
	int simintntrx = 0;
	int emintxtrx = 0;
	int simintxtrx = 0;
	int simintz;
	int emintz;
	int simintpT;
	int emintpT;
	int siminteta;
	int eminteta;
	int emintphi;
	int simintphi;
	string emdat;
	string simdat;
   //compare each emulation cluster to each simulation cluster to see if they exactly match.
	for(i = 0; i < nem; ++i){
		if(emused[i]){
			continue;
		}
		emdat = hex_to_bin(emclusts[i], 32);
		emintntrx = bin_to_int(emdat.substr(0,5));
		emintxtrx = bin_to_int(emdat.substr(5,4));
		emintz = bin_to_int(emdat.substr(9,4));
		eminteta = bin_to_int(emdat.substr(13, 5));
		emintphi = bin_to_int(emdat.substr(18, 5));
		emintpT = bin_to_int(emdat.substr(23, 9));
		for(j = 0; j < nsim; ++j){
			if(simused[j]){
				continue;
			}
			simdat = hex_to_bin(simclusts[j], 32);
			simintntrx = bin_to_int(simdat.substr(0,5));
			simintxtrx = bin_to_int(simdat.substr(5,4));
			simintz = bin_to_int(simdat.substr(9,4));
			siminteta = bin_to_int(simdat.substr(13, 5));
			simintphi = bin_to_int(simdat.substr(18, 5));
			simintpT = bin_to_int(simdat.substr(23, 9));
			if(emintz == simintz && eminteta == siminteta && emintphi == simintphi 
			   && emintpT == simintpT && emintntrx == simintntrx && emintxtrx == simintxtrx){
				outfile << emclusts[i] << endl 
					<< "  z: " << emintz << " eta: " << eminteta << " phi: " << emintphi 
					 << " pT: " << emintpT << " ntracks: " << emintntrx << " ndisplaced_tracks: " << emintxtrx   
					<< "\t z: " << simintz << " eta: " << siminteta << " phi: " << simintphi 
					 << " pT: " << simintpT << " ntracks: " << simintntrx << " ndisplaced_tracks: " << simintxtrx  
					<< "\tEvent: " << nevent << endl;
				matched++;
				emused[i] = 1;
				simused[j] = 1;
				break;		
			}
			
		}
	}
	tot_matched += matched;

    //If they don't exactly match, they might be close. Check again each of the unused sim and em clusters.
	for(i = 0; i < nem; ++i){
		if(emused[i]){
			continue;
		}
		emdat = hex_to_bin(emclusts[i], 32);
		emintntrx = bin_to_int(emdat.substr(0,5));
		emintxtrx = bin_to_int(emdat.substr(5,4));
		emintz = bin_to_int(emdat.substr(9,4));
		eminteta = bin_to_int(emdat.substr(13, 5));
		emintphi = bin_to_int(emdat.substr(18, 5));
		emintpT = bin_to_int(emdat.substr(23, 9));
		for(j = 0; j < nsim; ++j){
			if(simused[j]){
				continue;
			}
			simdat = hex_to_bin(simclusts[j], 32);
		//simdat starts with 11 0s for now...
			simintntrx = bin_to_int(simdat.substr(0,5));
			simintxtrx = bin_to_int(simdat.substr(5,4));
			simintz = bin_to_int(simdat.substr(9,4));
			siminteta = bin_to_int(simdat.substr(13, 5));
			simintphi = bin_to_int(simdat.substr(18, 5));
			simintpT = bin_to_int(simdat.substr(23, 9));
			if(emintz == simintz && emintphi == simintphi && fabs(eminteta - siminteta) <= 1 && fabs(emintpT - simintpT) <= 5){
				if(almost == 0) {
					outfile <<"\nAlmost Matched\n" << "Emulation clusters \t\t\t\t\t\tSimulation clusters\n";
				}
				outfile << emclusts[i] << "\t\t\t\t\t\t\t" << simclusts[j] << endl << "  z: " << emintz << " eta: " << eminteta << " phi: " << emintphi
				   << " pT: " << emintpT << " ntracks: " << emintntrx << " ndisplaced_tracks: " << emintxtrx 
				<< "\t" << " z: " << simintz << " eta: " << siminteta << " phi: " << simintphi
				   << " pT: " << simintpT << " ntracks: " << simintntrx << " ndisplaced_tracks: " << simintxtrx 
				<< "\tEvent: " << nevent << endl;
				almost++;
				emused[i] = 1;
				simused[j] = 1;   
				break;		
			}
			
		}
	}
	tot_almost += almost;
     //if not all emulation clusters have been matched, print out the extras
	if(matched + almost < nem) {
		outfile <<"\nUnmatched Emulation clusters\n";
		tot_unem += (nem - (matched + almost));
	}
    //print out all the clusters that weren't matched or almost matched.
	for(i = 0; i < nem; ++i){
		if(emused[i]){
			continue;
		}
		emdat = hex_to_bin(emclusts[i], 32);
		emintntrx = bin_to_int(emdat.substr(0,5));
		emintxtrx = bin_to_int(emdat.substr(5,4));
		emintz = bin_to_int(emdat.substr(9,4));
		eminteta = bin_to_int(emdat.substr(13, 5));
		emintphi = bin_to_int(emdat.substr(18, 5));
		emintpT = bin_to_int(emdat.substr(23, 9));
		outfile << emclusts[i] << endl << "  z: " << emintz << " eta: " << eminteta
		   << " phi: " << emintphi<< " pT: " << emintpT
		   << " ntracks: " << emintntrx << " ndisplaced_tracks: " << emintxtrx << "\tEvent: " << nevent << endl; 
	}

     //if not all simulation clusters have been matched, print out the extras
	if(matched + almost < nsim) {
		outfile <<"\nUnmatched Simulation clusters\n";
		tot_unsim += (nsim - (matched + almost));
	}

	for(j = 0; j < nsim; ++j){
		if(simused[j]){
			continue;
		}
			simdat = hex_to_bin(simclusts[j], 32);
			simintntrx = bin_to_int(simdat.substr(0,5));
			simintxtrx = bin_to_int(simdat.substr(5,4));
			simintz = bin_to_int(simdat.substr(9,4));
			siminteta = bin_to_int(simdat.substr(13, 5));
			simintphi = bin_to_int(simdat.substr(18, 5));
			simintpT = bin_to_int(simdat.substr(23, 9));
		outfile << simclusts[j] << endl << "  z: " << simintz << " eta: " << siminteta
		   << " phi: " << simintphi << " pT: " << simintpT << " ntracks: " << simintntrx << " ndisplaced_tracks: " << simintxtrx << endl; 
	}
      //Now goto the next event
	if(!simfile.eof() || !emfile.eof()) goto NEW_EVENT;
     //if both files are all read, we're done here.
	emfile.close();
	simfile.close();
	outfile.close();
	cout << tot_matched << " clusters matched." << endl;
	int xtrasim = tot_unsim;
	int xtraem = tot_unem;
   //matching is successful if there were no unmatched simulation or emulation clusters and Ht of all events matched.
	if(xtrasim == 0 && xtraem == 0){
		if(almost == 0 ){
			cout << "Matching successful!" << endl;
		}
		else {
			cout << tot_almost << " clusters almost matched." << endl;
				cout << "Close!" << endl;  
		}
	}
	else {
		cout << tot_almost << " clusters almost matched." << endl;
		cout << xtrasim << " unmatched simulation clusters and "
		    << xtraem << " unmatched emulation clusters." << endl;
	}
	return 0;
}
