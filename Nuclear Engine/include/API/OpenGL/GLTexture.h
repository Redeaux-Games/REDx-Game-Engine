#pragma once
#include <API\OpenGL\GLCommon.h>
#include <API\Texture_Types.h>
#include <array>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{
			class GLShader;
			class GLVertexShader;
			class GLPixelShader;

			class NEAPI GLTexture
			{
			public:
				GLTexture();
				~GLTexture();

				static void Create(GLTexture* texture, const Texture_Data& TexData, const Texture_Desc& Desc);
				static void Create(GLTexture* texture, const std::array<Texture_Data, 6>& data,const Texture_Desc& Desc);
				static void Delete(GLTexture* texture);

				void SetInShader(const char *samplerName, GLVertexShader *shader, unsigned int index);
				void SetInShader(const char *samplerName, GLPixelShader *shader, unsigned int index);

				void VSBind(unsigned int index);
				void VSBind(const char *samplerName, GLShader *shader, unsigned int index);
				void GSBind(unsigned int index);
				void GSBind(const char *samplerName, GLShader *shader, unsigned int index);
				void PSBind(unsigned int index);
				void PSBind(const char *samplerName, GLShader *shader, unsigned int index);

				unsigned int GLGetTextureID();

			private:
				GLuint textureID;

				GLenum type;
			};
		}
	}
}
#endif