/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: SegIntercept.cpp                                        */
/*    DATE: October 30, 2019                                                */
/*    This file looks at two seglists and compares seglists */
/*    for intercept point using the fast seglist intercept method */
/************************************************************/

#include "SegIntercept.h"

using namespace std;

//-----------------------------------------------------------------
//Procedure: getIntercept 
// This function takes two seglists and creates a class of intercepts
void SegIntercept::getIntercept(XYSegList os_seglist, XYSegList contact_seglist)
{
	//Method #1 
/*  Fast Line Segment Calculation 
     x1,y1            x4,y4
           \        /
            \      /
             \    / 
              \  /
            P_x,P_y (intercept point)
               /\ 
              /  \
             /    \
            /      \
       x3,y3        x2,y2 
*/
 int i;
 int j; 
for(i=1; i<os_seglist.size(); i++){ //Grabs the endpoints for each os segment
  double m_x1 = os_seglist.get_vx(i-1);
  double m_y1 = os_seglist.get_vy(i-1);
  double m_x2 = os_seglist.get_vx(i);
  double m_y2 = os_seglist.get_vy(i);
  
  for(j=1; j<contact_seglist.size(); j++){//Grabs the endpoints for each contact segment
    double m_x3 = contact_seglist.get_vx(j-1);
    double m_y3 = contact_seglist.get_vy(j-1);
    double m_x4 = contact_seglist.get_vx(j);
    double m_y4 = contact_seglist.get_vy(j);

    string con_spec = contact_seglist.get_spec();
    string con_short = biteStringX(con_spec, '}');
    string con_shorter = biteStringX(con_spec, '=');

    double Ax = m_x2 - m_x1;
    double Ay = m_y2 - m_y1;

    double Bx = m_x3 - m_x4;
    double By = m_y3 - m_y4;

    double Cx = m_x1 - m_x3;
    double Cy = m_y1 - m_y3;

    double m_f = (Ay*Bx)-(Ax*By);

    /*
    Basic equations and tests:

            (By*Cx)-(Bx*Cy)
    alpha = ----------------
            (Ay*Bx)-(Ax*By)

            (Ax*Cy)-(Ay*Cx)
     beta = ---------------
            (Ay*Bx)-(Ax*By)
    
    alpha AND beta MUST be on the interval [0,1]
    alpha AND beta MUST have the same signs

    */
    if(m_f != 0){ //1. Denominator check to make sure we arent dividing by zero
      //if the denominator is zero, the two segments are colinear
      double m_d = (By*Cx)-(Bx*Cy); 
      double m_e = (Ax*Cy)-(Ay*Cx);
      double m_alpha = m_d/m_f;
      double m_beta = m_e/m_f;
      if(m_alpha * m_beta > 0){ //2. Signs Check - must be the same
        if((m_alpha > 0) && (m_alpha < 1)){ //3. Range Check #1 alpha must be on the interval [0,1]
          if((m_beta > 0) && (m_beta < 1)){ //4. Range Check #2 beta must be on the interval [0,1]
          //5. if all 4 things are true, the segments intersect  
          // if any checks fail, there is no intersection
          //If Segments intersect, push back x,y points  
            double P_x = m_x1 + (m_alpha*(Ax)); 
            m_px.push_back(P_x);              
            double P_y = m_y1 + (m_alpha*(Ay));
            m_py.push_back(P_y);
            m_con_name.push_back(con_spec);
          }
        }
      }  
    }

  }
 } 


//Method #2
/* Matching Waypoints Check (sharing endpoints)

        x1,y1                      x2,y2
 x4, y4 *----------------------------*
        |
        |
        |
        |
        |
        |
        |
  x3,y3 *    

*/
  int r;
  int s;
for(r=0; r<os_seglist.size(); r++){ 
  double m_os_x1 = os_seglist.get_vx(r);
  double m_os_y1 = os_seglist.get_vy(r);
  
  for(s=0; s<contact_seglist.size(); s++){
    double m_con_x1 = contact_seglist.get_vx(s);
    double m_con_y1 = contact_seglist.get_vy(s);

    string con_pt_spec = contact_seglist.get_spec();
    string con_pt_short = biteStringX(con_pt_spec, '}');
    string con_pt_shorter = biteStringX(con_pt_spec, '=');

  if((m_os_x1 == m_con_x1) && (m_os_y1 == m_con_y1)){
    m_px.push_back(m_os_x1);
    m_py.push_back(m_os_y1);
    m_con_name.push_back(con_pt_spec);
    }  
  }    
}	
}

//---------------------------------------------------------------
// Procedure: get_px

double SegIntercept::get_px(unsigned int i) const
{
  if(i<m_px.size())
     return(m_px[i]);
  else
    return(0);
}

//---------------------------------------------------------------
// Procedure: get_py

double SegIntercept::get_py(unsigned int i) const
{
  if(i<m_py.size())
    return(m_py[i]);
  else
    return(0);
}

//---------------------------------------------------------------
// Procedure: get_pname

string SegIntercept::get_pname(unsigned int i) const
{
  if(i<m_con_name.size())
    return(m_con_name[i]);
  else
    return("");
}







