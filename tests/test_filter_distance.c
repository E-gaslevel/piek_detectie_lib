#include "peakdetect.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define READING_FILE_PATH "../../tests/readings/50khz50perc1p1.txt"
#define PEAKS_FILE_PATH "../../tests/readings/indices/50khz50perc1p1_indices.txt"
#define DISTANCE_FILTERED_PEAKS_FILE_PATH "../../tests/readings/indices/50khz50perc1p1_indices_distance_filtered.txt"

#define MAX_DATA_POINTS 5000
#define MAX_PEAKS (MAX_DATA_POINTS / 2)

uint16_t data[MAX_DATA_POINTS];
uint16_t peaks[MAX_PEAKS];
uint16_t sorted_peaks[MAX_PEAKS];
uint16_t expected_distance_filtered_peaks[MAX_PEAKS];

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
    FILE *exp_rptr = fopen(PEAKS_FILE_PATH, "r");
    if (exp_rptr == NULL) {
        perror("Failed to open expected peaks file");
        return 1;
    }
    char exp_line[8];
    while (fgets(exp_line, sizeof(exp_line), exp_rptr) != NULL) {
        if (file_index < MAX_PEAKS) {
            peaks[file_index++] = (uint16_t)atoi(exp_line);
        }
    }
    fclose(exp_rptr);

    filter_distance(data, MAX_DATA_POINTS, peaks, MAX_PEAKS, sorted_peaks, MAX_PEAKS, 100);

    for (size_t i = 0; i < MAX_PEAKS; i++) {
        if (peaks[i] != 0) {
            printf("index: %d, ", peaks[i] + 0);
        }
    }

    // Load filtered results from file into array
    file_index = 0;
    FILE *fptr = fopen(DISTANCE_FILTERED_PEAKS_FILE_PATH, "r");
    if (fptr == NULL) {
        perror("Failed to open filtered peaks file");
        return 1;
    }
    char f_line[8];
    while (fgets(f_line, sizeof(f_line), fptr) != NULL) {
        if (file_index < MAX_PEAKS) {
            expected_distance_filtered_peaks[file_index++] = (uint16_t)atoi(f_line);
        }
    }
    fclose(fptr);

    // Verify the filtered peaks against expected results
    for (size_t i = 0; i < MAX_PEAKS; i++) {
        assert(peaks[i] == expected_distance_filtered_peaks[i]);
    }

    return 0;
}
