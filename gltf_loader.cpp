#include "gltf_loader.h"
#include <cstdio>
#include <cstring>
#include "lib/cJson/cJSON.h"

#define BUFFER_SIZE 1024

extern "C" {

void load_file(char **content, const char *filename) {
    FILE *stream = fopen(filename, "r+t");
    if (stream != nullptr) {
        char buffer[BUFFER_SIZE] = "\0";
        size_t content_size = BUFFER_SIZE;
        *content = new char[content_size];
        memset(*content, 0, 1);
        while (!feof(stream)) {
            // Reset and read
            memset(buffer, 0, BUFFER_SIZE);
            fread(buffer, sizeof(char), 1024, stream);
            // Append to result
            *content = static_cast<char *>(realloc(*content, content_size += BUFFER_SIZE));
            strcat(*content, buffer);
            fflush(stream);
        }
        fclose(stream);
//        logd("Read: \n--------%10s--------\n%s\n--------    end   --------", filename, *content);
    } else {
        printf("Unable to open file: %s\n", filename);
    }
}

int decode_int(cJSON *root, const char *name) {
    cJSON *obj = cJSON_GetObjectItem(root, name);
    if (obj != nullptr) {
        return obj->valueint;
    } else {
        return 0;
    }
}

double decode_double(cJSON *root, const char *name) {
    cJSON *obj = cJSON_GetObjectItem(root, name);
    if (obj != nullptr) {
        return obj->valuedouble;
    } else {
        return 0.0;
    }
}

char *decode_string(cJSON *root, const char *name) {
    cJSON *obj = cJSON_GetObjectItem(root, name);
    if (obj->valuestring != nullptr) {
        return obj->valuestring;
    } else {
        return nullptr;
    }
}

GLTF_Array decode_int_array(cJSON *root, const char *name) {
    int *result;
    cJSON *array = cJSON_GetObjectItem(root, name);
    size_t arrayLength = cJSON_GetArraySize(array);
    result = new int[arrayLength];
    memset(result, 0, sizeof(int) * arrayLength);
    for (int i = 0; i < arrayLength; ++i) {
        result[i] = cJSON_GetArrayItem(array, i)->valueint;
    }
    return {result, arrayLength};
}

GLTF_Array decode_double_array(cJSON *root, const char *name) {
//    printf("decode_double_array(%s)\n", name);
    double *result;
    cJSON *array = cJSON_GetObjectItem(root, name);
    size_t arrayLength = cJSON_GetArraySize(array);
    result = new double[arrayLength];
    memset(result, 0, sizeof(double) * arrayLength);
    for (int i = 0; i < arrayLength; ++i) {
        result[i] = cJSON_GetArrayItem(array, i)->valuedouble;
//        printf("double item: %lf\n", cJSON_GetArrayItem(array, i)->valuedouble);
    }
    return {result, arrayLength};
}

GLTF_Pair decode_pair(cJSON *element) {
    return {element->string, element->valuestring};
}

GLTF_Array decode_pairs(cJSON *root, const char *name) {
    GLTF_Pair *result;
    cJSON *dictionary = cJSON_GetObjectItem(root, name);
    size_t dictionaryLength = cJSON_GetArraySize(dictionary);
    result = new GLTF_Pair[dictionaryLength];
    memset(result, 0, sizeof(GLTF_Pair) * dictionaryLength);
    for (int i = 0; i < dictionaryLength; ++i) {
        cJSON *pair = cJSON_GetArrayItem(dictionary, i);
        result[i] = {pair->string, pair->valuestring};
    }
    return {result, dictionaryLength};
}

}

