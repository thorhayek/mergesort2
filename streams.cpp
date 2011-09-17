#include "streams.h"  // check c++ book i
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>


int IStreamRead::opens(std::string & filename){

	// connect the IStream_one obj with a file 
	read_fd = open(filename.c_str(),O_RDONLY ) ;  
	if(read_fd < 0 ){
		 return -1 ;  
	}
	return 0 ;// on success 
	// handle error 	

}
int IStreamRead::read_next(){

	// reads one 
	int buf ; 
	int ret_code = read(read_fd,&buf,4);
	if (ret_code==0){
		return 0 ; //  0 means end of file 
	}
	else if(ret_code < 4 ) {
		return -1 ;
	} 
	return buf ; 
	
}
bool IStreamRead::end_of_stream(){

	// have we reached the end of file 
	// read returns 0 if end of file is reached 
	int temp_element ; 
	int ret_code = read(read_fd,&temp_element,4);
	if(ret_code  < 4 ) {
		// we have reached the end of the list 
		lseek(read_fd,-ret_code,SEEK_CUR);
		return true ; 
	}  
	else if(ret_code==4) {  // ret_code = 4 
		// eof stream not reached 
		lseek(read_fd,-4,SEEK_CUR);
		return false ;
	} 

}  

IStreamRead::~IStreamRead(){
	//delete [] buf ;
	close(read_fd); //check return value ? 
}
int OStreamWrite::create(std::string & filename){

	// set read and write right for other users creat already sets write ,append trunc flags	
	write_fd = creat(filename.c_str(),S_IRWXG|S_IRWXU);//user and group have read write exec permisson
	if(write_fd  < 0 ){
		return -1 ; 
	}
	return 0 ; // for success 
	

}
int OStreamWrite::closes(){
	// should there be a close for read as well 
	int ret_code = close(write_fd);
	return ret_code ; 

}
int OStreamWrite::writes(int num){
	int ret_code = write(write_fd,&num,4);
	if(ret_code < 4 ){
		return -1 ; 	
	} 
	return 0 ; // write was successful 
}

// BUFFERED READ AND WRITE
//default 
IStreamReadBuf::IStreamReadBuf(){
	buf = new int[1];
	elements_read = 1 ;
	b_size = 1 ; 
	file_end_flag = false ; 

} 
IStreamReadBuf::IStreamReadBuf(int buffer_size){
	buf = new int[buffer_size] ; 
	b_size = buffer_size ;  
	// initialize the elements read to 
	elements_read = b_size ; // ensures that firt time we use read call
	file_end_flag = false ;  
}
int IStreamReadBuf::opens(std::string & filename){

	// connect the IStream_one obj with a file 
	read_fd = open(filename.c_str(),O_RDONLY ) ;  
	if(read_fd < 0 ){
		 return -1 ;  
	}
	return 0 ;// on success 
	// handle error 	

}
int IStreamReadBuf::read_next(){

	int ret_code  ; 	
	int element ; 
	if(elements_read == b_size){
		// make a new 
		ret_code = read(read_fd,buf,b_size*sizeof(int)) ;
		if(ret_code == 0){
			// i cant think of case where this will happen if end of stream is used properly  
			return -2 ; 	
		}
		else if(ret_code < 0 ){
			return -1 ; 
		}
		// set b_size to number of elements in the buffer 
		if(ret_code < (b_size*sizeof(int)) ){
			// we assume that ret_code is a multiple of 4 
			b_size = ret_code/sizeof(int)  ;// will truncate to int if ret_code not divisible by 4(size of int)
			file_end_flag = true ; 
		}
		//update elemets read
		std::cout << "here" << std::endl ; 
		elements_read = 0 ;
		// read the element 
		element = buf[elements_read] ;// 0th element 
		elements_read++ ;
		
	}
	else{
		// read next element from buffer 
		element = buf[elements_read] ; 
		// update elements read 
		elements_read++ ; 
	}
	return element ; 
}
bool IStreamReadBuf::end_of_stream(){

	if(file_end_flag == true){
		// case where a buffer read did not fill the buffer
		return true ; 
	}
	else{
		// the case where buffered read just read the last element .
		int temp_element ; 
		int ret_code = read(read_fd,&temp_element,4); //replace 4 by size of int 
		if(ret_code  < 4 ) {
			// we have reached the end of the list 
			lseek(read_fd,-ret_code,SEEK_CUR); // not really necessary
			return true ; 
		}  
		else {  // ret_code = 4 
			// eof stream not reached 
			lseek(read_fd,-4,SEEK_CUR);
			return false ;
		} 
 
	}

}
IStreamReadBuf::~IStreamReadBuf(){
	delete [] buf ;
	close(read_fd); //check return value ? 
}
// Buffered Write 
//default constructor 
OStreamWriteBuf::OStreamWriteBuf(){
	buf = new int[1] ; 
	b_size  = 1 ;
	elements_written = 0 ; 

}
OStreamWriteBuf::OStreamWriteBuf(int buffer_size){
	buf = new int[buffer_size];
	b_size = buffer_size ; 
	elements_written = 0 ; 

}
int OStreamWriteBuf::create(std::string & filename){

	// set read and write right for other users creat already sets write ,append trunc flags	
	write_fd = creat(filename.c_str(),S_IRWXG|S_IRWXU);//user and group have read write exec permisson
	if(write_fd  < 0 ){
		return -1 ; 
	}
	return 0 ; // for success 
	

}
int OStreamWriteBuf::writes(int element){

	// check if buffer is full 
	// then write them to the buffer 
	buf[elements_written]  = element ;
	elements_written++ ;
	if(elements_written == b_size){
		// write to file
		std::cout << "writing to buf " <<element <<  std::endl ;
		int ret_code = write(write_fd,buf,b_size*sizeof(int)) ;
		
		if(ret_code <= 0 ){
			return -1 ; 
		}
		elements_written = 0 ; 
	}
	return 0 ; // return 0 means either written to buffer or to write
}
int OStreamWriteBuf::closes(){

	// make sure buffer elements are written if buffer is non-empty 
	if(elements_written > 0 ){
		int ret = write(write_fd,buf,elements_written*sizeof(int));
		if(ret <= 0 ){
			return -1 ;
		}
		elements_written = 0 ; // does not matter though 
	}
	int ret_code = close(write_fd);
	return ret_code ; 

}

