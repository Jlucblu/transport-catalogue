#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

#include <cassert>
#include <iostream>
#include <fstream>


using namespace std;
using namespace transport_catalogue;
using namespace input_handler;
using namespace statistics;

int main() {
	TransportCatalogue tc;
	/*
	std::string line = "tests\\tsC_case1_input.txt";
	std::ifstream in;
	in.open(line);
	if (in.is_open()) {
		UpdateCatalogue(in, tc);
		PrintInfo(in, cout, tc);
		}
	in.close();
	*/
	
	UpdateCatalogue(cin, tc);
	PrintInfo(cin, cout, tc);
	
}