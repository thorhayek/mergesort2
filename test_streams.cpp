#include"streams.h"
#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>

using namespace std ;
int main(int argc, char *argv[]) {
	//IStreamRead ss;
	//IStreamFRead ss;
	vector<IStreamReadBuf> ss(atoi(argv[1]),IStreamReadBuf(1024));
	//vector<IStreamReadBuf> ss(2,IStreamReadBuf(1024));
	//vector<IStreamMmap> ss(2,IStreamMmap(1024));
	//IStreamMmap ss(4096);
	
	//OStreamWrite sw;
	//OStreamFWrite sw ;
	vector<OStreamWriteBuf> sw(atoi(argv[1]),OStreamWriteBuf(1024));
	//vector<OStreamWriteBuf> sw(2,OStreamWriteBuf(1024));
	//vector<OStreamMmap> sw(2,OStreamMmap(1024));
	//OStreamMmap sw(4096);
	string s = "one.bin";
	sw[0].create(s);
	for(int i=0; i<100000; i++)
	{	sw[0].writes(i);
	}
	sw[0].closes();

	ss[0].opens(s);
	while(!ss[0].end_of_stream()) {
		cout << ss[0].read_next()<< " ";
	}
	if(ss[0].end_of_stream())
		cout << "EOF Reached" <<endl;
	else
		cout << "EOF Not Reached" <<endl;

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
