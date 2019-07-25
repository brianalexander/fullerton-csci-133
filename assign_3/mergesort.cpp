#include <iostream>

/*
 * mergesort.cpp
 * Implementation of a bitonic mergesort
 */

/* merge(input, size, output, asc)
   Merge the two halves of the array input (which has size elements) into
   output. If asc is true, then the output array should be in ascending order;
   otherwise it should be descending.
*/
void merge(int* input, int size, int* output, bool output_asc) {
    int mid = size/2;
    int i; //position in left
    int j; //position in right
    int k = 0; //position in output

    if(output_asc) {
        i = 0;
        j = size-1;
        while( i < mid && j >= mid ) {
            if(input[i] < input[j]) {
                output[k++] = input[i++];
            } else {
                output[k++] = input[j--];
            }
        }
        while( i < mid) {
            output[k++] = input[i++];
        }
        while( j >= mid) {
            output[k++] = input[j--];
        }
    } else {
        i=mid-1;
        j=mid;
        while( i >= 0 && j < size ) {
             if(input[i] > input[j]) {
                output[k++] = input[i--];
             } else {
                output[k++] = input[j++];
             }
        }
        while( i >= 0) {
            output[k++] = input[i--];
        }
        while( j < size) {
            output[k++] = input[j++];
        }
    }
}

/* mergesort(input, size, output, asc)
   Mergesort the input array (with size elements) into the output array. If
   asc is true, the output array should be sorted ascending, otherwise it should
   be descending.
*/
void mergesort(int *input, int size, int* output, bool output_asc) {
    if(size == 0) {
        return;
    } else if (size == 1) {
        output[0] = input[0];
    } else {
        int mid = size/2;

        mergesort(input, mid, output, true); //left half
        mergesort(input+mid, size-mid, output+mid, false); //right half

        merge(output, size, input, output_asc);
        
        for(int z = 0; z < size; z++)
            output[z] = input[z];
    }
}

/* mergesort(input, size)
   Sorts size elements in the array pointed to by input, using the MergeSort
   algorithm. Output is returned as a newly allocated array, which the caller
   is responsible for freeing.
*/
int* mergesort(int* input, int size) {
    int* output = new int[size];
    mergesort(input, size, output, true);
    return output;
}