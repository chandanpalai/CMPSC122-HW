// Device Driver Simulation
// A simplified version of the behavior of a device driver.
// It is notified of when a request arrives, and then it
// determines when that action begins and ends.

#include "proclist.h"

class Process;
class Device
{
    private:
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
        void request( int, int, Process* [], ProcList &);
		void request(int, int, Process*[], ProcList &, int[], int &); //overridden method to be called by shell device
};


extern Device disk, net, cpu, console, shell;
