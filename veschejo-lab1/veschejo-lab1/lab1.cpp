//   lab1.cpp
//   Programming Assignment 1.  Memory access time
//   CSE 325, Fall 2022

//   This program uses access to the Time Stamp Counter register to
//   measure the effect of caching on memory access time.

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <x86intrin.h>	//	Linux dev
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
using namespace std;

#define PAGELEN  4096
#define NUMPAGES 64

// function to print message in case of invalid usage
void usage();

// function to trace execution
void trace(string s);

unsigned long long times[NUMPAGES];

ofstream tracestream;

//	main(int argc, char * argv[])
//	This is a 'C' standard format to handle command-line parameters
//		argc -	the number of command-line parameters.  This includes
//				the name of the executable, so argc will always be >= 1.
//		argv -	An array of pointers to 'C' strings, not instances of the 'C++'
//				String class.  This array of pointers is NULL terminated.
int main(int argc, char * argv[]) 
{

    // argc should be 2: executable name and one parameter, a filename
    if(argc != 2)
    {
        usage();
        return(-1);
    }

    int i = 0;                       // counter
    char *bytearray = NULL;          // will point to an array of characters (bytes)
    int p;                           // number of page of the array to be probed
    char x;                          // character used to probe the array
    unsigned long long start, stop;  // start and stop rdtsc "timer"
                                     // NOTE: any variables storing rdtsc-obtained values need to be unsigned long long.

	tracestream.open ("trace.txt", ios::app);

	ifstream input_file(argv[1]);
	if (!input_file.is_open()) {
	  cerr << "ERROR: Could not open the file - '" << argv[1] << "'" << endl;
	  exit(0);
	}

	//allocate space for array
	bytearray = (char*)malloc(NUMPAGES*PAGELEN);
	if (bytearray == NULL)
	{
	  cout << "Memory allocation error!\n";
	  exit(0);
	}
	
	cout << "Input page numbers:\n";

	// in a loop, read one number at a time from the input_file into variable p and check its validity
	// print out the number, p. if it is invalid, print an error message and call exit(0).
	// if it is valid, use it to access the bytearray, specifically, read the first byte of the pth page of the
	// array and store the value in x (remember, each page contains PAGELEN bytes)
	while (input_file >> p) {
	  cout << "Probe page number: " << p << "\n";
          trace("Probe page number: " + to_string(p) + "\n");
	  

	  // chek validity: p must be between 0 and NUMPAGES-1.
	  // if p is invalid, print error and exit
	  // else "probe" the bytearray by reading the first byte of page p into variable x

	  // [ENTER YOUR CODE HERE]
	  
	  if (p < 0 || p > NUMPAGES-1){
		cerr << "ERROR: Invalid number" << endl;
		exit(0);
	}

	 x = bytearray[p*PAGELEN];


	}
	cout << "\n";
	input_file.close();


	//  In a loop, read the first byte of each page of the bytearray into the variable x.
	//  Use __rdtsc() to measure the cycles elapsed for each read operation using the start and
	//  stop variables, and record the number of cycles elapsed in the times[] array.
	//  Note: don't put any I/O in this loop!

	//  [ENTER YOUR CODE HERE]
	for(int j=0;j<NUMPAGES;j++){ 
		start = __rdtsc();
		x=bytearray[j*PAGELEN];
		stop = __rdtsc();
		times[j] = stop - start;
	}

	//  In a loop, for each of the NUMPAGES pages, output a line that includes the page number and the number
        //  of cycles to read from that page (times[i] for page i).
	//  If the value of times[i] is < 200 cycles, print another line stating, "NOTE: FAST ACCESS TIME!"

        // [ENTER YOUR CODE HERE]

	for(;i<NUMPAGES;i++){
		cout << "Page number: " << i << ". Number of cycles to read: " << times[i] << "." << endl;
		
		if (times[i] < 200){
			cout << "NOTE: FAST ACCESS TIME!" << endl;
		}
	}


	// we increment x in order to silence a compiler warning
	x++;

	return(0);
}

void usage()
{
	printf("Usage: lab1 inputfile \n");
	printf("Where:\n");
	printf("lab1 - The name of the executable.\n");
	printf("input - File containing a list of integers 0 <= n <  1048576 (2^20)");
}


void trace(string s)
{
	tracestream << "Trace: " << s;
	usleep(10000);
	tracestream.flush();
}
