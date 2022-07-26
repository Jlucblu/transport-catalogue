#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

#include <cassert>
#include <iostream>


using namespace std;

int main() {
	TransportCatalogue tc;
	UpdateCat(cin, tc);
	PrintInfo(cin, cout, tc);
}