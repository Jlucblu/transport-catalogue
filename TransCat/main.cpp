#include "transport_catalogue.h"
#include "stat_reader.h"
#include "json.h"
#include "json_reader.h"

#include <cassert>
#include <iostream>
#include <fstream>


using namespace std;
using namespace transport_catalogue;
using namespace reading_queries;
using namespace statistics;


void Test7() {
	TransportCatalogue tc;
	JSONReader rq(tc);

	std::ifstream file;
	file.open("TransCat\\tests\\input2.json");
	if (!file) {
		std::cout << "No File!\n"s;
		return;
	}

	json::Document doc = json::Load(file);

	rq.ParseBaseRequest(doc);

	PrintInfo(std::cin, std::cout, tc);
	system("pause");
}


int main() {
	Test7();
	

	/*
	std::string line = "TransCat\\tests\\tsC_case1_input.txt";
	std::ifstream in;
	in.open(line);
	if (in.is_open()) {
		UpdateCatalogue(in, tc);
		PrintInfo(in, cout, tc);
		}
	system("pause");
	in.close();
	*/
	
	/*
	UpdateCatalogue(cin, tc);
	PrintInfo(cin, cout, tc);
	*/
}