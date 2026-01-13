#include <cuda_runtime.h>
#include <stdint.h>

// DEVRİM: Standart miner'ların hantallığını bitiren sıkıştırma algoritması
__device__ __forceinline__ uint32_t x_compress(uint32_t x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

__global__ void X_Miner_Kernel(uint32_t* dag, uint32_t* output, uint32_t nonce) {
    // SHARED MEMORY: L1 Cache hızında işlem alanı
    __shared__ uint32_t cache[256];
    
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    cache[threadIdx.x % 256] = x_compress(dag[tid % 1024]);
    __syncthreads();

    uint32_t state = nonce ^ tid;
    
    // UNROLLED LOOP: Döngüsel gecikmeyi sıfırlıyoruz
    #pragma unroll 32
    for(int i = 0; i < 32; i++) {
        state = x_compress(state ^ cache[i % 256]);
    }

    output[tid] = state;
}

extern "C" void launch_x_kernel(uint32_t* d_dag, uint32_t* d_out, uint32_t nonce) {
    X_Miner_Kernel<<<4096, 256>>>(d_dag, d_out, nonce);
}
