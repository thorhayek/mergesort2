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
	const int M =  2*pagesize  ;   // M =  2 * 4096
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
			 qsort(temp_array,M/sizeof(int),sizeof(int),compare);
			 // now write these M / sizeof(int) elements to a file

			 string filename = make_file_name(i) ;
			 // open file 
			 cout << "creating filename " << filename << endl ;
			 OStreamWriteBuf wb(1024) ; // CHANGE THE BUFFER SIZE TO OPTIMAL
			 wb.create(filename);
			 for(int j=0 ; j < (M/sizeof(int)) ; j++){
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
	{
		vector<IStreamReadBuf>  rbs_vector(N/M,buffer_size);
		// open the new streams 
		for(int i =0; i < N/M ; i++){
			
			 string filename = make_file_name(i) ;
			 rbs_vector[i].opens(filename);
			 // store references of the streams in the queue  
			 rbs_queue.push(&(rbs_vector[i]));
		}
	}	
	
	// PART 3
	 // while elements remain pop d elements from the queue then store them in  
	 while(!q.empty()){
		             
		    IStreamReadBuf *temp  = q.front();
					             //cout << "item at the front == before popping "<<temp << endl ;
            q.pop();
								             
   }

	
//	for(int i=0;i < 4; i++){
//			// dynmaic polymorphism at work 
//			offset = i * 40 ;  // offset = 0  
//			if(i == 3){
//				base_istream[i]->opens(s,pagesize);
//			}
//			else{
//				base_istream[i]->opens(s,offset);
//			}	
//			//offset += 40 ; 
//	}	
//	
//	// NOW READ 10 elements each 
//	for(int i = 0 ; i < 4 ; i++){
// 		 for(int j=0; j < 10 ; j++){
//				int temp = base_istream[i]->read_next();
//				cout << "element read = " << temp  << " object number "<< i << endl ;
//		 }	 	
//	}
//
//	for(int i=0;i < 4; i++){
//			// dynmaic polymorphism at work 
//			//offset = i * 40 ;  // offset = 0  
//			int ret_code = base_istream[i]->closes();
//			//offset += 40 ; 
//	}
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

int compare (const void * a, const void * b)
{
	  return ( *(int*)a - *(int*)b );
}
