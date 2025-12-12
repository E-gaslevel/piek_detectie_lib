#include "peakdetect.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main() {
    uint16_t peaks[] = {0, 5, 0, 10, 0, 0, 15, 0};
    uint16_t expected[] = {5, 10, 15};
    size_t peaks_size = sizeof(peaks) / sizeof(peaks[0]);

    defrag_peaks(peaks, peaks_size);

    // Print defragged peaks for visual verification
    printf("Defragged peaks:\n");
    for (size_t i = 0; i < peaks_size; i++) {
        printf("%d ", peaks[i]);
    }
    printf("\n");

    assert(memcmp(peaks, expected, sizeof(expected)) == 0);

    uint16_t empty[] = {0, 0, 0, 0, 0, 0, 0, 0};
    defrag_peaks(empty, peaks_size);
    for (size_t i = 0; i < peaks_size; i++) {
        assert(empty[i] == 0);
    }

    uint16_t full[] = {1, 2, 3, 4, 5, 6, 7, 8};
    defrag_peaks(full, peaks_size);
    for (size_t i = 0; i < peaks_size; i++) {
        assert(full[i] == i + 1);
    }

    return 0;
}