void load_gltf(GLTF **gltf, const char *json) {
    cJSON *root = cJSON_Parse(json);
    if (!root) {
        printf("Error Parsing gltf\n");
        return;
    }

    *gltf = new GLTF();

//    printf("Decode Accessors\n");
    // Decode Accessors
    cJSON *accessors = cJSON_GetObjectItem(root, "accessors");
    (*gltf)->accessors_size = cJSON_GetArraySize(accessors);
    (*gltf)->accessors = new GLTF_Accessor[(*gltf)->accessors_size];
    for (int i = 0; i < (*gltf)->accessors_size; ++i) {
        cJSON *accessor = cJSON_GetArrayItem(accessors, i);
        (*gltf)->accessors[i].name = decode_string(accessor, "name");
        (*gltf)->accessors[i].componentType = decode_int(accessor, "componentType");
        (*gltf)->accessors[i].count = decode_int(accessor, "count");
        GLTF_Array min = decode_double_array(accessor, "min");
        (*gltf)->accessors[i].min = static_cast<double *>(min.array);
        (*gltf)->accessors[i].min_size = min.size;
        GLTF_Array max = decode_double_array(accessor, "max");
        (*gltf)->accessors[i].max = static_cast<double *>(max.array);
        (*gltf)->accessors[i].max_size = max.size;
        (*gltf)->accessors[i].type = decode_string(accessor, "type");
        (*gltf)->accessors[i].bufferView = decode_int(accessor, "bufferView");
        (*gltf)->accessors[i].byteOffset = decode_int(accessor, "byteOffset");
    }

    // Decode Asset
//    printf("Decode Asset\n");
    cJSON *asset = cJSON_GetObjectItem(root, "asset");
    (*gltf)->asset.generator = decode_string(asset, "generator");
    (*gltf)->asset.version = decode_string(asset, "version");

    // Decode Buffers
//    printf("Decode Buffers\n");
    cJSON *buffers = cJSON_GetObjectItem(root, "buffers");
    (*gltf)->buffers_size = cJSON_GetArraySize(buffers);
    (*gltf)->buffers = new GLTF_Buffer[(*gltf)->buffers_size];
    for (int i = 0; i < (*gltf)->buffers_size; ++i) {
        cJSON *buffer = cJSON_GetArrayItem(buffers, i);
        (*gltf)->buffers[i].name = decode_string(buffer, "name");
        (*gltf)->buffers[i].byteLength = decode_int(buffer, "byteLength");
        (*gltf)->buffers[i].uri = decode_string(buffer, "uri");
    }

    // Decode BufferViews
//    printf("Decode BufferViews\n");
    cJSON *bufferViews = cJSON_GetObjectItem(root, "bufferViews");
    (*gltf)->bufferViews_size = cJSON_GetArraySize(bufferViews);
    (*gltf)->bufferViews = new GLTF_BufferView[(*gltf)->bufferViews_size];
    for (int i = 0; i < (*gltf)->bufferViews_size; ++i) {
        cJSON *bufferView = cJSON_GetArrayItem(bufferViews, i);
        (*gltf)->bufferViews[i].name = decode_string(bufferView, "name");
        (*gltf)->bufferViews[i].buffer = decode_int(bufferView, "buffer");
        (*gltf)->bufferViews[i].byteLength = decode_int(bufferView, "byteLength");
        (*gltf)->bufferViews[i].byteOffset = decode_int(bufferView, "byteOffset");
        (*gltf)->bufferViews[i].byteStride = decode_int(bufferView, "byteStride");
        (*gltf)->bufferViews[i].target = decode_int(bufferView, "target");
    }

    // Decode Images
//    printf("Decode Images\n");
    cJSON *images = cJSON_GetObjectItem(root, "images");
    (*gltf)->images_size = cJSON_GetArraySize(images);
    (*gltf)->images = new GLTF_Image[(*gltf)->images_size];
    for (int i = 0; i < (*gltf)->images_size; ++i) {
        cJSON *image = cJSON_GetArrayItem(images, i);
        (*gltf)->images[i].name = decode_string(image, "name");
        (*gltf)->images[i].uri = decode_string(image, "uri");
    }

    // Decode Materials
//    printf("Decode Materials\n");
    cJSON *materials = cJSON_GetObjectItem(root, "materials");
    (*gltf)->materials_size = cJSON_GetArraySize(materials);
    (*gltf)->materials = new GLTF_Material[(*gltf)->materials_size];
    for (int i = 0; i < (*gltf)->materials_size; ++i) {
        cJSON *material = cJSON_GetArrayItem(materials, i);
        (*gltf)->materials[i].name = decode_string(material, "name");
        GLTF_Array emissiveFactor = decode_double_array(material, "emissiveFactor");
        (*gltf)->materials[i].emissiveFactor_size = emissiveFactor.size;
        (*gltf)->materials[i].emissiveFactor = static_cast<double *>(emissiveFactor.array);
        (*gltf)->materials[i].alphaMode = decode_string(material, "alphaMode");
        (*gltf)->materials[i].doubleSided = decode_int(material, "doubleSided");
    }

    // Decode Meshes
//    printf("Decode Meshes\n");
    cJSON *meshes = cJSON_GetObjectItem(root, "meshes");
    (*gltf)->meshes_size = cJSON_GetArraySize(meshes);
    (*gltf)->meshes = new GLTF_Mesh[(*gltf)->meshes_size];
    for (int i = 0; i < (*gltf)->meshes_size; ++i) {
        cJSON *mesh = cJSON_GetArrayItem(meshes, i);
        (*gltf)->meshes[i].name = decode_string(mesh, "name");
        cJSON *primitives = cJSON_GetObjectItem(mesh, "primitives");
        (*gltf)->meshes[i].primitives_size = cJSON_GetArraySize(primitives);
        (*gltf)->meshes[i].primitives = new GLTF_Primitive[(*gltf)->meshes[i].primitives_size];
        for (int j = 0; j < (*gltf)->meshes[i].primitives_size; ++j) {
            cJSON *primitive = cJSON_GetArrayItem(primitives, j);
            GLTF_Array attributes = decode_pairs(primitive, "attributes");
            (*gltf)->meshes[i].primitives[j].attributes = static_cast<GLTF_Pair *>(attributes.array);
            (*gltf)->meshes[i].primitives[j].attributes_size = attributes.size;
            (*gltf)->meshes[i].primitives[j].indices = decode_int(primitive, "indices");
            (*gltf)->meshes[i].primitives[j].material = decode_int(primitive, "material");
            (*gltf)->meshes[i].primitives[j].mode = decode_int(primitive, "mode");
        }
    }

    // Decode Nodes
//    printf("Decode Nodes\n");
    cJSON *nodes = cJSON_GetObjectItem(root, "nodes");
    (*gltf)->nodes_size = cJSON_GetArraySize(nodes);
    (*gltf)->nodes = new GLTF_Node[(*gltf)->nodes_size];
    for (int i = 0; i < (*gltf)->nodes_size; ++i) {
        cJSON *node = cJSON_GetArrayItem(nodes, i);
        (*gltf)->nodes[i].name = decode_string(node, "name");
        (*gltf)->nodes[i].mesh = decode_int(node, "mesh");
    }

    // Decode Samplers
//    printf("Decode Samplers\n");
    cJSON *samplers = cJSON_GetObjectItem(root, "samplers");
    (*gltf)->samplers_size = cJSON_GetArraySize(samplers);
    (*gltf)->samplers = new GLTF_Sampler[(*gltf)->samplers_size];
    for (int i = 0; i < (*gltf)->samplers_size; ++i) {
        cJSON *sampler = cJSON_GetArrayItem(samplers, i);
        (*gltf)->samplers[i].magFilter = decode_int(sampler, "magFilter");
        (*gltf)->samplers[i].minFilter = decode_int(sampler, "minFilter");
        (*gltf)->samplers[i].wrapS = decode_int(sampler, "wrapS");
        (*gltf)->samplers[i].wrapT = decode_int(sampler, "wrapT");
    }

    // Decode Scene
//    printf("Decode Scene\n");
    (*gltf)->scene = decode_int(root, "scene");

    // Decode Scenes
//    printf("Decode Scenes\n");
    cJSON *scenes = cJSON_GetObjectItem(root, "scenes");
    (*gltf)->scenes_size = cJSON_GetArraySize(scenes);
    (*gltf)->scenes = new GLTF_Scene[(*gltf)->scenes_size];
    for (int i = 0; i < (*gltf)->scenes_size; ++i) {
        cJSON *scene = cJSON_GetArrayItem(nodes, i);
        GLTF_Array scene_nodes = decode_int_array(scene, "nodes");
        (*gltf)->scenes[i].nodes = static_cast<int *>(scene_nodes.array);
        (*gltf)->scenes[i].nodes_size = scene_nodes.size;
    }

    // Decode Textures
//    printf("Decode Textures\n");
    cJSON *textures = cJSON_GetObjectItem(root, "textures");
    (*gltf)->textures_size = cJSON_GetArraySize(textures);
    (*gltf)->textures = new GLTF_Texture[(*gltf)->scenes_size];
    for (int i = 0; i < (*gltf)->textures_size; ++i) {
        cJSON *texture = cJSON_GetArrayItem(textures, i);
        (*gltf)->textures[i].name = decode_string(texture, "name");
        (*gltf)->textures[i].sampler = decode_int(texture, "sampler");
        (*gltf)->textures[i].source = decode_int(texture, "source");
    }
}

void load_gltf_file(GLTF **gltf, const char *file) {
    char *json;
    load_file(&json, file);
    load_gltf(gltf, json);
}


