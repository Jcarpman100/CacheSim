// File: cachesimulator.cpp
// Author(s): Joseph Carpman, Aditya Pethe
// Date: 04/22/2020
// Section: 510
// E-mail: jcarpman100@tamu.edu, apeth100@tamu.edu

#include <iostream>
#include <fstream>
#include <string>
#include<bits/stdc++.h> 
using namespace std;


int readFromFile(const char* filename, string* memory){
	ifstream ifs;
	ifs.open(filename);
	if (!ifs) {
		cout << "Unable to open file " << filename << endl;
		exit(1);   // call system to stop
	}
	int i = 0;
	string line;
	while(getline(ifs, line) && i < 256){
		memory[i] = line;
		i++;
	}
	int end = i;
	while(i < 256){
		memory[i] = "00";
		i++;
	}
	ifs.close();
	return end - 1;
}

int main(int argc, const char * argv[]) {
	cout <<  "*** Welcome to the cache simulator *** \ninitialize the RAM: " << endl;
    string ram[256];
	int end = readFromFile(argv[1], ram);
	cout << "init-ram 0x00 0x" << hex << end << endl << "ram successfully initialized!" << endl;
	//configure config = new configure();
	//simulate cache = config.getCache();
	string choice;
	while(choice != "quit"){
		cout << ("*** Cache simulator menu *** \ntype one command:\n1. cache-read\n2. cache-write\n3. cache-flush\n4. cache-view\n5. memory-view\n6. cache-dump\n7. memory-dump\n8. quit\n****************************\n");
		cin >> choice;
		transform(choice.begin(), choice.end(), choice.begin(), ::tolower); 
	}
	
	
}

