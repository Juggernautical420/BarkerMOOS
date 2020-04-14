/***********************************************************
    NAME: Jason Barker                                             
    ORGN: MIT                                             
    FILE: SegListEdit.h                                          
    DATE: November 12, 2019   
                                            
***********************************************************/
 #ifndef SEGLIST_EDIT_HEADER
 #define SEGLIST_EDIT_HEADER

#include <iterator>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include "XYObject.h"
#include "GeomUtils.h"
#include "XYSegList.h"
#include "XYFormatUtilsSegl.h"
#include "SegIntercept.h"

using namespace std;

//------------------------------------------------------------
// Procedure: add_origin
// Takes an input seglist and x,y coords and returns a new seglist with
// input x,y as the first vertex

XYSegList add_origin(XYSegList original_seglist, double x, double y)
{
  XYSegList new_seglist;
  new_seglist.add_vertex(x,y);
  for(int i=0; i<original_seglist.size(); i++){
    double xx = original_seglist.get_vx(i);
    double yy = original_seglist.get_vy(i);
    new_seglist.add_vertex(xx,yy);
  }
  return(new_seglist);
}


//------------------------------------------------------------
// Procedure: biteSegList
// Like biteString, returns seglist up to the input cx,cy coord (returns everything to the left of the cx,cy)


XYSegList biteSegList(XYSegList original_seglist, double cx, double cy)
{
  XYSegList new_seglist;
  bool done = false;
  for(int i=1; i<original_seglist.size(); i++){
    if(!done){
      double ax = original_seglist.get_vx(i-1);
      double ay = original_seglist.get_vy(i-1);

      double bx = original_seglist.get_vx(i);
      double by = original_seglist.get_vy(i);

      double ac = distPointToPoint(ax,ay,cx,cy);
      double cb = distPointToPoint(cx,cy,bx,by);
      double ab = distPointToPoint(ax,ay,bx,by);

      if(ac + cb != ab)
        new_seglist.add_vertex(ax,ay);
      else{
        new_seglist.add_vertex(cx,cy);
        done = true;
      }
    }
  }
  return(new_seglist);
}







////////////WORK IN PROGRESS BELOW//////////////////////////

//------------------------------------------------------------
// Procedure: biteSegListLeft
// Does not work 



// string biteSegListLeft(XYSegList os_seglist, double m_x, double m_y, int index) 
// {
//  //int index = os_seglist.closest_segment(m_x, m_y);
//  // os_seglist.insert_vertex(m_x, m_y);
 
//  os_seglist.alter_vertex(m_x, m_y);
//  os_seglist.delete_vertex(index);
//  string remaining = os_seglist.get_spec();
//  // double time = (os_remaining.length())/m_spd; 
//  // string remaining = doubleToString(time) + "secs";
//  return(remaining);
// }






#endif 
