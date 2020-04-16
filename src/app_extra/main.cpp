/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: main.cpp                                             */
/*    DATE: Dec 22nd, 2019                                       */ 
/*                                                               */
/* This is unreleased BETA code. No permission is granted or     */
/* implied to use, copy, modify, and distribute this software    */
/* except by the author(s).                                      */
/*****************************************************************/
  
#include <cstdlib>
#include <string> 
#include <vector>
#include <iostream>
#include "MBUtils.h"
#include "SegListExtrapolator.h"
#include "XYFormatUtilsSegl.h"
#include "XYSegList.h"
#include "XYPoint.h"

using namespace std; 
   
void showHelpAndExit();
 
   
//--------------------------------------------------------
// Procedure: main()
  
int main(int argc, char *argv[])
{

    string segl = "pts={10,-180: 110,-100: 70,-60: 190,60}";
    string name  = "usv";
    double spd = 2;
    XYSegList seglist = string2SegList(segl);
    SegListExtrapolator test;
    test.setContactName(name);
    test.setContactSpd(spd);
    test.extrapolateSegList(seglist);
    

    for(int i=1; i<argc; i++){ 
    double time  = stod(argv[i]);
    XYPoint point = test.extrapolate_point(time);
    }
    
    test.print();
  return (0);
}

 
//--------------------------------------------------------
// Procedure: showHelpAndExit() 

void showHelpAndExit()
{
  cout << "Usage                                               " << endl;
  cout << "  buildtss [OPTIONS] filename.tss                      " << endl;
  cout << "                                                    " << endl;
  cout << "Synopsis:                                           " << endl;
  cout << "  The buildtss utility populates a traffic scheme class " << endl;
  cout << "                                                    " << endl;
  cout << "Options:                                            " << endl;
  cout << "  -h,--help      Displays this help message         " << endl;
  cout << "                                                    " << endl;
  cout << "  <type>  Traffic Separation Scheme Object Type     " << endl;
  cout << "   Examples: separation zone or separation          " << endl;
  cout << "             precaution area or precaution          " << endl;
  cout << "             inbound lane or inbound                " << endl;
  cout << "             outbound lane or outbound              " << endl;
  cout << "                                                    " << endl;
  cout << "                                                    " << endl;
  cout << "  <points> XY Polygon or SegList Coordinates        " << endl;
  cout << "   Examples: 250,-50:275,-50:275,-25:250,-25        " << endl;
  cout << "             x=50, y=50, radius=60                  " << endl;
  cout << "                                                    " << endl;
  cout << "  <start>  XY Starting Point for Inbound/Outbound Lane " << endl;
  cout << "   Example:  x=90, y=80                             " << endl;
 cout << "                                                    " << endl;
  cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    " << endl;
  cout << " !  NOTE: If start point is not explicit on the !     " << endl;
  cout << " !        inbound/outbound lane points, it will !     " << endl;
  cout << " !        not populate!!!                       !     " << endl;
  cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!     " << endl;
  cout << "                                                    " << endl;  
  exit(0);  
}
  

