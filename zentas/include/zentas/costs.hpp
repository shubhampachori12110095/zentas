// Copyright (c) 2016 Idiap Research Institute, http://www.idiap.ch/
// Written by James Newling <jnewling@idiap.ch>

#ifndef ZENTAS_COSTS_HPP
#define ZENTAS_COSTS_HPP

#include <map>
#include <string>

namespace costs
{

bool isws(const char& c);

std::vector<std::string> split(const std::string& tosplit);

void set_costs(std::string filename,
               std::map<std::pair<char, char>, double>& substitution_costs,
               std::map<char, double>& indel_costs);
}

#endif
