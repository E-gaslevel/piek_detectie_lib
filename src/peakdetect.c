#include "peakdetect.h"

static const uint16_t *g_data_for_qsort;

int local_maxima(const uint16_t data[], size_t data_size, uint16_t peaks[], size_t peaks_size) {
    // Check if peaks array is big enough
    if (peaks_size < data_size / 2) return -1;

    size_t peaks_i = 0; // Points to place to put the peak

    size_t i = 1; // Points to current sample, first sample cannot be a peak
    const size_t i_max = data_size - 1; // Points to the last sample cannot be a peak

    while (i < i_max) {
        // Test if previous sample is smaller
        if (data[i - 1] < data[i]) {
            size_t i_ahead = i + 1; // Points to sample ahead of the current sample

            // Find the next sample that is unequal to the current sample
            while (i_ahead < i_max && data[i_ahead] == data[i]) i_ahead++;

            // Maxima is found if next unequal sample is smaller than data[i]
            if (data[i_ahead] < data[i]) {
                const size_t left_edge = i;
                const size_t right_edge = i_ahead - 1;
                peaks[peaks_i] = (left_edge + right_edge) / 2;
                peaks_i++;
                // Skip samples that cannot be maximum
                i = i_ahead; 
            }
        }
        i++;
    }
    return 0;
}

int defrag_peaks(uint16_t peaks[], size_t peaks_size) {
    if (peaks_size == 0) return 0;

    size_t write_index = 0;

    for (size_t read_index = 0; read_index < peaks_size; read_index++) {
        if (peaks[read_index] != 0) {
            if (write_index != read_index) {
                peaks[write_index] = peaks[read_index];
                peaks[read_index] = 0; // zet oude plek direct op nul
            }
            write_index++;
        }
    }

    return 0;
}

int filter_height(const uint16_t data[], size_t data_size, uint16_t peaks[], size_t peaks_size, uint16_t height) {
    size_t i_max = peaks_size - 1;
    while (peaks[i_max] == 0 && i_max != 0) i_max--;

    for (size_t i = 0; i <= i_max; i++) {
        if (data[peaks[i]] < height) {
            peaks[i] = 0;
        }
    }

    return defrag_peaks(peaks, peaks_size);
}

// static int compare_values(const void *a, const void *b, )
// {
//     uint16_t i = *(const uint16_t *)a;
//     uint16_t j = *(const uint16_t *)b;

//     if (g_data_for_qsort[i] < g_data_for_qsort[j]) return -1;
//     if (g_data_for_qsort[i] > g_data_for_qsort[j]) return 1;
//     return 0;
// }

int argsort(const uint16_t data[], size_t data_size, uint16_t argsortArray[], size_t argsortArray_size) {
    if (data_size == 0 || argsortArray_size == 0) {
        return -1;
    }

    if (argsortArray_size > data_size) {
        argsortArray_size = data_size; // beperk argsortArray_size tot data_size
    }

    for (size_t i = 0; i < argsortArray_size; i++) {
        argsortArray[i] = (uint16_t)i; // initialiseer index-array
    }
    
    uint16_t coppied_data[data_size];
    for (size_t i = 0; i < data_size; i++) {
        coppied_data[i] = data[i]; // initialiseer index-array
    }

    for (size_t i = 0; i < data_size-1; i++) {
        for (size_t j = 0; j < data_size-i-1; j++) {
            if (coppied_data[j] > coppied_data[j+1]) {
                uint16_t temp = coppied_data[j];
                coppied_data[j] = coppied_data[j+1];
                coppied_data[j+1] = temp;

                uint16_t temp_index = argsortArray[j];
                argsortArray[j] = argsortArray[j+1];
                argsortArray[j+1] = temp_index;
            }
        }
    }

    return 0;
}

int filter_distance(const uint16_t data[], size_t data_size, uint16_t peaks[], size_t peaks_size, size_t distance) {
    return 0;
}