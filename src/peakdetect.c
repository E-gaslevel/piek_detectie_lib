#include "peakdetect.h"

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

int argsort(const uint16_t data[], uint16_t peaks[], size_t peaks_size, uint16_t peaks_order[]) {
    if (peaks_size == 0) {
        return -1;
    }

    for (size_t i = 0; i < peaks_size; i++) {
        peaks_order[i] = i; // initialiseer index-array
    }

    for (size_t i = 0; i < peaks_size-1; i++) {
        for (size_t j = 0; j < peaks_size-i-1; j++) {
            if (data[peaks[peaks_order[j]]] > data[peaks[peaks_order[j+1]]]) {
                size_t tmp = peaks_order[j];
                peaks_order[j] = peaks_order[j+1];
                peaks_order[j+1] = tmp;
            }
        }
    }

    return 0;
}

int filter_distance(const uint16_t data[], size_t data_size, 
                    uint16_t peaks[], size_t peaks_size, 
                    uint16_t argsortArray[], 
                    size_t distance) {

    if(data_size == 0 || peaks_size == 0) {
        return -1;
    }

    size_t peak_to_keep_index;
    int16_t diff;

    for(int16_t i = peaks_size-1; i >= 0; i--) {
        size_t peak_to_keep_index = argsortArray[i];
        // Als deze al verwijderd is, dan ga naar volgende index
        if(peaks[peak_to_keep_index] == 0) continue;

        // Check rechts van peak_to_keep alleen binnen de distance
        for(size_t j = peak_to_keep_index; j < peaks_size; j++) {
            if (peaks[j] == 0) continue;
            diff = (int16_t)peaks[j] - (int16_t)peaks[peak_to_keep_index];

            if(diff == 0) continue;
            if (diff > distance) break;
            peaks[j] = 0;
        }

        // Check links van peak_to_keep alleen binnen de distance
        for (size_t j = peak_to_keep_index; j-- > 0;) {
            if (peaks[j] == 0) continue;

            diff = (int16_t)peaks[peak_to_keep_index] - (int16_t)peaks[j];

            if(diff == 0) continue;
            if (diff > distance) break;
            peaks[j] = 0;
        }
    } 
    
    defrag_peaks(peaks, peaks_size);
    
    return 0;
}

int sum_samples(uint32_t sum[], size_t sum_size, uint16_t current_sample[], size_t current_sample_size) {

    if (current_sample_size > sum_size) {
        current_sample_size = sum_size;
    }

    for(size_t i = 0; i < sum_size; i++) {
        sum[i] += current_sample[i];
    }

    return 0;
}