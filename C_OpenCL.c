#define _CRT_SECURE_NO_DEPRECATE
#define SIZE 1024
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
/*
-GPU Architecture
-OpenCL
*/
/*
GPU Architecture
-Data independence between triangles and pixels
-SIMD single instruction multiple data 
-Many threads -> No synchronization

-Streaming Multiprocessors ->multiple streaming processs
-Warp : a group of instruction execution
*/

/*
Open Computing Language (Source : Lecture note and https://subscription.packtpub.com/book/application-development/9781849692342/1/ch01lvl1sec12/an-example-of-opencl-program)
-> framework for parallel computing
from mobile devices to supercomputers
Supported by many vendors e.g. Intel, Apple, AMD, ARD, NVIDIA...

-Host program + OpenCL program (a set of kernels-Basic unit of executable code on compute devices such as CPU GPU)
-OpenCL Platform model - single host + one or more compute devices
-Host processor + main memory
		<Interconnect bus>
Compute device A, B, C, ... with own global memory and constant memory
-Compute devices may not access main memory
-Compute device is a collection of Compute Units(CU)
--> Each CU has local memory (Not accessable by host)
-->CU is SM in GPU and has Processing Elements(PE, ALU in SM
CU with local memory (Shared Context)  > PE(ALU) with private memory(Context) 

Work-items are organized into work-groups
Work-group execues on a compute unit
A set of work-groups are on an SM
*/
//Host program
//Kernel source code -run for each work-item created
const char* kernel_src =
"__kernel void vec_add(float alpha, "
"__global const float* A, "
"__global const float* B, "
"__global float* C) {"
"	int id= get_global_id(0);"
"	C[id]=A[id]+B[id];"
"}";

int main()
{
	float alpha = 2.0;
	float* A;
	float* B; 
	float* C;
	size_t sizeA, sizeB, sizeC;

	sizeA = sizeof(float)* SIZE;
	sizeB = sizeof(float) * SIZE;
	sizeC = sizeof(float) * SIZE;

	//Allocate space for vectors A,B, and C
	A = (float*)malloc(sizeA);
	B = (float*)malloc(sizeB);
	C = (float*)malloc(sizeC);
	int i;
	for(i = 0; i < SIZE ; i++)
	{
		A[i] = (float) i;
		B[i] = (float)i * alpha;
		C[i] = 0;
	}



	//Get platform and device info
	cl_platform_id* platform = NULL;
	cl_uint num_platforms;

	//Set up the platform
	//First get the possible number of OpenCL platforms in the system at &num_platforms
	cl_int clStatus=clGetPlatformIDs(0, NULL, &num_platforms);
	//Second, allocate memory
	platform = (cl_platform_id*)malloc(sizeof(cl_platform_id) * num_platforms);
	//Get information(ID) of platforms in the array of platforms (second parameter)
	clStatus = clGetPlatformIDs(num_platforms, platform, NULL);


	//Get the device list and choose the device you want
	//Same procedure as the platforms
	cl_device_id* device_list = NULL;
	cl_uint num_devices;

	clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
	device_list = (cl_device_id*)malloc(sizeof(cl_device_id) * num_devices);
	clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_ALL, num_devices, device_list, NULL);

	//Create one OpenCL context for each device in the platform
	//-> environment in which the kernel executes (memory management and synchronization is defined)
	cl_context context;
	context = clCreateContext(NULL, num_devices, device_list, NULL, NULL, &clStatus);

	//Create a command queue (executed on a specific compute device)
	cl_command_queue queue=clCreateCommandQueueWithProperties(context, device_list[0], 0, &clStatus);

	//Create memory buffers on the device for each vector
	cl_mem A_clmem = clCreateBuffer(context, CL_MEM_READ_ONLY, SIZE*sizeof(float),NULL,&clStatus);
	cl_mem B_clmem = clCreateBuffer(context, CL_MEM_READ_ONLY, SIZE*sizeof(float),NULL,&clStatus);
	cl_mem C_clmem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, SIZE * sizeof(float), NULL, &clStatus);
	
	//Copy the buffer A and B to the device 
	clStatus = clEnqueueWriteBuffer(queue, A_clmem, CL_TRUE, 0, SIZE * sizeof(float), A, 0, NULL, NULL);
	clStatus = clEnqueueWriteBuffer(queue, B_clmem, CL_TRUE, 0, SIZE * sizeof(float), B, 0, NULL, NULL);

	//Create a program from the kernel source
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&kernel_src, NULL, &clStatus);

	//Build the program (compile and link the kernel code)
	clStatus = clBuildProgram(program, 1, device_list, NULL, NULL, NULL);

	//Create OpenCL kernel
	cl_kernel kernel = clCreateKernel(program, "vec_add", &clStatus);

	//Set the arguments of the kernel
	clStatus = clSetKernelArg(kernel, 0, sizeof(float), (void*)&alpha);
	clStatus = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&A_clmem);
	clStatus = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&B_clmem);
	clStatus = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&C_clmem);

	//Execute the OpenCL kernel on the list
	size_t global_size = SIZE; 
	size_t local_size = 64;

	clStatus = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);

	//Read the cl memory C_clmem on device to the host variable C
	clStatus = clEnqueueReadBuffer(queue, C_clmem, CL_TRUE, 0, SIZE * sizeof(float), C,0,NULL,NULL);

	//Clean up and wait for all the commands to complete
	clStatus = clFlush(queue);
	clStatus = clFinish(queue);

	//Display result to the screen
	for (i = 0; i < SIZE; i++)
	{
		printf("%f*%f+%f = %f\n", alpha, A[i], B[i], C[i]);
	}

	//Finally release all OpenCL allocated objects and host buffers

	clStatus = clReleaseKernel(kernel);
	clStatus = clReleaseProgram(program);
	clStatus = clReleaseMemObject(A_clmem);
	clStatus = clReleaseMemObject(B_clmem);
	clStatus = clReleaseMemObject(C_clmem);
	clStatus = clReleaseCommandQueue(queue);
	clStatus = clReleaseContext(context);
	free(A);
	free(B);
	free(C);
	free(platform);
	free(device_list);

	return 0;
}