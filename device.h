// Device Driver Simulation
// A simplified version of the behavior of a device driver.
// It is notified of when a request arrives, and then it
// determines when that action begins and ends.

#include "proclist.h"

class Process;
class Device
{
    protected:
	ProcList requests;
	char action;		// letter used for display
	int duration;		// how long the operation might take
	int readyTime;		// when next operation may start
    public:
	Device( char a, int d ) : action(a), duration(d), readyTime(0) { }
	void restart()
	{
	    readyTime = 0;
	}
        void request( int, int, Process* [], ProcList & );
};

class Console : public Device
{
	public:
		Console(char a, int d) : Device(a, d) {}

		/*
		Simulates a user actively launching new processes
		Accounts for the fact that a user may have multiple computations or downloads, but an interaction with the console
			will likely discourage the user from launching other processes, and it's unlikely to have more than one interactive
			process going on at once
		Also accounts for the fact that arrival times are more likely to be separated on the order of seconds than the previously
			assumed milliseconds
		This will handle at least 4 processes being active at the same time
		Pre: no processes are currently active
		Post: all "desired" processes have at least arrived at the scheduler
		*/
		void simulateUser() 
		{

		}
};

extern Device disk, net, cpu;
extern Console console;
