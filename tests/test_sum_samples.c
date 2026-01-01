#include "peakdetect.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define SAMPLE_FILE_PATH_1 "../../tests/readings/50khz50perc1p1.txt"
#define SAMPLE_FILE_PATH_2 "../../tests/readings/50khz50perc1p1.txt"
#define SAMPLE_FILE_PATH_3 "../../tests/readings/50khz50perc1p1.txt"
#define SAMPLE_FILE_PATH_4 "../../tests/readings/50khz50perc1p1.txt"
#define SAMPLE_FILE_PATH_5 "../../tests/readings/50khz50perc1p1.txt"

#define EXPECTED_SUM_PATH "../../tests/readings/expected_sum.txt"

#define MAX_DATA_POINTS 5000

uint16_t current_sample[MAX_DATA_POINTS];
uint32_t sum[MAX_DATA_POINTS];

uint16_t expected_sum[MAX_DATA_POINTS];

int main() {
    size_t file_index;
    FILE *rptr;
    // Initialiseer alle array elementen met waarde: 0
    for(size_t i = 0; i < MAX_DATA_POINTS; i++) {
        sum[i] = 0;
    }
    // Loop dit 5 keer door, elke iteratie haalt de data uit .txt bestand
    // Na het inladen van waardes in current_sample[] tel die bij sum op
    for(size_t i = 0; i < 5; i++) {
        // Load reading from file into array
        file_index = 0;
        switch(i) {
            case 0: 
                rptr = fopen(SAMPLE_FILE_PATH_1, "r");
                break;
            case 1: 
                rptr = fopen(SAMPLE_FILE_PATH_2, "r");
                break;
            case 2: 
                rptr = fopen(SAMPLE_FILE_PATH_3, "r");
                break;
            case 3: 
                rptr = fopen(SAMPLE_FILE_PATH_4, "r");
                break;
            case 4: 
                rptr = fopen(SAMPLE_FILE_PATH_5, "r");
                break;
            default:
                break;
        }
        
        if (rptr == NULL) {
            perror("Failed to open readings file");
            return 1;
        }
        char line[8];
        while (fgets(line, sizeof(line), rptr) != NULL) {
            if (file_index < MAX_DATA_POINTS) {
                current_sample[file_index++] = (uint16_t)atoi(line);
            }
        }
        fclose(rptr);

        sum_samples(sum, MAX_DATA_POINTS, current_sample, MAX_DATA_POINTS);
    }

    // Laad de verwachte som uit .txt
    file_index = 0;
    FILE *exp_rptr = fopen(EXPECTED_SUM_PATH, "r");
    if (exp_rptr == NULL) {
            perror("Failed to open expected_sum file");
            return 1;
        }
        char line[8];
        while (fgets(line, sizeof(line), exp_rptr) != NULL) {
            if (file_index < MAX_DATA_POINTS) {
                expected_sum[file_index++] = (uint16_t)atoi(line);
            }
        }
        fclose(exp_rptr);

    for (int i = 0; i < MAX_DATA_POINTS; i++) {
        assert(sum[i] == expected_sum[i]);  // Placeholder for actual data verification
    }

    return 0;
}
