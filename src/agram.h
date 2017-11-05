/*
 * agram.h
 *
 *  Created on: Oct 28, 2017
 *      Author: Sourav
 */

#ifndef AGRAM_H_
#define AGRAM_H_

#include <string>
#include <cstdlib>
#include <cstdio>

namespace ag
{
using namespace std;
struct MapNode;
class Hmap;
class Letter_Invent;
class Stk;
struct Stk_Node;
//Other friend or public functions here
void CondFileScanner(Letter_Invent*, Hmap*);
void AgramSolver();
void BtrackLooper(Hmap*, Letter_Invent, Stk*, int);
void Btrack(Hmap*, Letter_Invent, Stk*, int, int,int);
}

struct ag::MapNode
{
	/* This will act as the key */
	string str1;
	/* This will act as the value */
	Letter_Invent* iv_ptr;
	MapNode(string str):str1{str} { }
};


/* Functions needed for Hmap
 * a function to insert strings into the map and their associated inventories
 * a function to clean up the hmap
 * a function to ensure no duplicate strings are inserted //Not needed as dictionary contains unique words only (no repeat)
 * a function to fetch inventory pointer of a particular node given a string input.
 *
 */
class ag::Hmap
{
private:
	MapNode **arr;
	int size; //Total size of the Hmap
	void DumpDict();
public:
	Hmap();
	~Hmap();
	void insert(string,Letter_Invent*);
	Letter_Invent* fetch_inven(string);
	friend void ag::CondFileScanner(Letter_Invent*, Hmap*);
	friend void ag::AgramSolver();
	friend void ag::BtrackLooper(Hmap*, Letter_Invent, Stk*, int);
	friend void ag::Btrack(Hmap*, Letter_Invent, Stk*, int, int,int);
};

/* Functions needed for this class:
 * a constructor that inits the array
 * a function that takes string input and populates the array
 * a function that takes a letter inventory object input and performs a subtract operation and returns
 * true if that dict object is a feasible one or returns false.
 */
class ag::Letter_Invent
{
private:
	int arr[26];
public:
	Letter_Invent();
	void CreateInvent(string);
	bool subtract(Letter_Invent*);
	friend void ag::CondFileScanner(Letter_Invent*, Hmap*);
	friend void ag::AgramSolver();
	friend void ag::BtrackLooper(Hmap*, Letter_Invent, Stk*, int);
	friend void ag::Btrack(Hmap*, Letter_Invent, Stk*, int, int,int);
protected:
	void ClearInvent();
	void AdjustInvent(string);
	bool isInventoryClean();
};

struct ag::Stk_Node
{
	string str;
	Stk_Node* next;
};


class ag::Stk
{
private:
	Stk_Node* st_ptr;
	int size;
public:
	Stk():st_ptr(NULL),size(0) { }
	void Push(string);
	string Pop(bool);
	friend void ag::CondFileScanner(Letter_Invent*, Hmap*);
	friend void ag::AgramSolver();
	friend void ag::BtrackLooper(Hmap*, Letter_Invent, Stk*, int);
	friend void ag::Btrack(Hmap*, Letter_Invent, Stk*, int, int,int);
};
/* Optimizations are:
 * Only sleect dict words whose subtract return >0
 * Prepare and keep inventory for those dict words before starting the backtrack operation
 *
 * To DO in this assignment:
 * a) take string input and number of words.
 * b) for the string input form a letter inventory;
 * c) read the dictionary file and only store words whose string.subtract operation returns non zero
 * d)
 */
#endif /* AGRAM_H_ */
