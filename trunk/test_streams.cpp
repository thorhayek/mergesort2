#include"streams.h"
#include<iostream>
#include<string>

int main() {
	using namespace std ;
	IStreamFRead ss ;
	//IStreamRead ss ;
	//OStreamWriteBuf sw ;
	OStreamFWrite sw ;
	string s = "one.bin";
	string s2 = "onew.bin";
	string s3 = "onew.bin";
	ss.opens(s3);
	//sw.create(s3);
	//sw.writes(222);
	cout << ss.read_next()  << endl ;
	
	//cout << ss.read_next()  << endl ;
	  

}
