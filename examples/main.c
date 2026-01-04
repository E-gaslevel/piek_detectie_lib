#include "peakdetect.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define READING_FILE_PATH "../../tests/readings/50khz50perc1p1.txt"
#define PEAKS_INDICES "../../tests/readings/indices/50khz50perc1p1_indices.txt"
#define ARGSORTED_INDICES_FILE_PATH "../../tests/readings/indices/50khz50perc1p1_indices_argsort_sorted_stable.txt"

#define MAX_DATA_POINTS 5000
#define MAX_PEAKS (MAX_DATA_POINTS / 2)

#define N_OF_PEAKS 1283

uint16_t data[MAX_DATA_POINTS];
uint16_t peaks[N_OF_PEAKS];
uint16_t argsorted_peak_indices[N_OF_PEAKS];

int main() {
    // Load reading from file into array
    size_t file_index = 0;
    FILE *rptr = fopen(READING_FILE_PATH, "r");
    if (rptr == NULL) {
        perror("Failed to open readings file");
        return 1;
    }
    char line[8];
    while (fgets(line, sizeof(line), rptr) != NULL) {
        if (file_index < MAX_DATA_POINTS) {
            data[file_index++] = (uint16_t)atoi(line);
        }
    }
    fclose(rptr);

    // for (size_t i = 0; i < MAX_DATA_POINTS; i++) {
    //     printf("index: %d, ", argsorted_indices[i] + 0);
    // }

    // Load peaks indices from file into array
    file_index = 0;
    FILE *fptr = fopen(PEAKS_INDICES, "r");
    if (fptr == NULL) {
        perror("Failed to open peaks indices file");
        return 1;
    }
    char exp_line[8];
    while (fgets(exp_line, sizeof(exp_line), fptr) != NULL) {
        if (file_index < MAX_DATA_POINTS) {
            peaks[file_index++] = (uint16_t)atoi(exp_line);
        }
    }
    fclose(fptr);

    // print sorted array for visual inspection
    // for (int i = 0; i < MAX_DATA_POINTS; i++) {
    //     if (peaks[i] == 0) continue;
    //     //assert(argsorted_indices[i] == expected_argsorted_indices[i]);
    //     printf("index: %d\n", peaks[i] + 0);
    // }

    filter_distance(data, MAX_PEAKS, peaks, N_OF_PEAKS, argsorted_peak_indices, 100);
}