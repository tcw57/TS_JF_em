#this script extracts the vtx z-positions from the human_readable_patterns.txt file
fileIn = "human_readable_patterns.txt";
vFile = open(fileIn,"r");
fileOut = "v_data.dat";
Outfile = open(fileOut,"w");
nevents = 100;
for i in range(nevents):
	vFile.readline();
	for j in range(1711):
		vFile.readline();
	line = vFile.readline();
	vtx = line[42:48] + "\n";
	if (vtx[0] == "+"):
		vtx = vtx[1:len(vtx)-1] + "\n";
	Outfile.write(vtx);
	vFile.readline();
vFile.close();
Outfile.close();
