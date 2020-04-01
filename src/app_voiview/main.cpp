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
#include "TrafficScheme.h"

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
     

    if(!handled) {
      cout << "voiview: Bad Arg: " << argi << endl;
      exit(1);
    }      
  }

  cout << "Hello World" << endl;
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
  

