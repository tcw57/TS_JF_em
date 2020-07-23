#include "jet_header.h"

vector<maxzbin> all_zbins(nzbins);
//input array of track_data, output zbin of maximum ht.
maxzbin L2_cluster(vector<track_data> tracks, int nzbins, int ntracks){
	maxzbin mzb = all_zbins[0];
    //returns NULL if there are no tracks for this event.
        if(ntracks == 0){
	      mzb.isEmpty = true;
	      return mzb;
	}
	else {
	     mzb.isEmpty = false;
	}
//zstep is HALF of the width of a zbin!
        const float zstep = 2.0 * maxz / nzbins;
        
	float zmin = -1.0*maxz;
	float zmax = zmin + 2*zstep;
	//Create grid of phibins! 
	etaphibin epbins[nphibins][netabins];
	float phi = -1.0 * M_PI;
	float eta;
	float etamin, etamax, phimin, phimax;
	for(int i = 0; i < nphibins; ++i){
	    eta = -1.0 * maxeta;
            for(int j = 0; j < netabins; ++j){
		phimin = phi;
		phimax = phi + phistep;
		etamin = eta;
		eta = eta + etastep;
		etamax = eta;
		epbins[i][j].phi = (phimin + phimax) / 2;
		epbins[i][j].eta = (etamin + etamax) / 2;
	     }//for each phibin
	     phi = phi + phistep;
	 } //for each etabin (finished creating epbins)

	 //Last zbin won't be used (goes beyond maximum z)
	for(int zbin = 0; zbin < nzbins-1; ++zbin){
	
	      //First initialize pT, numtracks, nx_tracks, used to 0 (or false)
	        for(int i = 0; i < nphibins; ++i){
			for(int j = 0; j < netabins; ++j){
				epbins[i][j].pTtot = 0;
				epbins[i][j].used = false;
				epbins[i][j].numtracks = 0;
				epbins[i][j].nx_tracks = 0;
			}//for each phibin
		} //for each phibin

	      //Fill in etaphibins grid with pT from each track.
		for(int k = 0; k < ntracks; ++k) {
			for(int i = 0; i < nphibins; ++i){
				for(int j = 0; j < netabins; ++j){
					if((zmin <= tracks[k].z && zmax >= tracks[k].z) &&
					  ((epbins[i][j].eta - etastep / 2 <= tracks[k].eta && 
					    epbins[i][j].eta + etastep / 2 >= tracks[k].eta) && 
					    epbins[i][j].phi - phistep / 2 <= tracks[k].phi && 
					    epbins[i][j].phi + phistep / 2 >= tracks[k].phi && 
					    tracks[k].pT > 0 && tracks[k].bincount != 2)){
						++tracks[k].bincount;
						if((epbins[i][j].pTtot + tracks[k].pT) > 511) {
							epbins[i][j].pTtot = 511;
						}
						else {
							epbins[i][j].pTtot += tracks[k].pT;
						}
						++epbins[i][j].numtracks;
						if(tracks[k].xbit) {
							++epbins[i][j].nx_tracks;
						}
						//cout << "track " << k << " zbin " << zbin << " phibin " << i << " etabin " << j << " pT " << tracks[k].pT << " xbit " << tracks[k].xbit << endl; 
					} //if right bin
				} //for each phibin: j loop
			}//for each phibin: i loop
		} //for each track: k loop

    //Uncomment to print out pT of each eta and phi bin.
		for(int i = 0; i < nphibins; ++i)
			for(int j = 0; j < netabins; ++j)
				if(epbins[i][j].pTtot != 0) {
//				        //cout << "zmin " << zmin << " zmax " << zmax << endl;
					//cout << "zbin " << zbin << " epbins[" << i << "][" << j << "] pTtot: " << epbins[i][j].pTtot << endl;
                                      //  cout << epbins[i][j].phi << "\t" << epbins[i][j].pTtot << endl;					
				}
	

	  //First do clustering in Layer 1: maximum possible nclust for each eta slice would be a cluster in every other phibin.
		vector<etaphibin> L1clusters[nphibins];
                for(int phislice = 0; phislice < nphibins; ++phislice){
			L1clusters[phislice] = L1_cluster(epbins[phislice]);
			for(int ind = 0; L1clusters[phislice][ind].pTtot != 0; ++ind){
				L1clusters[phislice][ind].used = false;
				cout << "phi bin: " << phislice << " pT = " << L1clusters[phislice][ind].pTtot << endl;
			}
		}

	//Create clusters array to hold output cluster data for Layer2; can't have more clusters than tracks.
		vector<etaphibin> L2cluster;// = (etaphibin *)malloc(ntracks * sizeof(etaphibin));

	//Find eta-phibin with maxpT, make center of cluster, add neighbors if not already used.
		int hipT = 0;
		int nclust = 0;
		int phibin = 0;
		int imax;
	     //index of clusters array for each phislice.
		int index1;
		int E1 =0;
		int E0 =0;
		int E2 =0;
		int trx1, trx2;
		int xct1, xct2;
		int used1, used2, used3, used4;

			//Find eta-phibin with highest pT.
		for(phibin = 0; phibin < nphibins; ++phibin){
		    while(true){
			hipT = 0;
			for(index1 = 0; L1clusters[phibin][index1].pTtot > 0; ++index1){
				if(!L1clusters[phibin][index1].used && L1clusters[phibin][index1].pTtot >= hipT){
					hipT = L1clusters[phibin][index1].pTtot;
					imax = index1;
				}
			}//for each index within the phibin
		      //If highest pT is 0, all bins are used.
			if(hipT == 0){
				break;
			}
			E0 = hipT;   //E0 is pT of first phibin of the cluster.
			E1 = 0;
			E2 = 0;
			trx1 = 0;
			trx2 = 0;
			xct1 = 0;
			xct2 = 0;
			L2cluster.push_back(L1clusters[phibin][imax]);
			L1clusters[phibin][imax].used = true;
		//Add pT of upper neighbor.
		//E1 is pT of the middle phibin (should be highest pT)
			if(phibin != nphibins-1){
				used1 = -1;
				used2 = -1;
				for (index1 = 0; L1clusters[phibin+1][index1].pTtot != 0; ++index1){
					if(L1clusters[phibin+1][index1].used){
						continue;
					}
					if(fabs(L1clusters[phibin+1][index1].eta - L1clusters[phibin][imax].eta) <= 1.5*etastep){
						E1 += L1clusters[phibin+1][index1].pTtot;
						trx1 += L1clusters[phibin+1][index1].numtracks;
						xct1 += L1clusters[phibin+1][index1].nx_tracks;
						if(used1 < 0)
							used1 = index1;
						else
							used2 = index1;
					}//if cluster is within one phibin
				} //for each cluster in above phibin
			//if E1 isn't higher, E0 and E1 are their own cluster.
				if(E1 < E0){
					L2cluster[nclust].pTtot += E1;   
					L2cluster[nclust].numtracks += trx1;
					L2cluster[nclust].nx_tracks += xct1;
					if(used1 >= 0)
						L1clusters[phibin+1][used1].used = true;
					if(used2 >= 0)
						L1clusters[phibin+1][used2].used = true;
					++nclust;
					continue;
				}
				
				if(phibin != nphibins-2){
                                      //E2 will be the pT of the third phibin (should be lower than E1).
					used3 = -1;
					used4 = -1;
					for (index1 = 0; L1clusters[phibin+2][index1].pTtot != 0; ++index1){
						if(L1clusters[phibin+2][index1].used){
							continue;
						}
						if(fabs(L1clusters[phibin+2][index1].eta - L1clusters[phibin][imax].eta) <= 1.5*etastep){
							E2 += L1clusters[phibin+2][index1].pTtot;
							trx2 += L1clusters[phibin+2][index1].numtracks;
							xct2 += L1clusters[phibin+2][index1].nx_tracks;
							if(used3 < 0)
								used3 = index1;
							else
								used4 = index1;
						}
		
					}
				     //if indeed E2 < E1, add E1 and E2 to E0, they're all a cluster together.
				     //  otherwise, E0 is its own cluster.
					if(E2 < E1){
						L2cluster[nclust].pTtot += E1 + E2;
						L2cluster[nclust].numtracks += trx1 + trx2;
						L2cluster[nclust].nx_tracks += xct1 + xct2;
						L2cluster[nclust].phi = L1clusters[phibin+1][used1].phi;	
						if(used1 >= 0)
							L1clusters[phibin+1][used1].used = true;
						if(used2 >= 0)
							L1clusters[phibin+1][used2].used = true;
						if(used3 >= 0)
							L1clusters[phibin+2][used3].used = true;
						if(used4 >= 0)
							L1clusters[phibin+2][used4].used = true;
					}
					++nclust;
					continue;
				} // end Not nphibins-2
				else{
					L2cluster[nclust].pTtot += E1;
					L2cluster[nclust].numtracks += trx1;
					L2cluster[nclust].nx_tracks += xct1;
					L2cluster[nclust].phi = L1clusters[phibin+1][used1].phi;
					if(used1 >= 0)
						L1clusters[phibin+1][used1].used = true;
					if(used2 >= 0)
						L1clusters[phibin+1][used2].used = true;
					++nclust;
					continue;
				}
			}//End not last phibin(26)
			else { //if it is phibin 26
				L1clusters[phibin][imax].used = true;
				++nclust;
			}
		    }//while hipT not 0
		}//for each phibin
		//for(int db=0;db<nclust;++db)cout<<L2cluster[db].phi<<"\t"<<L2cluster[db].pTtot<<"\t"<<L2cluster[db].numtracks<<endl;	
	//Now merge clusters, if necessary
		for(int m = 0; m < nclust -1; ++m){
                     for(int n = m+1; n < nclust; ++n)
                        if(L2cluster[n].eta == L2cluster[m].eta && (fabs(L2cluster[n].phi - L2cluster[m].phi) < 1.5*phistep || fabs(L2cluster[n].phi - L2cluster[m].phi) > 6.0)){
                                if(L2cluster[n].pTtot > L2cluster[m].pTtot){
                                        L2cluster[m].phi = L2cluster[n].phi;
                                }
                                L2cluster[m].pTtot += L2cluster[n].pTtot;
                                L2cluster[m].numtracks += L2cluster[n].numtracks;
				L2cluster[m].nx_tracks += L2cluster[n].nx_tracks;
                                for(int m1 = n; m1 < nclust-1; ++m1){
                                        L2cluster[m1] = L2cluster[m1+1];
                                }
                                nclust--;
                                m = -1;
                                break; //?????
                        }//end if clusters neighbor in eta
                }//end for (m) loop     
//		for(int db=0;db<nclust;++db)cout<<L2cluster[db].phi<<"\t"<<L2cluster[db].pTtot<<"\t"<<L2cluster[db].numtracks<<endl;	
          //sum up all pTs in this zbin to find ht.
		int ht = 0;
		for(int k = 0; k < nclust; ++k){
			//(pTint < 50 || (bin_nt != "00001" && pTint < 100) || (bin_nt != "00001" && bin_nt != "00010")
			if(L2cluster[k].pTtot < 50 || (L2cluster[k].pTtot < 100 && L2cluster[k].numtracks >= 2) || L2cluster[k].numtracks >= 3) {
				ht += L2cluster[k].pTtot;
			}
			if(L2cluster[k].pTtot > 511) L2cluster[k].pTtot = 511;
                }
		if(ht > 511) ht = 511;

	   //if ht is larger than previous max, this is the new vertex zbin.
		all_zbins[zbin].znum = zbin;
		all_zbins[zbin].nclust = nclust;
		all_zbins[zbin].clusters.clear();
		etaphibin allzclust;
		for(int k = 0; k < nclust; ++k){
			allzclust.phi = L2cluster[k].phi;                               
			allzclust.eta = L2cluster[k].eta;                             
			allzclust.pTtot = L2cluster[k].pTtot;
			allzclust.numtracks = L2cluster[k].numtracks;
			if(allzclust.numtracks > MAXNT) allzclust.numtracks = MAXNT;
			allzclust.nx_tracks = L2cluster[k].nx_tracks;
			if(allzclust.nx_tracks > MAXNX) allzclust.nx_tracks = MAXNX;
			all_zbins[zbin].clusters.push_back(allzclust);
		}
	//	for(int db=0;db<nclust;++db)cout<<all_zbins[zbin].clusters[db].phi<<"\t"<<all_zbins[zbin].clusters[db].pTtot<<endl;	
		all_zbins[zbin].ht = ht;
		if(ht >= mzb.ht){
			mzb = all_zbins[zbin];
		}
	       //Prepare for next zbin!
		zmin = zmin + zstep;
		zmax = zmax + zstep;
	     } //for each zbin
       return mzb;
}
