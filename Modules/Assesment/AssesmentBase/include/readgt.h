#ifndef READGT_H_
#define READGT_H_
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

void readGT(const char * fileName, ttt::AdherensJunctionVertices::Pointer & gt){
	ifstream file ( fileName);
	string value;
	//Discard header
	gt->clear();
    getline ( file, value); // read a string until next comma: http://www.cplusplus.com/reference/string/getline/
	while ( file.good() )
	{
		itk::Index<3> point;
		//getline ( file, value, ',' ); // read a string until next comma: http://www.cplusplus.com/reference/string/getline/
		getline ( file, value, ',' );
	    point[0]=atoi(value.c_str());
	    getline ( file, value, ',' );
	    point[1]=atoi(value.c_str());
	    getline ( file, value);
	    point[2]=atoi(value.c_str());

	    if(file.good()){
	    	ttt::AdherensJunctionVertex::Pointer vertex = ttt::AdherensJunctionVertex::New();
	    	vertex->SetPosition(point);
	    	gt->push_back(vertex);

	    }
	}
#if 0
	for(PointContainer::iterator it=gt.begin();it!=gt.end();it++){
		std::cout << *it << std::endl;
	}
#endif

}



#endif /* READGT_H_ */
