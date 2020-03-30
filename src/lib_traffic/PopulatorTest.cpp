#include <iostream>
//#include <ifstream>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include "FileBuffer.h"
#include <string>
#include <sstream>
#include <vector>
#include "TrafficObject.h"
#include "MBUtils.h"
// #include "PopulatorTest.h"
#include "TrafficScheme.h"
#include <string>
#include <vector>


using namespace std;




//----------------------------------------------------------
// bool PopulatorTest::populateTSS(string std){	
// TrafficObject new_object;
// string str = tolower(std);	
// string param = biteStringX(str, '=');

// if(param == "type")
// 	new_object.setTrafficType(str);
// if(param == "poly")
// 	new_object.setPolyType(str);
// if(param == "points")
// 	new_object.setPolyPts(str);
// if(param == "label")
// 	new_object.setPolyName(str);
// if(param == "start")
// 	new_object.setStartPt(str);

// new_object.print();
// my_tss.addTrafficObject(new_object);
// return(true);
// }

//------------------------------------------------------------

int main(int argc, char **argv)
{	
TrafficScheme my_tss;	
for(int i=1; i<argc; i++){
	// string line;	
	// fstream m_current_file(argv[i]);
	// 	while(getline(m_current_file,line)){
	// 	TrafficObject new_object;
	string filename = argv[i];
	vector<string> lines = fileBuffer(filename);
  	if(lines.size() == 0)
   	 return(false);

  for(unsigned int i=0; i<lines.size(); i++) {
    string str = stripBlankEnds(lines[i]);
    if(strBegins(str, "//"))
      continue;
    if(str == "")
      continue;

		// string str = line;
		string param = biteStringX(str, '=');
		TrafficObject new_object;
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

		new_object.print();
		my_tss.addTrafficObject(new_object);	
//		bool ok = populateTSS(&line);
		}
	}
my_tss.print();
}


