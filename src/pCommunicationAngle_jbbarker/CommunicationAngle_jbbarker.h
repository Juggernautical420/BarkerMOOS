/***********************************************************
    NAME: Jason Barker                                              
    ORGN: MIT                                             
    FILE: CommunicationAngle_jbbarker.h                                          
    DATE: 26 FEB 2019                                                
************************************************************/

#ifndef CommunicationAngle_jbbarker_HEADER
#define CommunicationAngle_jbbarker_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

class CommunicationAngle_jbbarker : public CMOOSApp
{
 public:
   CommunicationAngle_jbbarker();
   ~CommunicationAngle_jbbarker();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
