#include <stdio.h>

int is_prime( unsigned long n ) {
	int i;
	if (n == 1 || n == 0) return 0;
	if (n == 2) return 1;
	for (i = 2; i <= n/2; i++) {
		if (!(n % i)) {
			return 0;
		}}
	return 1;
	}

int main(void) {
	unsigned long y;
	scanf("%lu", &y);
	if (is_prime(y)==1) printf("yes\n");
	else printf("no\n");
	return 0;
}
