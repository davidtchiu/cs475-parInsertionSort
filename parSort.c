/*
 * This program spawns two threads to sort (using insertion sort)
 *
 *  Created on: Jan 22, 2016
 *      Author: dchiu
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "parSort.h"

/**
 * Global defs
 */
int *unsorted;
int *sorted;

/**
 * Main function
 */
int main(int argc, char* argv[]) {
  // allocate arrays
  unsorted = malloc(SIZE * sizeof(int));
  sorted = malloc(SIZE * sizeof(int));

  // allocate and fill the unsorted array
  fillArray(unsorted, SIZE);
  printf("Unsorted: ");
  printArray(unsorted, SIZE);

  // define the indices of the two sublists
  int start1 = 0;
  int end1 = SIZE / 2;
  int start2 = end1 + 1;
  int end2 = SIZE - 1;

  // 2 sorting threads, and 1 merge thread
  pthread_t *threads = malloc(3 * sizeof(pthread_t));

  // prepare sorting params and fire off sorting threads
  struct insertionSortParams *sArgs = malloc(2 * sizeof(struct insertionSortParams));
  sArgs[0].start = start1;
  sArgs[0].end = end1;
  pthread_create(&threads[0], NULL, insertionSort, &sArgs[0]); // deal with first sublist

  sArgs[1].start = start2;
  sArgs[1].end = end2;
  pthread_create(&threads[1], NULL, insertionSort, &sArgs[1]); // deal with second sublist

  // wait for sorting threads to terminate
  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);

  // prepare params and fire off a thread to merge the two halves of the list
  struct mergeParams mArgs;
  mArgs.begin = start1;
  mArgs.mid = start2;
  mArgs.end = end2;
  pthread_create(&threads[2], NULL, merge, &mArgs); // merge the sublists into sorted[]!

  // wait for merging thread to terminate
  pthread_join(threads[2], NULL);

  // main thread prints out the sorted array
  printf("\n\nSorted: ");
  printArray(sorted, SIZE);

  // clean up malloc'd memory
  free(unsorted);
  free(sorted);
  free(threads);
  free(sArgs);

  return 0;
}

/**
 * Fills unsorted array with random ints
 * @param array the array to fill
 * @param size the size of the array
 */
void fillArray(int *array, const int size) {
  int i;
  srand(time(NULL));
  for (i = 0; i < size; i++)
    array[i] = rand() % 100;
}

/**
 * Merges two sorted halves of the unsorted[] array into sorted[]
 *
 * @param begin index of first element in first sublist
 * @param mid index of the first element in the second sublist
 * @param end index of the last element in the second sublist
 */
void *merge(void *args) {
  // unpack parameters
  struct mergeParams *params = (struct mergeParams *) args;
  int begin = params->begin;
  int mid = params->mid;
  int end = params->end;

  int i = begin, j = mid, tpos = begin;

  while (i < mid && j <= end) {
    if (unsorted[i] < unsorted[j]) {
      sorted[tpos++] = unsorted[i++];
    }
    else {
      sorted[tpos++] = unsorted[j++];
    }
  }

  // still elements left over in first list. copy over
  while (i < mid) {
    sorted[tpos++] = unsorted[i++];
  }

  // still elements left over in first list. copy over
  while (j <= end) {
    sorted[tpos++] = unsorted[j++];
  }
  return NULL;
}

/**
 * Insertion sort
 *
 * @param index of the first element in unsorted sublist
 * @param index of the last element in unsorted sublist
 */
void *insertionSort(void *args) {
  struct insertionSortParams *params = (struct insertionSortParams *)args;
  int start = params->start;
  int end = params->end;

  int i = start, j, itemToInsert;

  // everything to the left of i is sorted
  while (i <= end) {
    itemToInsert = unsorted[i]; // a must, or else unsorted[i] gets overwritten when shifting elements

    // since everything in this sequence is sorted,
    // starting from i, and going in reverse order, shift the elements to the right
    // until an element less than unsorted[i] is found
    j = i - 1;
    while (j >= start && itemToInsert < unsorted[j]) {
      unsorted[j + 1] = unsorted[j];
      j--;
    }
    // insert the element into s[j]
    unsorted[j + 1] = itemToInsert;
    i++;
  }
  return NULL;
}

/**
 * Prints the given array
 */
void printArray(int *list, int size) {
  int i;
  for (i = 0; i < size - 1; i++) {
    printf("%d, ", list[i]);
  }
  // print final element
  printf("%d\n", list[i]);
}