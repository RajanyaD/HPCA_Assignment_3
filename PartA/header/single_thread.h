void singleThread(int N, int *mat_A, int *mat_B, int *mat_C) {
	for (int j = 0; j < N; j += 2) {
		int col_1[N];
		int col_2[N];

		for (int k = 0; k < N; k++) {
			col_1[k] = mat_B[(k*N) + j];
			col_2[k] = mat_B[(k*N) + j+1];
		} 

		for (int i = 0; i < N; i += 2) {
			int sum = 0;

			for (int k = 0; k < N; k++) {
				sum += mat_A[(i*N)+k] * col_1[k];
				sum += mat_A[(i*N)+k] * col_2[k];
				sum += mat_A[(i+1)*(N)+k] * col_1[k];
				sum += mat_A[(i+1)*(N)+k] * col_2[k];
			}

			int r = i >> 1;
			int offset = j >> 1;
			
			mat_C[(r*(N>>1)) + offset] = sum;
		}
	}
}
