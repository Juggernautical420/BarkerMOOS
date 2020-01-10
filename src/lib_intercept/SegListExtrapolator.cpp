/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT - 2N                                             */
/*    FILE: SegListExtrapolator.cpp                                         */
/*    DATE: January 7, 2020                            */
/************************************************************/

#include "SegListExtrapolator.h"
#include <math.h>

using namespace std;

//-----------------------------------------------------------------
//Procedure: seglistExtrapolate

 void SegListExtrapolator::seglistExtrapolate(XYSegList os_seglist, SegParts os_details, double m_navSpd)
 {

 int i;
 for(i=0; i<os_seglist.size(); i++){
  double m_x1 = os_seglist.get_vx(i);
  double m_y1 = os_seglist.get_vy(i);

  double lenleg = os_details.get_legLength(i);
  double t_limit = lenleg/m_navSpd;
  double hdg = os_details.get_legHeading(i);
//  double hdg_radians = hdg * M_PI/180;

// Going to start with int and then graduate to double to verify it works
// Going to remove hdg at this time for a first principle approach
  int time_limit = int(t_limit);
  int t;

//Real code that has hdg in it....is it really getting used though? Is it? 
  // for(t=0; t<time_limit; t++){
  // 	double xt = m_x1 + (cos(hdg) * m_navSpd * t);
  // 	m_xt.push_back(xt);
  // 	double yt = m_y1 + (sin(hdg) * m_navSpd * t);
  // 	m_yt.push_back(yt);
  // 	m_hdgt.push_back(hdg);
  //  }
 


// First principle approach
   for(t=0; t<time_limit; t++){
  	double xt = m_x1 +  (m_navSpd * t);
  	m_xt.push_back(xt);
  	double yt = m_y1 + (m_navSpd * t);
  	m_yt.push_back(yt);	
  	m_hdgt.push_back(hdg);
   } 


  }
 }

 //---------------------------------------------------------------
// Procedure: get_xt

double SegListExtrapolator::get_xt(unsigned int i) const
{
  if(i<m_xt.size())
     return(m_xt[i]);
  else
    return(0);
}
//---------------------------------------------------------------
// Procedure: get_yt

double SegListExtrapolator::get_yt(unsigned int i) const
{
  if(i<m_xt.size())
     return(m_yt[i]);
  else
    return(0);
}
//---------------------------------------------------------------
// Procedure: get_hdgt

double SegListExtrapolator::get_hdgt(unsigned int i) const
{
  if(i<m_xt.size())
     return(m_hdgt[i]);
  else
    return(0);
}

//---------------------------------------------------------------
// Procedure: get_t

int SegListExtrapolator::get_t(double m_px, double m_py) const
{
	int i;
	for(i=0; i<m_xt.size(); i++){
		double Mx = m_xt[i];
		double My = m_yt[i];

		double distance = sqrt(pow((Mx - m_px), 2) + pow((My - m_py), 2));
		if(distance <2)
		  return(i);
	}
}

