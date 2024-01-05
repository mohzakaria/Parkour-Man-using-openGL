#include "mesh-renderer.hpp"
#include "../asset-loader.hpp"

namespace our {
    // Receives the mesh & material from the AssetLoader by the names given in the json object
    void MeshRendererComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        // Notice how we just get a string from the json file and pass it to the AssetLoader to get us the actual asset
        //TOD: (Req 8) Get the material and the mesh from the AssetLoader by their names
        // which are defined with the keys "mesh" and "material" in data.
        // Hint: To get a value of type T from a json object "data" where the key corresponding to the value is "key",
        // you can use write: data["key"].get<T>().
        // Look at "source/common/asset-loader.hpp" to know how to use the static class AssetLoader.

        /*
        First we vistied the asset-Loader.hpp and .cpp file to understand more about how it works
        then we discovered the get function takes string so we need to convert the json object to string
        then use it to access the assetLoader map and retrieve the asset.
        */
        std::string getMesh = data["mesh"].get<std::string>();
        mesh = AssetLoader<Mesh>::get(getMesh);
        std::string getMaterial = data["material"].get<std::string>();
        material = AssetLoader<Material>::get(getMaterial);
    }
}