import math
def twos_comp(val, bits):
    #"""compute the 2's complement of int value val"""
    if (val & (1 << (bits - 1))) != 0: # if sign bit is set e.g., 8bit: 128-255
        val = val - (1 << bits)        # compute negative value
    return val                         # return positive value as is

#This script will convert a coe file (in binary format) to a usable hexadecimal file for input to C++ emulation.
nfibers = 18 #how many fibers have data in the coe file
nphi = 18 #how many total phi sectors there are (zero out unused ones)
wordlength = 96
tpe = 95  #tracks per event (for each fiber)
nevents = 1 #how many events to make inputs for
coe = open("trackerRegion_alltracks_sectors_2x9_TTbar_PU200_64EventsPerFile_0.coe", "r")
vtx = open("v_data.dat","r");
#read info lines at top of file
for i in range(10):
	coe.readline()

for event in range(nevents):
	ntrks = [0 for i in range(nphi)];
	vtxz = float(vtx.readline());
	for track in range(tpe):
		all_tracks = coe.readline()
		for phi in range(nphi):
			
			if phi < nfibers:
				data = ""
				datab = ""
				for i in range(phi*wordlength, (phi+1)*wordlength, 4):
					#print event;
					hexnum = hex(int(all_tracks[i:i+4], 2))
					#print "hexnum: " + str(hexnum)
					data = data + str(hexnum)[2]
					datab = datab + str(all_tracks[i:i+4]);
				zlist = ['0' for i in range(wordlength)]
				zero = "".join(zlist)
				#print(data)
				if datab != zero:
					zb = datab[41:53];
					#print(zb);
					zf = 0.0;
					#for i in range(5):
					#	if ((zb[i] == "1") & (i == 0)):	
					#		zf = zf - pow(2,4);
					#	elif (zb[i] == "1"):
					#		zf = zf + pow(2,4-i);
					#for i in range(7):	
					#	if (zb[i+5] == "1"):
					#		zf = zf + pow(0.5,i+1);
					#print(zf);
					#if (abs(zf-vtxz) < 1.0) != 1:
					#	isdat = 0; #is there data to write? 0 for no and 1 for yes.
					#else:
					#	isdat=1;
			else:
				zlist = ['0' for i in range(wordlength)]
				#print str(zlist)
				data = "".join(zlist)
				
			if phi<9:
				fname = "phi" + str(phi) + "_n.dat"
			else:
				fname = "phi" + str(phi-9) + "_p.dat"		
			
			if event == 0 and track == 0:
				let = 'w'
			else:
				let = 'a' #if not first time opening file, append to it (don't overwrite)
			phifile = open(fname, let)
			#if isdat == 1:
			phifile.write("0x" + data + "\n")
			ntrks[phi]=ntrks[phi]+1;
			phifile.close()
	#now write 0s to signify end of event.
	for phi in range(nphi):
		if phi<9:
			fname = "phi" + str(phi) + "_n.dat"
		else:
			fname = "phi" + str(phi-9) + "_p.dat"		
		phifile = open(fname, 'a')
		for i in range(tpe-ntrks[phi]+1):
			phifile.write("0x" + "".join(['0' for i in range(int(math.ceil(wordlength/4)))]) + "\n")
		#print(ntrks[phi])
print "Data written successfully"
coe.close()		
