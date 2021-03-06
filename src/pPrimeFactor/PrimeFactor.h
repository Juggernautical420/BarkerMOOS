/***********************************************************
   NAME: Jason Barker                                              
   ORGN: MIT                                             
    FILE: PrimeFactor.h                                          
    DATE: Feb 21, 2019                                              
************************************************************/

#ifndef PrimeFactor_HEADER
#define PrimeFactor_HEADER

#include "MOOS/libMOOS/MOOSLib.h" //Standard MOOS App Library
//#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h" // Allows AppCasting
#include <iterator>
#include "MBUtils.h"
#include <string>
#include <vector>
#include <cstdint>
#include <list>
#include <sstream>
#include <math.h> 
#include <stdio.h>
#include "PrimeEntry.h"

using namespace std;

class PrimeFactor : public CMOOSApp
{
 public:
   PrimeFactor();
   ~PrimeFactor();

  

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();
   list<PrimeEntry> m_list_primes;
   unsigned int  m_recd_index;
   unsigned int  m_calcd_index;
 private: // Configuration variables

 private: // State variables
 //bool buildReport();  // AppCasting Output buildReport()
};

#endif 