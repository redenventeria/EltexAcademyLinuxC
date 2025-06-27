#include<stdio.h>
#include<stdlib.h>

int main() {
	
	size_t n = 6;
	int m[6][6] = {{0}};

	size_t num = 1;

	/*
	 * Main idea - splitting matrix into "rings", like this:
	 *
	 * N = 6
	 * 000000
	 * 011110
	 * 012210
	 * 012210
	 * 011110
	 * 000000
	 * 
	 * Each iteration of the outer for fills one ring,
	 * from outer rings to inner in spiral motion.
	 * Inner fors fill one side of the ring each.
	 * */

	for(size_t ring = 0; ring < (n + 1) / 2; ring++)
	{

		for(size_t j = ring; j < n - ring; j++) {
			m[ring][j] = num;
			num++;
		}

		for(size_t i = ring + 1; i < n - ring; i++) {
			m[i][n - ring - 1] = num;
			num++;
		}

		for(size_t j = n - ring - 1; j --> ring;) { // index in range [n - ring - 1; ring]
			m[n - ring - 1][j] = num;
			num++;			
		}

		for(size_t i = n - ring - 1; i --> ring + 1;) { // index in range [n - ring - 2; ring + 1]
			m[i][ring] = num;
			num++;
		}
	
	}

	for(size_t i = 0; i < n; i++) {
		for(size_t j = 0; j < n; j++)
			printf("%d ", m[i][j]);
		printf("\n");
	}
	return 0;
}
