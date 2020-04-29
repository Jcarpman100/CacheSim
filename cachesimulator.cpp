// File: cachesimulator.cpp
// Author(s): Joseph Carpman, Aditya Pethe
// Date: 04/22/2020
// Section: 510
// E-mail: jcarpman100@tamu.edu, apeth100@tamu.edu

#include <iostream>
#include <fstream>
#include <string>
#include<bits/stdc++.h> 
#include "configure.h"
#include "cache.h"
using namespace std;


int readFromFile(const char* filename, string* memory){
	ifstream ifs;
	
	//atempt to open the file, if that doesn't work, print and stop the program
	ifs.open(filename);
	if (!ifs) {
		cout << "Unable to open file " << filename << endl;
		exit(1);   // call system to stop
	}
	
	//parse through memory, making sure not to go over 256 values
	int i = 0;
	string line;
	while(getline(ifs, line) && i < 256){
		line = line.substr(0,2);
		memory[i] = line;
		i++;
	}
	int end = i;
	// if there are less than 256 values, remember the end and initialize the rest of the list with "00"
	while(i < 256){
		memory[i] = "00";
		i++;
	}
	
	//close and return where the values ended
	ifs.close();
	return end - 1;
}

void memView(string* memory, int size){
	cout << "memory_size:" << dec << size << "\nmemory_content:\nAddress:Data" << endl;
	for (int i = 0; i < size; i += 8){
		cout << "0x" << setfill('0') << setw(2) << hex << i << ":";
		string data = "";
		for (int j = 0; j < 8; j++){
			data = data + memory[i+j] + " ";
		}
		cout << data << endl;
	}
}

void memDump(string* memory, int size){
	for (int i = 0; i < size; i++){
		cout << memory[i] << endl;
	}
}

int main(int argc, const char * argv[]) {
	
	cout <<  "*** Welcome to the cache simulator *** \ninitialize the RAM: " << endl;
	
	//declare and fill RAM, as well as remembering how many values there actually is. Then print success
    string ram[256];
	int end = readFromFile(argv[1], ram);
	cout << "init-ram 0x00 0x" << hex << end << endl << "ram successfully initialized!" << endl;
	
	//get user input of the cache, and construct it
	configure config = configure(ram);
	cache cache = config.getCache();
	
	//initialize temporary variables
	string choice;
	string address;
	string data;
	int numaddress = 0;
	
	//loop for user input, exits when the user gives quit.
	while(choice != "quit"){
		//print menu then take input of choice
		cout << ("*** Cache simulator menu *** \ntype one command:\n1. cache-read\n2. cache-write\n3. cache-flush\n4. cache-view\n5. memory-view\n6. cache-dump\n7. memory-dump\n8. quit\n****************************\n");
		cin >> choice;
		
		//lowercase the entire input for simplicity's sake
		transform(choice.begin(), choice.end(), choice.begin(), ::tolower); 
		if (choice.find("cache-read") != string::npos){
			cin >> address;
			numaddress = stoi(address, 0, 16);
			cout << numaddress << endl;
			cache.read(numaddress);
		} else if (choice.find("cache-write") != string::npos){
			cin >> address;
			cin >> data;
			numaddress = stoi(address, 0, 16);
			//cache.write(numaddress, data);
		} else if (choice.find("cache-flush") != string::npos){
			//cache.flush();
		} else if (choice.find("cache-view") != string::npos){
			//cache.view();
		} else if (choice.find("memory-view") != string::npos){
			memView(ram, end);
		} else if (choice.find("cache-dump") != string::npos){
			//cache.dump();
		} else if (choice.find("memory-dump") != string::npos){
			memDump(ram, end);
		} else if (choice.find("quit") != string::npos){
		} else {
			cout << "Invalid Command" << endl;
		}
	}
	
	
}

