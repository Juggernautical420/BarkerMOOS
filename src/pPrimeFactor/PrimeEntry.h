/***********************************************************
    NAME: Jason Barker                                             
    ORGN: MIT                                             
    FILE: PrimeEntry.h                                          
    DATE: Feb 25, 2019    
    Template provived in Lab 5                                          
***********************************************************/
 #include "MOOS/libMOOS/MOOSLib.h" //Standard MOOS App Library
//#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h" // Allows AppCasting
#include "MBUtils.h"
 #include <string> 
 #include <vector> 
 #include <cstdint>
 #include <list>
 #include <sstream>
 #include <math.h> 
 #include <stdio.h>
 #include <iterator>

 using namespace std;
 
 #ifndef PRIME_ENTRY_HEADER
 #define PRIME_ENTRY_HEADER

 class PrimeEntry
 {
 public:

   PrimeEntry();
   ~PrimeEntry() {};

   void setOriginalVal(unsigned long int v){m_orig=v;};
   void setReceivedIndex(unsigned int v)    {m_received_index=v;};
   void setStartTime(unsigned int v)  {m_start_index=v;};
   void setCalculatedIndex(unsigned int v)  {m_calculated_index=v;};
   void setCurrentValue(unsigned int v) {m_current=v;};
   void setDone(bool v)                     {m_done=v;};


   bool   done() {return(m_done);};

   bool   factor(unsigned long int max_steps);

   std::string  getReport();

   bool          m_done;
   uint64_t      m_current;
  protected:
  uint64_t      m_start_index;
  unsigned int  m_received_index;
  unsigned int  m_calculated_index;
  uint64_t      m_orig;
  unsigned int  m_stored;
  std::vector<uint64_t> m_factors;
 };
 #endif 