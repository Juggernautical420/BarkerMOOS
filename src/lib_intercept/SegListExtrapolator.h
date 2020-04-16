/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT - 2N                                             */
/*    FILE: SegListExtrapolator.h                                          */
/*    DATE: January 7, 2020                            */
/************************************************************/

#ifndef SEGLIST_EXTRAPOLATOR_HEADER
#define SEGLIST_EXTRAPOLATOR_HEADER

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



class SegListExtrapolator 
{
 public:
   SegListExtrapolator();
   virtual ~SegListExtrapolator() {}

 public:
   void extrapolateSegList(XYSegList seglist);
   void setContactName(std::string str);
   void setContactSpd(double spd);              
   void createTimeLimit();
   void predict_point(double time);
   void pointCalculate(XYSegList seglist, double heading);


/// Getters ///
   void print();
   XYPoint extrapolate_point(double time);


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

   
};

#endif 