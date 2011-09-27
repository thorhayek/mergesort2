#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include<sstream>
#include <sys/types.h>
#include "streams.h"

using namespace std;

int *a,*b;
void merge(int,int,int);
void merge_sort(int low,int high)
{
 int mid;
 if(low<high)
 {
  mid=(low+high)/2;
  merge_sort(low,mid);
  merge_sort(mid+1,high);
  merge(low,mid,high);
 }
}
void merge(int low,int mid,int high)
{
 int h,i,j,k;
 //int *b = new int[(high-low+1)];

 h=low;
 i=low;
 j=mid+1;

 while((h<=mid)&&(j<=high)) {
	if(a[h]<=a[j]) {
   		b[i]=a[h];
   		h++;
  	}
  	else {
   		b[i]=a[j];
   		j++;
  	}
  	i++;
 }
 if(h>mid) {
  	for(k=j;k<=high;k++) {
   		b[i]=a[k];
   		i++;
  	}
 }
 else {
  for(k=h;k<=mid;k++) {
   	b[i]=a[k];
   	i++;
  }
 }

 for(k=low;k<=high;k++) 
	 a[k]=b[k];
 
}

int main(int argc, char *argv[]) {
	
	if(argc != 3) {
		cout<<"Need 2 arguments: num_elements filename"<<endl;
		return -1;
	}

	ulong num_elements = atol(argv[1]);
	string sname=argv[2];

	a = new int [num_elements];
	b = new int [num_elements];
	vector<IStreamRead> ss(1,IStreamRead());
	vector<OStreamWrite> sw(1,OStreamWrite());

	ss[0].opens(sname);
	ulong i=0;
	for(; !ss[0].end_of_stream() && i<num_elements; i++) {
		a[i] = ss[0].read_next();
	}

	ss[0].closes();

	if(i!=num_elements) {
		cout<<"Less number of elements in the file"<<endl;
		return -1;
	}
	
	merge_sort(0,num_elements-1);

	//for(i=0; i<num_elements; i++) {
	//	std::cout<<a[i]<<" ";
	//}

	sname.append(".out");
	if(sw[0].create(sname)== -1) {
		cout<<"Could not create file for writing"<<endl;
		return -1;
	}
	
	for(i=0; i<num_elements; i++) {
		sw[0].writes(a[i]);
	}
	sw[0].closes();

	delete[] a;
	delete[] b;

	return 0;
}
