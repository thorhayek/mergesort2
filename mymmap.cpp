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

using namespace std;
int main()
{

int write_fd = open("one_mmap.bin",  O_RDWR|O_CREAT|O_TRUNC,S_IRWXG|S_IRWXU) ;

if (write_fd < 0) {
	cout << "could not open file" << errno; //predefined macro in errno.h
}
int buf_size = getpagesize();
int ret_code;
int *buf;

ret_code = lseek(write_fd,buf_size-1,SEEK_END);
if(ret_code == -1) {
	cout << "Error while lseeking" << endl;
	return -1;
}
write(write_fd,"",1);

//check the file size
struct stat sbuf;
stat("one_mmap.bin",&sbuf);
int filelength = sbuf.st_size;
cout << "File length after lseek=" << filelength<<endl;

buf = (int *)mmap(0,buf_size,PROT_WRITE,MAP_SHARED,write_fd,0);
if(buf == (int*)-1) {
	cout << "Error could not execute mmap" << endl;
	return -1;
}

int elements_written = 0;

for(int i=0; i<10; i++,elements_written++) {
	buf[i] = i+1;
}
cout << "elements_written="<<elements_written<<endl;

//unmap, truncate the file, close it
ret_code = munmap(buf,buf_size);
if(ret_code == -1) {
	cout << "couldn't munmap"<<endl;
}
ret_code = ftruncate(write_fd,elements_written*sizeof(int));
if(ret_code == -1) {
	cout << "couldn't truncate"<<endl;
}
close(write_fd);

int read_fd = open("one_mmap.bin",  O_RDWR,S_IRWXG|S_IRWXU) ;
 
//check the file size
stat("one_mmap.bin",&sbuf);
filelength = sbuf.st_size;
cout << "File length after truncate=" << filelength<<endl;
int temp_element;
for(int i=0; i<elements_written+10; i++) {
	ret_code = read(read_fd,&temp_element,sizeof(int));
	if(ret_code <4) {
		cout << "Have reached end of file after "<<i<<"reads"<<endl;
		cout << "Element "<<i<<"="<<temp_element<<endl;
		close(read_fd);
		return 0;
	}
	cout << "Element "<<i<<"="<<temp_element<<endl;
}
close(read_fd);

//ifstream ifile("one_mmap.bin",ios::binary|ios::in);


//int temp_fd = open("temp.txt",  O_RDWR|O_TRUNC,S_IRWXG|S_IRWXU) ;
//ret_code = ftruncate(temp_fd,elements_written*sizeof(int));
//if(ret_code == -1) {
//	cout << "couldn't truncate"<<endl;
//}

return 0;
}
