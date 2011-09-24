#include "heap.h"
#include<iostream>


Item::Item(){
	number = 0 ;
	filename = "";

}	
Item::Item(int n,std::string& f){
	number = n ;
	filename = f ; // string is going to call its assignment operator  

}
/*Item::Item(int n,std::string f){
	number = n ;
	filename = f ; // string is going to call its assignment operator  

}*/
void PQ::exch(Item & a,Item & b){


		Item temp ;
		temp = a ; // default assignment operator .
		a = b ;
		b = temp ; 

		
}	
void PQ::fixUp(Item *a, int k){

	
		    while (k > 1 && a[k/2] < a[k])
				      { 
						  exch(a[k], a[k/2]); 
						k = k/2; 
					  }
	
}

void PQ::fixDown(Item *a, int k,int N){

		while (2*k <= N)
			      { int j = 2*k;
				    if (j < N && a[j] < a[j+1]) j++;
					if (!(a[k] < a[j])) break;
					exch(a[k], a[j]); k = j;
				  }


}
bool Item::operator<(Item & a){
			// b<a
	if (number < a.number ){
				return true;
	}	
	return false ;

}
bool Item::operator>(Item &a){

	if (number > a.number ){
				return true;
	}	
	return false ;


}	

