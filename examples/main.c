#include "peakdetect.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_DATA_POINTS 5000
#define MAX_PEAKS (MAX_DATA_POINTS / 2)

uint16_t data[MAX_DATA_POINTS];
uint16_t peaks[MAX_PEAKS];
uint16_t expected_peak_indices[MAX_PEAKS];

int main() {
    // Load reading from file into array
    FILE *rptr = fopen("tests/readings/50khz50perc1p1.txt", "r");
    if (rptr == NULL) {
        perror("Failed to open file");
        return 1;
    }
    char line[8];
    while (fgets(line, sizeof(line), rptr) != NULL) {
        static int index = 0;
        if (index < MAX_DATA_POINTS) {
            data[index++] = (uint16_t)atoi(line);
        }
    }
    fclose(rptr);

    // Load peak indices from file into array
    FILE *pkrptr = fopen("tests/readings/peaks/50khz50perc1p1_indices.txt", "r");
    if (pkrptr == NULL) {
        perror("Failed to open peak indices file");
        return 1;
    }
    char peak_line[8];
    while (fgets(peak_line, sizeof(peak_line), pkrptr) != NULL) {
        static int peak_index = 0;
        if (peak_index < MAX_PEAKS) {
            expected_peak_indices[peak_index++] = (uint16_t)atoi(peak_line);
        }
    }
    fclose(pkrptr);

    local_maxima(data, MAX_DATA_POINTS, peaks, MAX_PEAKS);
}