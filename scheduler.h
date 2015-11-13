#include <iostream>
using namespace std;

#include "histo.h"

// Process Scheduler
// This represents the part of an operating system that manages processes,
// choosing which to assign the CPU (assuming a single-processor system).
// It will maintain a collection of processes that are currently ready
// to use the CPU, and then choose which to run next.
//
// The time on the system clock is not required for this object,
// so all of the time fields in the linked list objects are set to zero.

class Scheduler
{
    protected:
	ProcList readySet;	// set of processes ready to run
	ProcList future;	// list of future events
	int clock;		// current clock time for simulation
	string name;		// name of the scheduling algorithm
    public:
	virtual void addProcess( int procId )
	{
	    readySet.pushBack( procId, 0, 'X');
	}
	virtual void chooseProcess( int &procId )
	{
	    char hold;
	    readySet.popFront( procId, hold );
	}
	virtual int allowance()	
	{
	    return 100000;	
	}
	virtual bool noneReady()
	{
	    return readySet.empty();
	}
    virtual void runScheduler( Process* [], int[], int );
};

class FCFS : public Scheduler  
{
    public:
	FCFS() { name="First Come First Served"; }
};

class RoundRobin : public Scheduler
{
    public:
	RoundRobin() { name="Round Robin"; }
	int allowance()
	{
	    return 70;
	}   
};

class Priority : public Scheduler
{
    public:
	Priority() { name="Priority"; }
	void addProcess( int procId )
	{
		int insertionTime = -(procId); //negates the process ID so that it will be inserted with the larges procId as the highest priority
		readySet.insert(procId, insertionTime, 'X');  
	}
};

class Preempt : public Priority
{
    public:
	Preempt() { name="Preemptive Priority"; }
	
	int allowance()
	{
		if (future.empty()) //nothing of higher priority will be added to readySet
			return 100000;
		else if (readySet.empty() && (future.leadTime() - clock < 100000)) //a process will be available before current execution is finished
			return future.leadTime() - clock;
		else if (readySet.empty())
			return 100000;
		else if ((future.begin().process() > readySet.begin().process()) && (future.leadTime() - clock < 100000)) //a process of higher priority than the next item in readySet will be available before current execution is finished
			return future.leadTime() - clock;
		else //default allowance of Priority scheduler
			return 100000;
	}
	
};

/*
Schedules processes so that the process requiring the shortest amount of time always takes priority over slower processes
	-Shorter processes get scheduled first and can take CPU away from process with more time remaining than new process needs
Restrictions:
	-No linked lists or tree structures (assume that no more than 20 processes will be handled at a time
	-Needs to be better than O(n) and O(1) with respect to a constant <= number of processes
*/
class SRT :public Scheduler
{
	//create new container, algorithm, and overwrite previous methods so that runScheduler() still works
	private:
		Process **procs; //this scheduler's way of getting process info
		Process *readySet[20];	// set of processes ready to run
		int last_insertion = -1; //index of the most recently inserted value
		Process *temp_storage; //for swapping
		int final_loc; //ending location of a value
		int tracking_index; //index of that "follows" the final_loc index as the process stored at final_loc moves towards its ending location
	public:
		SRT() { name = "SRT"; }

		//grabs the process information, and then runs the simulation
		void runScheduler(Process* tasks[], int arrival[], int size)
		{
			procs = tasks;
			readySet[0] = NULL;
			Scheduler::runScheduler(tasks, arrival, size);
		}
		void addProcess(int procId)
		{
			last_insertion++;
			readySet[last_insertion] = procs[procId];
			final_loc = last_insertion;
			tracking_index = last_insertion;
			if (last_insertion > 0) //no moving necessary if process is inserted at the root
			{
				//moves child to parent as long as child is not root and parent has more remaining time than child
				while (final_loc != 0 && (readySet[((final_loc - 1) / 2)]->getRemainingTime() > readySet[final_loc]->getRemainingTime()))
				{
					final_loc = ((final_loc - 1) / 2);
					temp_storage = readySet[final_loc];
					readySet[final_loc] = readySet[tracking_index];
					readySet[tracking_index] = temp_storage;
					tracking_index = ((tracking_index - 1) / 2);
				}
			}

		}
		void chooseProcess(int &procId)
		{
			procId = readySet[0]->getId();
			if (last_insertion > 0) //otherwise there is nothing in the array to move
			{
				final_loc = 0;
				readySet[0] = readySet[last_insertion]; //most recent insertion moved to root
				//runs until there are no more children to swap with or until the parent has less remaining time than both children
				while (last_insertion >= (2 * final_loc) + 2 && final_loc <= 8 && (readySet[final_loc]->getRemainingTime() > readySet[(2 * final_loc) + 1]->getRemainingTime() || readySet[final_loc]->getRemainingTime() > readySet[(2 * final_loc) + 2]->getRemainingTime()))
				{
					if (readySet[final_loc]->getRemainingTime() > readySet[(2 * final_loc) + 1]->getRemainingTime() && readySet[(2 * final_loc) + 1]->getRemainingTime() < readySet[(2 * final_loc) + 2]->getRemainingTime())
					{
						tracking_index = final_loc;
						final_loc = (final_loc * 2) + 1;
						temp_storage = readySet[tracking_index];
						readySet[tracking_index] = readySet[final_loc];
						readySet[final_loc] = temp_storage;
					}
					else if (readySet[final_loc]->getRemainingTime() > readySet[(2 * final_loc) + 2]->getRemainingTime() && readySet[(2 * final_loc) + 1]->getRemainingTime() > readySet[(2 * final_loc) + 2]->getRemainingTime())
					{
						tracking_index = final_loc;
						final_loc = (final_loc * 2) + 2;
						temp_storage = readySet[tracking_index];
						readySet[tracking_index] = readySet[final_loc];
						readySet[final_loc] = temp_storage;
					}
				}
				if (final_loc == 9 && last_insertion >= 19)
				{
					if (readySet[final_loc]->getRemainingTime() > readySet[(2 * final_loc) + 1]->getRemainingTime() && readySet[(2 * final_loc) + 1]->getRemainingTime() < readySet[(2 * final_loc) + 2]->getRemainingTime())
					{
						tracking_index = final_loc;
						final_loc = (final_loc * 2) + 1;
						temp_storage = readySet[tracking_index];
						readySet[tracking_index] = readySet[final_loc];
						readySet[final_loc] = temp_storage;
					}
				}
			}
			readySet[last_insertion] = NULL; //most recent insertion removed
			last_insertion--;	
		}
		int allowance()
		{
			if (future.empty()) //nothing of higher priority will be added to readySet
				return 100000;
			else if (noneReady() && (future.leadTime() - clock < 100000)) //a process will be available before current execution is finished
				return future.leadTime() - clock;
			else if (noneReady())
				return 100000;
			else if ((procs[future.begin().process()]->getRemainingTime() < readySet[0]->getRemainingTime()) && (future.leadTime() - clock < 100000)) //a process of higher priority than the next item in readySet will be available before current execution is finished
				return future.leadTime() - clock;
			else //default allowance of Priority scheduler
				return 100000;
		}
		bool noneReady()
		{
			return readySet[0] == NULL;
		}
		//void runScheduler(Process*[], int[], int);
};
