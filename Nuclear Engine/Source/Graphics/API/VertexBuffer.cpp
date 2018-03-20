#include <Graphics/API/VertexBuffer.h>
#ifdef NE_USE_RUNTIME_RENDER_API
#include <Graphics\API\Context.h>
#include <Graphics/API/InputLayout.h>
#include <Graphics/API/VertexShader.h>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {
			VertexBuffer::VertexBuffer()
			{
			}

			VertexBuffer::~VertexBuffer()
			{
			}

			void VertexBuffer::Create(VertexBuffer * buffer, const VertexBufferDesc& desc)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLVertexBuffer::Create(&buffer->GLObject, desc);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11VertexBuffer::Create(&buffer->DXObject, desc);
				}
			}

			void VertexBuffer::Delete(VertexBuffer * buffer)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					OpenGL::GLVertexBuffer::Delete(&buffer->GLObject);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DirectX::DX11VertexBuffer::Delete(&buffer->DXObject);
				}
			}

			void VertexBuffer::Update(const void * data, unsigned int size)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLObject.Update(data, size);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DXObject.Update(data, size);
				}
			}

			void VertexBuffer::SetInputLayout(InputLayout * layout, VertexShader * shader)
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLObject.SetInputLayout(layout, &shader->GLObject);
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DXObject.SetInputLayout(layout, &shader->DXObject);
				}
			}

			void VertexBuffer::Bind()
			{
				if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
				{
					GLObject.Bind();
				}
				else if (Graphics::API::Context::GetRenderAPI() == Core::RenderAPI::DirectX11)
				{
					DXObject.Bind();
				}
			}
		}
	}
}
#endif