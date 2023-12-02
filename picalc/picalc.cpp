/*
 * C++ program to calculate pi using Machin-like formula using libmpfr-dev and std::thread
 */

#include <stdio.h>
#include <math.h>
#include <thread>
#include <mutex>
#include <mpfr.h>

std::mutex pi_mutex;

unsigned long precision = 100000;

mpfr_t one;
mpfr_t pi;

int nCA[7] = { 183, 32, -68, 12, -100, -12, 12};

const char *pszCB[7] = {"239", "1023", "5832", "113021", "6826318", "33366019650", "43599522992503626068"};

void print_val(mpfr_t *ptVal);

void *calcThread(int i)
{
	mpfr_t x;
	mpfr_t sum;

	// This needs to go first!
	mpfr_set_default_prec(precision);

	mpfr_inits(sum, NULL);
	mpfr_set_ui(sum, 0, MPFR_RNDN);

	mpfr_init_set_str(x, (char *)pszCB[i], 10, MPFR_RNDN); 
	mpfr_atan2(sum, one, x, MPFR_RNDN);

	mpfr_mul_ui(sum, sum, abs(nCA[i]), MPFR_RNDN);

	{
		std::lock_guard<std::mutex> guard(pi_mutex);
		// printf("%d: %d %s ", i+1, nCA[i], pszCB[i]);
		// print_val(&sum);
		if (nCA[i] > 0)
   			mpfr_add(pi, pi, sum, MPFR_RNDN);
		else
  			mpfr_sub(pi, pi, sum, MPFR_RNDN);
	}

	mpfr_clears(sum, x, NULL);

    	mpfr_free_cache();
		
        return NULL;
}

void print_val(mpfr_t *ptVal)
{
	char *pszVal = new char[precision + 100];
    	if(pszVal == NULL) 
	{
        	fprintf(stderr, "Failed to allocated memory!\n");
        	return;
    	}

    	mpfr_sprintf(pszVal, "%.*R*f", precision, MPFR_RNDN, *ptVal);

        for(unsigned long i=0; i<(unsigned long)(precision/3.35)+2; i++) 
	{
            printf("%c", pszVal[i]);
        }

        printf("\n");

	delete pszVal;
}

int main(int argc, char *argv[])
{ 
	std::thread threadArray[7];

 	if (argc == 2)
	{
		sscanf(argv[1], "%lu", &precision);
	}

	precision *= 3.35;

	mpfr_set_default_prec(precision);
	mpfr_inits(one, pi, NULL);
	mpfr_set_si(pi, 0, MPFR_RNDN);
	mpfr_set_si(one, 1, MPFR_RNDN);

	// Start threads to calculate all seven parts
	for(int i=0; i< 7; i++)
	{
		threadArray[i] = std::thread(calcThread,i);
	}


	// Wait for all seven threads to finish
	for(int i=0; i< 7; i++)
	{
		threadArray[i].join();
	}

	// Calculate the final result
	mpfr_mul_ui(pi, pi, 4, MPFR_RNDN);

	// Print the value
	print_val(&pi);

}

	

	



	

