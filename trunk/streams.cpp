#include "streams.h"  // check c++ book i
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/mman.h>

#include<iostream>

using namespace std;

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

// BUFFERED READ AND WRITE with mmap and munmap
//default 
IStreamMmap::IStreamMmap(){
	//buf = new int[1];
	buf = (int *)-1; // will be alloted space by mmap 
	elements_read = 1 ;
	b_size = 1 ; 
	file_end_flag = false ; 
	//pagesize = getpagesize();
	offset = (-1)*b_size;

} 
IStreamMmap::IStreamMmap(int buffer_size){
	//buf = new int[buffer_size] ; 
	buf = (int *)-1;
	b_size = buffer_size ;  
	// initialize the elements read to 
	elements_read = b_size ; // ensures that firt time we use read call
	file_end_flag = false ;  
	//pagesize = getpagesize();
	offset = (-1)*b_size;

}
int IStreamMmap::opens(std::string & filename){

	//offset = 0; // ??? WRONG 
	// int pagesize = getpagesize(); // are we using this ?
	// connect the IStream_one obj with a file 
	read_fd = open(filename.c_str(),O_RDONLY ) ;  
	if(read_fd < 0 ){
		 return -1 ;  
	}
	//extract file length
	struct stat sbuf; // changed by vishay
	stat((char*)(filename.c_str()), &sbuf); //TODO check for error in the function call
	filelength = sbuf.st_size; 

	return 0 ;// on success 
	// handle error 	

}
int IStreamMmap::read_next(){

	int ret_code  ; 	
	int element=-2; 
	if(elements_read*sizeof(int) == b_size){
		//munmap previous mapping, if any
		if (buf != (int *)-1) {
			// TODO check return value  
			munmap(buf, b_size);
		}
		//increment offset to next b_size location in the file
		offset +=b_size;
		if(offset < filelength) { // we can do mapping
			buf =(int *) mmap((char *)0, b_size, PROT_READ, MAP_SHARED, read_fd, offset);
			//Error checking
			if(buf == (int *)-1) {
				std::cout << "MMAP read_next()::mmap error"<<endl;
			}
		}
		else {
			//XXX we can do this in destructor as well
			file_end_flag = true ;
			//munmap previous mapping, if any
			if (buf != (int *)-1) {
				munmap(buf, b_size);
			}
			return element;  //return -2
		}

		elements_read = 0 ;
		// read the element 
		element = buf[elements_read] ;// 0th element 
		elements_read++ ;
		
	}
	else{
		// read next element from buffer 
		if((elements_read*sizeof(int) + offset) < filelength) {
			element = buf[elements_read] ; 
			// update elements read 
			elements_read++ ;
		}
		else {
			file_end_flag = true;
		}
	}
	return element ; 
}
bool IStreamMmap::end_of_stream(){

	if(file_end_flag == true){
		// case where a buffer read did not fill the buffer
		return true ; 
	}
	else if((elements_read*sizeof(int) + offset) > filelength) {
		file_end_flag == true;
		return true;
	}
	else {
		return false;
	}
}

IStreamMmap::~IStreamMmap(){
	//munmap previous mapping, if any
	if (buf != (int *)-1) {
		munmap(buf, b_size);
	}

	close(read_fd); //check return value ? 
}

// Buffered Write 
//default constructor 
OStreamMmap::OStreamMmap(){

	buf = (int *)-1 ; 
	b_size  = getpagesize() ; 
	elements_written = 0 ;
	//offset = (-1)*b_size; 
	offset = 0 ; 
		

}
OStreamMmap::OStreamMmap(int buffer_size){
	// we are assuming that the buffersize is a multiple of pagesize and pagesize is a multiple of integer size  
	buf = (int *) -1 ; // initlize using some invalid memory location .. will be assigned memory by mmap 
	b_size = buffer_size ; 
	elements_written = 0 ;
	//offset = (-1)*b_size ; 
	offset = 0 ; 

}
int OStreamMmap::create(std::string & filename){
	// creates a file of 0 bytes 
	// set read and write right for other users creat already sets write ,append trunc flags	
	write_fd = open((char *)(filename.c_str()), O_RDWR|O_CREAT|O_TRUNC, 0666) ; // we NEED both read and write permissions here 
	//write_fd = creat(filename.c_str(),S_IRWXG|S_IRWXU);//user and group have read write exec permisson
	if(write_fd  < 0 ){
		return -1 ; 
	}
	return 0 ; // for success 

}
int OStreamMmap::writes(int element){

	int ret_code ;
	// check if buffer is full 
	// then write them to the buffer
	if(buf == (int *)-1){
		// FIRST TIME 
		// first do an lseek to increase the size  of the file by buffer_size  
		ret_code = lseek(write_fd,b_size -1 ,SEEK_END);
		write(write_fd,"",1); // necessary step 
		if (ret_code == -1){
			// error while lseeking 
			return -1 ; 
		}
		struct stat sbuf;
		stat("one.bin", &sbuf);
		cout << "one.bin size="<<sbuf.st_size<<endl;
		buf = (int *)mmap(0, b_size, PROT_WRITE, MAP_SHARED, write_fd, offset);// should we use MAP_PRIVATE instead
		if(buf == (int *)-1){ // should be int* instead of char * or void * 
			return -1 ;
		}
		elements_written = 0 ;
		// increase offset 
		offset = offset + b_size ;   
		// use memcpy or sprintf  ?? 
		buf[elements_written] = element ;
		elements_written++ ; 
	}
	else{
 		// mapping already done just write the element 
		buf[elements_written] = element ;  
		elements_written++ ; 
		// check if buffer is full 
		if(elements_written*sizeof(int) == b_size) // this will match sometime as pagesize mutiple of int 
		{
			// unmap file 
			ret_code = munmap(buf,b_size);
			if (ret_code == -1 ){
				// error while unmapping 
				return -1 ; 
			}
			// create NEW Mapping 
			// lseek 
			ret_code = lseek(write_fd,b_size -1,SEEK_END );
			write(write_fd,'\0',1); // this will actually incerase the size 
			if (ret_code == -1 ){
			// error while lseeking 
				return -1 ; 
			}
			// map after offset bytes
			buf = (int *)mmap(0, b_size, PROT_WRITE, MAP_SHARED, write_fd, offset);// should we use MAP_PRIVATE instead
			if(buf == (int *)-1){ // should be int* instead of char * or void * 
				return -1 ;
			}
			elements_written = 0 ;	
			// set offset for next mmap
			offset = offset + b_size ;   

		}	
		
	}
	/*buf[elements_written]  = element ;
	elements_written++ ;
	if(elements_written == b_size){
		// write to file
		std::cout << "writing to buf " <<element <<  std::endl ;
		int ret_code = write(write_fd,buf,b_size*sizeof(int)) ;
		
		if(ret_code <= 0 ){
			return -1 ; 
		}
		elements_written = 0 ; 
	}*/
	return 0 ; // return 0 means either written to buffer or to write
}
int OStreamMmap::closes(){

	// make sure we have unmapped the file 
	int ret_code = munmap(buf,b_size);
	if (ret_code == -1 ){
		// error while unmapping 
		return -1 ; 
	}
	/*if(elements_written > 0 ){
		int ret = write(write_fd,buf,elements_written*sizeof(int));
		if(ret <= 0 ){
			return -1 ;
		}
		elements_written = 0 ; // does not matter though 
	}*/
	// closing a file does not unmap the file 
	ret_code = close(write_fd);
	return ret_code ; 

}

OStreamMmap::~OStreamMmap(){

}
