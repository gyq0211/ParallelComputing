#include <iostream>
#include "ee155_utils.hxx"
#include "matrix.hxx"
#include <cuda.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_functions.h"
using namespace std;
#include <cuda_runtime.h>
#include <math.h>
#include <stdlib.h>
const int BS = 32;	// The blocks are BS x BS.


///////////////////////////////
// This is the CUDA kernel function for you to write.
//
__global__ void mat_mult (float *d_A, float *d_B, float *d_C, int N) {
    //int rB=blockIdx...,
    int rB = blockIdx.x;
    int cB = blockIdx.y;
    int rI = threadIdx.x;
    int cI = threadIdx.y;

    __shared__ float SA[BS][BS], SB[BS][BS];
    //printf("In thread with r=(%d,%d) c=(%d,%d)\n", rB,rI,cB,cI);

    //...
    
    float Temp = 0.0;
	SA[cI][rI] = 0.0;
	SB[cI][rI] = 0.0;
    for (size_t kB=0; kB<N; ++kB)
    {
        SA[cI][rI] = *(d_A + rB * BS * N * BS + cI * N * BS + kB * BS + rI);
        SB[cI][rI] = *(d_B + kB * BS * N * BS + cI * N * BS + cB * BS + rI);
		__syncthreads();
        for (size_t j=0; j<BS; ++j)
        {
            Temp += ((int)(SA[cI][j]))*SB[j][rI];
            __syncthreads();
        }
    }
    *(d_C + rB * BS * N * BS + cI * N * BS + cB * BS + rI) = Temp;
}


///////////////////////////////
// This is the host function for you to write.
// It allocates memory and moves data between CPU<->GPU
void Matrix::mpy1 (const Matrix &A, const Matrix &B, int BS) {

    // Copy A from host memory to device memory.
    int numElem=N()*N(), sizeBytes = numElem*4;
    float *d_A = NULL;
    cudaError_t err = cudaMalloc((void **)&d_A, sizeBytes);
    ERR_CHK (err, "Failed to allocate device matrix A");

    err = cudaMemcpy (d_A, A.data.data(), sizeBytes, cudaMemcpyHostToDevice);
    ERR_CHK (err, "Failed to copy matrix A from host to device");

    // Allocate device memory for B.
    //...
    float* d_B = NULL;
    err = cudaMalloc((void**)&d_B, sizeBytes);
    ERR_CHK(err, "Failed to allocate device matrix B");

    // Copy B from host memory to device memory.
    //...
    err = cudaMemcpy(d_B, B.data.data(), sizeBytes, cudaMemcpyHostToDevice);
    ERR_CHK(err, "Failed to copy matrix B from host to device");

    // Allocate device memory for C.
    //...
    float* d_C = NULL;
    err = cudaMalloc((void**)&d_C, sizeBytes);
    ERR_CHK(err, "Failed to allocate device matrix C");

    // Launch the CUDA Kernel
    //...
    int NBLK = this->N() / BS;
    dim3 thBlocks(NBLK, NBLK), threads(BS, BS);
    mat_mult <<<thBlocks, threads>>> (d_A, d_B, d_C, NBLK);

    // Copy the result from device memory to host memory.
    //...
    err = cudaMemcpy(this->data.data(), d_C, sizeBytes, cudaMemcpyDeviceToHost);
    ERR_CHK(err, "Failed to copy data back from GPU to CPU");

    // Free device memory.
    err = cudaFree(d_A);
    ERR_CHK (err, "Failed to free CUDA matrix A");
    //...
    err = cudaFree(d_B);
    ERR_CHK(err, "Failed to free CUDA matrix B");
    err = cudaFree(d_C);
    ERR_CHK(err, "Failed to free CUDA matrix C");
}
