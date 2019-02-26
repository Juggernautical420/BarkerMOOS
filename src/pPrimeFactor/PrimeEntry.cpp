/***********************************************************
   NAME: Jason Barker                                              
   ORGN: MIT                                             
   FILE: PrimeEntry.cpp                                         
   DATE: Feb 26, 2019    
   This program computes prime factors                                        
***********************************************************/
#include "PrimeEntry.h"
#include "PrimeFactor.h"


PrimeEntry::PrimeEntry()
{
  m_start_index = 0;
  m_orig = 0;
  m_done = false;
  m_received_index = 0;
  m_calculated_index =0;
  m_current = 0;
  m_stored = 3;
}

//-------------------------------------------------------------
// Procedure:factor (from PrimeEntry.h)

bool PrimeEntry::factor(unsigned long int max_steps)
{
  //Factorization function/template found from online resource
  //https://www.geeksforgeeks.org/print-all-prime-factors-of-a-given-number/
  int iter = 0;
  while(m_current % 2 == 0){
    m_factors.push_back(2); // Print the number of 2s that divide m_current
    m_current = m_current/2;
    iter = iter + 1; //Counts the number of iterations required
   if (iter >= max_steps)
    return(false);
   //Exits this function if max steps reached

  }
  // m_current must be odd at this point
  for(int i= m_stored; i <= sqrt(m_current); i = i+2){
    m_stored = m_stored+2;
    // While i divides m_current, print i and divide m_current
    iter = iter +1;
    while (m_current%i == 0){
      m_factors.push_back(i);
      m_current = m_current/i;
    }
    if (iter >= max_steps)
      return(false);
     //Exits this function if max steps reached
    
  } 
  // This condition is to handle the case when m_current  
  // is a prime number greater than 2 
    if (m_current > 2)
      m_factors.push_back(m_current);
  return(true);
}

//-------------------------------------------------------------
// Procedure:getReport (from PrimeEntry.h)
std::string PrimeEntry::getReport()
{
  stringstream ss_orig, ss_primes;
  ss_orig << m_orig;
  string orig = "orig" + ss_orig.str();//Lab 5 Section 2.3
  string received = " ,received="+intToString(m_received_index);
  string calculated = " ,calculated=" + intToString(m_calculated_index);
  string solve_time = " ,solve_time=" + doubleToString(MOOSTime() - m_start_index,3);
  copy(m_factors.begin(),m_factors.end(), ostream_iterator<uint64_t>(ss_primes, ":"));
  string primes = ss_primes.str();
  primes = " ,primes=" + primes.substr(0, primes.length()-1);
  string user = " ,username=jbbarker";
  string output = orig + received + calculated + solve_time+ primes + user;
  return(output);
}