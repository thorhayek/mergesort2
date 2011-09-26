//streams.h -- stream classes 
#ifndef STREAMS_H_
#define STREAMS_H_

#include<iostream>
#include<string>
#include<sys/types.h>

// ABSTRACT BASEIStream class 
class BaseIStream {

  // pvt data members 
  	// STATIC COUNT ??? variable 
  public:
	// we can have constructors for ABC if we have data types  

	// virtual has an extra overhead ..make sure its reqd
	virtual int read_next() = 0; // will return the next int 
	virtual int opens(std::string & filename) = 0 ; //  assign value to fd 
	// CONCEPT .. when you redefine one base class overloaded function it hides all overloaded functions  so we have to redefine all overloaded funcs 
	virtual int opens(std::string & filename, ulong offset) = 0 ; // offset is usually the size of M  
	virtual bool end_of_stream() = 0 ;
	virtual int closes() = 0;
	

	// virtual destructor is a good practice for all base classes
	virtual ~BaseIStream() {}  // inline empty definition  
	
}; 
// ABSTRACT BASE OSTREAM class 
class BaseOStream {

    public:
	// we can have constructors for ABC if we have data types  
	virtual int create(std::string &filename ) = 0;
	virtual int writes(int number) = 0;
	virtual int closes() = 0; // closes the file and detaches streami
	//virtual std::string getFilename();
	// virtual destructor is a good practice for all base classes
	virtual ~BaseOStream()  {} 

};
// uses read  system calls with buf = 1 
class IStreamRead :public BaseIStream{
	
	private :
		int  read_fd ; // the read fd
		std::string filename ;
		//int  buf ;  // the one int that is read 
	public : 
		// default constructor
		//IStreamRead(); 
		virtual int opens(std::string & filename)  ; //  assign value to fd 
		virtual int opens(std::string & filename,ulong offset)  ; //  assign value to fd 
		virtual int read_next() ; // will return the next int 
		virtual bool end_of_stream()  ;
		std::string getFilename(){   // function virtual in base class automatically becomes virtual in child / derived class
			return filename ;
		}	
		virtual int closes();
		
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
		//XXX Shantha: you can make read_fd a protected member in BaseIStream
		int read_fd ; // have to redefine here as inhertiance does not include private read_f
	 	int   *buf ; // will be initialized by constructor we need this to be persistent during diff calls 
		ulong elements_read ; 
		ulong b_size ; // we have to keep track  of size ??  // no of elements in buffer
		bool file_end_flag ; 
		ulong total_reads;
		ulong filelength;
		std::string filename ;
	public :
		// default constructor 
		IStreamReadBuf(); // creates buffer of size 1 
		IStreamReadBuf(ulong buffer_size); // where buffer size is the num of elements 
		IStreamReadBuf(const IStreamReadBuf &b); // where buffer size is the num of elements 
		virtual int opens(std::string & filename)  ; //  assign value to fd 
		virtual int opens(std::string & filename,ulong offset)  ; //  assign value to fd 
		virtual int read_next() ; // will return the next int and will implement buffering
		virtual bool end_of_stream() ;
		virtual int closes();
		std::string getFilename(){   // function virtual in base class automatically becomes virtual in child / derived class
			return filename ;
		}
		virtual ~IStreamReadBuf() ;  
		// destructor reqd but should it be virtual ?? stack overflow ??  
};
class OStreamWriteBuf : public BaseOStream{
	private :
		int write_fd ; // have to redefine here as inhertiance does not include private read_fd
		int *buf ;
		ulong b_size ;  // no if elements the buffer can hold 
		ulong elements_written ;  // no of elements already written 
	public:
		//default construct 
		OStreamWriteBuf(); // creates buffer of size 1 
		OStreamWriteBuf(ulong buffer_size); 
		OStreamWriteBuf(const OStreamWriteBuf &b); 
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
		ulong b_size ;
		ulong elements_read ; 
		std::string filename ;
 	
			
	public 	:	
		// default constructor 
		IStreamFRead();
		// other constructors 
		IStreamFRead(ulong buffer_size);
		IStreamFRead(const IStreamFRead& b);
		virtual int opens(std::string & filename)  ; //  assign value to fd 
		virtual int opens(std::string & filename,ulong offset)  ; //  assign value to fd 
		virtual int  read_next() ; // will return the next int 
		virtual bool end_of_stream()  ;
		std::string getFilename(){   // function virtual in base class automatically becomes virtual in child / derived class
			return filename ;
		}
		virtual int closes();
		// DEFINE DESTRUCTOR !!!!
		~IStreamFRead();

};
// uses fwrites 
class OStreamFWrite : public BaseOStream{
	
	private:
		FILE *write_ptr ;
		int  *buf ; 
		ulong b_size ; 
		static const int SIZE = sizeof(int); 
		ulong elements_written ; 
	public :
		// constructors 
	 	OStreamFWrite();
		OStreamFWrite(ulong buffer_size);
		OStreamFWrite(const OStreamFWrite& b);
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
		int *buf;
		ulong elements_read;
		ulong b_size; // This will be  a multiple of pagesize
		bool file_end_flag;
		ulong offset;	//Current offset from which to start reading from file. It is aligned with virtual page size
		//int pagesize; //page size of the operating system  reqd ?? 
		ulong filelength; //length of the file being opened
		std::string filename ;

	public :
		// default constructor 
		IStreamMmap() ; // initalizes page size 
		IStreamMmap(ulong buffer_size) ; // initalizes page size 
		// COPY CONSTRUCTOR 
		IStreamMmap(const IStreamMmap &b);  
		virtual int read_next() ; // will return the next int 
		virtual int opens(std::string & filename)  ; //  assign value to fd 
		virtual int opens(std::string & filename, ulong offset)  ; //  assign value to fd 
		virtual bool end_of_stream() ;
		std::string getFilename(){   // function virtual in base class automatically becomes virtual in child / derived class
			return filename ;
		}
		virtual int closes();
		~IStreamMmap() ; // this does not have to be virtual 

};

// uses mmap with buf of size B 
class OStreamMmap : public BaseOStream{

	private: 
		// add other stuff
		int write_fd ; 
		int *buf ;
		ulong b_size ;  // no if elements the buffer can hold 
		ulong elements_written ;  // no of elements already written
		ulong total_elements;  // no of elements already written
		ulong offset ;  

	public:
		//default construct 
		OStreamMmap(); // creates buffer of size 1 
		OStreamMmap(ulong buffer_size); 		
		OStreamMmap(const OStreamMmap &b); 
		virtual int create(std::string &filename);
		virtual int writes(int n);
		virtual int closes();
		// TODO destructor 
		~OStreamMmap() ;
		
};

#endif
