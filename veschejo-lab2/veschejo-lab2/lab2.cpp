#include <stdio.h>
#include <signal.h>
#include <x86intrin.h>
#include <sys/time.h>
#include <cstdint>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;

// state of this process
int mynum;
pid_t mypid;
pid_t pid;
string tracefile;
ofstream tracestream;

// function to print message in case of invalid usage
void usage();

// function to trace execution
void trace(string s);


void sigint_handler(int signo)
{
  char c;
  if (signo == SIGINT)
	{
	  cout << "received SIGINT\n";
	  trace("lab2: PID " + to_string(mypid) + " received SIGINT\n");

	  cout << "What do you want to do?\n";
	  cout << "  Enter 'c' to continue execution of this process.\n";
	  cout << "  Enter 'q' to terminate only this process.\n";
	  cout << "  Enter 't' to terminate all processes in round robin order.\n";
	  cin >> c;

	  switch (c) {
          case 'c' :
	    cout << "  user entered c\n";
	    trace("  user entered c\n");
	    return;

	  case 'q' :
		cout << "  user entered q\n";
		trace("  user entered q\n");

		// print a final message
		cout << "Process " + to_string(mynum) + " (PID " + to_string(mypid) + ") is exiting.\n";
		trace("lab2: PID " + to_string(mypid) + " exiting.\n");
		fflush(stdout);
		exit(0);
		break;

	  case 't' :
		cout << "  user entered t\n";
		trace("  user entered t\n");

		// post SIGUSR1 signal against the next process in the ring
		// sleep for one second (use usleep)
		// print a message indicating that process mypid is exiting

		// [enter code here]

		kill(pid, SIGUSR1);		
		usleep(1000000);
		cout << "Process " + to_string(mynum) + " (PID " + to_string(mypid) + ") is exiting.\n";

		trace("lab2: PID " + to_string(mypid) + " exiting.\n");
		fflush(stdout);
		exit(0);
		break;

	  default:
		cout << "  illegal answer, continuing\n";
		return;
	  }
	}
}

void sigusr1_handler(int signo)
{
  if (signo == SIGUSR1)
	{
	  trace("lab2: PID " + to_string(mypid) + " received SIGUSR1\n");
	  cout << "received SIGUSR1\n";

	  // post SIGUSR2 signal against next process in the loop
	  // use usleep() to sleep for 2 seconds
	  // print final message indicating this process is exiting

	  // [enter code here]
	
	  kill(pid, SIGUSR2);
	  usleep(2000000);
	  cout << "Process " + to_string(mynum) + " (PID " + to_string(mypid) + ") is exiting.\n";

	  trace("lab2: PID " + to_string(mypid) + " exiting.\n");
	  fflush(stdout);
	  exit(0);
	}
}

void sigusr2_handler(int signo)
{
  if (signo == SIGUSR2)
	{
	  trace("lab2: PID " + to_string(mypid) + " received SIGUSR2\n");
	  cout << "received SIGUSR2\n";

	  // use usleep() to sleep for 3 seconds
	  // print final message and exit

	  // [enter code here]
		
	  usleep(3000000);
	  cout << "Process " + to_string(mynum) + " (PID " +  to_string(mypid) + ") is exiting.\n";
	   


	  trace("lab2: PID " + to_string(mypid) + " exiting.\n");
	  fflush(stdout);
	  exit(0);
	}
}




// main(int argc, char * argv[])
//   This is the main function for lab2.cpp
//   Usage: lab2 number tracefile
//   Parameters in argv[]:
//     argv[0] - name of the executable: lab2
//     argv[1] - a number: 0, 1, or 2.
//     argv[2] - name of tracefile used by the trace function.

int main(int argc, char * argv[])
{

  // should be two command line parameters, number of pages and probe value
  if(argc != 3)
	{
	  usage();
	  return(-1);
	}

  // convert ascii command line parameters to integers
  mynum = atoi( argv[1] );
  mypid = getpid();
  tracefile = argv[2];

  // open the trace file for writing
  tracestream.open (tracefile, ios::app);
  trace("lab2: process " + to_string(mynum) + " with PID " + to_string(mypid) + " starting. \n");

  // declare structures for catching signals
  struct sigaction sint;
  struct sigaction susr1;
  struct sigaction susr2;

  cout << "I am process " + to_string(mynum) + ".  My PID is " + to_string(mypid) + "\n";

  // ask the user for the PID of the next process in the ring; store it in a (global) variable of type pid_t

  // [enter code here]
  if (mynum == 2)
{
  cout << "Enter the PID of process 0: ";
  cin >> pid;
 }

  else if (mynum == 0 || mynum == 1)
{
	cout << "Enter the PID of process " + to_string(mynum + 1) + ": ";
	cin >> pid;
} 

  // register sigint_handler to catch SIGINT signals

  // [enter code here]
  memset(&sint, 0, sizeof(sint));
  sint.sa_handler = sigint_handler;
  if(sigaction(SIGINT, &sint, NULL) < 0){
	printf("\ncan't carch SIGINT\n");
}
	
  // register sigusr1_handler to catch SIGUSR1 signals

  // [enter code here]
  memset(&susr1, 0, sizeof(susr1));
  susr1.sa_handler = sigusr1_handler;
  if (sigaction(SIGINT, &susr1, NULL) < 0){
  	printf("\ncan't catch SIGINT\n");
}



  // register sigusr2_handler to catch SIGUSR2 signals

  // [enter code here]
  memset(&susr2, 0, sizeof(susr2));
  susr2.sa_handler = sigusr2_handler;
  if (sigaction(SIGINT, &susr2, NULL) < 0){
  	printf("\ncan't catch SIGINT\n");
}
  

  // Loop, sleeping for 1 second at a time
  while(1) {
    cout << "Process " + to_string(mynum) + " (PID " + to_string(mypid) + ") looping happily along...\n";
    usleep(1000000);
  }
  return(0);
}



void usage()
{
  cout << "Usage: lab2 mynum tracefile\n";
  cout << "Where:\n";
  cout << "lab2 - The name of the executable.\n";
  cout << "mynum - 0, 1, or 2.\n";
  cout << "tracefile - file used by the trace function.\n";
}


void trace(string s)
{
  struct timeval t;
  gettimeofday(&t, NULL);
  tracestream << "Time: " << to_string(t.tv_sec % 10000) << "." << to_string((int) t.tv_usec/100) << ": " << s;
  usleep(100000);
  tracestream.flush();
}
