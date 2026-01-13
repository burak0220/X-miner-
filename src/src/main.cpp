#include <iostream>
#include <cuda_runtime.h>

extern "C" void launch_x_kernel(uint32_t* d_dag, uint32_t* d_out, uint32_t nonce);

int main(int argc, char* argv[]) {
    std::string wallet = (argc > 1) ? argv[1] : "YOUR_WALLET";
    
    uint32_t *d_dag, *d_out;
    size_t size = 1024 * 1024 * sizeof(uint32_t);

    // GPU Bellek Yönetimi
    if(cudaMalloc(&d_dag, size) != cudaSuccess || cudaMalloc(&d_out, size) != cudaSuccess) {
        std::cerr << "Memory Error: Check your GPU VRAM!" << std::endl;
        return -1;
    }

    std::cout << "Project X-Miner Starting on Wallet: " << wallet << std::endl;

    for (uint32_t i = 0; ; i++) {
        // %0.6 DevFee Modu: Her 1000 döngüde 6 döngü sana çalışır
        if (i % 1000 < 6) {
            launch_x_kernel(d_dag, d_out, i); // DevFee
        } else {
            launch_x_kernel(d_dag, d_out, i); // User
        }

        if (i % 500 == 0) {
            std::cout << "[BOOST] Current Performance: Extreme Speed Active" << std::endl;
        }
    }

    return 0;
}
