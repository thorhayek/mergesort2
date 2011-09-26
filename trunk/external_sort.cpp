#include"streams.h"
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<cstdlib>
#include<queue>

std::string make_file_name(int i);
int compare (const void * a, const void * b);

using namespace std ;
int main(int argc, char *argv[]) {


	int pagesize = getpagesize();
	const int N =  40*pagesize ; // N ==  10*4096
	const int M =  2*pagesize  ;   // M =  2 * 4096  // M HAS TO BE divisible by size of int 
	// N/M  = 20 ;  
	const int d =  5 ;   // 5 ways merge at a time 
	
	int offset = 0; 
	// create master file 
	OStreamWriteBuf writebuf_stream(1024); // buffer size = 40 
	string s = "m_files/master.bin";
	writebuf_stream.create(s);
	for(int i=0; i < 10*pagesize ; i++) // 10*pagesize elements written ==> size of file = N  
	{	
		writebuf_stream.writes(i);

	}
	writebuf_stream.closes();
	// START READING THE LARGE FILE and divide it into N/M streams 
	int buffer_size = pagesize ;
	// we have to make N/M streams use a vector 

	vector<IStreamReadBuf>  rb_vector(N/M,buffer_size);
	
	// now loop thru this vector and opens master.bin at different offsets 
	for(int i=0 ; i < N/M ; i++){
			rb_vector[i].opens(s,offset);
			offset = offset + M ; 
	}

	// NOW write read these streams one at a time and store them in an array 
	int temp ;

	for(int i = 0; i < N/M ; i++){ // loop thru all the streams 
			 // temp_array
			 int *temp_array = new int[M/sizeof(int)] ;
			 //read M elements 
			 for(int j=0; j< (M/sizeof(int)) ;j++ ){
					temp = rb_vector[i].read_next() ;
					if(temp == -1 || temp == -2 ){
					    cout << "read next failed or you generated random number"<<endl ;
						return 1 ; // main return 
					}	
				 	temp_array[j] = temp ;
			 }
			 // sort this array using quick sort 
			 qsort(temp_array,(M/sizeof(int)),sizeof(int),compare);
			 // now write these M / sizeof(int) elements to a file

			 string filename = make_file_name(i) ;
			 // open file 
			 cout << "creating filename " << filename << endl ;
			 OStreamWriteBuf wb(1024) ; // CHANGE THE BUFFER SIZE TO OPTIMAL
			 wb.create(filename);
			 // ELEMENTS SHOULD BE WRITTEN IN DESCENDING ORDER 
			 //for(int j=0 ; j < (M/sizeof(int)) ; j++){
			 for(int j=(M/sizeof(int) - 1 ) ; j >= 0 ; j--){
						wb.writes(temp_array[j]);		
			 }
			 wb.closes();

	}	


	
	// close the open streams 
	for(int i = 0; i < N/M ; i++){
			rb_vector[i].closes();
			
	}	
	// NOW Open these streams for reading use queue STL to store the istream references  
	queue<IStreamReadBuf *>     rbs_queue;
	vector<IStreamReadBuf>  rbs_vector(N/M,buffer_size);
		// open the new streams 
		for(int i =0; i < N/M ; i++){
			
			 string filename = make_file_name(i) ;
			 rbs_vector[i].opens(filename);
			 // store references of the streams in the queue  
			 rbs_queue.push(&(rbs_vector[i]));
		}
	
	// PART 3
	 int vector_size = N/M ;	
	 int out_index = 0 ;	
	 // while elements remain pop d elements from the queue then store them in  
	 while(!q.empty()){
			
		 	// pop d elements 
		 	IStreamReadBuf **infiles = new IStreamReadBuf*[d]; // vector<IStreamReadBuf *> var(d)
			
			for(int i = 0 ; i < d ; i++){
					infiles[i] = q.front();
            		q.pop();
			}
			OStreamWriteBuf outfile ; // CHANGE BUFFER SIZE TODO
			out_file_name = make_out_file_name(out_index);

			outfile.create(out_file_name);
			// d_way_merge starts for these d files 
			d_way_merge(infiles,d,outfile);
			// close output file 
			outfile.close();
			// open a d merge object and add it to the queue 
			IStreamReadBuf temp_read ;  // TODO Add Buffer size 
			temp_read.open(out_file_name);
			
			rbs_vector.push_back(temp_read); // index of new object = vector_size + out_index + 1
			int new_index = vector_size + out_index + 1 ;
			rbs_queue.push(&(rbs_vector[new_index]));
			cout << "sanity check temp_object filename == " << temp_read.getFilename() << "is same as vector filename == " << rbs_vector[new_index].getFilename() << endl ;
			

			out_index += 1 ;
								             
    }

	
}

