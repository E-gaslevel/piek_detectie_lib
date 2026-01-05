#ifndef PEAKDETECT_H
#define PEAKDETECT_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

int local_maxima(const uint16_t data[], size_t data_size, uint16_t peaks[], size_t peaks_size);

int defrag_peaks(uint16_t peaks[], size_t peaks_size);

int filter_height(const uint16_t data[], size_t data_size, uint16_t peaks[], size_t peaks_size, uint16_t height);

int argsort(const uint16_t data[], uint16_t peaks[], size_t peaks_size, uint16_t peaks_order[]);

int filter_distance(const uint16_t data[], size_t data_size, 
                    uint16_t peaks[], size_t peaks_size, 
                    uint16_t argsortArray[], size_t distance);

int sum_samples(uint32_t sum[], size_t sum_size, uint16_t current_sample[], size_t current_sample_size);

void smooth_signal(int *x, int n, const double *c, int window);

#endif
