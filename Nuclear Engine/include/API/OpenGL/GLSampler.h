#pragma once
#include <API\OpenGL\GLCommon.h>
#include <API\Texture_Types.h>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{

			class NEAPI GLSampler
			{
			public:
				GLSampler();
				~GLSampler();

				static void Create(GLSampler* texture, const SamplerDesc& Desc);
				static void Delete(GLSampler* texture);

				void VSBind(unsigned int Slot);
				void GSBind(unsigned int Slot);
				void PSBind(unsigned int Slot);
			private:
				GLuint SamplerID;
			};
		}
	}
}
#endif