#include <stdio.h>

const int vector_1[] = {1,2,3};
const int vector_2[] = {4,5,6};

int scalar_product(const int v1[], const int v2[], size_t v) {
	size_t i;
	int result = 0;
	for ( i = 0; i < v; i++) 
		result += v1[i]*v2[i];
	return result;
}

int main(void){
	printf("The scalar product is: %d\n", scalar_product(vector_1, vector_2, sizeof(vector_1)/ sizeof(vector_1[0])));
	return 0;
}

