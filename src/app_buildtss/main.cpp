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
  return (0);
}

 
//--------------------------------------------------------
// Procedure: showHelpAndExit() 

void showHelpAndExit()
{
  cout << "Usage                                               " << endl;
  cout << "  voiview [OPTIONS] image.tif                      " << endl;
  cout << "                                                    " << endl;
  cout << "Synopsis:                                           " << endl;
  cout << "  The voiview utility renders                       " << endl;
  cout << "                                                    " << endl;
  cout << "Options:                                            " << endl;
  cout << "  -h,--help      Displays this help message         " << endl;
  cout << "                                                    " << endl;
  cout << "  --osx=<num>    Ownship X-position                 " << endl;
  cout << "  --osy=<num>    Ownship Y-position                 " << endl;
  cout << "  --osh=<num>    Ownship Heading                    " << endl;
  cout << "  --osv=<num>    Ownship Velocity/Speed             " << endl;
  cout << "                                                    " << endl;
  cout << "  --poly<Polygon> The obstacle                      " << endl;
  cout << "                                                    " << endl;
  cout << "  --noimg        Do not open with a background image" << endl;
  cout << "                                                    " << endl;
  cout << "Examples:                                           " << endl;
  cout << "  voiview --poly=250,-50:275,-50:275,-25:250,-25   " << endl;
  cout << "         --x1=20 --y1=30 --x2=50 --y2=80            " << endl;
  exit(0);  
}
  

