#include<string>
#include<iostream>

#ifndef HEAP_H_
#define HEAP_H_

class Item {

	private :
		int number ;
		std::string filename ; 
	public :
		Item();
		Item(int n,std::string& f);
		bool operator<(Item & a);
		bool operator>(Item & a);
		int getNumber(){
						return number ;

		}
		std::string & getFile(){
						return filename ;
		}	

		// should we overlaod[] operator ?? 
		// there is a default assignment operator ..so not redefine 

};	
class PQ{ 

	      private:
			        Item *pq; 
			  		int N; // number of current elements in the array 
					void fixDown(Item a[],int k , int N);
					void fixUp(Item a[], int k );
					void  exch(Item &a,Item &b);
		  public:
					PQ(int maxN){
						 pq = new Item[maxN+1]; 
						 N = 0; // array starts from 1 . 
					}
										int empty() const{
							return N == 0; }
				 	void insert(Item item){
							 	pq[++N] = item;  fixUp(pq, N); }
					 Item& getmax()
					 { 
								exch(pq[1], pq[N]); 
							    fixDown(pq, 1, N-1); 
								return pq[N--]; 
					 }
};
#endif
