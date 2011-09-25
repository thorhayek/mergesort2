#include"streams.h"
#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>

using namespace std ;
int main(int argc, char *argv[]) {
	//IStreamRead ss;
	//IStreamFRead ss;
	//vector<IStreamReadBuf> ss(atoi(argv[1]),IStreamReadBuf(1024));
	//vector<IStreamReadBuf> ss(2,IStreamReadBuf(1024));
	int pagesize = getpagesize();
	cout << "the pagesize is " << pagesize <<endl ;
	//cout << "1024/pagesize " << 1024/pagesize << endl ;

	//vector<IStreamMmap> ss(2,IStreamMmap(pagesize*2));
	//IStreamMmap ss(4096);
	
	//OStreamWrite sw;
	//OStreamFWrite sw ;
	//vector<OStreamWriteBuf> sw(atoi(argv[1]),OStreamWriteBuf(1024));
	//vector<OStreamWriteBuf> sw(2,OStreamWriteBuf(1024));
	//vector<OStreamMmap> sw(2,OStreamMmap(pagesize*2));i
	//OStreamRead read_stream(pagesize);
	OStreamWriteBuf writebuf_stream(40); // buffer size = 40 
	//OStreamFRead  fread_stream(pagesize);
	//OStreamMmap mmap_stream(pagesize);
	string s = "master.bin";
	writebuf_stream.create(s);	
	// write 40 integers 
	for(int i=0; i < pagesize/2; i++) // 2048 elements written 10 k bytes in file 
	{	
		writebuf_stream.writes(i);

	}
	// NOW read those streams one by one 	

	//IStreamRead read_stream(pagesize);
	//IStreamReadBuf readbuf_stream(pagesize);
	//IStreamFRead  fread_stream(pagesize);
	//IStreamMmap   mmap_stream(pagesize) ;
	
	
	// TEST WITH DIFFERENT OFFSETS 
	//IStreamRead read_stream();
	//read_stream.opens(s,0);
	//readbuf_stream.opens(s,40);
    //fread_stream.opens(s,80);
	//mmap_stream.opens(s,120);

	// WE CAN CREATE AN ARRAY OF pointers of base class  and then use that obect to call different implementations 	
	BaseIStream *base_istream[4] ;
	base_istream[0] = new IStreamRead();
	base_istream[1] = new IStreamReadBuf(pagesize);
	base_istream[2] = new IStreamFRead();
	base_istream[3] = new IStreamMmap(pagesize);
	
	
	int offset = 0; 
	for(int i=0;i < 4; i++){
			// dynmaic polymorphism at work 
			offset = i * 40 ;  // offset = 0  
			if(i == 3){
				base_istream[i]->opens(s,pagesize);
			}
			else{
				base_istream[i]->opens(s,offset);
			}	
			//offset += 40 ; 
	}	
	
	// NOW READ 10 elements each 
	for(int i = 0 ; i < 4 ; i++){
 		 for(int j=0; j < 10 ; j++){
				int temp = base_istream[i]->read_next();
				cout << "element read = " << temp  << " object number "<< i << endl ;
		 }	 	
	}

	for(int i=0;i < 4; i++){
			// dynmaic polymorphism at work 
			//offset = i * 40 ;  // offset = 0  
			int ret_code = base_istream[i]->closes();
			//offset += 40 ; 
	}
	//OStreamMmap sw(4096);
	//string s = "one.bin";
	//sw[0].create(s);
	//for(int i=0; i<100000; i++)
	//{	sw[0].writes(i);
	//}
	//sw[0].closes();

//	ss[0].opens(s);
//	while(!ss[0].end_of_stream()) {
//		cout << ss[0].read_next()<< " ";
//	}
//	if(ss[0].end_of_stream())
//		cout << "EOF Reached" <<endl;
//	else
//		cout << "EOF Not Reached" <<endl;

//	sw.create(s);
//	for(int i=0; i<100000; i++)
//	{	sw.writes(i);
//	}
//	sw.closes();
//
//	ss.opens(s);
//	while(!ss.end_of_stream()) {
//		cout << ss.read_next()<< " ";
//	}
//	if(ss.end_of_stream())
//		cout << "EOF Reached" <<endl;
//	else
//		cout << "EOF Not Reached" <<endl;
}
