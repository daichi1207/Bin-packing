#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int partition_rev(int *a, int start, int end) {
  int piv = *(a + end);
  int i = start - 1;

  for (int j = start; j <= end - 1; j++) {
    if (*(a + j) > piv) {
      i++;
      swap(a + i, a + j);
    }
  }

  swap(a + i + 1, a + end);
  return (i + 1);
}

void quicksort_rev(int *a, int start, int end) {
  if (start < end) {
    int p = partition_rev(a, start, end);
    quicksort_rev(a, start, p - 1);
    quicksort_rev(a, p + 1, end);
  }
}

int next_fit(int *a, int n, int size, double *_clock, int *total_item_size) {
  clock_t begin = clock();
  int bin_count = 1;
  int i, s = size;
  for (i = 0; i < n; ++i) {
    if (s >= *(a + i)) {
      s -= *(a + i);
      continue;
    } else {
      s = size;
      bin_count++;
      i--;
    }
  }
  clock_t end = clock();
  *_clock = (double)(end - begin) / CLOCKS_PER_SEC;

  *total_item_size = 0;
  for (i = 0; i < n; ++i) {
    *total_item_size += *(a + i);
  }
  return bin_count;
}

int best_fit(int *a, int n, int size, double *_clock, int *total_item_size) {
  clock_t begin = clock();
  int bin_count = 0;
  int bin[n]; // there can only be n bins max
  int i;
  for (i = 0; i < n; ++i) {
    int best_bin = -1;
    int j;
    for (j = 0; j < bin_count; ++j) {
      if (bin[j] >= *(a + i)) {
        if (best_bin == -1 || (best_bin != -1 && bin[best_bin] > bin[j])) {
          best_bin = j;
        }
      }
    }
    if (best_bin < 0) {
      bin[bin_count] = size - *(a + i);
      bin_count++;
    } else {
      bin[best_bin] -= *(a + i);
    }
  }
  clock_t end = clock();
  *_clock = (double)(end - begin) / CLOCKS_PER_SEC;

  *total_item_size = 0;
  for (i = 0; i < n; ++i) {
    *total_item_size += *(a + i);
  }
  return bin_count;
}

int first_fit(int *a, int n, int size, double *_clock, int *total_item_size) {
  clock_t begin = clock();
  int bin_count = 0;
  int bin[n]; // there can only be n bins max
  int i;
  for (i = 0; i < n; ++i) {
    int j;
    for (j = 0; j < bin_count; ++j) {
      if (bin[j] >= *(a + i)) {
        bin[j] -= *(a + i);
        break;
      }
    }
    if (j == bin_count) {
      bin[bin_count] = size - *(a + i);
      bin_count++;
    }
  }
  clock_t end = clock();
  *_clock = (double)(end - begin) / CLOCKS_PER_SEC;

  *total_item_size = 0;
  for (i = 0; i < n; ++i) {
    *total_item_size += *(a + i);
  }
  return bin_count;
}

int next_fit_decreasing_utilization(int *a, int n, int size, double *clock,
                                    int *total_item_size) {

  quicksort_rev(a, 0, n - 1);
  return next_fit(a, n, size, clock, total_item_size);
}

int best_fit_decreasing_utilization(int *a, int n, int size, double *_clock,
                                    int *total_item_size) {
  quicksort_rev(a, 0, n - 1);
  return best_fit(a, n, size, _clock, total_item_size);
}

int first_fit_decreasing_utilization(int *a, int n, int size, double *_clock,
                                     int *total_item_size) {
  quicksort_rev(a, 0, n - 1);
  return first_fit(a, n, size, _clock, total_item_size);
}

void generate_data(int *a, int n, int *size) {
  // printf("items: ");
  for (int i = 0; i < n; ++i) {
    *(a + i) = (rand() % 100) + 1;
    // printf("%d ", *(a + i));
  }

  // *size = 200;
  *size = (rand() % 50) + 150;

  printf("\nbin size: %d \n", *size);
}

int main() {
  int n;
  double clock1, clock2, clock3, clock4, clock5, clock6, clock7;
  int total_item_size1, total_item_size2, total_item_size3, total_item_size4,
      total_item_size5, total_item_size6, total_item_size7;
  srand(time(NULL));
  printf("Enter size of items in a set: ");
  scanf("%d", &n);
  int a[n], i, size;
  // printf("Enter the items in the set: ");
  // for (i = 0; i < n; ++i) {
  //   scanf("%d", &a[i]);
  // }
  // printf("enter the bin size: ");
  // scanf("%d", &size);

  generate_data(a, n, &size);

  int bin_count1 = next_fit(a, n, size, &clock1, &total_item_size1);
  int bin_count2 = first_fit(a, n, size, &clock2, &total_item_size2);
  int bin_count3 = best_fit(a, n, size, &clock3, &total_item_size3);
  int bin_count5 =
      next_fit_decreasing_utilization(a, n, size, &clock5, &total_item_size5);
  int bin_count6 =
      first_fit_decreasing_utilization(a, n, size, &clock6, &total_item_size6);
  int bin_count7 =
      best_fit_decreasing_utilization(a, n, size, &clock7, &total_item_size7);

  double utilization1 = (double)total_item_size1 / (bin_count1 * size);
  double utilization2 = (double)total_item_size2 / (bin_count2 * size);
  double utilization3 = (double)total_item_size3 / (bin_count3 * size);
  double utilization5 = (double)total_item_size5 / (bin_count5 * size);
  double utilization6 = (double)total_item_size6 / (bin_count6 * size);
  double utilization7 = (double)total_item_size7 / (bin_count7 * size);

  printf("next fit bin count: %d, time: %f, utilization: %f\n", bin_count1,
         clock1, utilization1);
  printf("first fit bin count: %d, time: %f, utilization: %f\n", bin_count2,
         clock2, utilization2);
  printf("best fit bin count: %d, time: %f, utilization: %f\n", bin_count3,
         clock3, utilization3);
  printf("next fit decresing bin count: %d, time: %f, utilization: %f\n",
         bin_count5, clock5, utilization5);
  printf("first fit decresing bin count: %d, time: %f, utilization: %f\n",
         bin_count6, clock6, utilization6);
  printf("best fit decresing bin count: %d, time: %f, utilization: %f\n",
         bin_count7, clock7, utilization7);

  return 0;
}
