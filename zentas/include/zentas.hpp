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
#ifndef ZENTAS_HPP
#define ZENTAS_HPP

#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <fstream>


#include "tdatain.hpp"

namespace nszen{

void hello();

// dense vectors 
template <typename T>
void vzentas(size_t ndata, size_t dimension, const T * const ptr_datain, size_t K, const size_t * const indices_init, std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double maxtime, size_t * const indices_final, size_t * const labels, std::string metric, size_t nthreads, size_t maxrounds, bool patient, std::string energy, bool rooted, double critical_radius, double exponent_coeff);

// sparse vectors 
template <typename T>
void sparse_vector_zentas(size_t ndata, const size_t * const sizes, const T * const ptr_datain, const size_t * const ptr_indices_s, size_t K, const size_t * const indices_init, std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double maxtime, size_t * const indices_final, size_t * const labels, std::string metric, size_t nthreads, size_t maxrounds, bool patient, std::string energy, bool rooted, double critical_radius, double exponent_coeff);

// sequences, defined for T in {char, int}
template <typename T>
void szentas(size_t ndata, const size_t * const sizes, const T * const ptr_datain, size_t K, const size_t * const indices_init, std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double maxtime, size_t * const indices_final, size_t * const labels, std::string metric, size_t nthreads, size_t maxrounds, bool patient, std::string energy, bool rooted, bool with_cost_matrices, size_t dict_size, double c_indel, double c_switch, const double * const c_indel_arr, const double * const c_switches_arr, double critical_radius, double exponent_coeff);

// strings, from txt file (for fasta files or ordinary text files)
void textfilezentas(std::vector<std::string> filenames, std::string outfilename, std::string costfilename, size_t K, std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double maxtime, std::string metric, size_t nthreads, size_t maxrounds, bool patient, std::string energy, bool rooted, double critical_radius, double exponent_coeff);


} //namespace nszen

#endif

