#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine\Components\CameraComponent.h>
#include <Engine\Components\Light.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>
#include <Diligent/Common/interface/RefCntAutoPtr.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Texture.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Sampler.h>
#include <Diligent/Graphics/GraphicsEngine/interface/DepthStencilState.h>
#include <vector>

namespace NuclearEngine
{
	namespace Graphics
	{

		struct RenderingPipelineDesc
		{
			Uint32 DirLights = 0;
			Uint32 SpotLights = 0;
			Uint32 PointLights = 0;
			bool UseNormalMaps = false;

			IBuffer* CameraBufferPtr = nullptr;
			IBuffer* LightsBufferPtr = nullptr;
		};

		class NEAPI RenderingPipeline
		{
		public:
			virtual bool Bake(const RenderingPipelineDesc& desc) = 0;

			IPipelineState* GetPipeline();

		protected:
			RefCntAutoPtr<IPipelineState> mPipeline;

		};

	}
}