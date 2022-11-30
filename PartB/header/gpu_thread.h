#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include <time.h>
#include<cuda_runtime.h>

#define N 1024

_global_ void matrixMultiply(const int *a, const int *b, int *c){
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;

    if ((!(row %2)) && (row<N) && (col<N-1))
    {
    int sum=0;
        for(int i = 0; i < N; i++) 
        {
        sum += a[row * N + i] * b[i * N + col];
        sum += a[(row+1) * N + i] * b[i * N + col];
        sum += a[row * N + i] * b[i * N + (col+1)];
        sum += a[(row+1) * N + i] * b[i * N + (col+1)];
        }
        row = row>>1;
        col = col>>1;

        int indexC = row * (N>>1) + col;
        c[indexC] = sum;
    }
}


int main() {
  
  int *a=(int*)malloc(N*N,sizeof(int));
  int *b=(int*)malloc(N*N,sizeof(int));
  int *c=(int*)malloc((N*N)/4,sizeof(int));
  int *dev_a, *dev_b, *dev_c;
  for(int i=0;i<N*N;i++)
  {
    a[i]=rand()%10+9;
    b[i]=rand()%10+9;
  }
  int total = N*N;

  cudaError_t result;
  result = cudaMalloc((void**)&dev_a, total*sizeof(int));
  if(result!=cudaSuccess){
  printf("Error occured in allocating device memory\n");
  }
  result = cudaMalloc((void**)&dev_b, total*sizeof(int));
  if(result!=cudaSuccess){
  printf("Error occured in allocating device memory\n");
  }
  result = cudaMalloc((void**)&dev_c, o_total*sizeof(int));
  if(result!=cudaSuccess){
  printf("Error occured in allocating device memory\n");
  }

  // Copy data to the device
  result=cudaMemcpy(dev_a, a, total*sizeof(int), cudaMemcpyHostToDevice);
  if(result!=cudaSuccess){
  printf("Error occured in copying data from host to device\n");
  }
  result = cudaMemcpy(dev_b, b, total*sizeof(int), cudaMemcpyHostToDevice);
   if(result!=cudaSuccess){
  printf("Error occured in copying data from host to device\n");
  }

  int THREADS = 32;

  int BLOCKS = N / THREADS;

  dim3 threads(THREADS, THREADS);
  dim3 blocks(BLOCKS, BLOCKS);

  // Launching...
  printf("Calling matrixMultiply\n);");
  matrixMultiply<<<blocks, threads>>>(dev_a,dev_b,dev_c);

  // Back to the host...
  result = cudaMemcpy(c, dev_c, (total/4)*sizeof(int),cudaMemcpyDeviceToHost);
  if(result!=cudaSuccess){
  printf("Error occured in copying data from device to host\n");
  }

  printf("Done successfully\n");

  // Free memory...
  cudaFree(dev_a);
  cudaFree(dev_b);
  cudaFree(dev_c);

 for(i=0;i<N/2*N/2;i++)
   printf("%d\t",c[i]);
  

  return 0;
}