OStreamWriteBuf::~OStreamWriteBuf(){
	delete [] buf ; 
}
// IStreamFRead 
IStreamFRead::IStreamFRead(){
	
	buf = new int[1] ;
	b_size = 1 ; 
	elements_read = b_size ;
}
IStreamFRead::IStreamFRead(int buffer_size){
	buf = new int[buffer_size];
	b_size = buffer_size ; 
	elements_read = b_size ; 
}
int IStreamFRead::opens(std::string & filename){
	
	read_ptr = fopen(filename.c_str(),"rb");
	if(read_ptr == NULL){
		return -1 ;
	}
	return 0 ;
}
int IStreamFRead::read_next(){
	
	int ret ;
	int element ; 
	if(elements_read == b_size){
		// return the no of elements read not bytes
		ret = fread(buf,SIZE,b_size,read_ptr);
		if(ret < b_size ){
			// either feof or error
			// if error then return 
			if(ferror(read_ptr)) {
					
				return -1 ; 
			}
			b_size = ret ;
		}
		
		elements_read = 0 ;
		element = buf[elements_read] ; 
		elements_read++ ;
	}
	else{
		element = buf[elements_read] ; 
		elements_read++ ;
	}
	return element; 
}
// end_of_stream will be called after reading every element
bool IStreamFRead::end_of_stream(){

	if(feof(read_ptr)){
		return true ;
	}
	else{
		return false ; 
	}

}
IStreamFRead::~IStreamFRead(){
	delete [] buf ;
	fclose(read_ptr); //check return value ? 
}
// fwrite stream
OStreamFWrite::OStreamFWrite(){
	buf = new int[1] ;
	b_size = 1 ; 
	elements_written = 0 ; 

}
OStreamFWrite::OStreamFWrite(int buffer_size){
	buf = new int[buffer_size] ;
	b_size = buffer_size ; 
	elements_written = 0 ; 

}
int OStreamFWrite::create(std::string & filename){
	write_ptr = fopen(filename.c_str(),"wb");
	if(write_ptr == NULL){
		return -1 ;
	}
	return 0 ;
}
int OStreamFWrite::writes(int element){

	buf[elements_written] = element ;
	elements_written++ ;
	if(elements_written== b_size){
		// write buf 
		int ret = fwrite(buf,SIZE,b_size,write_ptr);
		if(ret != b_size){
			return -1 ; 
		}
		elements_written = 0 ;
	}
	return 0 ;
		
	
}
int OStreamFWrite::closes(){
	
	if(elements_written > 0 ){
		int ret =fwrite(buf,SIZE,elements_written,write_ptr);
		if(ret != b_size){
			return -1 ;	
		}
	}	
	fclose(write_ptr);
}	
OStreamFWrite::~OStreamFWrite(){
	delete [] buf ; 
}
