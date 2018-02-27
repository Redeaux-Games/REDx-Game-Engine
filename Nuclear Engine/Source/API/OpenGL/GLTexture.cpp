#include <API\OpenGL\GLTexture.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <API\OpenGL\GLVertexShader.h>
#include <API\OpenGL\GLPixelShader.h>

#include <API\OpenGL\GLError.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{

			int GetGLInternalFormat(Format format);
			int GetGLFormatType(Format format);
			int GetGLFormat(Format format);
			
			GLTexture::GLTexture() : textureID(0) , type(0)
			{
			}

			GLTexture::~GLTexture()
			{
				textureID = 0;
				type = 0;
			}

			void GLTexture::Create(GLTexture* result,const Texture_Data& Data, const Texture_Desc& Desc)
			{
				result->desc = Desc;

				if (Desc.Type == TextureType::Texture1D)
				{
					result->type = GL_TEXTURE_1D;
				}
				else if (Desc.Type == TextureType::Texture2D)
				{
					result->type = GL_TEXTURE_2D;
				}
				else if (Desc.Type == TextureType::Texture3D)
				{
					result->type = GL_TEXTURE_3D;
				}

				GLCall(glGenTextures(1, &result->textureID));
				GLCall(glBindTexture(result->type, result->textureID));

				if (Desc.Format == Format::R8_UNORM)
				{
					GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
				}
			
				if (Desc.Type == TextureType::Texture1D)
				{
					GLCall(glTexImage1D(result->type,
						0,
						GetGLInternalFormat(Desc.Format),
						Data.Width,
						0,
						GetGLFormat(Desc.Format),
						GetGLFormatType(Desc.Format),
						Data.Img_Data_Buf));
				}
				else if (Desc.Type == TextureType::Texture2D)
				{
					GLCall(glTexImage2D(result->type,
						0,
						GetGLInternalFormat(Desc.Format),
						Data.Width,
						Data.Height,
						0,
						GetGLFormat(Desc.Format),
						GetGLFormatType(Desc.Format),
						Data.Img_Data_Buf));
				}
				else if (Desc.Type == TextureType::Texture3D)
				{
					GLCall(glTexImage3D(result->type,
						0,
						GetGLInternalFormat(Desc.Format),
						Data.Width,
						Data.Height,
						Data.Depth,
						0,
						GetGLFormat(Desc.Format),
						GetGLFormatType(Desc.Format),
						Data.Img_Data_Buf));
				}
				
				if (Desc.GenerateMipMaps == true)
				{
					GLCall(glGenerateMipmap(result->type));
				}

				if (Desc.Format == Format::R8_UNORM)
				{
					GLCall(glPixelStorei(GL_PACK_ALIGNMENT, 1));
				}
				GLCall(glBindTexture(result->type, 0));
			}

			void GLTexture::Create(GLTexture * result,const std::array<API::Texture_Data, 6>& data, const Texture_Desc& Desc)
			{
				result->desc = Desc;

				GLCall(glGenTextures(1, &result->textureID));
				result->type = GL_TEXTURE_CUBE_MAP;

				GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, result->textureID));

				for (GLuint i = 0; i < data.size(); i++)
				{
					GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
						GetGLInternalFormat(Desc.Format),
						data[i].Width,
						data[i].Height,
						0,
						GetGLFormat(Desc.Format),
						GetGLFormatType(Desc.Format),
						data[i].Img_Data_Buf));
				}
			
				if (Desc.GenerateMipMaps == true)
				{
					GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
				}
						
				GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
			}

			void GLTexture::Delete(GLTexture * texture)
			{
				if (texture->textureID != 0)
				{
					GLCall(glDeleteTextures(1, &texture->textureID));
				}

				texture->textureID = 0;
				texture->type = 0;
			}			

			void GLTexture::VSBind(unsigned int slot)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + slot));
				GLCall(glBindTexture(type, textureID));
			}

		
			void GLTexture::PSBind(unsigned int slot)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + slot));
				GLCall(glBindTexture(type, textureID));
			}

			void GLTexture::GSBind(unsigned int slot)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + slot));
				GLCall(glBindTexture(type, textureID));
			}

			Texture_Desc GLTexture::GetTextureDesc()
			{
				return desc;
			}
		
			
			unsigned int GLTexture::GLGetTextureID()
			{
				return textureID;
			}
			int GetGLInternalFormat(Format format)
			{
				switch (format)
				{
				case Format::R8_UNORM: return GL_R8;
				case Format::R8G8_UNORM: return GL_RG8;
				case Format::R8G8B8_UNORM: return GL_RGB8;
				case Format::R8G8B8A8_UNORM: return GL_RGBA8;
				case Format::R16_FLOAT: return GL_R16F;
				case Format::R16G16_FLOAT: return GL_RG16F;
				case Format::R16G16B16_FLOAT: return GL_RGB16F;
				case Format::R16G16B16A16_FLOAT: return GL_RGBA16F;
				case Format::R32_FLOAT: return GL_R32F;
				case Format::R32G32_FLOAT: return GL_RG32F;
				case Format::R32G32B32_FLOAT: return GL_RGB32F;
				case Format::R32G32B32A32_FLOAT: return GL_RGBA32F;
				default: return GL_RGBA8;
				}

			}

			int GetGLFormat(Format format)
			{
				switch (format)
				{
				case Format::R8_UNORM:
				case Format::R16_FLOAT:
				case Format::R32_FLOAT:
					return GL_RED;

				case Format::R8G8_UNORM:
				case Format::R16G16_FLOAT:
				case Format::R32G32_FLOAT:
					return GL_RG;

				case Format::R8G8B8_UNORM:
				case Format::R16G16B16_FLOAT:
				case Format::R32G32B32_FLOAT:
					return GL_RGB;

				case Format::R8G8B8A8_UNORM:
				case Format::R16G16B16A16_FLOAT:
				case Format::R32G32B32A32_FLOAT:
					return GL_RGBA;
				default: return GL_RGBA;
				}

			}
			int GetGLFormatType(Format format)
			{
				switch (format)
				{
				case Format::R8_UNORM:
				case Format::R8G8_UNORM:
				case Format::R8G8B8_UNORM:
				case Format::R8G8B8A8_UNORM:
					return GL_UNSIGNED_BYTE;
				case Format::R16_FLOAT:
				case Format::R16G16_FLOAT:
				case Format::R16G16B16_FLOAT:
				case Format::R16G16B16A16_FLOAT:
					return GL_HALF_FLOAT;
				case Format::R32_FLOAT:
				case Format::R32G32_FLOAT:
				case Format::R32G32B32_FLOAT:
				case Format::R32G32B32A32_FLOAT:
					return GL_FLOAT;
				default: return GL_UNSIGNED_BYTE;
				}

			}
		}
	}
}

#endif