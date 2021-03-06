/**********************************************************************************
* File: montecarlo.c
* Author: Anuja Sahu
* Procedures:
* LRU - Least recently used replacement algorithm. Built using an array.
*
* FIFO - First in first out replacement algorithm. Built using an array.
*
* Clock - Walking pointer algorithm. Built using arrays.
*
* randNum - Function for randomly generating a number from a normal distribution.
* Built using Box-Muller strategy.
*
* main - Test program which interatively generates one-thousand page number
* traces for each virtual memory algorithm (LRU, FIFO and Clock). Tests each
* algorithm for working set sizes from four to twenty.
**********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

int LRU(int wss, int data[]);						// Function declarations for the three virtual memory replacement algorithms
int FIFO(int wss, int data[]);
int Clock(int wss, int data[]);

int randNum(int mu, int sigma);						// Function declaration for random normal number generator

/**********************************************************************************
* int main(int argc, char *argv[])
* Author: Anuja Sahu
* Date: 16 November 2021
* Description: Run Monte Carlo simulation on virtual memory replacement algorithms
* (LRU, FIFO, and Clock). Iteratively generates one-thousand page number traces
* (experiments). Tests each algorithm for working set sizes from four to twenty.
*
* Parameters:
* argc		I/P	int	The number of arguments on the command line
* argv		I/P	char*[] The arguments on the command line
* main 		O/P	int	Status code (not currently used)
**********************************************************************************/
int main(int argc, char *argv[])
{
	int LRUResults[21];								// Initialize result array sizes for each algorithm
	int FIFOResults[21];
	int ClockResults[21];
	int data[1000];									// Initialize data array size

	for(int k = 0; k < 21; k++)							// Initialize each value of each array to 0
	{
		LRUResults[k] = 0;
		FIFOResults[k] = 0;
		ClockResults[k] = 0;
	}

	for (int i = 0; i < 1000; i++) 							// Experiments loop
	{
		for(int j = 0;j < 1000; j++) 						// Trace loop
		{
			data[j] = ( 10 * ((int) ( j / 100 )) ) + randNum(10, 2);	// Generate a random number from a normal distribution
											// with a mean of ten and a standard deviation of two.
											// There are ten regions which have their own base address.
		}
											// Determine and accumulate the number of page
											// faults for each algorithm base on the current
											// working set size and the current trace.
		for(int wss = 4; wss <= 20; wss++)
		{
			LRUResults  [wss] += LRU(wss, data);
			FIFOResults [wss] += FIFO(wss, data);
			ClockResults[wss] += Clock(wss, data );
		}
	}

	for(int wss = 4;wss <= 20;wss++)
	{
		printf("%d LRU: %d\n", wss, LRUResults[wss]/1000);			// Output statistics
		printf("%d FIFO: %d\n", wss, FIFOResults[wss]/1000);
		printf("%d Clock: %d\n\n", wss, ClockResults[wss]/1000);
	}

	return 0;									// End program
}

/*********************************************************************************
* int randNum(int mu, int sigma)
* Author: Phoxis, edited by Anuja Sahu
* Date: 16 November 2021
* Description: Function that generates a random number from a normal distribution
* with a given mean and standard deviation.
*
* Parameters:
* mu		I/P	 int	Average (mean) of normal distribution
* sigma		I/P	 int	Standard deviation of normal distribution
* X1		O/P	 int	Variable that helps return random normal number
* X2		O/P      int	Variable that helps return random normal number
*********************************************************************************/
int randNum(int mu, int sigma)
{
	double U1, U2, W, mult;
  	static double X1, X2;
  	static int call = 0;

	if (call == 1)
    	{
     		call = !call;
		return (int)((mu + sigma * (double) X2));
    	}

	do
	{
		U1 = -1 + ((double)rand () / RAND_MAX) * 2;				// Generate a random uniform number
      		U2 = -1 + ((double)rand () / RAND_MAX) * 2;
      		W = pow (U1, 2) + pow (U2, 2);
    	}
	while (W >= 1 || W == 0);							// Continue generation until condiitons are met

	mult = sqrt((-2 * log(W)) / W);							// Make number normal
	X1 = U1 * mult;
	X2 = U2 * mult;

	call = !call;

	return (int)((mu + sigma * (double) X1));					// Return number made normal based on given mean and standard deviation
}

