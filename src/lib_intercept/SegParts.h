/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT - 2N                                             */
/*    FILE: SegParts.h                                          */
/*    DATE: January 7, 2020                            */
/************************************************************/

#ifndef SEG_PARTS_HEADER
#define SEG_PARTS_HEADER

#include <iterator>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include "XYObject.h"
#include "XYSegList.h"
#include "XYFormatUtilsSegl.h"


class SegParts : public XYObject {
 public:
   SegParts() {}
   virtual ~SegParts() {}

 public:
   void getParts(XYSegList os_seglist, double m_navSpd);

   unsigned int size() const  {return(m_legHeading.size());}
   double get_legHeading(unsigned int) const;
   double get_legLength(unsigned int) const;
   double get_legTime(unsigned int) const;


 protected:


   XYSegList os_seglist;
   double m_navSpd;


   std::vector<double> m_legHeading;
   std::vector<double> m_legLength;
 ////////////////////////////////////////  
   std::vector<double> m_legTime;
   
};

#endif 