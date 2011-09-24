#include<iostream>
#include"heap.h"



int main(){

	using namespace std ;

	string one = "one";
	string two = "two";
    Item a(1,one);
    Item b(2,two);
	Item c ; 
   // check assignment 
	c = a ; 
	if(a < b ){
			cout << " assignment operator worked  a is less than b " << endl ;  
	}	
	if(b > a ){

			cout << " assignment operator worked  b is more than a " << endl ;  
	}

	int size = 50 ;
	PQ  heap(50);
	heap.insert(a);
	heap.insert(b);
	string three = string("fiftysix");
	heap.insert(Item(56,three)); // C++ Concept Anonymous objects CAN ONLY BE PASSED BY VALUE 
	Item d = heap.getmax();
	cout << "the max element out of a and b is " << d.getNumber() << " filename " << d.getFile() << endl ;  
	d = heap.getmax();
	cout << "the max element out of a and b is " << d.getNumber() << " filename " << d.getFile() << endl ;  
	d = heap.getmax();
	cout << "the max element out of a and b is " << d.getNumber() << " filename " << d.getFile() << endl ;  

}	
