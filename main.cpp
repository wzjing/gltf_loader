#include <iostream>
#include "gltf_loader.h"
#include <fstream>
#include <vector>

using namespace std;

char *loadBin(const char *filename, long offset, long length, int stride) {
    ifstream input(filename, ios::binary);
    vector<char> buffer((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
    return buffer.data();
}

void printBin(char *bin, size_t size) {
    printf("Bin:\n");
    for (int i = 0; i < size; ++i) {
        char n = bin[i];
        while (n) {
            if (n & 1) printf("1");
            else printf("0");
            n >>= 1;
        }
    }
    printf("\n");
}

int main() {
//    GLTF *gltf;
//    load_gltf_file(&gltf, "cube.gltf");
//    GLTF_BufferView bufferView = gltf->bufferViews[0];
//    printf("Offset: %ld, Length: %ld, Stride: %d\n", bufferView.byteOffset, bufferView.byteLength, bufferView.byteStride);
//    char *bin = loadBin("cube.bin", bufferView.byteOffset, bufferView.byteLength, bufferView.byteStride);
//    printBin(bin, 100);
    bool isTrue = 'a' & 1;
    printf("%d", isTrue);
    return 0;
}