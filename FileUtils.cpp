
#include <fstream>
#include <string>
#include <cstdlib>
#include <list>
#include <iostream>
#include "MotionProfile.h"
#include "FileUtils.h"

using namespace std;


FileUtils::FileUtils() {
	
}


void FileUtils::loadProfile(MotionProfile* mp) {
	
	//MotionProfile mp;
	
    ifstream file ( "/home/pi/Projects/CatToy/profile.csv" ); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
    string value;
    list<string> values;
    while ( file.good() )
    {
        getline ( file, value, ',' ); // read a string until next comma: http://www.cplusplus.com/reference/string/getline/
        if (value.find('\n') != string::npos) {
            split_line(value, "\n", values);
        } else {
            values.push_back(value);
        }
    }

    list<string>::const_iterator it = values.begin();
    int stepCounter = 0;
    

    
 //   cout << "values.size=" << values.size() << endl;
    
    for (it = values.begin(); it != values.end(); ++it) {
        double d;
		
			string tmp = *it;
			d = strtod(tmp.c_str(), NULL);
			cout << "Double val: " << right << showpoint << d << endl;
			it++;
			mp->pan[stepCounter] = d;
			
			tmp = *it;
			d = strtod(tmp.c_str(), NULL);
			cout << "Double val: " << right << showpoint << d << endl;
			it++;
			mp->tilt[stepCounter] = d;	
					
			tmp = *it;
			d = strtod(tmp.c_str(), NULL);
			cout << "Double val: " << right << showpoint << d << endl;
			it++;			
			mp->speed[stepCounter] = d;
			
			tmp = *it;
			d = strtod(tmp.c_str(), NULL);
			cout << "Double val: " << right << showpoint << d << endl;
			mp->pause[stepCounter] = d;
			cout << endl;
					
			stepCounter++;
			        
    }
    
    mp->numSteps = stepCounter;
    
    file.close();
    
}

void FileUtils::split_line(string& line, string delim, list<string>& values)
{
    size_t pos = 0;
    while ((pos = line.find(delim, (pos + 1))) != string::npos) {
        string p = line.substr(0, pos);
        values.push_back(p);
        line = line.substr(pos + 1);
    }

    if (!line.empty()) {
        values.push_back(line);
    }
}


void FileUtils::saveProfile(MotionProfile mp) {
	
   ofstream oFile ( "/home/pi/Projects/CatToy/profile.csv" ); 				// declare file stream: http://www.cplusplus.com/reference/iostream/ofstream/
	

	for (int x=0; x < mp.numSteps; x++) {
		
		oFile << to_string(mp.pan[x]) << "," <<
			to_string(mp.tilt[x]) << "," <<
			to_string(mp.speed[x]) << "," <<
			to_string(mp.pause[x]) << endl;
					
	}
	
	oFile.close();
	
}


