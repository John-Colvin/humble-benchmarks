#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long double fisher_exact(const long vals[]);

int main(int argc, char* argv[]) {
    // checking num of agruments
    if(argc < 2) {
        printf("Specify number of iterations!\n");
        return 0;
    }
    
    // our contigency table
	const long vals[] = {
		1982, 3018,
		2056, 2944
	};
    
    long double pvalue = 0;
	for(size_t i = 0; i < atoi(argv[1]); i++) {
		pvalue = fisher_exact(vals);
	}
    
    printf("pvalue = %Lf\n", pvalue);
    
	return 0;
}

void logFactorial(const long n, long double* fs) {
	fs[0] = 0;
	for(long i = 1; i < n+1; i++) {
		fs[i] = fs[i-1] + log(i);
	}
}

long double logHypergeometricProbability(const long vals[], const long double* fs) {
	return (
		fs[vals[0] + vals[1]] +
		fs[vals[2] + vals[3]] +
		fs[vals[0] + vals[2]] +
		fs[vals[1] + vals[3]] -
		fs[vals[0]] -
		fs[vals[1]] -
		fs[vals[2]] -
		fs[vals[3]] -
		fs[vals[0] + vals[1] + vals[2] + vals[3]]
	);
}

long double fisher_exact(const long vals[]) {
    // sum all table values
	const long grandTotal = vals[0] + vals[1] + vals[2] + vals[3];
    
    // save factorial values for repeated use in the loop below
	long double* factorials = malloc(sizeof(long double) * (grandTotal + 1));
	logFactorial(grandTotal, factorials);
    
    // calculate our rejection threshold
	const long double pvalThreshold = logHypergeometricProbability(vals, factorials);
	
    long double pvalFraction = 0;
	for(long i = 0; i <= grandTotal; i++) {
		if((vals[0] + vals[1] - i >= 0) && (vals[0] + vals[2] - i >= 0) && (vals[3] - vals[0] + i >=0)) {
			long temp[4] = {
				i,
				vals[0] + vals[1] - i,
				vals[0] + vals[2] - i,
				vals[3] - vals[0] + i
			};

			long double lhgp = logHypergeometricProbability(temp, factorials);

			if(lhgp <= pvalThreshold) {
				pvalFraction += exp(lhgp - pvalThreshold);
			}
		}
	}

    free(factorials);

	return (exp(pvalThreshold) * pvalFraction);
}
