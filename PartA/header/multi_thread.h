#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#define N 1024
// Create other necessary functions here
int num_threads = 4;
int *mat1, *mat2, *mat3;

void *split(void *arg)
{
  int threadid, thread_cover, row_start, row_end;
  double sum;
  
  threadid = *(int *)(arg); 
  thread_cover = N / num_threads;
  row_start = threadid * thread_cover;
  row_end = (threadid+1) * thread_cover;

    for (int j = row_start; j < row_end; j+=4)
        {   
            int col[N],col2[N],col3[N],col4[N];
            for (int k=0; k<N; k++){
                col[k] = mat2[(k*N) + j];
                col2[k] = mat2[(k*N) + j+1];
                col3[k] = mat2[(k*N) + j+2];
                col4[k] = mat2[(k*N) + j+3];       
            } 
            for(int i=0; i<N; i+=4){
                int sum = 0;
                for (int k=0; k<N; k+=8){
                  for(int jj=0;jj<4;jj++){
                    sum += mat1[(i+jj)*(N)+k] * col[k];
                    sum += mat1[(i+jj)*(N)+k] * col2[k];
                    sum += mat1[(i+jj)*(N)+k] * col3[k];
                    sum += mat1[(i+jj)*(N)+k] * col4[k]; 
                  }
                }
            int r = i>>2;
            int s = j>>2;
            mat3[(r*(N>>1))+s] = sum; 
        }  
    }
}



// Fill in this function
void multiThread(int n, int *matA, int *matB, int *output)
{
  mat1 = &matA[0];
  mat2 = &matB[0];
  mat3 = &output[0];
  pthread_t *threads;

  threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));

  for (int i = 0; i < num_threads; i++) {
    int *tid;
    tid = (int *)malloc( sizeof(int));
    *tid = i;
    pthread_create( &threads[i], NULL, split, (void *)tid );
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join( threads[i], NULL );
  }  
}
