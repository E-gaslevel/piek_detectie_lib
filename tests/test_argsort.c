#include "peakdetect.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define READING_FILE_PATH "../../tests/readings/50khz50perc1p1.txt"
#define PEAKS_INDICES_FILE_PATH "../../tests/readings/indices/50khz50perc1p1_indices.txt"
#define EXPECTED_ARGSORTED_INDICES_FILE_PATH "../../tests/readings/indices/50khz50perc1p1_peak_indices_argsorted.txt"

#define MAX_DATA_POINTS 5000
#define MAX_PEAKS (MAX_DATA_POINTS / 2)

// Explicit number of peaks needed, because you dont want to sort through garbage
// Normally this number will be updated and stored in a variable
#define N_OF_PEAKS 1283


uint16_t data[MAX_DATA_POINTS];
uint16_t peaks[N_OF_PEAKS];
uint16_t argsorted_indices[N_OF_PEAKS];
uint16_t expected_argsorted_indices[N_OF_PEAKS];

int main() {
    // Load reading from file into array
    size_t file_index = 0;
    FILE *rptr = fopen(READING_FILE_PATH, "r");
    if (rptr == NULL) {
        perror("Failed to open readings file");
        return 1;
    }
    char r_line[8];
    while (fgets(r_line, sizeof(r_line), rptr) != NULL) {
        if (file_index < MAX_DATA_POINTS) {
            data[file_index++] = (uint16_t)atoi(r_line);
        }
    }
    fclose(rptr);

    // Load peaks indices from file into array
    file_index = 0;
    FILE *pptr = fopen(PEAKS_INDICES_FILE_PATH, "r");
    if (pptr == NULL) {
        perror("Failed to open peak indices file");
        return 1;
    }
    char p_line[8];
    while (fgets(p_line, sizeof(p_line), pptr) != NULL) {
        if (file_index < MAX_PEAKS) {
            peaks[file_index++] = (uint16_t)atoi(p_line);
        }
    }
    fclose(pptr);

    argsort(data, peaks, N_OF_PEAKS, argsorted_indices);

    // Load expected argsorted indices from file into array
    file_index = 0;
    FILE *fptr = fopen(EXPECTED_ARGSORTED_INDICES_FILE_PATH, "r");
    if (fptr == NULL) {
        perror("Failed to open argsorted indices file");
        return 1;
    }
    char exp_line[8];
    while (fgets(exp_line, sizeof(exp_line), fptr) != NULL) {
        if (file_index < N_OF_PEAKS) {
            expected_argsorted_indices[file_index++] = (uint16_t)atoi(exp_line);
        }
    }
    fclose(fptr);

    // Validate the argsort results
    for (size_t i = 0; i < N_OF_PEAKS; i++) {
        assert(argsorted_indices[i] == expected_argsorted_indices[i]);
    }
    return 0;
}