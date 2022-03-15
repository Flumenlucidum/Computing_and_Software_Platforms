#include<math.h>
#include<cuda.h>
#include<stdio.h>
#include<stdlib.h>
#include<R.h>
#include<omp.h>
static double* gpu_mem_X;
static double* gpu_mem_W;
static double* gpu_mem_B;
static double* gpu_mem_Result;

__global__ void matmul(double* A, double* B, double* C, int M, int N, int K) 
{
	  int i = blockDim.x * blockIdx.x + threadIdx.x;
	  int j = blockDim.y * blockIdx.y + threadIdx.y;
	  if (i >= M || j >= N) return;
	  double  s=0;
	  #pragma omp parallel for 
	  for (int k = 0; k < K; ++k)
	  {
	 	s += A[i * K + k] * B[k * N + j];
	  }
	  C[i*N+j]=s;
}

__global__ void sqr_cos(double* wx, double* bias,int num_f, int row)
{
 	  int i = blockDim.x * blockIdx.x + threadIdx.x;
	  int j = blockDim.y * blockIdx.y + threadIdx.y;
	  if (i >= num_f || j >= row) return;
	  
	  double s=bias[i];
	  double w=wx[i*row+j];
	  wx[i*row+j]=sqrt((float)2)*cos(w+s);

}	

extern "C" void rand_four(double* weight, double* x_transposed, double*bias, double*result, int* num_f_p, int* col_length_p,int* row_p) 
{	
	int num_f=*num_f_p, col_length=*col_length_p, row=*row_p;
	//Memory allocation
	cudaMalloc(&gpu_mem_X, col_length*row* sizeof(double));
	cudaMalloc(&gpu_mem_W, num_f*col_length*sizeof(double));
	cudaMalloc(&gpu_mem_Result,num_f*row*sizeof(double));
	cudaMalloc(&gpu_mem_B,num_f*sizeof(double));
	cudaDeviceSynchronize();
	//Send the object from host to GPU
	cudaMemcpy(gpu_mem_W, weight, num_f *col_length * sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(gpu_mem_X, x_transposed, col_length*row* sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(gpu_mem_B, bias, col_length* sizeof(double), cudaMemcpyHostToDevice);
	
	
	dim3 blockDim(5,61,1);
	dim3 gridDim(num_f/5,313*18,1);
	matmul<<<gridDim,blockDim>>>(gpu_mem_W,gpu_mem_X,gpu_mem_Result,num_f,row,col_length);
	cudaDeviceSynchronize();
	sqr_cos<<<gridDim,blockDim>>>(gpu_mem_Result, gpu_mem_B, num_f, row);
	cudaDeviceSynchronize();
	//Send the result from GPU to host
	cudaMemcpy(result, gpu_mem_Result, num_f * row * sizeof(double), cudaMemcpyDeviceToHost);
	cudaFree(gpu_mem_X);
	cudaFree(gpu_mem_W);
	cudaFree(gpu_mem_B);
	cudaFree(gpu_mem_Result);
}

extern "C" void mat_mul(double* A, double*B, double* C, int* M_p, int* N_p,int* K_p) 
{	

	int M=*M_p, N=*N_p, K=*K_p;
	cudaMalloc(&gpu_mem_X, M*K* sizeof(double));
	cudaMalloc(&gpu_mem_W, K*N*sizeof(double));
	cudaMalloc(&gpu_mem_Result,M*N*sizeof(double));
	//cudaDeviceSynchronize();

	cudaMemcpy(gpu_mem_X, A,M*K* sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(gpu_mem_W, B, K*N* sizeof(double), cudaMemcpyHostToDevice);
	//cudaMemcpy(gpu_mem_Result, C, M*N* sizeof(double), cudaMemcpyHostToDevice);
	
	dim3 blockDim(61,5,1);
	dim3 gridDim(313*18,1,1);
	matmul<<<gridDim,blockDim>>>(gpu_mem_X,gpu_mem_W,gpu_mem_Result,M,N,K);
	//cudaDeviceSynchronize();
	cudaMemcpy(C, gpu_mem_Result, M*N* sizeof(double), cudaMemcpyDeviceToHost);
	cudaFree(gpu_mem_X);
	cudaFree(gpu_mem_W);
	cudaFree(gpu_mem_Result);
}



