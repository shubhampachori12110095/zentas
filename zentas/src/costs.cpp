/*Copyright (c) 2016 Idiap Research Institute, http://www.idiap.ch/
Written by James Newling <jnewling@idiap.ch>

zentas is a k-medoids library written in C++ and Python. This file is part of zentas.
zentas is free software: you can redistribute it and/or modify it under the terms of
the GNU General Public License version 3 as published by the Free Software Foundation.
zentas is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details. You should have received a copy of
the GNU General Public License along with zentas. If not, see
<http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <map>
 
namespace costs{


bool isws(const char & c){
  return (c == ' ' ||  c == '\t' || c == '\n');
}

std::vector<std::string> split(const std::string & tosplit){
  std::vector<std::string> spv2;
  unsigned it = 0;	
  while (it != tosplit.size()){
    while (isws(tosplit[it]) and it != tosplit.size()){
      ++it;
    }
    unsigned start = it;
    while (!isws(tosplit[it]) and it != tosplit.size()){
      ++it;
    }
    unsigned end = it;
    if (!isws(tosplit[end -1])){
      spv2.push_back(tosplit.substr(start, end - start));
    }
  }
  return spv2;
} 

void set_costs(std::string filename, std::map<std::pair<char, char>, double> & substitution_costs, std::map<char, double> & indel_costs){

  std::ifstream input(filename);
  if(!input.good()){
    throw std::runtime_error( "Error opening '" + filename + "'. ");
  }
  
  std::string line, name, content;
  
  std::vector<std::vector<std::string>> splitlines; 
  while( std::getline(input, line).good() ){
    if (line.size() != 0){
      if (line[0] != '#'){
        auto splitline = split(line);
        splitlines.push_back(splitline);
      }
    }
  }
  
  if (splitlines.size() == 0){
    throw std::runtime_error("File " + filename + " appears to be empty ");
  }

  if (splitlines.size() == 1){
    throw std::runtime_error("File " + filename + " only contains 1 line, something is wrong ");
  }  
  
  if (splitlines.size() == 2 and splitlines[0][0] == "*"){
    double indel_cost;
    double substitution_cost;
    std::vector<std::string> splitline_indel;
    std::vector<std::string> splitline_substitution;
    
    if (splitlines[0].size() == 2 and splitlines[1].size() == 3){
      splitline_indel = splitlines[0];
      splitline_substitution = splitlines[1];
    }
    
    else if (splitlines[1].size() == 2 and splitlines[0].size() == 3){
      splitline_indel = splitlines[1];
      splitline_substitution = splitlines[0];
    }
    
    else{
      throw std::runtime_error("File " + filename + " has been deduced to contain global indel and substitution coefficients, and therefore should contain two lines, one : * v and the other * * v. ");
    }
    
    indel_cost = std::stod(splitline_indel[1]);
    substitution_cost = std::stod(splitline_substitution[2]);
 
    substitution_costs[std::pair<char, char> {'*', '*'} ] = substitution_cost;
    indel_costs['*'] = indel_cost; 
  }
  
  else{
    for (auto & splitline : splitlines){
      if (splitline.size() != 0 && splitline.size() != 3 && splitline.size() != 2){
        throw std::runtime_error("It appears that file " + filename + " contains a line which is not of the form '[char]  [char]   [double]' (substitution) or '[char]   [double]' (indel). ");
      }
      else{
        
        if ( splitline.size() == 2 && ((splitline[0].size() != 1))){
          throw std::runtime_error("It appears that file " + filename + " does not have [char]  [double] on one of the lines which is of length 2 -- one of the 'chars' is too long");
        }
        
        if ( splitline.size() == 3 && ((splitline[0].size() != 1) || (splitline[1].size() != 1))){
          throw std::runtime_error("It appears that file " + filename + " does not have [char] [char] [double] on one of the lines which is of length 3 -- one of the 'chars' is too long");
        }
        
        
        if (splitline[0][0] == splitline[1][0]){
          throw std::runtime_error("The characters are expected to be different - as this file should should specify distances, we implicitly assume 0 for like characters. Remove lines with like characters");
        }
        
        if (splitline.size() == 3){
          std::pair<char, char> key1(splitline[0][0], splitline[1][0]);
          std::pair<char, char> key2(splitline[1][0], splitline[0][0]);
          double value = std::stod(splitline[2]);
          
          for (auto & x : std::vector<std::pair<char, char>> {key1, key2} ){
            if (substitution_costs.count(x) != 0){
              if (value != substitution_costs[x]){
                throw std::runtime_error("contradictory input values in file " + filename + " . Note that [char1] [char2] must have the same value as [char2] [char1]");
              }
            }
          }
          
          substitution_costs[key1] = value;
          substitution_costs[key2] = value;
        }
      
        else if (splitline.size() == 2){
          char key = splitline[0][0];
          double value = std::stod(splitline[1]);
          if (indel_costs.count(key) != 0){
            throw std::runtime_error("Duplicate entry for indel cost of key " + splitline[0]);
          }
          
          indel_costs[key] = value;
        }
      }
    }
  }
}


} //namespace costs
