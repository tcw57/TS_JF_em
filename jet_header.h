#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <vector>
using namespace std;
//Header file for integer emulation of the 2-layer clustering Track Jet algorithm

//Holds data for each input track of the event.
struct track_data {

	int pT;
	float eta;
	float z;
	float phi;
	int bincount;  //How many zbins it's gone into (to make sure it doesn't go into more than 2): start at 0
	bool xbit;

};

//Each individual box in the eta and phi dimension.
//  Also used to store final cluster data for each zbin.
struct etaphibin {

	int pTtot;
	int numtracks;
	bool used;
   //average phi value (halfway b/t min and max)
        float phi;
   //average eta value
        float eta;
	int nx_tracks; //number of tracks with xbit true (displaced?)

};

//store important information for plots
struct maxzbin {
	int znum;            //Numbered from 0 to nzbins-2 (4 ) //16, 32, or 64) in order.
	int nclust;          //number of clusters in this bin.
//	etaphibin * clusters;     //list of all the clusters in this bin.
	vector<etaphibin> clusters;
	int ht;   //sum of all cluster pTs--only the zbin with the maximum ht is stored.
	bool isEmpty; //true if 0 tracks in the event.
};

const int netabins = 24;
const float maxz = 15.0;
const float maxeta = 2.4;
const float maxt = sinh(maxeta);
const float maxphi = M_PI;
//etastep is the width of an etabin
const float etastep = 2.0 * maxeta / netabins;
//Any tracks with pT > 200 GeV should be capped at 200
const float pTmax = 511.0;//600.0;
//Upper bound on number of tracks per event.
const int numtracks = 5000;
const int nphibins = 27;       
const int MAXNT = 31;
const int MAXNX = 15;
//phistep is the width of a phibin.
const float phistep = 2*M_PI / nphibins;
//function to find all clusters, find zbin with max ht. In file find_clusters.cpp
//maxzbin * L2_cluster(track_data*, int, int);
maxzbin L2_cluster(vector<track_data>, int, int);
//etaphibin * L1_cluster(etaphibin*);
vector<etaphibin> L1_cluster(etaphibin[netabins]);
//extern maxzbin * all_zbins; //stores cluster data for all the zbins for any given event.
extern vector<maxzbin> all_zbins; //stores cluster data for all the zbins for any given event.
extern int nzbins;
