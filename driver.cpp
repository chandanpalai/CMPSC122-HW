// A quick driver to test the History Display routine
// It just fabricates a single job history to see what it does
//
#include <iostream>
using namespace std;

#include "scheduler.h"
#include "device.h"

int main()
{
	/*
   Scheduler *scheds[] = 	    // make some process schedulers
	{ new FCFS(), new RoundRobin(), new Priority(), new Preempt(), new SRT() };
   Process *tasks[] = 		    // 4 processes
   { new Computation(0), new Interact(1), new Shell(2), new Computation(3)};
   int arrival[] = {0, 100, 250, 375};   // arrive at these times


   for (int i = 0; i < 5; i++)
   {
	   scheds[i]->runScheduler(tasks, arrival, 4);
	   displayHistory(tasks, 4, 0, 5000);
	   cout << endl;
   }
   */
	
	int size = 1;
	Scheduler *scheds = new RoundRobin();
	Process *tasks[20];
	tasks[0] = new Shell(0);
	int arrival[20];
	arrival[0] = 0;
	scheds->runScheduler(tasks, arrival, size);
	displayHistory(tasks, size, 0, 10000);
	cout << endl;
}

