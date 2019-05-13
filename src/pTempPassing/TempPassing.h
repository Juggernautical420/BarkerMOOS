/************************************************************/
/*    NAME: Team Awesome                                              */
/*    ORGN: MIT                                             */
/*    FILE: TempPassing.h                                          */
/*    DATE: May 9, 2019                             */
/************************************************************/

#ifndef TempPassing_HEADER
#define TempPassing_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "TempEntry.h"
#include <vector>
#include <iterator>
#include <cstdlib>
#include <cstdint>
#include <stdint.h>
#include "MBUtils.h"
#include "NodeMessage.h"
#include "BuildUtils.h"
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
#include "ACTable.h"

class TempPassing : public AppCastingMOOSApp
{
 public:
   TempPassing();
   ~TempPassing();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
  double m_range;
  bool in_range;
  bool in_period;




 private: // State variables
  TempEntry m_new_entry;
  list<TempEntry> m_temperature_queue;
  unsigned int m_received_index;
  unsigned int m_sent_time;
  std::string m_veh_name;




};

#endif 
