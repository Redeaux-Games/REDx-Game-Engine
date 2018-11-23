#include "ImGui_Renderer.h"

namespace NuclearEngine
{
	namespace Graphics
	{
	
		static API::VertexBuffer       gVB;
		static API::IndexBuffer        gIB;
		static API::VertexShader       gVertexShader;
		static API::InputLayout        gInputLayout;
		static API::ConstantBuffer     gVertexConstantBuffer;
		static API::PixelShader		   gPixelShader;
		static API::Sampler			   gFontSampler;
		static API::Texture			   gFontTextureView;
		static API::RasterizerState	   gRasterizerState;
		static API::BlendState		   gBlendState;
		static API::DepthStencilState  gDepthStencilState;
		static int  g_VertexBufferSize = 5000, g_IndexBufferSize = 10000;

		bool ImGui_Renderer::Initialize()
		{

			gInputLayout.AppendAttribute("POSITION", 0, Graphics::API::DataType::Float2);
			gInputLayout.AppendAttribute("COLOR0", 0, Graphics::API::DataType::Float4);
			gInputLayout.AppendAttribute("TEXCOORD0", 0, Graphics::API::DataType::Float2);
			return false;
		}
		void ImGui_Renderer::Shutdown()
		{
		}
		void ImGui_Renderer::NewFrame()
		{
		}
		void ImGui_Renderer::RenderDrawData(ImDrawData * draw_data)
		{
			// Create and grow vertex/index buffers if needed
			if (g_VertexBufferSize < draw_data->TotalVtxCount)
			{
				API::VertexBuffer::Delete(&gVB);

				g_VertexBufferSize = draw_data->TotalVtxCount + 5000;

				API::VertexBufferDesc desc;
				desc.usage = API::BufferUsage::Dynamic;
				desc.size = g_VertexBufferSize * sizeof(ImDrawVert);
				desc.data = NULL;
				API::VertexBuffer::Create(&gVB, desc);

				gVB.SetInputLayout(&gInputLayout, &gVertexShader);
			}
			if (g_IndexBufferSize < draw_data->TotalIdxCount)
			{
				API::IndexBuffer::Delete(&gIB);

				g_IndexBufferSize = draw_data->TotalIdxCount + 10000;

				API::IndexBuffer::Create(&gIB, NULL, g_IndexBufferSize * sizeof(ImDrawIdx));
			}

			// Copy and convert all vertices into a single contiguous buffer

			/*D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
			if (ctx->Map(gVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK)
				return;
			if (ctx->Map(gIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK)
				return;
				*/


			for (int n = 0; n < draw_data->CmdListsCount; n++)
			{
				const ImDrawList* cmd_list = draw_data->CmdLists[n];
				const ImDrawIdx* idx_buffer_offset = 0;

				gVB.Update((const void*)cmd_list->VtxBuffer.Data, (ptrdiff_t)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));

				gIB.Update((const void*)cmd_list->IdxBuffer.Data, (ptrdiff_t)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

			}


			// Setup orthographic projection matrix into our constant buffer
			// Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). 

			float L = draw_data->DisplayPos.x;
			float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
			float T = draw_data->DisplayPos.y;
			float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

			if (Graphics::API::Context::isOpenGL3RenderAPI())
			{				
				float mvp[4][4] =
				{
					{ 2.0f / (R - L),   0.0f,         0.0f,   0.0f },
					{ 0.0f,         2.0f / (T - B),   0.0f,   0.0f },
					{ 0.0f,         0.0f,        -1.0f,   0.0f },
					{ (R + L) / (L - R),  (T + B) / (B - T),  0.0f,   1.0f },
				};

				gVertexConstantBuffer.Update(mvp, sizeof(mvp));
			}
			else if (Graphics::API::Context::isDirectX11RenderAPI())
			{
				float mvp[4][4] =
				{
					{ 2.0f / (R - L),   0.0f		  ,     0.0f,       0.0f },
					{ 0.0f			,   2.0f / (T - B),     0.0f,       0.0f },
					{ 0.0f,         0.0f,           0.5f,       0.0f },
					{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
				};

				gVertexConstantBuffer.Update(mvp, sizeof(mvp));
			}
		

			// Backup DX state that will be modified to restore it afterwards (unfortunately this is very ugly looking and verbose. Close your eyes!)
			/*struct BACKUP_DX11_STATE
			{
				UINT                        ScissorRectsCount, ViewportsCount;
				D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
				D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
				ID3D11RasterizerState*      RS;
				ID3D11BlendState*           BlendState;
				FLOAT                       BlendFactor[4];
				UINT                        SampleMask;
				UINT                        StencilRef;
				ID3D11DepthStencilState*    DepthStencilState;
				ID3D11ShaderResourceView*   PSShaderResource;
				ID3D11SamplerState*         PSSampler;
				ID3D11PixelShader*          PS;
				ID3D11VertexShader*         VS;
				UINT                        PSInstancesCount, VSInstancesCount;
				ID3D11ClassInstance*        PSInstances[256], *VSInstances[256];   // 256 is max according to PSSetShader documentation
				D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
				ID3D11Buffer*               IndexBuffer, *VertexBuffer, *VSConstantBuffer;
				UINT                        IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
				DXGI_FORMAT                 IndexBufferFormat;
				ID3D11InputLayout*          InputLayout;
			};
			BACKUP_DX11_STATE old;
			old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
			ctx->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
			ctx->RSGetViewports(&old.ViewportsCount, old.Viewports);
			ctx->RSGetState(&old.RS);
			ctx->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
			ctx->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
			ctx->PSGetShaderResources(0, 1, &old.PSShaderResource);
			ctx->PSGetSamplers(0, 1, &old.PSSampler);
			old.PSInstancesCount = old.VSInstancesCount = 256;
			ctx->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
			ctx->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
			ctx->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
			ctx->IAGetPrimitiveTopology(&old.PrimitiveTopology);
			ctx->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
			ctx->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
			ctx->IAGetInputLayout(&old.InputLayout);
			*/

			// Setup viewport
			Graphics::API::Context::SetViewPort(0, 0, draw_data->DisplaySize.x, draw_data->DisplaySize.y);
			

			// Bind shader and vertex buffers
			/*unsigned int stride = sizeof(ImDrawVert);
			unsigned int offset = 0;*/

			///Binds Input Layout too
			gVB.Bind();
			gIB.Bind();				//ctx->IASetIndexBuffer(gIB, sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
			Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);
			gVertexShader.Bind();

			///ctx->VSSetConstantBuffers(0, 1, &gVertexConstantBuffer);
			gPixelShader.Bind();
			gFontSampler.PSBind(0);
		
			// Setup render state
			gBlendState.Bind(Graphics::Color(0.f, 0.f, 0.f, 0.f), 0xffffffff);
			gDepthStencilState.Bind();
			gRasterizerState.Bind();

			// Render command lists
			int vtx_offset = 0;
			int idx_offset = 0;
			ImVec2 pos = draw_data->DisplayPos;
			for (int n = 0; n < draw_data->CmdListsCount; n++)
			{
				const ImDrawList* cmd_list = draw_data->CmdLists[n];
				for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
				{
					const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
					if (pcmd->UserCallback)
					{
						// User callback (registered via ImDrawList::AddCallback)
						pcmd->UserCallback(cmd_list, pcmd);
					}
					else
					{
						// Apply scissor/clipping rectangle
						const D3D11_RECT r = { (LONG)(pcmd->ClipRect.x - pos.x), (LONG)(pcmd->ClipRect.y - pos.y), (LONG)(pcmd->ClipRect.z - pos.x), (LONG)(pcmd->ClipRect.w - pos.y) };
						//ctx->RSSetScissorRects(1, &r);

						// Bind texture, Draw
						ID3D11ShaderResourceView* texture_srv = (ID3D11ShaderResourceView*)pcmd->TextureId;
						///ctx->PSSetShaderResources(0, 1, &texture_srv);
						Graphics::API::Context::DrawIndexed(pcmd->ElemCount, idx_offset, vtx_offset);
					}
					idx_offset += pcmd->ElemCount;
				}
				vtx_offset += cmd_list->VtxBuffer.Size;
			}

/*			ctx->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
			ctx->RSSetViewports(old.ViewportsCount, old.Viewports);
			ctx->RSSetState(old.RS); if (old.RS) old.RS->Release();
			ctx->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) old.BlendState->Release();
			ctx->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) old.DepthStencilState->Release();
			ctx->PSSetShaderResources(0, 1, &old.PSShaderResource); if (old.PSShaderResource) old.PSShaderResource->Release();
			ctx->PSSetSamplers(0, 1, &old.PSSampler); if (old.PSSampler) old.PSSampler->Release();
			ctx->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
			for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
			ctx->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
			ctx->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) old.VSConstantBuffer->Release();
			for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
			ctx->IASetPrimitiveTopology(old.PrimitiveTopology);
			ctx->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
			ctx->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) old.VertexBuffer->Release();
			ctx->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();
			*/
		}

		void ImGui_Renderer::InvalidateDeviceObjects()
		{
		}
		bool ImGui_Renderer::CreateDeviceObjects()
		{
			return false;
		}
	}
}
