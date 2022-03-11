#include <stdio.h>

void print_array(int a[], int n) {
  for (int i = 0; i < n; ++i) {
    printf("%d ", a[i]);
  }
  printf("\n");
}

int main() {
  int n;
  scanf("%d", &n);
  int a[n];
  for (int i = 0; i < n; ++i) {
    scanf("%d", &a[i]);
  }

  for (int i = 0; i < n - 1; ++i) {
    /*
     * 1) Find the minimum element among a[i + 1] ~ a[n - 1]
     * 2) Swap a[i] and the minimum element
     */

    // YOUR CODE HERE (~15 lines)
	  int min_index=i+1;
	for (int start= i+2;start<= n-1;++start)
	{	if (a[start]<=a[min_index])
		{min_index=start;}
		
	}
		if (a[i]>=a[min_index]){
		int t=a[i];
		a[i]=a[min_index];
		a[min_index]=t;
		}
    print_array(a, n);
  }

  return 0;
}
