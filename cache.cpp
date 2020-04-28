// File: cachesimulator.cpp
// Author(s): Joseph Carpman, Aditya Pethe
// Date: 04/22/2020
// Section: 510
// E-mail: jcarpman100@tamu.edu, apeth100@tamu.edu


//define all the configure funcitons here

#include <iostream>
#include <fstream>
#include <string>
#include<bits/stdc++.h> 
#include "cache.h"
using namespace std;

void test(){
    int numaddress;
    string address;
    cin >> address;
    numaddress = stoi(address, 0, 16);
    stringstream stream;
	stream << numaddress;
	stream >> hex >> numaddress;
    
    cache c = cache();
    c.read(numaddress);
}

int main(){
    test();
};