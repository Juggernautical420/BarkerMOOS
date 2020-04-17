/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT - 2N                                             */
/*    FILE: SegListContact.cpp                                         */
/*    DATE: Revised April 17, 2020 (original January 7, 2020)        */
/************************************************************/

#include "SegListContact.h"
#include <math.h>
#include <numeric>

using namespace std;

//-----------------------------------------------------------
// Constructor
SegListContact::SegListContact()
{
  m_got_name = false;
  m_got_spd = false;
  m_got_predict = false;
  m_vname = "";
  m_time_limit = 0;
}

//-----------------------------------------------------------------
//Procedure: extrapolateSegList

void SegListContact::expandSegList(XYSegList seglist)
{
  if(m_got_spd){
  for(int i=1; i<seglist.size(); i++){
    double x1 = seglist.get_vx(i-1);
    double y1 = seglist.get_vy(i-1);

    double x2 = seglist.get_vx(i);
    double y2 = seglist.get_vy(i);

    XYSegList current_seglist;
    current_seglist.add_vertex(x1,y1);
    current_seglist.add_vertex(x2,y2);
    m_leg_seglist.push_back(current_seglist);
    
    double heading = relAng(x1,y1,x2,y2);
    m_leg_heading.push_back(heading);

    double length = current_seglist.length();
    m_leg_length.push_back(length);
  }

  string veh_name = seglist.get_label();
    if((!m_got_name)&&(veh_name != "")){
      m_vname = veh_name;
      m_got_name = true;
    }
  createTimeLimit();
  }
}

//-----------------------------------------------------------------
//Procedure: setContactName

void SegListContact::setContactName(string str)
{
  if(!m_got_name){
  m_vname = str;
  m_got_name = true;
  }
}

//-----------------------------------------------------------------
//Procedure: setContactSpd
void SegListContact::setContactSpd(double spd)
{
  if(!m_got_spd){
    m_nav_spd = spd;
    m_got_spd = true;
  }
}

//-----------------------------------------------------------------
//Procedure: createTimeLimit
void SegListContact::createTimeLimit()
{
  for(int i=0; i<m_leg_seglist.size(); i++){
    double leg_limit = m_leg_length[i]/m_nav_spd;
    m_time_limit = (m_time_limit + leg_limit);
    m_time_leg.push_back(m_time_limit);
  }
}




//-----------------------------------------------------------------
//Procedure: print
void SegListContact::print() 
{
  cout << "Seglist " << endl;

  if(m_vname != "")
    cout << "veh name = " << m_vname << " , spd = " << m_nav_spd << endl;
  for(int i=0; i<m_leg_seglist.size(); i++){
    cout << m_leg_seglist[i].get_spec_pts() << " , heading = " << m_leg_heading[i] << ", leg length = " << m_leg_length[i] << " , time at leg end = " << m_time_leg[i] << endl;
  }
  if(m_got_predict)
    cout << "Predicted Point = " << m_locate.get_spec() << endl;

}


//-----------------------------------------------------------------
//Procedure: predict_point
void SegListContact::predict_point(double time)
{
  if(m_got_spd){
  for(int i=0; i<m_leg_seglist.size(); i++){
    if(time <= m_time_leg[0]){
      m_time_remain = time;
      m_locate.set_label("vname = " +m_vname + "; heading = " + doubleToString(m_leg_heading[0]));
      pointCalculate(m_leg_seglist[0], m_leg_heading[0]);
      m_got_predict = true;
      return;
    }
    else if((time <= m_time_leg[i]) && (time > m_time_leg[i-1])){
      m_time_remain = time - m_time_leg[i-1];
      m_locate.set_label("vname = " +m_vname + "; heading = " + doubleToString(m_leg_heading[i]));
      pointCalculate(m_leg_seglist[i], m_leg_heading[i]);
      m_got_predict = true;
      return;
    }

  }
  }
}

//-----------------------------------------------------------------
//Procedure: pointCalculate
void SegListContact::pointCalculate(XYSegList seglist, double heading)
{
    double x1 = seglist.get_vx(0);
    double y1 = seglist.get_vy(0);
    
    double hdgconvert = angle180(90 - heading);
    double radhdg = degToRadians(hdgconvert);

    double x = x1 + ((cos(radhdg))* m_nav_spd * m_time_remain);
    double y = y1 + ((sin(radhdg))* m_nav_spd * m_time_remain);

    m_locate.set_vertex(x,y);
}

//-----------------------------------------------------------------
//Procedure: extrapolate_point
XYPoint SegListContact::extrapolate_point(double time)
{
  predict_point(time);
  XYPoint predicted_point = m_locate;
  return(predicted_point);
}