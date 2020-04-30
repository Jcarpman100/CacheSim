// File: cachesimulator.cpp
// Author(s): Joseph Carpman, Aditya Pethe
// Date: 04/22/2020
// Section: 510
// E-mail: jcarpman100@tamu.edu, apeth100@tamu.edu


#include <iostream>
#include <fstream>
#include <string>
#include<bits/stdc++.h> 
#include<cmath>

#ifndef CACHE_H
	#define CACHE_H
	
	
	using namespace std;
	
	class cache{
		private:
		int cache_size;
		int block_size;
		int associativity;
		int replacement;
		int write_hit;
		int write_miss;
		string* ram;
		int tagBits;
		int setBits;
		int offsetBits;
		int hits = 0; 
		int misses = 0;
		int rows = 0;
		int rowSize = 0;
		string temp_set, temp_tag;
		string** cach;
		
		public:
		
		//Default Constructor, made so that the compiler wouldn't yell at me, should not be used in reality.
		cache() { 
			cache_size = 0;
			block_size = 0;
			associativity = 0;
			replacement = 0;
			write_hit = 0;
			write_miss = 0;
			ram = new string();
		}
		
		//parameterized constructor, should take in the user specified parameters from the configure file, 
		//as well as the RAM from the main driver file
		cache(string* memory, int cacheSize, int blockSize, int associative, int replace, int hit, int miss) {
			ram = memory; //shallow copy the RAM
			
			//set member variables
			cache_size = cacheSize; 
			block_size = blockSize;
			associativity = associative;
			replacement = replace;
			write_hit = hit;
			write_miss = miss;
			
			//set number of bits each segment takes up
			offsetBits = (int)ceil(log2(block_size));
			setBits = (int)ceil(log2(cache_size/(block_size*associativity)));
			tagBits = (8) - (setBits + offsetBits);
			cout << tagBits << " " << setBits << " " << offsetBits << endl;
			
			//initialize the cache depending on settings
			rows = (int)pow(2,setBits);
			cach = new string*[rows];
			if(replacement == 2) {
				rowSize = associativity * (4 + block_size);
				for (int i = 0; i < rows; i++){
					cach[i] = new string[rowSize]; //initialize the other dimension, each block requires a tag, valid bit, and data 
					for(int j = 0; j < rowSize; j++){
						cach[i][j] = "0";
					}
				}
				} else {
				rowSize = associativity * (3 + block_size);
				for (int i = 0; i < rows; i++){
					cach[i] = new string[rowSize]; //initialize the other dimension, each block requires a tag, valid bit, and data 
					for(int j = 0; j < rowSize; j++){
						cach[i][j] = "0";
					}
				}
			}
		}
		
        void read(int numaddress){
			
			//turn the address to a bitstring
            bitset<8> b(numaddress);
            string bitstring = b.to_string();
            //cout<<bitstring<<endl;
			
            string tag = (bitstring.substr(0, tagBits));
			string tempIndex;
			
			if (offsetBits > 0){
				string index = (bitstring.substr(tagBits + setBits,offsetBits));
				stringstream tt;
				tt<<dec<<stoul(index,nullptr,2);
				tt>>tempIndex;
			}
			
			
			if (setBits > 0) {
				string set = (bitstring.substr(tagBits, setBits));
				cout<<"set: "<<hex<<stoul(set, nullptr, 2)<<endl;
				stringstream ss;
				ss<<hex<<stoul(set,nullptr,2);
				ss>>temp_set;
			}
			
            cout<<"tag: "<<hex<<stoul(tag, nullptr, 2)<<endl;
            stringstream tt;
			tt<<hex<<stoul(tag,nullptr,2);
			tt>>temp_tag;
			
			
            //Needs finishing: should return cache hit, eviction line, ram address, data
            //what r the private members/memory structure
			int row;
			if (setBits > 0) {
				row = stoi(temp_set);
				cout << row << endl;
			} else {
				row=0;
			}
			int hitrow, hitcol;
			bool hit = false;
			if (replacement == 1){
					for(int col = 2; col < rowSize; col+=3+block_size){
						if(temp_tag == cach[row][col] && cach[row][col-2] == "1"){
							hit = true;
							hitrow = row;
							hitcol = col;
							break;
						}
						hit = false;
					}
				} else if (replacement == 2){
					for(int col = 3; col < rowSize; col+=4+block_size){
						if(temp_tag == cach[row][col] && cach[row][col-3] == "1"){
							hit = true;
							hitrow = row;
							hitcol = col;
							break;
						}
						hit = false;
					}
			}
			
			
			if(hit){
				cout<<"cache_hit:yes"<<endl;
				cout<<"eviction_line:-1" << endl;
				cout<<"ram_address: -1" << endl;
				cout<<"data:0x" << cach[row][hitcol + stoi(tempIndex)] << endl; 
				hits++;
				}else{	
				cout<<"cache_hit:no"<<endl;
					if (replacement == 1){
						if (associativity == 1){
							cach[row][0] = "1";
							cach[row][2] = temp_tag;
							cout<<"eviction_line:" << (row) << endl;
							cout<<"ram_address:0x" << numaddress << endl;
							cout<<"Data:0x" << ram[numaddress] << endl;
							for(int i = 0; i < block_size; i++){
								cach[row][3+i] = ram[(numaddress - stoi(tempIndex)) + i];
							}
						}
					} else {
						if (associativity == 1){
							cach[row][0] = "1";
							cach[row][3] = temp_tag;
							cout<<"eviction_line:" << (row) << endl;
							cout<<"ram_address:0x" << numaddress << endl;
							cout<<"Data:0x" << ram[numaddress] << endl;
							for(int i = 0; i < block_size; i++){
								cach[row][4+i] = ram[(numaddress - stoi(tempIndex)) + i];
							}
						}
					}
				misses++;
			}
		}
		//32 4 2 1 1 1
		// git add . commit push pull
        void write(string data, int address){
			//cout<<"Reading..."<<endl;
			this->read(address);
			//cout<<"Done reading: "<<endl;
			bool hit = false;
			int hitrow, hitcol;
            //cout<<"set: "<<temp_set<<endl;
			//cout<<"tag: "<<temp_tag<<endl;
			if (replacement == 1){
				for(int row = 0; row<rows;row++){
					for(int col = 2; col < rowSize; col+=3+block_size){
						if(temp_tag == cach[row][col] && cach[row][col+1] == "1"){
							hit = true;
							hitrow = row;
							hitcol = col;
							break;
						}
						hit = false;
					}
					if(hit==true)break;
				}
				} else if (replacement == 2){
				for(int row = 0; row<rows;row++){
					for(int col = 3; col < rowSize; col+=4+block_size){
						if(temp_tag == cach[row][col] && cach[row][col+1] == "1"){
							hit = true;
							hitrow = row;
							hitcol = col;
							break;
						}
						hit = false;
					}
					if(hit==true)break;
				}
			}
			
			
			if(hit){
				cout<<"write_hit:yes"<<endl;
				cout<<"eviction_line:" + cach[hitrow][hitcol+2]<<endl;
				hits++;
				cach[hitrow][hitcol+2] = data;
				}else{
				cout<<"write_hit:no"<<endl;
				cout<<"eviction_line:0"<<endl;
				misses++;
			}
			cout<<"ram_address: 0x" << address <<endl;
			cout<<"data:"+ data<<endl;
			
		}
		
        void flush(){
			for(int row=0; row<rows;row++){
				for(int col=0; col<rowSize; col++){
					cach[row][col]="0";
				}
			}
		}
		
		
        void view(){
			cout << "cache_size: " << cache_size << endl;
			cout << "block_size: " << block_size << endl;
			cout << "associativity: " << associativity << endl;
			cout << "replacement_policy: " << replacement << endl;
			cout << "write_hit_policy: " << write_hit << endl;
			cout << "write_miss_policy: " << write_miss << endl;
			cout << "number_of_cache_hits: " << hits << endl;
			cout << "number_of_cache_misses: " << misses << endl;
			cout << "cache_content:" << endl;
			if (replacement == 1){
				for (int row = 0; row < rows; row++){
					for (int col = 0; col < rowSize; col+=(3+block_size)){
						cout << cach[row][col] << " " << cach[row][col+1] << " " << cach[row][col+2] << " ";
						for (int i = 3; i < (3+block_size); i++) {
							cout << cach[row][col+i] << " ";
						}
						cout << endl;
					}
				}
				} else {
				for (int row = 0; row < rows; row++){
					for (int col = 0; col < rowSize; col+=(4+block_size)){
						cout << cach[row][col] << " " << cach[row][col+1] << " " << cach[row][col+3] << " ";
						for (int i = 4; i < (4+block_size); i++) {
							cout << cach[row][col+i] << " ";
						}
						cout << endl;
					}
				}
			}
			cout << endl;
		}
		
		void dump(){
			ofstream ofs("cache.txt", ios::trunc);
			if (replacement == 1){
				for (int row = 0; row < rows; row++){
					for (int col = 0; col < rowSize; col+=(3+block_size)){
						for (int i = 3; i < (3+block_size); i++) {
							ofs << cach[row][col+i] << " ";
						}
						ofs << endl;
					}
				}
				} else {
				for (int row = 0; row < rows; row++){
					for (int col = 0; col < rowSize; col+=(4+block_size)){
						for (int i = 4; i < (4+block_size); i++) {
							ofs << cach[row][col+i] << " ";
						}
						ofs << endl;
					}
				}
			}
			ofs.close();
		}
		
		void memView(int size){
			cout << "memory_size:" << dec << size << "\nmemory_content:\nAddress:Data" << endl;
			for (int i = 0; i < size; i += 8){
				cout << "0x" << setfill('0') << setw(2) << hex << i << ":";
				string data = "";
				for (int j = 0; j < 8; j++){
					data = data + ram[i+j] + " ";
				}
				cout << data << endl;
			}
		}
		
		void memDump(int size){
			ofstream ofs("ram.txt", ios::trunc);
			for (int i = 0; i < size; i++){
				ofs << ram[i] << endl;
			}
			ofs.close();
		}
		
		
	};
	
#endif