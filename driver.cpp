// A quick driver to test the History Display routine
// It just fabricates a single job history to see what it does
//
#include <iostream>
using namespace std;

#include "scheduler.h"
#include "device.h"

int main()
{
   Scheduler *scheds[] = 	    // make some process schedulers
	{ new FCFS(), new RoundRobin(), new Priority(), new Preempt() };
   Process *tasks[] = 		    // 4 processes
	{ new Interact(0), new Download(1), new Computation(2), new Download(3), new Computation(4), new Computation(5), new Computation(6), new Interact(7)};
   int arrival[] = {0, 100, 250, 375, 400, 430, 490, 500};   // arrive at these times


   for (int i = 0; i < 4; i++)
   {
	   scheds[i]->runScheduler(tasks, arrival, 8);
	   displayHistory(tasks, 8, 0, 7000);
	   cout << endl;
   }
   

}