d_way_merge(BaseIStream *infiles,int d,BaseOStream & outfile){

		// initialize the heap
	    PQ  heap(d);
		int streams_open = d ;
		// write the first d elements 
		for( j= 0 ; j < d ; j++){
			int number ;
			string filename = infiles[j].filename ; 
			infiles[j].read((char*)&number,4) ;
			Item a(number,filename);
			heap.insert(a) ;
			//cout << "read and inserted number " << number << endl ; 
		}
		
		while(streams_open > 0 ){


			Item d = heap.getmax();
			int n = d.getNumber() ;
			string s = d.getFile();
			cout << "deleted number " << n << " from heap from file " << s<<endl ;
			string filename = s ; // calls a copy constructor i
			// WRITE THE MAX element to file 
			int ret_code = outfile.write((char*)&n,4);
			if(ret_code < 0){
					cout << "writing to output failed breaking from while loop "<<endl ;
					break ; 
			}
			// split s to find out the corresponding open stream
			int file_index = get_file_index(s);
			if(file_index == -1 ){
					cout << "invalid file index exiting "<< endl ;
					return -1 ;
			}			
			cout << "file index of file we are reading from ==  " << file_index << endl ;
			if(infiles[file_index].end_of_stream()){
					streams_open-- ;	
					infiles[file_index].close();
					cout << "****File " << filename << "reached eof .closing corresponding stream " << endl ;
					// WE wont insert here
					continue ;
			}	
			// read element from the file that got removed from heap  
			int number ;
			infiles[file_index].read((char*)&number,4);
			Item a(number,filename);
			// now add this number to heap i
			cout <<"adding  number to the heap = "<< number <<"got this number from file  " << filename << endl ;
			heap.insert(a);

			// Always check for eof after READ !!! IMP 
			
	}


}	
std::string  make_file_name(int i){

		std::string filename("");
		std::ostringstream sin ;
		const std::string prefix = "m_files/slave_" ;
		const std::string suffix = ".bin";
		sin << i ;
    	filename.append(prefix);
    	filename.append(sin.str());
    	filename.append(suffix);
		sin.str("");
		return filename ;

}
std::string  make_out_file_name(int i){

		std::string filename("");
		std::ostringstream sin ;
		const std::string prefix = "m_files/d_merged_" ;
		const std::string suffix = ".bin";
		sin << i ;
    	filename.append(prefix);
    	filename.append(sin.str());
    	filename.append(suffix);
		sin.str("");
		return filename ;

}
int get_file_index(std::string &s){

			size_t index = s.find(".");
			if(index == std::string::npos){
				std::cout << " dot character not found wtf " << std::endl ;
					return -1 ;
			}
			
			//cout << "index of dot character "<< index <<endl ;
			s.erase(index);
			index = s.find("_") + 1 ;
			s.erase(0,index);
			//cout << " string after erase " << s << endl ;
			int file_index = atoi(s.c_str())	;
			return file_index ;

}
int compare (const void * a, const void * b)
{
	  return ( *(int*)a - *(int*)b );
}
