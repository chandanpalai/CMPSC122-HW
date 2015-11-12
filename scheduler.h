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
	void chooseProcess( int &procId )
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
        void runScheduler( Process* [], int[], int );
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
class ShortestRem :public Scheduler
{
	//create new container, algorithm, and overwrite previous methods so that runScheduler() still works
	protected:
		Process readySet[20];	// set of processes ready to run
	public:
		ShortestRem() { name = "Shortest Remaining Time"; }
		virtual void addProcess(int procId)
		{
			//readySet.pushBack(procId, 0, 'X');
		}
		void chooseProcess(int &procId)
		{
			//char hold;
			//readySet.popFront(procId, hold);
		}
		//modify this- should function similarly to preemptive priority, but using remaining time
		virtual int allowance()
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
		virtual bool noneReady()
		{
			//return readySet.empty();
		}
		void runScheduler(Process*[], int[], int);
};
