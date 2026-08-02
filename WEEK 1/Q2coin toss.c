#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TRIALS 100000
#define INTERVAL 100

int main() {

    srand((unsigned int)time(NULL));

    FILE *fp = fopen("results.csv", "w");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    fprintf(fp, "Trials,Fair,Biased\n");

    int fairCount = 0;
    int biasedCount = 0;

    for (int trial = 1; trial <= MAX_TRIALS; trial++) {

        // Fair Coin (P(H) = 0.5)
        if (rand() % 2 == 1)
            fairCount++;

        // Biased Coin (P(H) = 0.7)
        double randomValue = (double)rand() / RAND_MAX;
        if (randomValue < 0.7)
            biasedCount++;

        if (trial % INTERVAL == 0) {

            double fairProbability = (double)fairCount / trial;
            double biasedProbability = (double)biasedCount / trial;

            fprintf(fp, "%d,%.6f,%.6f\n",
                    trial,
                    fairProbability,
                    biasedProbability);
        }
    }

    fclose(fp);

    printf("Simulation completed successfully.\n");
    printf("Results saved in results.csv\n");

    return 0;
}
