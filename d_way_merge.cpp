#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

const static int d = 5 ;
const static int count = 10 ;

int main(){

	using namespace std ; 
	int i,j ;
	ostringstream sin ;
	const string prefix = "d_files/" ;
	const string suffix = ".bin";
	ifstream infiles[d];

	for(i=0 ; i < d ; i++){
			string filename("");
			sin << i ;
    		filename.append(prefix);
    		filename.append(sin.str());
    		filename.append(suffix);
			cout << "opening gile "<<filename << endl ;
			infiles[i].open(filename.c_str(),ios::binary);
			sin.str("");

	}

	
	for( j= 0 ; j < d ; j++){

		//int number ;
		while( !infiles[j].eof() ){
			int number ;
			infiles[j].read((char*)&number,4) ;
			cout << "reading number " << number << endl ; 
		//clear stream 
		}
	}
		// written to this file now close 
	for(i=0 ; i < d ; i++){

		infiles[i].close();
	}	
}

