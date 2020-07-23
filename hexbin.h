#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
using namespace std;
//Function to convert a hexadecimal string to a binary string.
string hex_to_bin(string hex, int bits){ 
	string binstring = "";
	char byte;
	for(unsigned int l = 0; l < hex.length(); ++l){
		byte = hex.at(l);
		switch(byte){
			case '0': binstring = binstring + "0000";
				  break;
			case '1': binstring = binstring + "0001";
				  break;
			case '2': binstring = binstring + "0010";
				  break;
			case '3': binstring = binstring + "0011";	
				  break;
			case '4': binstring = binstring + "0100";
				  break;
			case '5': binstring = binstring + "0101";
				  break;
			case '6': binstring = binstring + "0110";
				  break;
			case '7': binstring = binstring + "0111";
				  break;
			case '8': binstring = binstring + "1000";
				  break;
			case '9': binstring = binstring + "1001";
				  break;
			case 'a': binstring = binstring + "1010";	
				  break;
			case 'b': binstring = binstring + "1011";
				  break;
			case 'c': binstring = binstring + "1100";
				  break;
			case 'd': binstring = binstring + "1101";	
				  break;
			case 'e': binstring = binstring + "1110";
				  break;
			case 'f': binstring = binstring + "1111";
				  break;
		}
	}
	return binstring.substr(binstring.length() - bits);	
}

//Function to convert a binary string to an integer.
int bin_to_int(string bin) {    
	int outint = 0;
	int value = 1;
	for(unsigned int m = 0; m < bin.length(); ++m){
		if(bin.at(bin.length()-1-m) == '1'){
			outint = outint + value;
		}
		value = 2 * value;
	}
	return outint;
}


//Convert an integer to its binary representation
string int_to_bin(int integer, int nbits) {
	string binstr = "";
	int val = pow(2, nbits - 1);
	for(int n = 0; n < nbits; ++n){
		if(integer >= val && integer != 0){
			binstr = binstr + "1";
			integer = integer - val;
		}
		else {
			binstr = binstr + "0";
		}
		val = val / 2;
	}
	return binstr;
}

//Convert a binary string to a hexadecimal string.
string bin_to_hex(string binary) {
	string quad;
	int digit;
	string str = "";
	while(binary.length() % 4 != 0){
		binary = "0" + binary;
	}
	for(unsigned int ii = 0; ii < binary.length(); ii = ii + 4) {
		quad = binary.substr(ii, 4);
		digit = bin_to_int(quad);
		switch(digit) {
			case 0: str = str + "0";
				break;
			case 1: str = str + "1";
				break;
			case 2: str = str + "2";
				break;
			case 3: str = str + "3";
				break;
			case 4: str = str + "4";
				break;
			case 5: str = str + "5";
				break;
			case 6: str = str + "6";
				break;
			case 7: str = str + "7";
				break;
			case 8: str = str + "8";
				break;
			case 9: str = str + "9";
				break;
			case 10: str = str + "a";
				break;
			case 11: str = str + "b";
				break;
			case 12: str = str + "c";
				break;
			case 13: str = str + "d";
				break;
			case 14: str = str + "e";
				break;
			case 15: str = str + "f";
				break;
		}
	}
	return str;
}
