// � 2019 Miguel Fern�ndez Arce - All rights reserved

#include "CoreEngine.h"
#include <EASTL/unordered_map.h>

#include "TextureData.h"
#include "MeshData.h"

struct Resources {
    eastl::unordered_map<u32, TextureData> textures;
    eastl::unordered_map<u32, MeshData> meshes;
};
