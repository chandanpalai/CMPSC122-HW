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
	{ new FCFS(), new RoundRobin(), new Priority(), new Preempt(), new SRT() };
   Process *tasks[] = 		    // 4 processes
	{ new Computation(0), new Computation(1), new Computation(2), 
	new Computation(3), new Download(4), 
	new Computation(5), new Computation(6), new Computation(7)};
   int arrival[] = {0, 100, 250, 375, 400, 425, 450, 475};   // arrive at these times


   for (int i = 4; i < 5; i++)
   {
	   scheds[i]->runScheduler(tasks, arrival, 8);
	   displayHistory(tasks, 8, 0, 10000);
	   cout << endl;
   }
   

}

