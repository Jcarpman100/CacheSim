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
		int temp_set;
		int temp_tag;
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
			temp_set = 0;
			temp_tag = 0;
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
			temp_set = 0;
			temp_tag = 0;
			cach = new string*[(int)pow(2,setBits)];
			for (int i = 0; i < (int)pow(2,setBits); i++){
				cach[i] = new string[associativity * (2 + block_size)]; //initialize the other dimension, each block requires a tag, valid bit, and data 
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
			}
			
            cout<<"tag: "<<hex<<stoul(tag, nullptr, 2)<<endl;
            
            //Needs finishing: should return cache hit, eviction line, ram address, data
            //what r the private members/memory structure
        }

        void write(int data, int address){
			this->read(address);
            
        }
        void flush(){

        }
        void view(){

        }

};

#endif