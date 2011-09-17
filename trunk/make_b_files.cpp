#include<iostream>
#include<fstream>
#include<string>


int main(){

using namespace std ; 
ofstream myfile ; 
myfile.open("one.bin",ios::out | ios::binary);
int a = 35 ; 
int b = 76 ; 
myfile.write((char *)&a,sizeof(int));
myfile.write((char *)&b,sizeof(int));
myfile.close(); 

}
