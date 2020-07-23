import math
import random
import sys

# Take floating point data stored in sample_data.txt and convert it to hexadecimal data that can be read by the jet_finding emulation SW
#  This version should be consistent with the NEW track definition (May, 2020):
#	15 bits of pT* (note: NOT 1/pT)
#	12 bits of phi*: relative to center of sector, NOT absolute phi
#	16 bits of eta*
#	12 bits of Z0*
#	13 bits of d0
#	4 bits of chi2
#	3 bits of bend-chi2
#	7 bits of hit mask
#	3 bits of Track quality MVA
#	6 bits of specialized MVA selections
#	5 bits reserved for future use
# For a total of a 96-bit track. For our purposes all will be 0 except the ones with *s.

#bin size for each variable.
#pTinvstep = 0.5 * 2 / (2 ** 16-1)
maxpT = 2**14 #511.0
nzbits = 12
nptbits = 15
netabits = 16
nphibits = 12
ptstep = 2*maxpT / (2**nptbits-1)
etastep = 2.4*2 / (2**netabits-1) 
zstep = 15.0 * 2 / (2 ** nzbits-1)
phistep = 2*(math.pi/9) / (2**nphibits - 1)
#phistep = 2*math.pi / (2 ** 16 - 1)
nphibins = 9 #27

#convert a list of binary digits to a single decimal number
def bin_to_dec (binlist):
   dec = 0
   for j in range (0, len(binlist)):
      dec = dec + 2**(len(binlist) - j - 1) * int(binlist[j])
   return dec

#convert a decimal number to a list of binary digits
def dec_to_bin(dec, fillto):
   inlist = list(bin(dec))
   inlist = inlist[2:len(inlist)]
   inbin = "".join(inlist).zfill(fillto)
   inlist = list(inbin)
   return inlist

#find 2's complement of a list of binary digits. (this isn't used right now)
def negate (binlist, fillto):
   for j in range (0, len(binlist)):
      if binlist[j] == '0':
         binlist[j] = '1'
      else:
         binlist[j] = '0'
   temp = bin_to_dec(binlist) + 1
      
   outbin = dec_to_bin (temp, fillto)
   if len(outbin) > fillto:
      outbin = outbin[1:len(outbin)] #Overflow occur at 0... 
   return outbin

#write data in hexadecimal format to the given output file
#first 3 inputs are all floats
def writeData (pT, eta, z, phi, outfile):
 #calculate the values that will be stored in the track.
 #then convert to an integer, in binary form.
#   pTinv = 1/pT
   #print "pT in = " + str(pT)
   if pT > 0: pT = min(pT, maxpT)
   else: pT = max(pT, -1*maxpT)
   pTinval = int(round(abs(pT)))
#   pTinval = int(round((maxpT + pT)/ptstep))
#   if pTinval < 2**(nptbits-1):
#      pTinval += 2**(nptbits-1)
#   else:
#      pTinval -= 2**(nptbits-1)
   #print "pTinv_bin: " + str(pTinv_bin)
#   t = math.sinh(eta)
   etaval = int(round((eta + 2.4) / etastep))
   if etaval < 2**(netabits-1):
      etaval += 2**(netabits-1)
   else:
      etaval -= 2**(netabits-1)
#phi that is input to this function should be the angle from the middle of the sector!

   phival = int(round((phi + math.pi/9.0) / phistep))
   #print("first phival:");
   #print(phival);
   if phival < 2**(nphibits-1):
      phival += 2**(nphibits-1)
   else:
      phival -= 2**(nphibits-1)
   #print("phi value:");
   #print(phi);
   #print(phival);
   zval = int(round((z + 15.0) / zstep))
   if zval < 2**(nzbits-1):
      zval += 2**(nzbits-1)
   else:
      zval -= 2**(nzbits-1)
   spare = random.randint(0, 3)   #randomly decide if this is a x-track
#   charge_bin = dec_to_bin(0, 1)
   pTinv_bin = dec_to_bin(pTinval, nptbits)
   phi0_bin = dec_to_bin(phival, 12)
   eta_bin = dec_to_bin(etaval, netabits)
   z_bin = dec_to_bin(zval, nzbits)
   d0_bin = dec_to_bin(0, 13)
   chi2_bin = dec_to_bin(0, 4)
   bendchi2_bin = dec_to_bin(0, 3)
#   stubc_bin = dec_to_bin(0, 5)
   hitm_bin = dec_to_bin(0, 7)
   trk_qual_bin = dec_to_bin(0, 3)
   spec_bin = dec_to_bin(0, 6)
   spare_bin = dec_to_bin(spare, 5)   
