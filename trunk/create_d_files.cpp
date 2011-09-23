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
	ofstream outfiles[d];
	//ofstream one ; 
	//one.open("one.bin",ios::out | ios::binary);
	int number = 1 ; 
	//one.write((char*)&number,4) ;
	//one.close();

	for(i=0 ; i < d ; i++){
			string filename("");
			sin << i ;
    		filename.append(prefix);
    		filename.append(sin.str());
    		filename.append(suffix);

			outfiles[i].open(filename.c_str(),ios::binary);
			sin.str("");

	}
	for(i = 0 ; i < count ; i++ ){

		for( j= 0 ; j < d ; j++){

			outfiles[j].write((char*)&number,4) ;
			//outfiles[j].flush();
			//cout << "writing to file "<< filename<< "the number "<< number << endl ;
			number++ ;
			//clear stream 
		}
		// written to this file now close 
	}
	for(i=0 ; i < d ; i++){
		outfiles[i].close();
	}
}
