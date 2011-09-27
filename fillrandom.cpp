#include"streams.h"
#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<sstream>
#include<sys/types.h>

using namespace std ;
int main(int argc, char *argv[]) {

	if(argc != 3) {
		cout<<"Need 2 arguments: num_elements outfilename"<<endl;
		return -1;
	}
	ulong num_elements = atol(argv[1]);
	string sname = argv[2];
	vector<OStreamWrite> sw(1,OStreamWrite());
	//vector<IStreamRead> ss(1,IStreamRead());

	srand(time(NULL));

	if(sw[0].create(sname) == -1) {
		cout<<"Could not create and open file:"<<sname.c_str()<<endl;
		return -1;
	}
	for(ulong i=0; i<num_elements; i++){
		sw[0].writes((int)(rand()%num_elements));
	}

	sw[0].closes();

	//ss[0].opens(sname);
	//while(!ss[0].end_of_stream()) {
	//	cout<<ss[0].read_next()<<" ";
	//}
	//ss[0].closes();

	return 0;
}
