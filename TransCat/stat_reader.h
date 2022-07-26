#pragma once
#include "transport_catalogue.h"
#include "input_reader.h"

#include <sstream>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <set>
#include <algorithm>


void PrintInfo(std::istream& in, std::ostream& out, TransportCatalogue& cat);