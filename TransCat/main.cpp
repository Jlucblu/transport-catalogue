#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

#include <cassert>
#include <iostream>
#include <fstream>


using namespace std;

int main() {
	TransportCatalogue tc;
	
	std::string line = "tests\\tsC_case1_input.txt";
	std::ifstream in;
	in.open(line);
	if (in.is_open()) {
		UpdateCat(in, tc);
		PrintInfo(in, cout, tc);
		}
	in.close();
	
	
	UpdateCat(cin, tc);
	PrintInfo(cin, cout, tc);
	
}