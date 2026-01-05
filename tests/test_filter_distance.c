#include "peakdetect.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define READING_FILE_PATH "../../tests/readings/50khz50perc1p1.txt"
#define PEAKS_FILE_PATH "../../tests/readings/indices/50khz50perc1p1_indices.txt"
#define ARGSORTED_FILE_PATH "../../tests/readings/indices/50khz50perc1p1_peak_indices_quicksorted.txt"
#define DISTANCE_FILTERED_PEAKS_FILE_PATH "../../tests/readings/indices/50khz50perc1p1_indices_distance_filtered.txt"

#define MAX_DATA_POINTS 5000
#define MAX_PEAKS (MAX_DATA_POINTS / 2)
#define N_OF_PEAKS 1283

uint16_t data[MAX_DATA_POINTS];
uint16_t peaks[N_OF_PEAKS];
uint16_t argsorted_peaks[N_OF_PEAKS];
uint16_t expected_distance_filtered_peaks[N_OF_PEAKS];

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

    // Load peak indices from file into array
    file_index = 0;
    FILE *pptr = fopen(PEAKS_FILE_PATH, "r");
    if (pptr == NULL) {
        perror("Failed to open expected peaks file");
        return 1;
    }
    char p_line[8];
    while (fgets(p_line, sizeof(p_line), pptr) != NULL) {
        if (file_index < N_OF_PEAKS) {
            peaks[file_index++] = (uint16_t)atoi(p_line);
        }
    }
    fclose(pptr);

    // Load argsorted indices(by quicksort) from file into array
    file_index = 0;
    FILE *aptr = fopen(ARGSORTED_FILE_PATH, "r");
    if (aptr == NULL) {
        perror("Failed to open argsorted peaks file");
        return 1;
    }
    char a_line[8];
    while (fgets(a_line, sizeof(a_line), aptr) != NULL) {
        if (file_index < N_OF_PEAKS) {
            argsorted_peaks[file_index++] = (uint16_t)atoi(a_line);
        }
    }
    fclose(aptr);

    // Use imported argsort array to compare purely filter_distance without dependence of sort function
    filter_distance(data, MAX_DATA_POINTS, peaks, N_OF_PEAKS, argsorted_peaks, 100);

    // Load filtered results from file into array
    file_index = 0;
    FILE *fptr = fopen(DISTANCE_FILTERED_PEAKS_FILE_PATH, "r");
    if (fptr == NULL) {
        perror("Failed to open filtered peaks file");
        return 1;
    }
    char f_line[8];
    while (fgets(f_line, sizeof(f_line), fptr) != NULL) {
        if (file_index < N_OF_PEAKS) {
            expected_distance_filtered_peaks[file_index++] = (uint16_t)atoi(f_line);
        }
    }
    fclose(fptr);

    // Verify the filtered peaks against expected results
    for (size_t i = 0; i < MAX_PEAKS; i++) {
        printf("Peak: %u vs Expected: %u\n", peaks[i], expected_distance_filtered_peaks[i]);
        assert(peaks[i] == expected_distance_filtered_peaks[i]);
        // If both are 0, both arrays are done because we move removed peaks to the end of array with defrag(tested)
        if (peaks[i] == 0 && expected_distance_filtered_peaks[i] == 0) break;
    }

    return 0;
}
