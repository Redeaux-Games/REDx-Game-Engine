#include "..\include\NuclearRendererDX11\DX11PipelineState.h"
#include <NuclearCommon\Common_API_Types.h>
#include <NuclearRendererDX11\DX11Context.h>

using namespace NuclearEngine;

namespace NuclearRenderer {

	D3D11_COMPARISON_FUNC GetDXComparisonFunc(Comparison_Func func)
	{
		switch (func)
		{
		case Comparison_Func::NEVER: return D3D11_COMPARISON_NEVER;
		case Comparison_Func::LESS: return D3D11_COMPARISON_LESS;
		case Comparison_Func::EQUAL: return D3D11_COMPARISON_EQUAL;
		case Comparison_Func::LESS_EQUAL: return D3D11_COMPARISON_LESS_EQUAL;
		case Comparison_Func::GREATER: return D3D11_COMPARISON_GREATER;
		case Comparison_Func::NOT_EQUAL: return D3D11_COMPARISON_NOT_EQUAL;
		case Comparison_Func::GREATER_EQUAL: return D3D11_COMPARISON_GREATER_EQUAL;
		case Comparison_Func::ALWAYS: return D3D11_COMPARISON_ALWAYS;

		default: return D3D11_COMPARISON_ALWAYS;
		}
	}

	D3D11_STENCIL_OP GetDXStencilOP(Stencil_OP func)
	{
		switch (func)
		{
		case Stencil_OP::KEEP: return D3D11_STENCIL_OP_KEEP;
		case Stencil_OP::ZERO: return D3D11_STENCIL_OP_ZERO;
		case Stencil_OP::REPLACE: return D3D11_STENCIL_OP_REPLACE;
		case Stencil_OP::INCREASE_SAT: return D3D11_STENCIL_OP_INCR_SAT;
		case Stencil_OP::DECREASE_SAT: return D3D11_STENCIL_OP_DECR_SAT;
		case Stencil_OP::INVERT: return D3D11_STENCIL_OP_INVERT;
		case Stencil_OP::INCREASE: return D3D11_STENCIL_OP_INCR;
		case Stencil_OP::DECREASE: return D3D11_STENCIL_OP_DECR;

		default: return D3D11_STENCIL_OP_KEEP;
		}
	}

	DX11PipelineState::DX11PipelineState()
	{
	}
	void DX11PipelineState::Create(PipelineStateDesc type)
	{
		desc = &type;

		if (desc->DepthStencilEnabled == true)
		{
			D3D11_DEPTH_STENCIL_DESC DSDesc;
			ZeroMemory(&DSDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		
			DSDesc.DepthEnable = true;
			if (desc->DepthMaskEnabled == true) 
			{ DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; }
			else 
			{ DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; }

			DSDesc.DepthFunc = GetDXComparisonFunc(desc->DepthFunc);
		
			DSDesc.StencilEnable = true;
			DSDesc.StencilReadMask = desc->StencilReadMask;
			DSDesc.StencilWriteMask = desc->StencilWriteMask;		

			DX11Context::GetDevice()->CreateDepthStencilState(&DSDesc, &DS_State);
		}
	}
	void DX11PipelineState::Delete()
	{
		DS_State.Reset();
	}
	void DX11PipelineState::Bind_DepthStencil()
	{
		DX11Context::GetContext()->OMSetDepthStencilState(DS_State.Get(), 0);
	}
	void DX11PipelineState::Unbind_DepthStencil()
	{
		DX11Context::GetContext()->OMSetDepthStencilState(NULL, 0);
	}
}