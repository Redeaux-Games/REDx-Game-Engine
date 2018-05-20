#pragma once
#include <Engine/Graphics/Color.h>
#include <Engine/Assets/Texture.h>
#include <Base\Utilities\Delegate.h>
#include <unordered_map>
#include <array>
#include <string>
namespace NuclearEngine {
	namespace Assets
	{
		class Mesh;
	}
	namespace Managers
	{

		struct MeshLoadingDesc
		{
			bool UseTexCoords = true;
			bool UseNormals = true;
			bool UseTangents = false;
			bool LoadDiffuseTextures = true;
			bool LoadSpecularTextures = false;
			bool LoadNormalTextures = false;
		};

		//typedef Utilities::Delegate<Assets::Mesh&(const std::string& Path, const Managers::MeshLoadingDesc& desc)> MeshImport;
		typedef Utilities::Delegate<Graphics::API::Texture_Data(const std::string& Path, const Graphics::API::Texture_Desc & Desc)> TextureImport;

		class NEAPI AssetManager {
		public:
			//Public Members
			//All loaded textures with their hashed names with crc32c (always saved)
			static std::unordered_map<Uint32, Assets::Texture> mImportedTextures;
			//Real pre-hashed texture names with paths (conditionally saved see SaveTextureNames)
			static std::unordered_map<Uint32,std::string> mHashedTexturesNames;
			//tells the asset manager whether to store the real texture name or not
			static bool mSaveTextureNames;
						
			//Methods
			static void Initialize(bool SaveTextureNames = false);
			static void ShutDown();
			
			static Assets::Mesh& Import(const std::string& Path, const MeshLoadingDesc& = MeshLoadingDesc());
			static Assets::Texture& Import(const std::string& Path, const Graphics::API::Texture_Desc& Desc = Graphics::API::Texture_Desc());
			
			static TextureImport DefaultTextureImporter;

			static bool DoesTextureExist(Uint32 hashedname, Assets::Texture* texture);

			//Order:  [+X (right)] [-X (left)] [+Y (top)] [-Y (bottom)] [+Z (front)] [-Z (back)]			
			static std::array<Graphics::API::Texture_Data, 6> LoadTextureCubeFromFile(const std::array<std::string, 6 >& Paths, const Graphics::API::Texture_Desc& Desc);
		private:
			static Graphics::API::Texture_Data TextureCube_Load(const std::string& Path, const Graphics::API::Texture_Desc& Desc);
		};
	}
}