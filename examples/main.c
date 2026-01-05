#include "peakdetect.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define SAMPLE_FILE_PATH_1 "../../tests/readings/beste_pulsen/fles5/26.9_f120000d75n5_0.txt"
#define SAMPLE_FILE_PATH_2 "../../tests/readings/beste_pulsen/fles5/26.9_f120000d75n5_0 (2).txt"
#define SAMPLE_FILE_PATH_3 "../../tests/readings/beste_pulsen/fles5/26.9_f120000d75n5_0 (3).txt"
#define SAMPLE_FILE_PATH_4 "../../tests/readings/beste_pulsen/fles5/26.9_f120000d75n5_0 (4).txt"
#define SAMPLE_FILE_PATH_5 "../../tests/readings/beste_pulsen/fles5/26.9_f120000d75n5_0 (5).txt"

#define MAX_DATA_POINTS 4500
#define WINDOW 51
#define MAX_PEAKS (MAX_DATA_POINTS/2)

#define min_height 12000
#define min_distance 92

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

uint16_t current_sample[MAX_DATA_POINTS];
uint32_t data[MAX_DATA_POINTS];

uint16_t peaks[MAX_PEAKS];
uint16_t argsorted_peaks[MAX_PEAKS];

int main() {

    // Initialiseer alle array elementen met waarde: 0
    for(size_t i = 0; i < MAX_DATA_POINTS; i++) {
        data[i] = 0;
    }

    // Load reading from file into array
    size_t file_index;
    FILE *rptr;

    for(size_t i = 0; i < 5; i++) {
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
        sum_samples(data, MAX_DATA_POINTS, current_sample, MAX_DATA_POINTS);
    }

    smooth_signal(data, MAX_DATA_POINTS, coeffs, WINDOW);
    local_maxima(data, MAX_DATA_POINTS, peaks, MAX_PEAKS);
    filter_height(data, MAX_DATA_POINTS, peaks, MAX_PEAKS, min_height);
    filter_distance(data, MAX_DATA_POINTS, peaks, MAX_PEAKS, argsorted_peaks, min_distance);

    // Print both arrays
    for (int i = 0; i < MAX_PEAKS; i++) {
        if (peaks[i] == 0) continue;
        //assert(argsorted_indices[i] == expected_argsorted_indices[i]);
        printf("index: %d, peak: %d\n", peaks[i], data[peaks[i]]);
    }

}