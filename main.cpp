#include <iostream>
#include "gltf_loader.h"

int main() {
    GLTF *gltf;
    load_gltf_file(&gltf, "cube.gltf");
    printf("count: %d\n"
           "max_size: %zu\n"
           "name1: %s",
           gltf->accessors[0].count,
           gltf->accessors[0].max_size,
           gltf->accessors[1].name);
    return 0;
}