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
#include <vector>
#include <iostream>
#include <map>

#include "fasta.hpp"
#include "zentas.hpp"



/* Test case : clustering sparse vectors. */
int cluster_sparse(){
  
  //generating random data
  size_t ndata = 5;
  std::vector<size_t> sizes = {2,3,2,1,2};
  size_t n_non_zeros = 10; //sum of sizes. This is the total number of non-zero values.

  std::vector<double> data_in (n_non_zeros);   
  for (size_t i = 0; i < n_non_zeros; ++i){
    data_in[i] = 3.1415111101111;
  }
  
  std::vector<size_t> indices { //the non-zero elements, should correspond to sizes. 
    3, 7,
    3,7,11,
    4, 12, 
    4, 
    4, 100000}; 
  
  size_t K = 2;
  std::vector<size_t> indices_init = {0,1};
  std::string algorithm = "clarans";
  size_t level = 0;
  size_t max_proposals = 100;
  bool capture_output = false;
  std::string text;
  size_t seed = 1011;
  double maxtime = 0.5;
  std::vector<size_t> indices_final (K);
  std::vector<size_t> labels (ndata);
  std::string metric = "l1";
  std::string energy = "identity";
  size_t nthreads = 1;
  size_t maxrounds = 100;
  bool patient = true;
  bool rooted = true;
  double critical_radius = 0;
  double exponent_coeff = 0;
 

  nszen::sparse_vector_zentas(ndata, sizes.data(), data_in.data(), indices.data(), K, indices_init.data(), algorithm, level, max_proposals,  capture_output, text, seed, maxtime, indices_final.data(), labels.data(), metric, nthreads, maxrounds, patient, energy, rooted, critical_radius, exponent_coeff);
  
  std::cout << std::endl;
  for (size_t i = 0; i < ndata; ++i){
    std::cout << "final label of element " << i << " is " << labels[i] << std::endl;
  }

  return 0;
}



int main(){
  //Choose your test and put it here. 
  return cluster_sparse();
}