#include <iostream>
using namespace std;

// List of Process ProcList Information
// This is a simple linked list, all of whose elements record
// information about a process in a simulated process scheduler.
// This list data structure will be used for three different purposes:
// -- a record of the past history of a particular process
// -- a collection for the job scheduler of processes currently wishing to run
// -- a task list for known future events for the simulation

#include "proclist.h"

// First, some helper functions to display the data

ostream &operator<<( ostream &stream, ProcListElement &ele )
{
   stream << "(" << ele.procID << "," << ele.state << "," 
	  << ele.time << ")";
   return stream;
}

ostream &operator<<( ostream &stream, ProcList &hist )
{
   for (ProcListElement *curr = hist.head; curr != NULL; curr = curr->next )
	stream << *curr;
   stream << endl;
   return stream;
}

// And some functionality for the list itself.
// First some easy iterator definitions:
ProcIterator ProcList::begin()
{
    return ProcIterator( this, head );
}

ProcIterator ProcList::end()
{
    return ProcIterator( this, NULL );
}

//  push a new element to the back end of the list
//  Primarily used for First-In, First-Out ordering
void ProcList::pushBack( int procId, int time, char state )
{
   ProcListElement *newEle = new ProcListElement( procId, time, state );
   if (tail == NULL)
   {
	   head = tail = newEle;
   }
   else
   {
	   newEle->prev = tail;
	   tail->next = newEle;
	   tail = newEle;
   }

   

}

//  remove the element at the front end of the list
//  Two reference parameters are provided to hold onto information
//  from the removed element.  The time index is considered unnecessary.
void ProcList::popFront( int &procId, char &state )
{
	if (!empty())
	{
		procId = head->procID;
		state = head->state;
		if (head->next != NULL)
		{
			head->next->prev = NULL;
			head = head->next;
		}
		else
		{
			head = tail = NULL;
		}
	}
	
}

//  adds a new element into a sorted linked list
//  which is sorted in increasing order according to the 'time' argument
void ProcList::insert(int procId, int time, char state)
{
	ProcListElement *newEle = new ProcListElement(procId, time, state);
	ProcListElement *currentEle;

	if (empty())
	{
		head = tail = newEle;
	}
	else
	{
		currentEle = head;
		if (head->time >= time)
		{
			newEle->next = head;
			head->prev = newEle;
			head = newEle;
		}
		else
		{
			while (currentEle->time <= time && currentEle->next != NULL)
			{
				currentEle = currentEle->next;
			}
			if (currentEle->time > time)
			{
				currentEle->prev->next = newEle;
				newEle->prev = currentEle->prev;
				newEle->next = currentEle;
				currentEle->prev = newEle;

			}
			else if(currentEle->next == NULL)
			{
				currentEle->next = newEle;
				newEle->prev = currentEle;
				tail = newEle;
			}
			 
		}
	}

	
	
	
}

// EXTRA CREDIT
// Remove the list element referred to by a particular iterator,
// returning an iterator that refers to the successor to the 
// removed element.
ProcIterator ProcIterator::erase()
{
	return *this;
}
