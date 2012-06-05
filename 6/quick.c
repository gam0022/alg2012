/*
 * quick.c: クイックソート法の実装
 */
#include <stdio.h>
#include <limits.h>
#include "sort.h"
#include "sort_util.h"

static int partition (int a[], int p, int r) 
{

	int left=p, right=r;
	int pivot = a[(left+right)/2];
	while(1) {
		while(compare(a[left], pivot) < 0) left++;
		while(compare(a[right],pivot) > 0) right--;
		if(compare(left, right)>=0) break;
		swap(a, left, right);
		left++;
		right--;
	}
	return right;
}

static void quicksort(int a[], int p, int r, int depth)
{
	if (ncalls == ULONG_MAX)
		/* オーバーフローが生じた */
		fprintf(stderr, "## warning: overflow of ncalls ##\n");
	ncalls++;

	if(depth>max_call_depth)
		max_call_depth = depth;

	if(p>=r) return;

	int k = partition(a, p, r);
	quicksort(a, p, k, depth+1);
	quicksort(a, k+1, r, depth+1);
}


void quick(int a[], unsigned n)
{
	quicksort(a,0,n-1,0);
}
