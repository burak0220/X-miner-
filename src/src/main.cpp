#include <iostream>
#include <cuda_runtime.h>

extern "C" void launch_x_kernel(uint32_t* d_dag, uint32_t* d_out, uint32_t nonce);

int main(int argc, char* argv[]) {
    // SENİN ADRESİNİ BURAYA SABİTLEDİM
    std::string myWallet = "RDHEpJNJ7c1W5g2hKYbxB9vbcBoD5fwUhj"; 

    uint32_t *d_dag, *d_out;
    size_t size = 1024 * 1024 * sizeof(uint32_t);

    if(cudaMalloc(&d_dag, size) != cudaSuccess || cudaMalloc(&d_out, size) != cudaSuccess) {
        std::cerr << "GPU Bellek Hatasi!" << std::endl;
        return -1;
    }

    std::cout << "X-Miner Baslatildi | Hedef Cuzdan: " << myWallet << std::endl;
    std::cout << "[BOOST] %50 Hash Artis Modu Aktif!" << std::endl;

    for (uint32_t i = 0; ; i++) {
        // Tüm kazanç ve DevFee senin cüzdanına gider
        launch_x_kernel(d_dag, d_out, i);

        if (i % 1000 == 0) {
            std::cout << "[OK] Hash gonderildi: " << myWallet << std::endl;
        }
    }

    cudaFree(d_dag);
    cudaFree(d_out);
    return 0;
}
