#define SIZE 500

extern int *unsorted;
extern int *sorted;

// structures to hold function parameters for pthread calls
struct mergeParams
{
  int begin;
  int mid;
  int end;
};

struct insertionSortParams
{
  int start;
  int end;
};

// function prototypes
void fillArray(int *array, const int size);
void *merge(void *args);
void *insertionSort(void *args);
void printArray(int *list, int size);