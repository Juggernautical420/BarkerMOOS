/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT - 2N                                             */
/*    FILE: SegIntercept.h                                          */
/*    DATE: October 29, 2019                            */
/************************************************************/

#ifndef SEG_INTERCEPT_HEADER
#define SEG_INTERCEPT_HEADER

#include <iterator>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include "XYObject.h"
#include "XYSegList.h"
#include "XYFormatUtilsSegl.h"
#include "GeomUtils.h"


class SegIntercept : public XYObject {
 public:
   SegIntercept() {}
   virtual ~SegIntercept() {}

 //void SegIntercept create and edit functions
 public:
   void getIntercept(XYSegList os_seglist, XYSegList contact_seglist);
   void removeDuplicates();

   unsigned int size() const  {return(m_px.size());}
   double get_px(unsigned int) const;
   double get_py(unsigned int) const;
   std::string get_pname(unsigned int) const;

 protected:
   std::string m_veh_name;
   std::string m_vname;
   std::string m_veh_handle;
   std::string str_os_seglist;
   std::string str_contact_seglist;	

   XYSegList os_seglist;
   XYSegList contact_seglist;


   std::vector<double> m_px;
   std::vector<double> m_py;
   std::vector<std::string> m_con_name;
};

#endif 




