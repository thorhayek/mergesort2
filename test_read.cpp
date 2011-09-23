#include<iostream>
#include<cerrno>
#include<cstring>
#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include<stdlib.h>
#include<fcntl.h>
int main(){
using namespace std ; 
int a ; 
int  *b ;
char d ; 
b = new int ;
int write_fd = open("one_mmap.bin", O_RDWR|O_CREAT|O_TRUNC,S_IRWXG|S_IRWXU) ;
//int write_fd = creat("one_mmap.bin",S_IRWXG|S_IRWXU);//user and group have read write exec permisson
//int fd = open("one_mmap_write.bin",O_WRONLY);
if(write_fd < 0){
	cout << "could not open file " << errno;
}
int ret_code ;
char *bufc ;
int buf_size = getpagesize();
printf ("page size is = buffer size =  %d\n ",buf_size);


ret_code = lseek(write_fd,buf_size-1,SEEK_END);
write(write_fd,"",1); // u have to write something here to create file holes !! 
//lseek(write_fd,0,SEEK_SET);
struct stat sbuf;
stat("one_mmap.bin", &sbuf); //TODO check for error in the function call
int filelength = sbuf.st_size;
cout << "new file length " << filelength << "\n" ;
if (ret_code == -1 ){
			// error while lseeking 
			return -1 ; 
}
void *buf  ;
int *bufi ;
printf("before mmap \n");
bufi =(int *) mmap(0, buf_size, PROT_WRITE, MAP_SHARED, write_fd, 0);// should we use MAP_PRIVATE instead
if(bufi == (int *)-1){ // should be int* instead of char * or void * 
	printf("here \n");
	return -1 ;
}
printf("meow \n");
//int *buf = (int *)bufc ;
int element  ;
element = 134 ; 
int elements_written = 0 ;
		// increase offset 
		// use memcpy or sprintf  ??
cout <<"buffer size " <<sizeof(bufi) << endl ;
//memcpy(buf,&element,4); 
bufi[elements_written] = element ;
element++ ;
elements_written++ ;
cout <<"buffer size " <<sizeof(bufi) << endl ;
//return 1 ;
// now read file 

while(elements_written*sizeof(int) != buf_size){ // this will match sometime as pagesize mutiple of int 
	
bufi[elements_written] = element ;
//memcpy( bufi+elements_written*4,&element,4); 
element++ ;
elements_written++ ; 
}
printf ("total number of elements written %d",elements_written);

int ret = munmap(buf,buf_size);
close(write_fd);
int fd = open("one_mmap.bin",O_RDONLY);
int ab ; 
read(fd,&ab,4);
cout << "element read " << ab << endl ;
read(fd,&ab,4);
cout << "element read " << ab << endl ;
/*int num  =  read(fd,&a,4) ;
cout << "num of bytes read " << num << endl  ;  
if ( num < 1 ){
	cout << "error while reading "<< errno ; 
	perror("could not read ");
	//exit(); //EXIT_FAILURE  1 = failure 
}
else {
 cout << a << endl ; 
}
lseek(fd,-4,SEEK_CUR);
num  =  read(fd,&a,4) ;
cout << "num of bytes read " << num << endl  ;  
if ( num < 1 ){
	cout << "error while reading "<< errno ; 
	perror("could not read ");
	//exit(); //EXIT_FAILURE  1 = failure 
}
else {
 cout << a << endl ; 
}
*/
/*int w = 4 ; 
int write_fd = creat("one_write.bin",S_IRWXG|S_IRWXU);
int num_write = write(write_fd,&w,4);
cout << "num written " << num_write << endl ;
*/
}
