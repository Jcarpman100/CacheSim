//configure file, takes user options and returns the created cache simulator object
//kinda like an advanced constructor.

#include <iostream>
#include <fstream>
#include <string>
#include<bits/stdc++.h> 
#include "cache.h"

#ifndef CONFIG_H
#define CONFIG_H

using namespace std;

class configure{
	private:
		int cache_size=0;
		int block_size=0;
		int associativity=0;
		int replacement=0;
		int write_hit=0;
		int write_miss=0;
		string input;
		cache cash;
	
	public:
	
		configure(string* ram){
		
			cout << "configure the cache: ";
			
			//only take cache sizes between 8 & 256
			while(cache_size < 8 || cache_size > 256){
				cout << endl << "cache size: ";
				cin >> input;
				cache_size = stoi(input);
			}
			
			//block_size must be between 1 & cache_size
			while(block_size < 1 || block_size > cache_size){
				cout << endl << "data block size: ";
				cin >> input;
				block_size = stoi(input);
			}
			
			//associativity must be 1, 2, or 4, also the block size * associativity must be less than the cache size
			while((associativity != 1 && associativity != 2 && associativity != 4) || (block_size * associativity > cache_size)) {
				cout << endl << "associativity: ";
				cin >> input;
				associativity = stoi(input);
			}
			
			//only 2 options, 1 & 2
			//Random replacement (RR) (1) or the Least Recently Used (LRU) (2)  policy.
			while(replacement != 1 && replacement != 2) {
				cout << endl << "replacement policy: ";
				cin >> input;
				replacement = stoi(input);
			}
			
			//only 2 options, 1 & 2
			//write-through (1): write the data in both the block in the cache and the block in RAM.
			//write-back (2): write the data only in the block in the cache. 	
			while(write_hit != 1 && write_hit != 2) {
				cout << endl << "write hit policy: ";
				cin >> input;
				write_hit = stoi(input);
			}
			
			//only 2 options, 1 & 2
			//write-allocate (1): load the block from RAM and write it in the cache.
			//no write-allocate (2): write the block in RAM and do not load it in the cache.
			while(write_miss != 1 && write_miss != 2) {
				cout << endl << "write miss policy: ";
				cin >> input;
				write_miss = stoi(input);
			}
			
			//pass the info to the cache constructor
			cash = cache(ram, cache_size, block_size, associativity, replacement, write_hit, write_miss);
			
			//print success
			cout << endl << "cache successfully configured!" << endl;
		}
		
		//returns the cache that was made
		cache getCache(){
			return cash;
		}
};

#endif