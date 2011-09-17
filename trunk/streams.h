//streams.h -- stream classes 
#ifndef STREAMS_H_
#define STREAMS_H_

#include<iostream>
#include<string>


// ABSTRACT BASEIStream class 
class BaseIStream {

  // pvt data members 
  	// STATIC COUNT ??? variable 
  public:
	// we can have constructors for ABC if we have data types  

	// virtual has an extra overhead ..make sure its reqd
	virtual int read_next() = 0; // will return the next int 
	virtual int opens(std::string & filename) = 0 ; //  assign value to fd 
	virtual bool end_of_stream() = 0 ;

	// virtual destructor is a good practice for all base classes
	virtual ~BaseIStream() {}  // inline empty definition  
	
}; 
// ABSTRACT BASE OSTREAM class 
class BaseOStream {

    public:
	// we can have constructors for ABC if we have data types  
	virtual int create(std::string &filename ) = 0;
	virtual int writes(int number) = 0;
	virtual int closes() = 0; // closes the file and detaches stream
	// virtual destructor is a good practice for all base classes
	virtual ~BaseOStream()  {} 

};
// uses read  system calls with buf = 1 
class IStreamRead :public BaseIStream{
	
	private :
		int  read_fd ; // the read fd  
		//int  buf ;  // the one int that is read 
	public : 
		// default constructor
		//IStreamRead(); 
		virtual int opens(std::string & filename)  ; //  assign value to fd 
		virtual int read_next() ; // will return the next int 
		virtual bool end_of_stream()  ;
	// we dont need to have a virtual destructor (default will do) here as the base class destructor which is virtual will be called automatically
		// destructor can closes the file  
		virtual ~IStreamRead();	

};
// uses writes with buf (of size 1 ) 
class OStreamWrite : public BaseOStream{
	private :
		int write_fd ; 
		int buf ; 
	public:
		virtual int create(std::string &filename ) ;
		virtual int writes(int number) ;
		virtual int closes() ; // closes the file and detaches stream
	
};
// uses read with buf B  
class IStreamReadBuf :public BaseIStream{
	private : 
		int read_fd ; // have to redefine here as inhertiance does not include private read_fd
	 	int   *buf ; // will be initialized by constructor we need this to be persistent during diff calls 
		int elements_read ; 
		int b_size ; // we have to keep track  of size ??  // no of elements in buffer
		bool file_end_flag ;  
	public :
		// default constructor 
		IStreamReadBuf(); // creates buffer of size 1 
		IStreamReadBuf(int buffer_size); // where buffer size is the num of elements 
		virtual int opens(std::string & filename)  ; //  assign value to fd 
		virtual int read_next() ; // will return the next int and will implement buffering
		virtual bool end_of_stream() ;
		virtual ~IStreamReadBuf() ;  
		// destructor reqd but should it be virtual ?? stack overflow ??  
};
class OStreamWriteBuf : public BaseOStream{
	private :
		int write_fd ; // have to redefine here as inhertiance does not include private read_fd
		int *buf ;
		int b_size ;  // no if elements the buffer can hold 
		int elements_written ;  // no of elements already written 
	public:
		//default construct 
		OStreamWriteBuf(); // creates buffer of size 1 
		OStreamWriteBuf(int buffer_size); 
		virtual int create(std::string &filename ) ;
		virtual int  writes(int number) ;
		virtual int  closes() ; // redefine because when close we have to empty the buffer
		// DEFINE VIRTUAL DESTRUCTOR 
		virtual ~OStreamWriteBuf() ;  
	
};
// uses the fread 
class IStreamFRead : public BaseIStream{
	private :
		FILE *read_ptr;
		// buffer
		int *buf ;
		static const int SIZE = sizeof(int); //
		// size of buffer 
		int b_size ;
		int elements_read ; 
 	
			
	public 	:	
		// default constructor 
		IStreamFRead();
		// other constructors 
		IStreamFRead(int buffer_size);
		virtual int opens(std::string & filename)  ; //  assign value to fd 
		virtual int  read_next() ; // will return the next int 
		virtual bool end_of_stream()  ;
		// DEFINE DESTRUCTOR !!!!
		~IStreamFRead();

};
// uses fwrites 
class OStreamFWrite : public BaseOStream{
	
	private:
		FILE *write_ptr ;
		int  *buf ; 
		int b_size ; 
		static const int SIZE = sizeof(int); 
		int elements_written ; 
	public :
		// constructors 
	 	OStreamFWrite();
		OStreamFWrite(int buffer_size);
		virtual int create(std::string & filename);
		virtual int writes(int n);
		virtual int closes();
		//Destructor
		~OStreamFWrite();
		
};
// uses mmap with buf B 
class IStreamMmap : public BaseIStream{
	private:
		int read_fd ; 
		int pagesize ;
		int buffer_pages ; // determines how many pages our buffer has so buffer_size = buffer_pages * pagesize  

	public :
		// default constructor 
		IStreamMmap() ; // initalizes page size 
		virtual int read_next() ; // will return the next int 
		virtual int opens(std::string & filename)  ; //  assign value to fd 
		virtual bool end_of_stream() ;

};

// uses mmap with buf of size B 
class OStreamMmap : public BaseOStream{

	private: 
		int fd ; 
		// add other stuff
	public:
		// TODO constructor
		
		virtual int read_next();
		virtual int opens(std::string &filename);
		virtual bool end_of_stream();
		// TODO destructor 
		
};

#endif
