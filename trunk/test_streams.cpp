#include"streams.h"
#include<iostream>
#include<string>

using namespace std ;
int main() {
	//IStreamRead ss;
	//IStreamFRead ss;
	//IStreamReadBuf ss;
	IStreamMmap ss;
	
	//OStreamWrite sw;
	//OStreamFWrite sw ;
	//OStreamWriteBuf sw ;
	OStreamMmap sw;
	string s = "one.bin";

	sw.create(s);
	sw.writes(10000);
	sw.closes();

	ss.opens(s);
	cout << ss.read_next()  << endl ;
	if(ss.end_of_stream())
		cout << "EOF Reached" <<endl;
	else
		cout << "EOF Not Reached" <<endl;
}
