import fileinput
#This script will convert a raw unformatted sim file into a correctly formatted one
valid = 0;

with open('result_0_plus2x18cnt.txt', 'r') as file:
	data = file.readlines();


for i in range(len(data)):
	if i > 0:
		if(data[i][59:60] == "1"):
			data[i-1] = data[i][71:79].lower() + "\n";
			valid = 1;
		elif(valid == 1):
			data[i-1] = "00000000" + "\n";
			valid = 0;
		else:
			data[i-1] = "";
data[len(data)-1] = "";

print data;

with open('sim_outL2.txt', 'w') as file:
	file.writelines(data);		
