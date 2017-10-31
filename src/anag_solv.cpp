

#include "agram.h"
#include <iostream>
#include <fstream>
using namespace ag;

void Stk::Push(string str)
{
	Stk_Node* tmp = this->st_ptr;
	if(tmp == NULL)
	{
		this->st_ptr = new Stk_Node;
		this->st_ptr->str = str;
		this->st_ptr->next = NULL;
	}
	else
	{
		while( tmp->next != NULL )
		{
			tmp = tmp->next;
		}

		tmp->next = new Stk_Node;
		tmp->str = str;
		tmp->next->next = NULL;
	}

}

string Stk::Pop()
{
	string tmp_str = "\0";
	if( this->st_ptr == NULL )
		return tmp_str;
	else
	{
		Stk_Node* tmp = this->st_ptr;
		Stk_Node* prev;
		while(tmp->next != NULL )
		{
			prev = tmp;
			tmp = tmp->next;
		}
		tmp_str = tmp->str;
		prev->next = NULL;
		delete tmp;
		return tmp_str;
	}
}


bool Letter_Invent::subtract(Letter_Invent* i_ptr)
{
	int count = 0;
	while(count < 26)
	{
		if( ( this->arr[count] - i_ptr->arr[count] < 0 ) )
			return false;
		count++;
	}
	return true;

}


void Letter_Invent::ClearInvent()
{
	int count = 0;
	while(count < 26 )
	{
		this->arr[count] = 0 ;
		count++;
	}
}

Letter_Invent::Letter_Invent()
{
	ClearInvent();
}


void Letter_Invent::CreateInvent(string str)
{
	int count = 0;
	char ch;
	while( str[count] != '\r')
	{
		ch = tolower(str[count]);
		this->arr[(ch % 97)]++;
		count++;

	}
}

void Letter_Invent::AdjustInvent(string d_str)
{
	int count =0;
	char ch;
	while(count < d_str.length())
	{
		ch = tolower(d_str[count]);
		this->arr[ch %97]--;
		count++
	}

}

bool Letter_Invent::isInventoryClean()
{
	for(int i=0; i< 26; i++)
	{
		if( this->arr[i] != 0 )
		{
			return false;
		}
	}
	return true;
}

Hmap::Hmap()
{
	this->size = 0;
	this->arr = new MapNode*;
}

void Hmap::insert(string str,Letter_Invent* v_ptr)
{

	this->arr[this->size] = new MapNode(str);
	this->arr[this->size]->iv_ptr = v_ptr;
	this->size++;

}

/* This function needs to be called in a friend function
 * as it returns a private member*/
Letter_Invent* Hmap::fetch_inven(string str)
{
	int count = 0;
	while(count < this->size )
	{
		if( this->arr[count]->str1 == str )
		{
			return this->arr[count]->iv_ptr;
		}
	}

	return NULL;
}


/* function to read the dictionary conditionallly given the input strings
 * Objects alphabet inentory
 *
 */
void CondFileScanner(Letter_Invent* iv_ptr, Hmap* hm_ptr)
{
	string line;
	cout<<"Enter name of the dictionary: ";
	getline(cin, line);
	line = line.substr(0,line.length()-1);
	ifstream fhl( line);
	if( fhl.is_open() )
	{
		/* Read every word from dict and:
		 * a) Check if the dictionary word is a valid candidate by calc its inventory and doing subtract
		 * b) If valid, store its iv ptr and string in HMAP
		 * c) else ignore and read next string.
		 */
		Letter_Invent* div_ptr;
		while( getline(fhl, line) )
		{
			line = line.substr(0,line.length()-1);  //Remove the /r
			div_ptr = new Letter_Invent;
			div_ptr->ClearInvent();   //This means this function has to be a friend now :}
			div_ptr->CreateInvent(line);
			if( iv_ptr->subtract(div_ptr) == true )
			{
				hm_ptr->insert(line, div_ptr);
			}
			else delete div_ptr;

		}
	}
	else
	{
		throw runtime_error{"Cannot open the file"};
	}
}

string StringProcesser(string str)
{
	int count = 0;
	int ict = 0;
	while(count < str.length())
	{
		if(str[count] == ' ')
		{
			//From count to size() -1 copy all chars to one pos before
			for(ict=count+1; ict < str.length(); ict++)
			{
				str[ict-1] = str[ict];
			}
			str.resize(str.length()-1);
		}
		count++;
	}
	str.resize(str.length()-1); //removes the /r at the end.
	return str;
}

/* Other functions needed:
 * a) BAcktrack handler func
 * b) Main processing function that handles main code.
 */
void AgramSolver()
{
	/* Take input string- process it remove spaces and /r at the end
	 * Create its inventory
	 * Creat hashmap
	 * call condfilescanner which creates the dict for storage
	 * create the stack
	 * then call the backtrack function which prints each
	 */

	string ip_str;
	cout<<"Enter the string whose anagrams you need:";
	getline(cin,ip_str);
	ip_str = StringProcesser(ip_str);

	Letter_Invent* iv_ptr = new Letter_Invent;
	iv_ptr->CreateInvent(ip_str);

	Letter_Invent mod_iv = *iv_ptr;

	Hmap* hm_ptr = new Hmap();
	CondFileScanner(iv_ptr,hm_ptr);

	Stk* stk_ptr = new Stk;

	int max = 2; //To be changed if max needs to be differed

	Btrack(hm_ptr,mod_iv,stk_ptr, &max);

	while(stk_ptr->Pop() != "\0" )
	{
		cout<<stk_ptr->Pop();
	}

}

/* TO DO:
 * Define destructors
 * Correct brack below
 * define friend funcs correctly.
 */

/* This function below needs correction*/
void Btrack(Hmap* hm_ptr, Letter_Invent iv, Stk* stk_ptr, int* max )
{
	static int hm_count = 0;
	Letter_Invent iv_cp = iv;
	while ( (hm_count < hm_ptr->size) && ( max > 0 ) )
	{
		stk_ptr->Push(hm_ptr->arr[hm_count]->str1);
		iv.AdjustInvent(hm_ptr->arr[hm_count]->str1);
		hm_count++;
		*max = *max -1;
		if( iv.isInventoryClean() == false )
		{
			Btrack(hm_ptr,iv,stk_ptr,max);
		}


	}
}
