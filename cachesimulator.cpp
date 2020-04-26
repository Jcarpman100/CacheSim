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
		line = line.substr(0,2);
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
    string ram[256];
	int end = readFromFile(argv[1], ram);
	cout << "init-ram 0x00 0x" << hex << end << endl << "ram successfully initialized!" << endl;
	//configure config = new configure();
	//simulate cache = config.getCache();
	string choice;
	string address;
	string data;
	int numaddress = 0;
	while(choice != "quit"){
		cout << ("*** Cache simulator menu *** \ntype one command:\n1. cache-read\n2. cache-write\n3. cache-flush\n4. cache-view\n5. memory-view\n6. cache-dump\n7. memory-dump\n8. quit\n****************************\n");
		cin >> choice;
		transform(choice.begin(), choice.end(), choice.begin(), ::tolower); 
		if (choice.find("cache-read") != string::npos){
			cin >> address;
			numaddress = stoi(address, 0, 16);
			std::stringstream stream;
			stream << numaddress;
			stream >> hex >> numaddress;
			//cache.read(numaddress);
		} else if (choice.find("cache-write") != string::npos){
			cin >> address;
			cin >> data;
			numaddress = stoi(address, 0, 16);
			std::stringstream stream;
			stream << numaddress;
			stream >> hex >> numaddress;
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

