/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT - 2N                                             */
/*    FILE: SegListContact.h                                          */
/*    DATE: Revised April 17, 2020 (original Jan 7, 2020)       */
/************************************************************/

#ifndef SEGLIST_CONTACT_HEADER
#define SEGLIST_CONTACT_HEADER

#include <iterator>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include "XYObject.h"
#include "XYSegList.h"
#include "XYFormatUtilsSegl.h"
#include "GeomUtils.h"
#include "AngleUtils.h"
#include "XYPoint.h"



class SegListContact 
{
 public:
   SegListContact();
   virtual ~SegListContact() {}

 public:
   void set_contact(std::string name, XYSegList seglist, double spd);
   void expandSegList(XYSegList seglist);
   void setContactName(std::string str);
   void setContactSpd(double spd);              
   void createTimeLimit();
   void predict_point(double time);
   void pointCalculate(XYSegList seglist, double heading);


/// Getters ///
   void print();
   XYPoint extrapolate_point(double time);

   std::string getContactName() const {return(m_vname);};
   double getContactSpd() {return(m_nav_spd);};


 protected:
///Class Variables ///
std::string m_vname;
bool m_got_name;

double m_nav_spd;
bool m_got_spd;

double m_time_limit;
std::vector<XYSegList> m_leg_seglist;
std::vector<double> m_leg_heading;
std::vector<double> m_leg_length;
std::vector<double> m_time_leg;

XYPoint m_locate;
double m_time_remain;
bool m_got_predict;

   
};

#endif 