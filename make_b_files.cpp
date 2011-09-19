#include<iostream>
#include<fstream>
#include<string>

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

using namespace std ; 
int main(){

//ofstream myfile ; 
//myfile.open("one.bin",ios::out | ios::binary);
//int a = 35 ; 
//int b = 76 ; 
//myfile.write((char *)&a,sizeof(int));
//myfile.write((char *)&b,sizeof(int));
//myfile.close(); 

/* Reading from a binary file */
// ifstream myfile;
// myfile.open("one.bin",ios::binary);
// int a,b,length;
// // get length of file:
// myfile.seekg (0, ios::end);
// length = myfile.tellg();
// myfile.seekg (0, ios::beg);
// 
// cout<<"length of the file="<<length<<endl;
// cout<<"contents:";
// myfile.read((char *)&a,sizeof(int));
// cout<<a<<" ";
// myfile.read((char *)&b,sizeof(int));
// cout<<b<<endl;
// 
// myfile.close();

// struct stat sbuf;
// 
// if (stat("make_b_files.cpp", &sbuf) == -1) {
// 	perror("stat");
// 	exit(1);
// }
// cout << sbuf.st_size << endl;
//

//get pagesize of the operating system
int pagesize = getpagesize();
cout << "pagesize="<<pagesize<<endl;

}
