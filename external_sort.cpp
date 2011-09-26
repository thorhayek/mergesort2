#include"streams.h"
#include"heap.h"
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<cstdlib>
#include<queue>

int get_file_index(std::string &s);
std::string make_file_name(int i);
std::string make_out_file_name(int i);
int d_way_merge(std::vector<IStreamReadBuf*> &infiles,int d ,OStreamWriteBuf &outfiles);
int compare (const void * a, const void * b);

using namespace std ;
int main(int argc, char *argv[]) {


	int pagesize = getpagesize() ;
	const int N =  40*pagesize ; // N ==  10*4096 ints 
	const int M =  2*pagesize  ;   // M =  2 * 4096  // M HAS TO BE divisible by size of int 
	// N/M  = 20 ;  
	const int d =  5 ;   // 5 ways merge at a time 
	
	int offset = 0; 
	// create master file 
	OStreamWriteBuf writebuf_stream(1024); // buffer size = optimum  
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
	int temp = -1 ;

	for(int i = 0; i < N/M ; i++){ // loop thru all the streams 
			 // temp_array
			 int *temp_array = new int[M/sizeof(int)] ;
			 //read M elements 
			 for(int j=0; j< (M/sizeof(int)) ;j++ ){
					if(!rb_vector[i].end_of_stream()) {
				 		temp = rb_vector[i].read_next();
					}
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
			 //for(int j=0 ; j < (M/sizeof(int)) ; j++)
			 for(int j=(M/sizeof(int) - 1 ) ; j >= 0 ; j--){
						wb.writes(temp_array[j]);		
			 }
			 wb.closes();
			 delete [] temp_array ;

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
	 int ret_code = 0;
 	 //IStreamReadBuf **infiles = new IStreamReadBuf*[d]; //or  vector<IStreamReadBuf *> var(d)
	 vector<IStreamReadBuf *> infiles(d) ;
	 // while elements remain pop d elements from the queue then store them in  
	 while(!rbs_queue.empty()){
			
		 	// pop d elements 
		 	cout << "reached top " << endl ;
			
			for(int i = 0 ; i < d ; i++){
					infiles[i] = rbs_queue.front();
            		rbs_queue.pop();
			}
			OStreamWriteBuf outfile(1024) ; // CHANGE BUFFER SIZE TODO
			std::string out_file_name = make_out_file_name(out_index);

		 	cout << "reached top 2" << endl ;
			outfile.create(out_file_name);
		 	cout << "reached top 3" << endl ;
			// d_way_merge starts for these d files 
			int d_ret = d_way_merge(infiles,d,outfile);
		 	cout << "reached top 4" << endl ;
			cout <<"ret_code for d_way_merge "<< d_ret << endl ;
			// close output file 
			int ret_code = outfile.closes();
			if(ret_code < 0){
					cout << "ret code for closes =" << ret_code <<" breaking for while loop " << endl ;
					break ;
			}
			// open a d merge object and add it to the queue 
			IStreamReadBuf temp_read(1024) ;  // TODO Add Buffer size
			ret_code =temp_read.opens(out_file_name);
			//cout << "opened file name " << out_file_name ;
			if(ret_code < 0){
					cout << "could not opene file " << out_file_name << endl ;
					cout << "ret code for opens = " << ret_code <<" breaking for while loop " << endl ;
					break ;
			}	
			rbs_vector.push_back(temp_read); // index of new object = vector_size + out_index + 1
			int new_index = vector_size + out_index  ;
			cout << "new index =  "<< new_index <<endl ;
			rbs_queue.push(&(rbs_vector[new_index]));
			cout << "sanity check temp_object filename == " << temp_read.getFilename() << "is same as vector filename == " << rbs_vector[new_index].getFilename() << endl ;
			

			out_index += 1 ;
			cout << "end of iteration " << endl ;
			//delete [];i
			//for(int i = 0 ; i < d ; i++){
					//cout << "deleting " << i ;
					//delete infiles[i];
			//}
			//delete [] infiles ;
			//cout << "end of iteration 2 " << endl ;
			//infiles = NULL ;
								             
    }

	
}

int d_way_merge(vector<IStreamReadBuf *> &infiles,int d,OStreamWriteBuf & outfile){

		// initialize the heap
	    PQ  heap(d);
		int streams_open = d ;
		cout << "inside d_way _merge " << endl ;
		// write the first d elements 
		for(int j= 0 ; j < d ; j++){
			//int number ;
			cout << "before this " << endl ;
			string filename = infiles[j]->getFilename() ; 
			cout << "after this with filename "<< filename  << endl ;
			if(!infiles[j]->end_of_stream()) {
				int number = infiles[j]->read_next() ;
				cout << "after this 2 " << endl ;
				Item a(number,filename);
				heap.insert(a) ;
			}
			//cout << "read and inserted number " << number << endl ; 
		}
		
		cout << "inside d_way _merge " << endl ;
		while(streams_open > 0 ){


			Item d = heap.getmax();
			int n = d.getNumber() ;
			string s = d.getFile();
			cout << "deleted number " << n << " from heap from file " << s<<endl ;
			string filename = s ; // calls a copy constructor i
			// WRITE THE MAX element to file 
			int ret_code = outfile.writes(n);
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
			if(infiles[file_index]->end_of_stream()){
					streams_open-- ;	
					infiles[file_index]->closes();
					cout << "****File " << filename << "reached eof closing corresponding stream " << endl ;
					// WE wont insert here
					continue ;
			}	
			// read element from the file that got removed from heap  
			//int number ;
			if(!infiles[file_index]->end_of_stream()) {
				int number = infiles[file_index]->read_next();
				Item a(number,filename);
				// now add this number to heap i
				cout <<"adding  number to the heap = "<< number <<"got this number from file  " << filename << endl ;
				heap.insert(a);
			}
			// Always check for eof after READ !!! IMP 
			
	}
	cout << "reached  end of d_way merge " << endl ;
	return 0 ;

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
			index = s.rfind("_") + 1 ;
			s.erase(0,index);
			//cout << " string after erase " << s << endl ;
			int file_index = atoi(s.c_str())	;
			return file_index ;

}
int compare (const void * a, const void * b)
{
	  return ( *(int*)a - *(int*)b );
}
