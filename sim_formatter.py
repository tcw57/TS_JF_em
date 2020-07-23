import fileinput
#This script will convert a raw unformatted sim file into a correctly formatted one


with open('sim_outL2.txt', 'r') as file:
	data = file.readlines();
print(data);

for i in range(len(data)):
	data[i] = data[i].strip() + "\n";

print(data);

with open('sim_outL2.txt', 'w') as file:
	file.writelines(data);		
