#include <iostream>
#include "gltf_loader.h"
#include <fstream>

#define BUFFER_SIZE 2

using namespace std;

char *loadBintary(const char *filename, long offset, long length) {
    char *buffer;
    ifstream file("cube.bin", ios::in | ios::binary | ios::ate);
    if (file.is_open()) {
        buffer = new char[length];
        file.seekg(0, ios::beg);
        file.read(buffer, length);
        file.close();
    } else {
        printf("Unable to open file: %s", filename);
    }
    return buffer;
}

int main() {
//    GLTF *gltf;
//    load_gltf_file(&gltf, "cube.gltf");
    loadBintary("cube.bin", 1000, 5000);
    return 0;
}