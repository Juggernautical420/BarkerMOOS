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
#include <vector>
#include <iostream>
#include "MBUtils.h"
#include "FileBuffer.h"
#include "TrafficScheme.h"
#include "TrafficObject.h"

using namespace std; 
   
void showHelpAndExit();
 
   
//--------------------------------------------------------
// Procedure: main()
  
int main(int argc, char *argv[])
{
  TrafficScheme my_tss;
  for(int i=1; i<argc; i++) { 
    string argi  = argv[i];

    bool handled = true;
    if((argi == "-h") || (argi == "--help"))
      showHelpAndExit();

    vector<string> lines = fileBuffer(argi);
  	if(lines.size() == 0){
     cout << "buildtss: Empty File: " << argi << endl; 		
   	 exit(1); 
  	}
  	TrafficObject new_object;
  	for(unsigned int i=0; i<lines.size(); i++){
  	string str = stripBlankEnds(lines[i]);
    if(strBegins(str, "//"))
      continue;
    if(str == "")
      continue;
  		string param = biteStringX(str, '=');
		if(param == "type")
			new_object.setTrafficType(str);
		if(param == "poly")
			new_object.setPolyType(str);
		if(param == "points")
			new_object.setPolyPts(str);
		if(param == "label")
			new_object.setPolyName(str);
		if(param == "start")
			new_object.setStartPt(str);	
  	}
  	//new_object.print();
  	my_tss.addTrafficObject(new_object);


    if(!handled) {
      cout << "buildtss: Bad Arg: " << argi << endl;
      exit(1);
    } 

         
  }
  my_tss.print();
  my_tss.printviewable();
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
  

