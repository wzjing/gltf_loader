//
// Created by wzjing on 4/22/2018.
//

#ifndef LIBUSE_GLSL_LOADER_H
#define LIBUSE_GLSL_LOADER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "lib/cJson/cJSON.h"

typedef struct {
    void *array;
    size_t size;
} GLTF_Array;

/** Accessor **/
typedef struct {
    const char *name;
    int componentType;
    int count;
    double *min;
    size_t min_size;
    double *max;
    size_t max_size;
    const char *type;
    int bufferView;
    int byteOffset;
} GLTF_Accessor;

/** Asset **/

typedef struct {
    const char *generator;
    const char *version;
} GLTF_Asset;

/** Buffer **/

typedef struct {
    const char *name;
    int byteLength;
    const char *uri;
} GLTF_Buffer;

/** BufferView **/

typedef struct {
    const char *name;
    int buffer;
    long byteLength;
    long byteOffset;
    int byteStride;
    long target;
} GLTF_BufferView;

/** Image **/

typedef struct {
    const char *name;
    const char *uri;
} GLTF_Image;

/** Material **/

typedef struct {
    int index;
} GLTF_BaseColorTexture;

typedef struct {
    GLTF_BaseColorTexture baseColorTexture;
    int *baseColorFactor;
    size_t baseColorFactor_size; // Size of baseColorFactor
    int metallicFactor;
    int roughnessFactor;
} GLTF_PbrMetallicRoughess;

typedef struct {
    const char *name;
    GLTF_PbrMetallicRoughess pbrMetallicRoughess;
    double *emissiveFactor;
    size_t emissiveFactor_size; // Size of the emissiveFactor
    const char *alphaMode; // OPAQUE OR TRANSPARENT
    int doubleSided; // This is a boolean type
} GLTF_Material;

/** Mesh **/

typedef struct {
    const char *key;
    void *value;
} GLTF_Pair;

typedef struct {
    GLTF_Pair *attributes;
    size_t attributes_size;
    int indices;
    int material;
    int mode;
} GLTF_Primitive;

typedef struct {
    const char *name;
    GLTF_Primitive *primitives;
    size_t primitives_size;
} GLTF_Mesh;

/** Node **/

typedef struct {
    const char *name;
    int mesh;
} GLTF_Node;

/** Sampler **/

typedef struct {
    int magFilter;
    int minFilter;
    int wrapS;
    int wrapT;
} GLTF_Sampler;

/** Scene **/

typedef struct {
    int *nodes;
    size_t nodes_size;
} GLTF_Scene;

/** Texture **/

typedef struct {
    const char *name;
    int sampler;
    int source;
} GLTF_Texture;

/** The main GLTF object **/

typedef struct {
    // Accessors
    GLTF_Accessor *accessors;
    size_t accessors_size;

    // Asset
    GLTF_Asset asset;

    // Buffers
    GLTF_Buffer *buffers;
    size_t buffers_size;

    // BufferViews
    GLTF_BufferView *bufferViews;
    size_t bufferViews_size;

    // Images
    GLTF_Image *images;
    size_t images_size;

    // Materials
    GLTF_Material *materials;
    size_t materials_size;

    // Meshes
    GLTF_Mesh *meshes;
    size_t meshes_size;

    // Nodes
    GLTF_Node *nodes;
    size_t nodes_size;

    // Sampler
    GLTF_Sampler *samplers;
    size_t samplers_size;

    // Scenes
    int scene;
    GLTF_Scene *scenes;
    size_t scenes_size;

    // Textures
    GLTF_Texture *textures;
    size_t textures_size;
} GLTF;

void load_gltf(GLTF **gltf, const char *json);

void load_file(char **content, const char *filename);

void load_gltf_file(GLTF **gltf, const char *file);

#ifdef __cplusplus
};
#endif

#endif //LIBUSE_GLSL_LOADER_H
