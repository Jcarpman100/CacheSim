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
			
			//initialize the cache depending on settings
			cach = new string*[(int)pow(2,setBits)];
			if(replacement == 2) {
				for (int i = 0; i < (int)pow(2,setBits); i++){
					cach[i] = new string[associativity * (4 + block_size)]; //initialize the other dimension, each block requires a tag, valid bit, and data 
				}
			} else {
				for (int i = 0; i < (int)pow(2,setBits); i++){
					cach[i] = new string[associativity * (3 + block_size)]; //initialize the other dimension, each block requires a tag, valid bit, and data 
				}
			}
		}
		
        void read(int numaddress){
		
			//turn the address to a bitstring
            bitset<8> b(numaddress);
            string bitstring = b.to_string();
            cout<<bitstring<<endl;
                
            string tag(bitstring.substr(0, tagBits));
			if (offsetBits > 0)
				string index(bitstring.substr(tagBits,tagBits + offsetBits));

			
			if (setBits > 0) {
				string set(bitstring.substr(tagBits + offsetBits,tagBits + offsetBits + setBits));
				cout<<"set: "<<dec<<stoul(set, nullptr, 2)<<endl;
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
        }
		//32 4 2 1 1 1
		// git add . commit push pull
        void write(string data, int address){
			cout<<"Reading..."<<endl;
			this->read(address);
			cout<<"Done reading: "<<endl;
			bool hit = false;
			int hitrow, hitcol;
            cout<<"set: "<<temp_set<<endl;
			cout<<"tag: "<<temp_tag<<endl;
			for(int row = 0; row<(int)pow(2,setBits);row++){
				for(int col = 0; col < associativity * (2 + block_size); col+=2+block_size){
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
			if(hit){
				cout<<"write_hit:yes"<<endl;
				cout<<"eviction_line:" + cach[hitrow][hitcol+2]<<endl;
				cach[hitrow][hitcol+2] = data;
			}else{
				cout<<"write_hit:no"<<endl;
				cout<<"eviction_line:0"<<endl;
			}
			cout<<"ram_address: 0x" + address <<endl;
			cout<<"data:"+ data<<endl;
			
        }

        void flush(){
			for(int row=0; row<(int)pow(2,setBits);row++){
				delete cach[row];
			}
        }
        void view(){

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
		for (int i = 0; i < size; i++){
			cout << ram[i] << endl;
		}
	}


};

#endif