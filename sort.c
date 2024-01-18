#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>
#include <math.h>
#define SIZE_1 50000
#define SIZE_20 1000000
#define NEXT 50000

int getrand(int min, int max) {
    return (double)rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

double wtime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

uint32_t find_max(uint32_t* arr, int n) {
    uint32_t max = 0;
    for (int i = 0; i < n; i++) {
        if (max < arr[i]) {
            max = arr[i];
        }
    }
    
    return max;
}

void swap(uint32_t * a, uint32_t* b) {
    int exp = *a;
    *a = *b;
    *b = exp;
}

void radix_sort(uint32_t* arr, int n) {
	int i;
    uint32_t comma = 1;
	uint32_t max = find_max(arr, n);
	uint32_t *buffer = (uint32_t*)malloc(sizeof(uint32_t) * n);
	
	while ((max / comma) > 0)
	{
		uint32_t nums[10] = {0};
		for(i = 0; i < n; i++) {
			nums[arr[i] / comma % 10]++;
        } // Подсчёт, сколько раз каждая цифра встречается
          // в очередном разряде

		for(i = 1; i < 10; i++) {
			nums[i] += nums[i - 1];
        } // Подсчёт, сколько ячеек массива займёт каждая
          // цифра и начиная с какой ячейки нужно записывать
          // элемент с данной цифрой

		for(i = n - 1; i >= 0; i--) {
            buffer[--nums[arr[i] / comma % 10]] = arr[i];
        } // Размещение каждой цифры в соответствующие
          // ячейки временного массива

		for(i = 0; i < n; i++) {
			arr[i] = buffer[i];
        } // Запись временного, частичного отсортированного
          // массива, в основной

		comma *= 10;
	}

	free(buffer);
}

void odd_even_step(uint32_t* arr, int n, int num) {
	int i, even, odd;

	if (num % 2 == 0) {
		for (i = 1; i < n; i += 2) {
			even = i - 1;
			if (arr[even] > arr[i]) {
				swap (&arr[even], &arr[i]);
			}
		}
    } else {
		for (i = 1; i < n - 1; i += 2) {
			odd = i + 1;
			if (arr[i] > arr[odd]) {
				swap (&arr[odd], &arr[i]);
			}
		}
    }
}

void odd_even_sort(uint32_t* arr, int n) {
	for (uint32_t i = 0; i < n; i++) {
        odd_even_step(arr, n, i);
    } 
}

void merge(uint32_t* arr, int low, int mid, int high) {
    uint32_t* buffer = (uint32_t*)malloc((high + 1) * sizeof(uint32_t));

    for (int i = low; i <= high; i++) {
        buffer[i] = arr[i];
    }

    int l = low;
    int r = mid + 1;
    int i = low;

    while ((l <= mid) && (r <= high)) {
        if (buffer[l] <= buffer[r]) {
            arr[i] = buffer[l];
            l++;
        } else {
            arr[i] = buffer[r];
            r++;
        }
        i++;
    }
    while (l <= mid) {
        arr[i] = buffer[l];
        l++;
        i++;
    }
    while (r <= high) {
        arr[i] = buffer[r];
        r++;
        i++;
    }
    free(buffer);
}

void merge_sort(uint32_t* arr, int low, int high) {
    if (low < high) {
        int mid = floor((low + high) / 2);
        merge_sort(arr, low, mid);
        merge_sort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
}

int main() {
    uint32_t* arr;
    uint32_t* radix;
    uint32_t* odd_even;
    uint32_t* merge;

    int i, j;
    double start, end;

    printf("Number\t      Radix Sort    Odd-Even Sort   Merge Sort\n");

    for (i = SIZE_1; i <= SIZE_20; i += NEXT) {
        arr = (uint32_t*)malloc(i * sizeof(uint32_t));
        radix = (uint32_t*)malloc(i * sizeof(uint32_t));
        odd_even = (uint32_t*)malloc(i * sizeof(uint32_t));
        merge = (uint32_t*)malloc(i * sizeof(uint32_t));

        for (j = 0; j < i; j++) {
            arr[j] = (uint32_t)getrand(0, 100001);
            radix[j] = arr[j];
            odd_even[j] = arr[j];
            merge[j] = arr[j];
        }

        printf("%d\t", i);

        start = wtime();
        radix_sort(radix, i);
        end = wtime() - start;
        printf("%15.7lf", end);

        start = wtime();
        odd_even_sort(odd_even, i);
        end = wtime() - start;
        printf("%15.7lf", end);

        start = wtime();
        merge_sort(merge, 0, i - 1);
        end = wtime() - start;
        printf("%15.7lf", end);

        printf("\n");
        free(arr);
        free(radix);
        free(odd_even);
        free(merge);
    }

    return 0;
}