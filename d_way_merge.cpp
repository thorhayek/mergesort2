#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include"heap.h"
#include<cstdlib>

std::string make_file_name(int i);
int get_next_open_stream(std::ifstream *infiles);
int get_file_index(std::string &s);
const static int d = 5 ;
const static int count = 10 ;

int main(){

	using namespace std ; 
	int i,j ;
	int streams_open = d ; 
	//ostringstream sin ;
		ifstream infiles[d];
	ofstream outfile ; 
	outfile.open("d_merge_out.bin",ios::binary);
	PQ  heap(d); // create heap that can contain d elements

	// 0.bin  has 0 as index .. 1.bin has 1 as index ..so on
	for(i=0 ; i < d ; i++){

			string filename = make_file_name(i);
			infiles[i].open(filename.c_str(),ios::binary);

	}

	// READ ONE ELEMENT 
	
	// ADD IT TO HEAP 
	for( j= 0 ; j < d ; j++){

			int number ;
			string filename = make_file_name(j);
			infiles[j].read((char*)&number,4) ;
			Item a(number,filename);
			heap.insert(a) ;
			//cout << "read and inserted number " << number << endl ; 
	}

	// now start delete element and and adding new element till no more element 
	while(streams_open > 0 ){


			Item d = heap.getmax();
			int n = d.getNumber() ;
			string s = d.getFile();
			cout << "deleted number " << n << " from heap from file " << s<<endl ;
			string filename = s ; // calls a copy constructor 

			// split s to find out the corresponding open stream
			int file_index = get_file_index(s);
			if(file_index == -1){
					cout << "invalid file index exiting "<< endl ;
					return -1 ;
			}			
			//cout << "file index of file we are reading from ==  " << file_index << endl ;
		 	// put the number  into ofstream
			outfile.write((char*)&n,4);
			if(outfile.fail()){
					cout << "writing to output failed breaking from while loop "<<endl ;
					break ; 
			}	
			// read element from the file that got removed from heap  
			int number ;
			infiles[file_index].read((char*)&number,4);
			// Always check for eof after READ !!! IMP 
			if(infiles[file_index].eof()){
					streams_open-- ;	
					infiles[file_index].close();
					cout << "****File " << make_file_name(file_index) << "reached eof .closing corresponding stream " << endl ;
				// WE wont insert here
					continue ;
	
			}
			Item a(number,filename);
			// now add this number to heap i
			cout <<"adding  number to the heap = "<< number <<"got this number from file  " << filename << endl ;
			heap.insert(a);
	}

	// close output file 
	outfile.close();
	// READ THE OUTPUT FILE 
	ifstream read_out ;
	read_out.open("d_merge_out.bin",ios::binary);
	if(read_out.fail()){
			cout << "open failed " << endl ;
			return 1 ;
	}	
	while(!read_out.eof() ) {
			int num ; 
			read_out.read((char*)&num,4);
			if(read_out.eof()){
					continue ;
			}

			cout << num << endl ;
   }
	read_out.close();
}
// passing by reference here is not a good idea since object will be destructed .. 
std::string  make_file_name(int i){

		std::string filename("");
		std::ostringstream sin ;
		const std::string prefix = "d_files/" ;
		const std::string suffix = ".bin";

			sin << i ;
    		filename.append(prefix);
    		filename.append(sin.str());
    		filename.append(suffix);
			sin.str("");
			return filename ;

}	
/*int  get_next_open_stream(std::ifstream *infiles){
		
		int i ; 
		for(i=0; i < d ;i++) {

				if( infiles[i].is_open() && !(infiles[i].eof()) ) {
					return i;
				}	
		}	
		return -1 ; 
}
*/
int get_file_index(std::string &s){

			size_t index = s.find(".");
			if(index == std::string::npos){
				std::cout << " dot character not found wtf " << std::endl ;
					return -1 ;
			}
			
			//cout << "index of dot character "<< index <<endl ;
			s.erase(index);
			index = s.find("/") + 1 ;
			s.erase(0,index);
			//cout << " string after erase " << s << endl ;
			int file_index = atoi(s.c_str())	;
			return file_index ;

}	
