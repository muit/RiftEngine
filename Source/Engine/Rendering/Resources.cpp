// � 2019 Miguel Fern�ndez Arce - All rights reserved

#include "CoreEngine.h"
#include <EASTL/unordered_map.h>

#include "Data/TextureData.h"
#include "Data/MeshData.h"
#include "Core/Strings/Name.h"

#include "Interface/Resources/RenderTexture.h"
#include "Interface/Resources/RenderMesh.h"
#include "Interface/Resources/RenderMaterial.h"


struct Resources {

private:

	eastl::unordered_map<Name, RenderTexture> textures;
	eastl::unordered_map<Name, RenderMesh> meshes;
	eastl::unordered_map<u32, RenderMaterial> materials;
};
