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
#include "SegParts.h"


class SegListExtrapolator : public XYObject {
 public:
   SegListExtrapolator() {}
   virtual ~SegListExtrapolator() {}

 public:
   void seglistExtrapolate(XYSegList os_seglist, SegParts os_details, double m_navSpd);

   unsigned int size() const  {return(m_xt.size());}
   double get_xt(unsigned int) const;
   double get_yt(unsigned int) const;
   double get_hdgt(unsigned int) const;

   int get_t(double m_px, double m_py) const;


 protected:

   SegParts os_details;
   XYSegList os_seglist;
   
   double m_nav_spd;
   double m_px;
   double m_py;


   std::vector<double> m_xt;
   std::vector<double> m_yt;
   std::vector<double> m_hdgt;
   
};

#endif 