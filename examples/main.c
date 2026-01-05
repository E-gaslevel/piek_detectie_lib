#include "peakdetect.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define READING_FILE_PATH "../../tests/readings/50khz50perc1p1.txt"
#define SAVGOL_FILTER_FILE_PATH "../../tests/readings/expected_savgol_filter.txt"

#define MAX_DATA_POINTS 5000
#define WINDOW 51

double coeffs[WINDOW] = {
    -0.026637069922305495,
    -0.021087680355158494,
    -0.015764796484629744,
    -0.010668418310719274,
    -0.0057985458334270405,
    -0.0011551790527530466,
    0.003261682031302701,
    0.007452037418740207,
    0.011415887109559469,
    0.015153231103760488,
    0.018664069401343263,
    0.02194840200230779,
    0.02500622890665408,
    0.027837550114382124,
    0.030442365625491925,
    0.03282067543998348,
    0.034972479557856796,
    0.03689777797911187,
    0.038596570703748695,
    0.04006885773176728,
    0.04131463906316761,
    0.042333914697949705,
    0.043126684636113556,
    0.04369294887765917,
    0.04403270742258653,
    0.04414596027089565,
    0.04403270742258653,
    0.04369294887765917,
    0.04312668463611356,
    0.042333914697949705,
    0.04131463906316761,
    0.04006885773176727,
    0.03859657070374869,
    0.03689777797911186,
    0.03497247955785679,
    0.03282067543998348,
    0.030442365625491918,
    0.027837550114382117,
    0.025006228906654073,
    0.021948402002307785,
    0.018664069401343256,
    0.015153231103760479,
    0.01141588710955946,
    0.007452037418740198,
    0.0032616820313026906,
    -0.001155179052753057,
    -0.005798545833427051,
    -0.010668418310719284,
    -0.015764796484629775,
    -0.021087680355158494,
    -0.026637069922305457
};

int32_t data[MAX_DATA_POINTS];
int32_t expected_filtered_data[MAX_DATA_POINTS];

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
            data[file_index++] = (int32_t)atoi(line);
        }
    }
    fclose(rptr);

    // Load filtered data from file into array
    file_index = 0;
    FILE *fptr = fopen(SAVGOL_FILTER_FILE_PATH, "r");
    if (fptr == NULL) {
        perror("Failed to open peaks indices file");
        return 1;
    }
    char exp_line[8];
    while (fgets(exp_line, sizeof(exp_line), fptr) != NULL) {
        if (file_index < MAX_DATA_POINTS) {
            expected_filtered_data[file_index++] = (float)atoi(exp_line);
        }
    }
    fclose(fptr);

    smooth_signal(data, MAX_DATA_POINTS, coeffs, WINDOW);

    // Print both arrays
    for (int i = 0; i < MAX_DATA_POINTS; i++) {
        //assert(argsorted_indices[i] == expected_argsorted_indices[i]);
        printf("index: %d: Calculated: %d vs imported: %d\n", i, data[i], expected_filtered_data[i]);
    }

}