#	15 bits of pT* (note: NOT 1/pT)
#	12 bits of phi*: relative to center of sector, NOT absolute phi
#	16 bits of eta*
#	12 bits of Z0*
#	13 bits of d0
#	4 bits of chi2
#	3 bits of bend-chi2
#	7 bits of hit mask
#	3 bits of Track quality MVA
#	6 bits of specialized MVA selections
#	5 bits reserved for future use
#put the various binary numbers together to form the track
   out_bin = pTinv_bin + phi0_bin + eta_bin + z_bin + d0_bin + chi2_bin + bendchi2_bin + hitm_bin + trk_qual_bin + spec_bin + spare_bin 
   #print "out_bin: " + str(out_bin)

#debugging: this shouldn't ever happen
   if not len(out_bin)==96:
       print("eta: " + str(eta) + " etaval: " + str(etaval) + " eta_bin: " + str(eta_bin))
       sys.exit("Error! invalid length of binary track: " + str(len(out_bin)))
        
#convert the track to hexadecimal format.
   out_dat = "%x" % bin_to_dec(out_bin)
#   out_dat = hex(bin_to_dec(out_bin))
#   print "out_dat: " + str(out_dat)
   outlist = list(out_dat)
   #print "outlist: " + str(outlist)
#   outlist = outlist[2:len(outlist)]
   out_dat = "".join(outlist).zfill(24)
   #print "out_dat: " + out_dat
   outfile.write("0x" + out_dat + "\n")
   return

infile = open("sample_data.txt", "r")
#create 9 output files, one for each phibin.
slices = []
for i in range(0, nphibins):
	for j in range(2):
		if j == 0:
			slices.append( open("phi" + str(i) + "_p.dat", "w"))
		else:
			slices.append(open("phi" + str(i) + "_n.dat", "w"))
eta_bins = 24
#startnum = int(input("Start number of events: "))
#endnum = int(input("End number of events: "))
#change these to only do a certain range of events.
startnum= 1
endnum = 999999
maxnlines = 4000 #255
line = infile.readline()
words = line.split()
count = 0
nevent = 1
ntrack = 0
tracksused = 0
tpe = [] #tracks per event
tpe_used = []
i = 0
event = []
tracks = 0
usedtracks = 0
loopcount = 0
totnlines = 0
while len(words) > 1 and totnlines < maxnlines:                                
	print words
	if nevent > endnum:
		break
   #for debugging
	loopcount += 1
   #event number
	evnum = words[1]
#	print(str(loopcount) + "line: " + line)
   #read each line of the jets, we don't need this information right now.
	while len(words) > 1 and words[0] == "Event" and words[1] == evnum:
		evnum = words[1]
		line = infile.readline()
		words = line.split()
	if nevent < startnum:
		while len(words) > 1 and words[0] != "Event":
			line = infile.readline()
			words = line.split()
		nevent += 1
		continue
	ntrack = 0
	maxntrk = 0
	ntrks = [0 for i in range(len(slices))]
	nevent += 1
  #for each track, find float values of each variable
	while len(words) > 1 and words[0] != "Event" and (totnlines + maxntrk) < maxnlines:
		ntrack = ntrack + 1
		pT = words[0]
		pTf = float(pT)
		eta = words[1]
		etaf = float(eta)
		phi = words[2]
		phif = float(phi)
		z = words[3]
		zf = float(z)
		print "words: " + str(words)
        #find correct phibin--we'll write to this file.
		phi_in = int(math.floor((phif + math.pi)*(nphibins)/(2*math.pi)))
	#which file we're writing to
		file_index = 2*phi_in
		if etaf < 0:
			file_index += 1
	#phi value at the middle of this sector
		middle_phi = -1*math.pi + math.pi / (nphibins) + phi_in * (2*math.pi / nphibins)
	#only accept the track if it's in the correct range for pT, eta, and z.
		if (abs(pTf)>=2) and (abs(etaf)<=2.4) and (abs(zf)<=15):
			print "phif: " + str(phif) + " phi_in: " + str(phi_in) + " middle_phi: " + str(middle_phi);
			tracksused = tracksused + 1
	#phi is measured from the center of the sector.
			phi_from_ctr = phif - middle_phi
		  #write the data to the correct output file 
			#if eta positive, write to _p file. Otherwise, _n file.
			writeData (pTf, etaf, zf, phi_from_ctr, slices[file_index])
		ntrks[file_index] += 1
		maxntrk = max(ntrks[file_index], maxntrk)
		line = infile.readline()
		words = line.split()
     #line of 0s separates each event.
	maxntrk =  112 #for now, 112 tracks per event
	for j in range(len(slices)):
		for k in range(ntrks[j], maxntrk+1):
			slices[j].write("0x000000000000000000000000\n")
	totnlines += maxntrk + 1
#print("event start: " + str(eventstart) + " eventend: " + str(eventend) + " words: " + str(words))
print("nevent: " + str(nevent) + " tracksused: " + str(tracksused))		
totlines = nevent + ntrack
print("writing ending 0s")
#extra line of 0s at end of all events.
for j in range(len(slices)):
#	slices[j].write("0x0000000000000000000000000\n")
	slices[j].close()
print("loopcount: " + str(loopcount))
infile.close()
