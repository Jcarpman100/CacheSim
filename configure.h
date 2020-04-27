//configure file, takes user options and returns the created cache simulator object
//kinda like an advanced constructor.

public class configure(){
	int cache_size;
	int block_size;
	int associativity;
	int replacement;
	int write_hit;
	int write_miss;
	string input;
	simulate cache;
	
	
	configure(){
	
		cout << "configure the cache: \ncache size: ";
		cin >> input;
		cache_size = stoi(input);
		
		cout << endl << "data block size: "
		cin >> input;
		block_size = stoi(input);
		
		cout << endl << "associativity: "
		cin >> input;
		associativity = stoi(input);
		
		cout << endl << "replacement policy: "
		cin >> input;
		replacement = stoi(input);
		
		cout << endl << "write hit policy: "
		cin >> input;
		write_hit = stoi(input);
		
		cout << endl << "write miss policy: "
		cin >> input;
		write_miss = stoi(input);
		
		//cache = new simulate(cache_size, block_size, associativity, replacement, write_hit, write_miss);
		cout << endl << "cache successfully configured!" << endl;
	}
	
	simulate getCache(){
		return cache;
	}
}