// File: cache.h
// Author(s): Joseph Carpman, Aditya Pethe
// Date: 04/22/2020
// Section: 510
// E-mail: jcarpman100@tamu.edu, apeth100@tamu.edu


#include <iostream>
#include <fstream>
#include <string>
#include<bits/stdc++.h> 
#include<cmath>
#include <experimental/random>

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
		int flagBits = 0;
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
		
		/*~cache() {
			cout << "destructor Called" << endl;
			for (int i = 0; i < rows; i++){
			delete [] cach[i];
			}
			//delete [] cach;
			//delete [] ram;
		}*/
		
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
			
			//initialize the cache depending on settings
			rows = (int)pow(2,setBits);
			cach = new string*[rows];
			if(replacement == 2) {
				flagBits = 3;
				} else {
				flagBits = 2;
			}
			rowSize = associativity * (flagBits + 1 + block_size);
			for (int i = 0; i < rows; i++){
				cach[i] = new string[rowSize]; //initialize the other dimension, each block requires a tag, valid bit, and data 
				for(int j = 0; j < rowSize; j++){
					cach[i][j] = "0";
				}
			}
		}
		
        void read(int numaddress){
			
			if (numaddress >= 256){
				cout << "Address is too large. \n";
				return;
				} else if (numaddress < 0){
				cout << "Address is too small, how did you manage that? \n";
				return;
			}
			
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
				} else {
				tempIndex = "0";
			}
			
			
			if (setBits > 0) {
				string set = (bitstring.substr(tagBits, setBits));
				cout<<"set:"<<hex<<stoul(set, nullptr, 2)<<endl;
				stringstream ss;
				ss<<hex<<stoul(set,nullptr,2);
				ss>>temp_set;
				} else {
				temp_set = "0";
				cout<<"set:0" << endl;
			}
			
			
			if (tagBits > 0) {
				cout<<"tag:"<<hex<<stoul(tag, nullptr, 2)<<endl;
				stringstream tt;
				tt<<hex<<stoul(tag,nullptr,2);
				tt>>temp_tag;
				} else {
				temp_tag = "0";
				cout<<"tag:0"<<endl;
			}
			
			int row;
			if (setBits > 0) {
				row = stoi(temp_set);
				} else {
				row=0;
			}
			int line = 0;
			int hitrow, hitcol;
			bool hit = false;
			for(int col = flagBits; col < rowSize; col+=flagBits+1+block_size){
				if((temp_tag == cach[row][col] || setBits == 8) && cach[row][col-flagBits] == "1"){
					hit = true;
					hitrow = row;
					hitcol = col;
					break;
				}
				line++;
				hit = false;
			}
			
			
			
			
			
			if(hit){
				cout<<"cache_hit:yes"<<endl;
				cout<<"eviction_line:-1" << endl;
				cout<<"ram_address: -1" << endl;
				cout<<"data:0x" << cach[row][hitcol + stoi(tempIndex)] << endl; 
				if (replacement == 2){
					shuffleUse(row, line);
				}
				hits++;
				}else{	
				cout<<"cache_hit:no"<<endl;
				int evicted = evictionLine(row);
				if(cach[row][evicted*(flagBits+1+block_size) + 1] == "1"){
					reWriteRam(row, evicted);
				}
				cach[row][evicted*(flagBits+1+block_size)] = "1";
				cach[row][evicted*(flagBits+1+block_size) + flagBits] = temp_tag;
				cout<<"eviction_line:" << (row*associativity + evicted) << endl;
				
				cout<<"ram_address:0x" << numaddress << endl;
				cout<<"Data:0x" << ram[numaddress] << endl;
				for(int i = 0; i < block_size; i++){
					cach[row][((evicted*(flagBits+1+block_size)) + flagBits+1+i)] = ram[(numaddress - stoi(tempIndex)) + i];
				}
				misses++;
			}
		}
		//32 4 2 1 1 1
		// git add . commit push pull
        void write(string data, int address){
			
			if (data.length() > 2){
				cout << "invalid data. \n";
				return;
				} else if (address >= 256){
				cout << "Address is too large. \n";
				return;
				} else if (address < 0){
				cout << "Address is too small, how did you manage that? \n";
				return;
			}
			
            //cout<<"set: "<<temp_set<<endl;
			//turn the address to a bitstring
            bitset<8> b(address);
            string bitstring = b.to_string();
            //cout<<bitstring<<endl;
			
            string tag = (bitstring.substr(0, tagBits));
			string tempIndex;
			
			if (offsetBits > 0){
				string index = (bitstring.substr(tagBits + setBits,offsetBits));
				stringstream tt;
				tt<<dec<<stoul(index,nullptr,2);
				tt>>tempIndex;
				} else {
				tempIndex = "0";
			}
			
			
			if (setBits > 0) {
				string set = (bitstring.substr(tagBits, setBits));
				cout<<"set:"<<hex<<stoul(set, nullptr, 2)<<endl;
				stringstream ss;
			ss<<hex<<stoul(set,nullptr,2);
			ss>>temp_set;
			} else {
			temp_set = "0";
			cout<<"set:0" << endl;
			}
			
			
			if (tagBits > 0) {
				cout<<"tag:"<<hex<<stoul(tag, nullptr, 2)<<endl;
				stringstream tt;
				tt<<hex<<stoul(tag,nullptr,2);
				tt>>temp_tag;
				} else {
				temp_tag = "0";
				cout<<"tag:0"<<endl;
			}
			
			
            //Needs finishing: should return cache hit, eviction line, ram address, data
            //what r the private members/memory structure
			int row;
			if (setBits > 0) {
				row = stoi(temp_set);
				} else {
				row=0;
			}
			int hitrow, hitcol;
			int line = 0;
			bool hit = false;
			for(int col = flagBits; col < rowSize; col+=flagBits+1+block_size){
				if(temp_tag == cach[row][col] && cach[row][col-flagBits] == "1"){
					hit = true;
					hitrow = row;
					hitcol = col;
					break;
				}
				line++;
				hit = false;
			}
			
			int dirtyBit = 0;
			if(hit){
				cout<<"write_hit:yes"<<endl;
				cout<<"eviction_line:-1"<<endl;
				cout<<"ram_address:-1" <<endl;
				if (replacement == 2){
					shuffleUse(row, line);
				}
				hits++;
				cach[hitrow][hitcol+stoi(tempIndex)+1] = data;
				if (write_hit == 1){
					ram[address] = data;
					} else {
					cach[hitrow][(hitcol-flagBits)+1] = "1";
					dirtyBit = 1;
				}
				}else{
				
				cout<<"write_hit:no"<<endl;
				if (write_miss == 2) {
					cout<<"ram_address: 0x" << address <<endl;
					ram[address] = data;
					} else {
					int evicted = evictionLine(row);
					if(cach[row][evicted*(flagBits+1+block_size) + 1] == "1"){
						reWriteRam(row, evicted);
					}
					cach[row][evicted*(flagBits+1+block_size)] = "1";
					cach[row][evicted*(flagBits+1+block_size) + flagBits] = temp_tag;
					cout<<"eviction_line:" << (row*associativity + evicted) << endl;
					cout<<"ram_address:0x" << address << endl;
					for(int i = 0; i < block_size; i++){
						cach[row][((evicted*(flagBits+1+block_size)) + flagBits+1+i)] = ram[(address - stoi(tempIndex)) + i];
					}
					cach[row][((evicted*(flagBits+1+block_size)) + flagBits+stoi(tempIndex)+1)] = data;
					if (write_hit == 1){
						ram[address] = data;
						} else {
						cach[row][(evicted*(flagBits+1+block_size))+1] = "1";
						dirtyBit = 1;
					}
				}
				misses++;
			}
			
			cout<<"data:"+ data<<endl;
			cout<<"dirty_bit:" << dirtyBit << endl;
			
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
			if (replacement == 1){
				cout << "replacement_policy:random_replacement" << endl;
				} else {
				cout << "replacement_policy:least_recently_used" << endl;
			}
			if (write_hit == 1){
				cout << "write_hit_policy:write_through" << endl;
				} else {
				cout << "write_hit_policy:write_back"<< endl;
			}
			if (write_miss == 1){
				cout << "write_miss_policy:write_allocate" << endl;
				} else {
				cout << "write_miss_policy:no_write_allocate" << endl;
			}
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
		
		int evictionLine(int set){
			if (associativity == 1) {
				return 0;
				} else if (associativity == 2) {
				if (cach[set][0] == "0"){
					if(replacement == 2){
						cach[set][2] = "1";
						if (cach[set][flagBits+3+block_size] == "1")
						cach[set][flagBits+3+block_size] = "2";
					}
					return 0;
					
					} else if (cach[set][flagBits+1+block_size] == "0"){
					if(replacement == 2){
						cach[set][flagBits+3+block_size] = "1";
						if (cach[set][2] == "1")
						cach[set][2] = "2";
					}
					return 1;
					} else {
					
					if(replacement == 2){
						
						if (cach[set][2] == "2"){
							cach[set][2] = "1";
							if (cach[set][flagBits+3+block_size] == "1")
							cach[set][flagBits+3+block_size] = "2";
							return 0;
							} else if (cach[set][flagBits+3+block_size] == "2"){
							cach[set][flagBits+3+block_size] = "1";
							if (cach[set][2] == "1")
							cach[set][2] = "2";
							return 1;
						}
						
						} else {
						return std::experimental::randint(0,1);
					}
				}
				
				} else {
				int lineSize = flagBits+1+block_size;
				if (cach[set][0*lineSize] == "0"){
					if(replacement == 2){
						cach[set][2] = "1";
						if (stoi(cach[set][lineSize + 2]) > 0)
						cach[set][lineSize + 2] = to_string(stoi(cach[set][lineSize + 2]) + 1);
						if (stoi(cach[set][2*lineSize + 2]) > 0)
						cach[set][2*lineSize + 2] = to_string(stoi(cach[set][2*lineSize + 2]) + 1);
						if (stoi(cach[set][3*lineSize + 2]) > 0)
						cach[set][3*lineSize + 2] = to_string(stoi(cach[set][3*lineSize + 2]) + 1);
						
					}
					return 0;
					
					} else if (cach[set][1*lineSize] == "0"){
					if(replacement == 2){
						cach[set][(1*lineSize) + 2] = "1";
						if (stoi(cach[set][2]) > 0)
						cach[set][2] = to_string(stoi(cach[set][2]) + 1);
						if (stoi(cach[set][2*lineSize + 2]) > 0)
						cach[set][2*lineSize + 2] =  to_string(stoi(cach[set][2*lineSize + 2]) + 1);
						if (stoi(cach[set][3*lineSize + 2]) > 0)
						cach[set][3*lineSize + 2] = to_string(stoi(cach[set][3*lineSize + 2]) + 1);
					}
					return 1;
					
					} else if (cach[set][2*lineSize] == "0"){
					if(replacement == 2){
						cach[set][(2*lineSize) + 2] = "1";
						if (stoi(cach[set][lineSize + 2]) > 0)
						cach[set][lineSize + 2] = to_string(stoi(cach[set][lineSize + 2]) + 1);
						if (stoi(cach[set][2]) > 0)
						cach[set][2] = to_string(stoi(cach[set][2]) + 1);
						if (stoi(cach[set][3*lineSize + 2]) > 0)
						cach[set][3*lineSize + 2] = to_string(stoi(cach[set][3*lineSize + 2]) + 1);
					}
					return 2;
					
					} else if (cach[set][3*lineSize] == "0"){
					if(replacement == 2){
						cach[set][(3*lineSize) + 2] = "1";
						if (stoi(cach[set][lineSize + 2]) > 0)
						cach[set][lineSize + 2] = to_string(stoi(cach[set][lineSize + 2]) + 1);
						if (stoi(cach[set][2*lineSize + 2]) > 0)
						cach[set][2*lineSize + 2] = to_string(stoi(cach[set][2*lineSize + 2]) + 1);
						if (stoi(cach[set][2]) > 0)
						cach[set][2] = to_string(stoi(cach[set][2]) + 1);
					}
					return 3;
					
					} else {
					if(replacement == 2){
						if (cach[set][2] == "4"){
							cach[set][2] = "1";
							if (stoi(cach[set][lineSize + 2]) > 0)
							cach[set][lineSize + 2] = to_string(stoi(cach[set][lineSize + 2]) + 1);
							if (stoi(cach[set][2*lineSize + 2]) > 0)
							cach[set][2*lineSize + 2] = to_string(stoi(cach[set][2*lineSize + 2]) + 1);
							if (stoi(cach[set][3*lineSize + 2]) > 0)
							cach[set][3*lineSize + 2] = to_string(stoi(cach[set][3*lineSize + 2]) + 1);
							return 0;
							
							} else if (cach[set][1*lineSize + 2] == "4"){
							cach[set][(1*lineSize) + 2] = "1";
							if (stoi(cach[set][2]) > 0)
							cach[set][2] = to_string(stoi(cach[set][2]) + 1);
							if (stoi(cach[set][2*lineSize + 2]) > 0)
							cach[set][2*lineSize + 2] = to_string(stoi(cach[set][2*lineSize + 2]) + 1);
							if (stoi(cach[set][3*lineSize + 2]) > 0)
							cach[set][3*lineSize + 2] = to_string(stoi(cach[set][3*lineSize + 2]) + 1);
							return 1;
							
							} else if (cach[set][2*lineSize + 2] == "4"){
							cach[set][(2*lineSize) + 2] = "1";
							if (stoi(cach[set][lineSize + 2]) > 0)
							cach[set][lineSize + 2] = to_string(stoi(cach[set][lineSize + 2]) + 1);
							if (stoi(cach[set][0*lineSize + 2]) > 0)
							cach[set][0*lineSize + 2] = to_string(stoi(cach[set][0*lineSize + 2]) + 1);
							if (stoi(cach[set][3*lineSize + 2]) > 0)
							cach[set][3*lineSize + 2] = to_string(stoi(cach[set][3*lineSize + 2]) + 1);
							return 2;
							
							} else if (cach[set][3*lineSize + 2] == "4"){
							cach[set][(3*lineSize) + 2] = "1";
							if (stoi(cach[set][lineSize + 2]) > 0)
							cach[set][lineSize + 2] = to_string(stoi(cach[set][lineSize + 2]) + 1);
							if (stoi(cach[set][2*lineSize + 2]) > 0)
							cach[set][2*lineSize + 2] = to_string(stoi(cach[set][2*lineSize + 2]) + 1);
							if (stoi(cach[set][0*lineSize + 2]) > 0)
							cach[set][0*lineSize + 2] = to_string(stoi(cach[set][0*lineSize + 2]) + 1);
							return 3;
							} else {
							return 0;
						}
						} else {
						return std::experimental::randint(0,3);
					}
				}
			}
		}
		
		void shuffleUse(int set, int line){
			int use = stoi(cach[set][(line*(flagBits+1+block_size)) + 2]);
			if (use > 0){
				cach[set][(line*(flagBits+1+block_size)) + 2] = to_string(use - 1);
				for (int i = 0; i < associativity ; i++){
					int temp = stoi(cach[set][(i*(flagBits+1+block_size)) + 2]);
					if (temp < use && temp > 0) {
						cach[set][(i*(flagBits+1+block_size)) + 2] = to_string(temp+1);
					}
				}
				cach[set][(line*(flagBits+1+block_size)) + 2] = "1";
			}
		}
		
		void reWriteRam(int row, int evicted){
			string evicTag = cach[row][evicted*(flagBits+1+block_size) + flagBits];
			bitset<8> b(stoi(evicTag));
			string bitstring = b.to_string();
			string tags = bitstring.substr(8-tagBits, tagBits);
			bitset<8> c(row);
			bitstring = c.to_string();
			string rowbit = bitstring.substr(8-setBits, setBits);
			bitset<8> a(0);
			bitstring = a.to_string();
			string offsetbit = bitstring.substr(8-offsetBits, offsetBits);
			string evicaddress = tags + rowbit + offsetbit;
			int numaddress = stoi(evicaddress, nullptr, 2);
			for(int i = 0; i < block_size; i++){
				ram[numaddress + i] = cach[row][((evicted*(flagBits+1+block_size)) + flagBits+1+i)];
			}
			cach[row][((evicted*(flagBits+1+block_size)) + 1)] = "0";
		}
	};
	
#endif	