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
#include "zentas.hpp"
#include "claransl0.hpp"
#include "claransl1.hpp"
#include "claransl2.hpp"
#include "claransl3.hpp"
#include "voronoil0.hpp"
#include "outputwriter.hpp"
#include "dispatch.hpp"
#include "zentaserror.hpp"
#include "vdimap.hpp"
#include "lpmetric.hpp"
#include "levenshtein.hpp"

namespace nszen{

/* dense vectors */

// 10% -> 80% faster if unrooted (!) :)
template <typename T>
void vzentas(size_t ndata, size_t dimension, const T * const ptr_datain, size_t K, const size_t * const indices_init, std::string initialisation_method, std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double max_time, double min_mE, size_t * const indices_final, size_t * const labels, std::string metric, size_t nthreads, size_t max_rounds, bool patient, std::string energy, bool with_tests, bool rooted, double critical_radius, double exponent_coeff, bool do_vdimap){
  
  auto bigbang = std::chrono::high_resolution_clock::now(); 
  
  std::vector<T> v_mapped;
  const T * true_ptr_datain; 
  size_t true_dimension;
  
  if (do_vdimap == false){
    true_ptr_datain = ptr_datain;
    true_dimension = dimension;
  }
  
  else{
    vdimap::vdimap<T>(v_mapped, ptr_datain, ndata, dimension, seed);
    true_ptr_datain = v_mapped.data();
    true_dimension = v_mapped.size()/ndata;
  }
  
  /* TODO here : ptr_datain: shuffle if requested. Another flag :) */

  LpMetricInitializer metric_initializer;
  metric_initializer.reset(metric);

  EnergyInitialiser energy_initialiser(critical_radius, exponent_coeff);

  ConstLengthInitBundle<T> datain_ib(ndata, true_dimension, true_ptr_datain);
  if (rooted == true){
    zentas_base  <VDataRooted <DenseVectorDataRootedIn < T > >, LpMetric<DenseVectorDataRootedIn < T >  > > (datain_ib, K, indices_init, initialisation_method, algorithm, level, max_proposals, capture_output, text, seed, max_time, min_mE, indices_final, labels, nthreads, max_rounds, patient, energy, with_tests, metric_initializer, energy_initialiser, bigbang);
  }
  
  else{
    zentas_base  <VData <DenseVectorDataUnrootedIn < T> >, LpMetric<DenseVectorDataUnrootedIn < T > > > (datain_ib, K, indices_init, initialisation_method, algorithm, level, max_proposals, capture_output, text, seed, max_time, min_mE, indices_final, labels, nthreads, max_rounds, patient, energy, with_tests, metric_initializer, energy_initialiser, bigbang);
  }
}

template void vzentas(size_t ndata, size_t dimension, const double * const ptr_datain, size_t K, const size_t * const indices_init, std::string initialisation_method, std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double max_time, double min_mE, size_t * const indices_final, size_t * const labels, std::string metric, size_t nthreads, size_t max_rounds, bool patient, std::string energy, bool with_tests, bool rooted, double critical_radius, double exponent_coeff, bool do_vdimap);

template void vzentas(size_t ndata, size_t dimension, const float * const ptr_datain, size_t K, const size_t * const indices_init, std::string initialisation_method,  std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double max_time, double min_mE, size_t * const indices_final, size_t * const labels, std::string metric, size_t nthreads, size_t max_rounds, bool patient, std::string energy, bool with_tests, bool rooted, double critical_radius, double exponent_coeff, bool do_vdimap);  


/* sparse vectors */

template <typename T>
void sparse_vector_zentas(size_t ndata, const size_t * const sizes, const T * const ptr_datain, const size_t * const ptr_indices_s, size_t K, const size_t * const indices_init, std::string initialisation_method, std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double max_time, double min_mE, size_t * const indices_final, size_t * const labels, std::string metric, size_t nthreads, size_t max_rounds, bool patient, std::string energy, bool with_tests, bool rooted, double critical_radius, double exponent_coeff){


  auto bigbang = std::chrono::high_resolution_clock::now(); 
    
  LpMetricInitializer metric_initializer;
  metric_initializer.reset(metric);
  
  EnergyInitialiser energy_initialiser(critical_radius, exponent_coeff);
    
  SparseVectorDataInitBundle<T> datain_ib(ndata, sizes, ptr_datain, ptr_indices_s);  
  
  if (rooted == true){ 
    zentas_base  <SparseVectorDataRooted <SparseVectorDataRootedIn < T> >, LpMetric<SparseVectorDataRootedIn < T > > > (datain_ib, K, indices_init, initialisation_method, algorithm, level, max_proposals, capture_output, text, seed, max_time, min_mE, indices_final, labels, nthreads, max_rounds, patient, energy, with_tests, metric_initializer, energy_initialiser, bigbang);
  }
  
  else{
    zentas_base  <SparseVectorData <SparseVectorDataUnrootedIn < T> >, LpMetric<SparseVectorDataUnrootedIn < T > > > (datain_ib, K, indices_init, initialisation_method, algorithm, level, max_proposals, capture_output, text, seed, max_time, min_mE, indices_final, labels, nthreads, max_rounds, patient, energy, with_tests, metric_initializer, energy_initialiser, bigbang);
  }
    

}


template void sparse_vector_zentas(size_t ndata, const size_t * const  sizes, const double * const ptr_datain, const size_t * const ptr_indices_s, size_t K, const size_t * const indices_init, std::string initialisation_method, std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double max_time, double min_mE, size_t * const indices_final, size_t * const labels, std::string metric, size_t nthreads, size_t max_rounds, bool patient, std::string energy, bool with_tests, bool rooted, double critical_radius, double exponent_coeff);

template void sparse_vector_zentas(size_t ndata, const size_t * const  sizes, const float * const ptr_datain, const size_t * const ptr_indices_s, size_t K, const size_t * const indices_init, std::string initialisation_method, std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double max_time, double min_mE, size_t * const indices_final, size_t * const labels, std::string metric, size_t nthreads, size_t max_rounds, bool patient, std::string energy, bool with_tests, bool rooted, double critical_radius, double exponent_coeff);  



/* strings */

template <typename T>
void szentas(size_t ndata, const size_t * const sizes, const T * const ptr_datain, size_t K, const size_t * const indices_init, std::string initialisation_method, std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double max_time, double min_mE, size_t * const indices_final, size_t * const labels, std::string metric, size_t nthreads, size_t max_rounds, bool patient, std::string energy, bool with_tests, bool rooted, bool with_cost_matrices, size_t dict_size, double c_indel, double c_switch, const double * const c_indel_arr, const double * const c_switches_arr, double critical_radius, double exponent_coeff){


  auto bigbang = std::chrono::high_resolution_clock::now(); 
  
  EnergyInitialiser energy_initialiser(critical_radius, exponent_coeff);

  std::vector<std::string> possible_metrics = {"levenshtein", "normalised levenshtein"};
  bool valid_metric = false;
  std::string possible_metric_string = " [ ";
  for (auto & possible_metric : possible_metrics){
    possible_metric_string += " ";
    possible_metric_string += possible_metric;
    possible_metric_string += " ";

    if (metric.compare(possible_metric) == 0){
      valid_metric = true;
      break;
    }
  }
  
  possible_metric_string += " ] ";
    
  if  (valid_metric == true){// (metric.compare("levenshtein") == 0 || metric.compare("normalised levenshtein") == 0){
    
    
    bool normalised = false;
    if (metric.compare("normalised levenshtein") == 0){
      normalised = true;
    }
    
    LevenshteinInitializer metric_initializer;

    
    if (with_cost_matrices == false){
      if (c_indel <= 0){
        throw zentas::zentas_error("with_cost_matrices == false : c_indel should be a positive real number");
      }
      
      if (c_switch <= 0){
        throw zentas::zentas_error("with_cost_matrices == false : c_switch should be a positive real number");
      }
      
      metric_initializer = LevenshteinInitializer (c_indel, c_switch, normalised);//
    }
  
    else{
      if (dict_size == 0){
        throw zentas::zentas_error("with_cost_matrics is true, and dict_size == 0. this is invalid.");
      }
      
      metric_initializer = LevenshteinInitializer (dict_size, c_indel_arr, c_switches_arr, normalised);//
    }
  
  
    //VariableLengthInitBundle datain_ib(ndata, sizes, ptr_datain, ptr_indices_s);  
    
  //  LevenshteinInitializer metric_initializer(1.0, 1.0);
    
    
    VariableLengthInitBundle<T> datain_ib(ndata, sizes, ptr_datain);
    if (rooted == true){
      zentas_base  <SDataRooted <StringDataRootedIn < T> >, LevenshteinMetric < StringDataRootedIn <T> > > (datain_ib, K, indices_init, initialisation_method, algorithm, level, max_proposals, capture_output, text, seed, max_time, min_mE, indices_final, labels, nthreads, max_rounds, patient, energy, with_tests, metric_initializer, energy_initialiser, bigbang);      
    }
    
    else{
      zentas_base  <SData <StringDataUnrootedIn< T> >, LevenshteinMetric < StringDataUnrootedIn<T> > > (datain_ib, K, indices_init, initialisation_method, algorithm, level, max_proposals, capture_output, text, seed, max_time, min_mE, indices_final, labels, nthreads, max_rounds, patient, energy, with_tests, metric_initializer, energy_initialiser, bigbang);
    }
  }
  
  else{
    
    std::string errm("Currently, only metrics ");
    errm = errm + possible_metric_string + " are implemented for string data";
    throw zentas::zentas_error(errm);
  }
}




template void szentas(size_t ndata, const size_t * const sizes, const int * const ptr_datain, size_t K, const size_t * const indices_init, std::string initialisation_method, std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double max_time, double min_mE, size_t * const indices_final, size_t * const labels, std::string metric, size_t nthreads, size_t max_rounds, bool patient, std::string energy, bool with_tests, bool rooted, bool with_cost_matrices, size_t dict_size, double c_indel, double c_switch, const double * const c_indel_arr, const double * const c_switches_arr, double critical_radius, double exponent_coeff);


template void szentas(size_t ndata, const size_t * const sizes, const char * const ptr_datain, size_t K, const size_t * const indices_init, std::string initialisation_method, std::string algorithm, size_t level, size_t max_proposals, bool capture_output, std::string & text, size_t seed, double max_time, double min_mE, size_t * const indices_final, size_t * const labels, std::string metric, size_t nthreads, size_t max_rounds, bool patient, std::string energy, bool with_tests, bool rooted, bool with_cost_matrices, size_t dict_size, double c_indel, double c_switch, const double * const c_indel_arr, const double * const c_switches_arr, double critical_radius, double exponent_coeff);



  

} // namespace nszen

