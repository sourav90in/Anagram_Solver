

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
		tmp->next->str = str;
		tmp->next->next = NULL;
	}
	this->size++;

}

string Stk::Pop(bool fifo)
{
	string tmp_str = "\0";
	if( this->st_ptr == NULL )
		return tmp_str;
	else
	{
		Stk_Node* tmp = this->st_ptr;
		Stk_Node* prev = this->st_ptr;
		/* Empty the stack in fifo manner for printing in dict order */
		if( fifo == true )
		{
			this->st_ptr = this->st_ptr->next;
			tmp_str = tmp->str;
			delete tmp;
			this->size--;
			if( this->size == 0 )  this->st_ptr = NULL;
			return tmp_str;
		}
		else
		{
			while( tmp->next != NULL )
			{
				prev = tmp;
				tmp = tmp->next;
			}
			prev->next = NULL;
			tmp_str = tmp->str;
			delete tmp;
			this->size--;
			if(this->size == 0 ) this->st_ptr = NULL;
			return tmp_str;
		}
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
	while( count < d_str.length() - 1 )
	{
		ch = tolower(d_str[count]);
		this->arr[ch %97]--;
		count++;
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
	//this->arr = NULL;
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
void ag::CondFileScanner(Letter_Invent* iv_ptr, Hmap* hm_ptr)
{
	string line;
	cout<<"Enter name of the dictionary: ";
	//getline(cin, line);
	line = "dict2.txt\r";
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
			//line = line.substr(0,line.length()-1);  //Remove the /r
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

void Hmap::DumpDict()
{
	int count = 0;
	while( count < this->size )
	{
		cout<<this->arr[count]->str1<<"\n";
		count++;
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
	//str.resize(str.length()-1); //removes the /r at the end.
	return str;
}

void ag::AgramSolver()
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
	//getline(cin,ip_str);
	ip_str = "dad may hem\r";
	ip_str = StringProcesser(ip_str);

	Letter_Invent* iv_ptr = new Letter_Invent;  //To be destoyed
	iv_ptr->CreateInvent(ip_str);

	Letter_Invent mod_iv = *iv_ptr;

	Hmap* hm_ptr = new Hmap();  // To be destroyed
	CondFileScanner(iv_ptr,hm_ptr);

	//Dump the reduced dictionary data to console
	//hm_ptr->DumpDict();
	Stk* stk_ptr = new Stk();  //Destructor not needed as Stack is cleaned up in BtrakLooper

	int max = 5; //To be changed if max needs to be differed

	BtrackLooper(hm_ptr,mod_iv,stk_ptr, max);

	//Destroy
	delete iv_ptr;
	delete hm_ptr;
	delete stk_ptr;

}

Hmap::~Hmap()
{
	int count = 0;
	while( count < this->size )
	{
		delete this->arr[count]->iv_ptr;
		delete this->arr[count];
		count++;
	}
	//delete this->arr;
}

void ag::BtrackLooper(Hmap* hm_ptr, Letter_Invent iv, Stk* stk_ptr, int max)
{
	for(int i=0; i< hm_ptr->size; i++ )
	{
		Btrack(hm_ptr, iv, stk_ptr, max, i, i);
		/* Dummy Cleanup stack for next iter usage
		 * incase any cleanup conditions have been missed */
		string tmp2 = stk_ptr->Pop(false);
		while(tmp2 != "\0")
		{
			tmp2 = stk_ptr->Pop(false);
		}
	}
}

void ag::Btrack(Hmap* hm_ptr, Letter_Invent iv, Stk* stk_ptr, int max, int i, int in_idx)
{
	if( (i == hm_ptr->size ) || (max == 0) )
		return;

	if( iv.subtract(hm_ptr->arr[i]->iv_ptr) == true )
	{
		iv.AdjustInvent(hm_ptr->arr[i]->str1);
		stk_ptr->Push(hm_ptr->arr[i]->str1);
		max--;
		if( ( iv.isInventoryClean() == false ) && (max > 0) )
		{
			for(int j=i+1; j < hm_ptr->size; j++ )
			{
				Btrack(hm_ptr,iv,stk_ptr, max, j, in_idx);
			}
			stk_ptr->Pop(false);

		}
		else if( iv.isInventoryClean() == true )
	    {
			string tmp_str;
			while( ( tmp_str = stk_ptr->Pop(true) ) != "\0" )
			{
				tmp_str.resize(tmp_str.length()-1);
				cout<<tmp_str<<" ";
			}
			cout<<"\n";
			//Push back only the initial starting word
            stk_ptr->Push(hm_ptr->arr[in_idx]->str1);
	    }
		else
		{
			/* Inventory hasnt been cleaned with the set of words used but max limit reached
			 * clean the stack contents
			 */
			string tmp2 = stk_ptr->Pop(false);
			while(tmp2 != "\0")
			{
				tmp2 = stk_ptr->Pop(false);
				//Push back only the initial starting word
				if( tmp2 == hm_ptr->arr[in_idx]->str1 )
				{
					stk_ptr->Push(tmp2);
					break;
				}
			}

		}

	}

}


/* TO DO:
 * Define destructors
 */