/**********************************************************************************************
* int LRU(int wss, int data[])
* Author: Anuja Sahu
* Date: 16 November 2021
* Description: Least recently used replacement algorithm. Built using an array
* that helps track which frames were used.
*
* Parameters
* wss		I/P	int 	Number of frames that the algorithm is using
* data		I/P	int[]	The 1000 elements generated by the main function
* pageFault 	O/P 	int 	Returns number of times algorithm generates a page fault
**********************************************************************************************/
int LRU(int wss, int data[])
{
	int lru[wss], temp[wss];
	int pointer = 0, pageFault = 0;
	bool found = false;
	int indexFound = 0;

	for(int h = 0; h < wss; h++)
	{
		lru[h] = -1;								// Initialize working set array
	}

	for(int j = 0; j < 1000; j++)
	{
		if(lru[pointer] == -1)							// Update pointer for each un-used frame
		{
			lru[pointer] = data[j];
			pointer = (pointer + 1) % wss;
		}
		else
		{
			for(pointer = 0; pointer < wss; pointer++)			// If matching page found, flag and record index
			{
				if(lru[pointer] == data[j])
				{
					found = true;
					indexFound = pointer;
				}
			}

			if(found)
			{
				for(int e = 0; e < indexFound; e++)
				{
					temp[e] = lru[e];				// Move previous LRU pages into temp array
				}

				temp[wss-1] = lru[indexFound];				// Move found page to end of array

				for(int p = indexFound; p < wss - 1; p++)
				{
					temp[p] = lru[p+1];				// Move all elements forward in array
				}

				for(int y = 0; y < wss; y++)
				{
					lru[y] = temp[y];				// Move elements back to original array from temp array
				}
			}
			else
			{
				for(int t = 0; t < wss-1; t++)
				{
					temp[t] = lru[t+1];				// Move everything forward
				}

				temp[wss-1] = data[j];					// Replace end with new page

				for(int q = 0; q < wss; q++)
				{
					lru[q] = temp[q];				// Move elements back to original array from temp array
				}
				pageFault++;						// Increment page fault counter
			}
		found = false;								// Reset flag for found page faults
		}
	}

	return pageFault;
}


/**********************************************************************************************
* int FIFO(int wss, int data[])
* Author: Anuja Sahu
* Date: 16 November 2021
* Description: First in first out replacement algorithm. Built using an array that keeps track
* of page frames used.
*
* Parameters
* wss		I/P	int 	Number of frames that the algorithm is using
* data		I/P	int[]	The 1000 elements generated by the main function
* pageFault 	O/P 	int 	Returns number of times algorithm generates a page fault
**********************************************************************************************/
int FIFO(int wss, int data[])
{
	int fifoArray[wss];								// Initialize working set array
	int pointer = 0, pageFault = 0;

	for(int y = 0; y < wss; y++)
	{
		fifoArray[y] = -1;							// Update frame for each un-used frame
	}

	for(int j = 0; j < 1000; j++)
	{
		if(fifoArray[pointer] == -1)
		{
			fifoArray[pointer] = data[j];					// If frame is unused, update pointer
			pointer = (pointer + 1) % wss;
		}

		else if(fifoArray[pointer] != data[j])
		{
			fifoArray[pointer] = data[j];					// If not matching page, replace with new page and update pointer to replace
			pointer = (pointer + 1) % wss;
			pageFault++;							// Increment page fault count
		}
	}

	return pageFault;
}

/**********************************************************************************************
* int Clock(int wss, int data[])
* Author: Anuja Sahu
* Date: 17 November 2021
* Description: Walking pointer clock algorithm. Built using two arrays, where second array
* stores information on useBit.
*
* Parameters
* wss		I/P	int 	Number of frames that algorithm is using
* data		I/P	int[]	The 1000 elements generated by the main function
* pageFault 	O/P 	int 	Returns number of times algorithm generates a page fault
**********************************************************************************************/
int Clock(int wss, int data[])
{
	int clock[wss], useBit[wss], pointer = 0;
	int pageFault = 0;

	for(int u = 0; u < wss; u++)
	{
		clock[u] = -1;								// Initialize both arrays
		useBit[u] = 0;
	}

	for(int j = 0; j < 1000; j++)							// For each element in data array, clock around working set
	{
   		for(int c = 0; c < wss; c++)
		{
			if(clock[pointer] ==  -1)					// If array has no pointers
			{
				clock[pointer] = data[j];				// Initialize elements in array with elements of data array
				useBit[pointer] = 1;					// Set use bit
				pointer = (pointer + 1) % wss;				// Incremenent pointer so it circles around
				break;
			}
			else if(clock[pointer] == data[j])				// If pages match
			{
				useBit[pointer] = 1;					// Set use bit to 1
				break;
			}
			else if(useBit[pointer] == 1)					// If pages do not match
			{
				useBit[pointer] = 0;					// Set use bit of 0
				pointer = (pointer + 1) % wss;				// Increment pointer so clock continues looping
			}
			else								// Usebit found = 0, pages do not match
			{
				clock[pointer] = data[j];				// Replace page
				useBit[pointer] = 1;					// Update use bit
				pointer = (pointer + 1) % wss;				// Increment pointer
				pageFault++;						// Increment page fault count
				break;
			}
    		}
	}

	return pageFault;
}
