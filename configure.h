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
			while(cache_size < 8 || cache_size > 256){
				cout << endl << "cache size: ";
				cin >> input;
				cache_size = stoi(input);
			}
			
			while(block_size < 1 || block_size > cache_size){
				cout << endl << "data block size: ";
				cin >> input;
				block_size = stoi(input);
			}
			
			while((associativity != 1 && associativity != 2 && associativity != 4) || (block_size * associativity > cache_size)) {
				cout << endl << "associativity: ";
				cin >> input;
				associativity = stoi(input);
			}
			
			while(replacement != 1 && replacement != 2) {
				cout << endl << "replacement policy: ";
				cin >> input;
				replacement = stoi(input);
			}
			
			while(write_hit != 1 && write_hit != 2) {
				cout << endl << "write hit policy: ";
				cin >> input;
				write_hit = stoi(input);
			}
			
			while(write_miss != 1 && write_miss != 2) {
				cout << endl << "write miss policy: ";
				cin >> input;
				write_miss = stoi(input);
			}
			
			cash = cache(ram, cache_size, block_size, associativity, replacement, write_hit, write_miss);
			cout << endl << "cache successfully configured!" << endl;
		}
		
		cache getCache(){
			return cash;
		}
};

#endif