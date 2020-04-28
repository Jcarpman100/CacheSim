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

class cache{
    private:

    public:
        void read(int numaddress){
            bitset<13> b(numaddress);
            string bitstring = b.to_string();
            cout<<bitstring<<endl;
            
            bitset<8>tag(bitstring.substr(0,8));
            bitset<3>index(bitstring.substr(8,10));
            bitset<2>set(bitstring.substr(11,12));

            cout<<"set: "<<dec<<set.to_ulong()<<endl;
            cout<<"tag: "<<hex<<tag.to_ulong()<<endl;
            
            //Needs finishing: should return cache hit, eviction line, ram address, data
            //what r the private members/memory structure
        }

        void write(){
            
        }
        void flush(){

        }
        void view(){

        }

};