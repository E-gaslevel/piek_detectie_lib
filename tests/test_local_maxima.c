#include "peakdetect.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define READING_FILE_PATH "../../tests/readings/50khz50perc1p1.txt"
#define PEAKS_FILE_PATH "../../tests/readings/indices/50khz50perc1p1_indices.txt"

#define MAX_DATA_POINTS 5000
#define MAX_PEAKS (MAX_DATA_POINTS / 2)

uint32_t data[MAX_DATA_POINTS];
uint16_t peaks[MAX_PEAKS];
uint16_t expected_peak_indices[MAX_PEAKS];

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

    // Load expected peak indices from file into array
    file_index = 0;
    FILE *exp_rptr = fopen(PEAKS_FILE_PATH, "r");
    if (exp_rptr == NULL) {
        perror("Failed to open expected peaks file");
        return 1;
    }
    char exp_line[8];
    while (fgets(exp_line, sizeof(exp_line), exp_rptr) != NULL) {
        if (file_index < MAX_PEAKS) {
            expected_peak_indices[file_index++] = (uint16_t)atoi(exp_line);
        }
    }
    fclose(exp_rptr);

    local_maxima(data, MAX_DATA_POINTS, peaks, MAX_PEAKS);

    for (int i = 0; i < MAX_PEAKS; i++) {
        assert(peaks[i] == expected_peak_indices[i]);  // Placeholder for actual data verification
    }

    return 0;
}
