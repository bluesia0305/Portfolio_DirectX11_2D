#include "Pipeline.h"

namespace Pipeline
{
	namespace
	{
		ID3D11Device*					Device;
		ID3D11DeviceContext*			DeviceContext;
		IDXGISwapChain*					SwapChain;	
		
		namespace Buffer
		{
			ID3D11Buffer*				Vertex;
			ID3D11Buffer*				Constant[2];		// Constant buffer for vertex shader
			ID3D11Buffer*				ConstantAlpha;		// Constant buffer for pixel shader

			template<typename T> void Update(ID3D11Buffer* buffer, T const& data)
			{
				D3D11_MAPPED_SUBRESOURCE SubResource = D3D11_MAPPED_SUBRESOURCE();
				MUST(DeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource));
				memcpy_s(SubResource.pData, SubResource.RowPitch, data, sizeof(data));
				DeviceContext->Unmap(buffer, 0);
			}
		}

		ID3D11RenderTargetView*			RenderTargetView;
	}

	namespace Transform
	{
		template<MATRIX_TYPE type> void Update(MATRIX const& matrix)
		{ Buffer::Update(Buffer::Constant[static_cast<UINT>(type)], matrix); }
	}

	namespace Transparency
	{
		void Update(float alphaLevel)
		{ Buffer::Update(Buffer::ConstantAlpha, VECTOR{ alphaLevel, 0.0f, 0.0f, 0.0f }); }
	}

	namespace Texture
	{
		void Create(Handle*& handle, SIZE const size, BYTE const* const data)
		{
			D3D11_TEXTURE2D_DESC const descriptor
			{
				static_cast<UINT>(size.cx),
				static_cast<UINT>(size.cy),
				1,
				1,
				DXGI_FORMAT_B8G8R8A8_UNORM,
				1,
				0,
				D3D11_USAGE_IMMUTABLE,
				D3D11_BIND_SHADER_RESOURCE
			};

			UINT constexpr BPP = 32;	// BPP: bits per pixel.
			D3D11_SUBRESOURCE_DATA const subResource
			{
				data,
				size.cx * BPP / 8
			};

			ID3D11Texture2D* texture = nullptr;
			MUST(Device->CreateTexture2D(&descriptor, &subResource, &texture));
			MUST(Device->CreateShaderResourceView(texture, nullptr, &(handle = new Handle())->ShaderResourceView));
			texture->Release();
		}

		void Render(Handle const* const& handle, RECT const area, float alphaLevel)
		{
			{
				DeviceContext->PSSetShaderResources(0, 1, &handle->ShaderResourceView);
				float const texcoord[4][2]
				{
					{ static_cast<float>(area.left),	static_cast<float>(area.top) },
					{ static_cast<float>(area.right),	static_cast<float>(area.top) },
					{ static_cast<float>(area.left),	static_cast<float>(area.bottom) },
					{ static_cast<float>(area.right),	static_cast<float>(area.bottom) }
				};
				Buffer::Update(Buffer::Vertex, texcoord);
				Transparency::Update(alphaLevel);
			}
			DeviceContext->Draw(4, 0);
		}

		void Delete(Handle const* const& handle)
		{
			handle->ShaderResourceView->Release();
			delete handle;
		}
	}

	namespace Text
	{
		void Render(HFONT const hFont, LPCSTR const string, COLORREF const color, POINT const center, SIZE const size)
		{
			IDXGISurface1* surface = nullptr;
			MUST(SwapChain->GetBuffer(0, IID_PPV_ARGS(&surface)));
			{
				HDC hdc = HDC();
				MUST(surface->GetDC(false, &hdc));
				{
					if (hFont != HFONT()) SelectObject(hdc, hFont);
					SetTextColor(hdc, color);
					
					RECT area
					{
						center.x - size.cx / 2,
						center.y - size.cy / 2,
						center.x + size.cx / 2,
						center.y + size.cy / 2,
					};
					DrawText(hdc, string, ~'\0', &area, DT_CENTER);
				}
				MUST(surface->ReleaseDC(nullptr));
			}

			// (주의) Surface release 하는 경우, render target view도 같이 해제된다.
			surface->Release();
			DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr);
		}
	}

	void Procedure(HWND const hWnd, UINT const uMsg, WPARAM const wParam, LPARAM const lParam)
	{
		switch (uMsg)
		{
		case WM_APP:
		{
			MUST(SwapChain->Present(0, 0));
			float const color[4]{ 0.0f, 0.0f, 0.0f, 1.0f };		// Black
			DeviceContext->ClearRenderTargetView(RenderTargetView, color);
			return;
		}

		case WM_SIZE:
		{
			{
				D3D11_VIEWPORT viewport = D3D11_VIEWPORT();
				viewport.Width	= LOWORD(lParam);
				viewport.Height = HIWORD(lParam);
				DeviceContext->RSSetViewports(1, &viewport);
			}

			{
				if (RenderTargetView != nullptr)
				{
					RenderTargetView->Release();
					MUST(SwapChain->ResizeBuffers(1, LOWORD(lParam), HIWORD(lParam), DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE));
				}

				ID3D11Texture2D* texture2D = nullptr;
				MUST(SwapChain->GetBuffer(0, IID_PPV_ARGS(&texture2D)));	// Back buffer
				{
					IDXGISurface1* surface = nullptr;
					MUST(texture2D->QueryInterface(IID_PPV_ARGS(&surface)));
					{
						HDC hdc = HDC();
						MUST(surface->GetDC(false, &hdc));
						{ SetBkMode(hdc, TRANSPARENT); }
						MUST(surface->ReleaseDC(nullptr));
					}
					surface->Release();
					MUST(Device->CreateRenderTargetView(texture2D, nullptr, &RenderTargetView));
					DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr);
				}
				texture2D->Release();
			}
			return;
		}

		case WM_CREATE:
		{
			{
				DXGI_SWAP_CHAIN_DESC descriptor = DXGI_SWAP_CHAIN_DESC();
				descriptor.BufferDesc.Format	= DXGI_FORMAT_B8G8R8A8_UNORM;
				descriptor.SampleDesc.Count		= 1;
				descriptor.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
				descriptor.BufferCount			= 1;
				descriptor.OutputWindow			= hWnd;
				descriptor.Windowed				= true;
				descriptor.Flags				= DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;

				MUST(D3D11CreateDeviceAndSwapChain(nullptr,
												   D3D_DRIVER_TYPE_HARDWARE,
												   nullptr,
												   0,
												   nullptr,
												   0,
												   D3D11_SDK_VERSION,
												   &descriptor,
												   &SwapChain,
												   &Device,
												   nullptr,
												   &DeviceContext));
			}

			// Vertex -> InputSlot : 0
			{
				float coordinates[4][2] =
				{
					{ -0.5f, +0.5f },
					{ +0.5f, +0.5f },
					{ -0.5f, -0.5f },
					{ +0.5f, -0.5f },
				};

				D3D11_SUBRESOURCE_DATA subResource{ coordinates };
				D3D11_BUFFER_DESC descriptor
				{
					sizeof(coordinates),
					D3D11_USAGE_IMMUTABLE,
					D3D11_BIND_VERTEX_BUFFER
				};
				ID3D11Buffer* bufferCoord = nullptr;
				MUST(Device->CreateBuffer(&descriptor, &subResource, &bufferCoord));

				const UINT stride = sizeof(*coordinates);
				const UINT offset = 0;
				DeviceContext->IASetVertexBuffers(0, 1, &bufferCoord, &stride, &offset);
				bufferCoord->Release();
			}

			// Texture -> InputSlot : 1
			{
				D3D11_BUFFER_DESC descriptor
				{
					sizeof(float[4][2]),
					D3D11_USAGE_DYNAMIC,
					D3D11_BIND_VERTEX_BUFFER,
					D3D11_CPU_ACCESS_WRITE
				};
				MUST(Device->CreateBuffer(&descriptor, nullptr, &Buffer::Vertex));

				const UINT stride = sizeof(float[2]);
				const UINT offset = 0;
				DeviceContext->IASetVertexBuffers(1, 1, &Buffer::Vertex, &stride, &offset);
			}
			
			{ DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); }

			{
#include "Shader/Bytecode/VertexShader.h"
				{
					ID3D11InputLayout* inputLayout = nullptr;
					D3D11_INPUT_ELEMENT_DESC descriptors[2] =
					{
						{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0 },
						{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1 }
					};
					
					MUST(Device->CreateInputLayout(descriptors, 2, bytecode, sizeof(bytecode), &inputLayout));
					DeviceContext->IASetInputLayout(inputLayout);
					inputLayout->Release();
				}

				{
					ID3D11VertexShader* vertexShader = nullptr;
					MUST(Device->CreateVertexShader(bytecode, sizeof(bytecode), nullptr, &vertexShader));
					DeviceContext->VSSetShader(vertexShader, nullptr, 0);
					vertexShader->Release();
				}
			}
				
			{
				D3D11_BUFFER_DESC descriptor
				{
					sizeof(float[4][4]),
					D3D11_USAGE_DYNAMIC,
					D3D11_BIND_CONSTANT_BUFFER,
					D3D11_CPU_ACCESS_WRITE
				};
				for (int i = 0; i < 2; ++i) { MUST(Device->CreateBuffer(&descriptor, nullptr, &Buffer::Constant[i])); }
				DeviceContext->VSSetConstantBuffers(0, 2, Buffer::Constant);
			}
			
			{
				ID3D11RasterizerState* rasterizerState;
				D3D11_RASTERIZER_DESC descriptor = D3D11_RASTERIZER_DESC();
				descriptor.FillMode = D3D11_FILL_SOLID;
				descriptor.CullMode = D3D11_CULL_NONE;
								
				MUST(Device->CreateRasterizerState(&descriptor, &rasterizerState));
				DeviceContext->RSSetState(rasterizerState);
				rasterizerState->Release();
			}

			{
#include "Shader/Bytecode/PixelShader.h"
				ID3D11PixelShader* pixelShader = nullptr;
				MUST(Device->CreatePixelShader(bytecode, sizeof(bytecode), nullptr, &pixelShader));
				DeviceContext->PSSetShader(pixelShader, nullptr, 0);
				pixelShader->Release();
			}

			// 투명도 상수 버퍼(dynamic usage)
			{
				D3D11_BUFFER_DESC descriptor
				{
					sizeof(float[4]),
					D3D11_USAGE_DYNAMIC,
					D3D11_BIND_CONSTANT_BUFFER,
					D3D11_CPU_ACCESS_WRITE
				};
				MUST(Device->CreateBuffer(&descriptor, nullptr, &Buffer::ConstantAlpha));
				DeviceContext->PSSetConstantBuffers(0, 1, &Buffer::ConstantAlpha);
			}

			// Blend state 설정 = OM 단계
			{
				D3D11_BLEND_DESC descriptor = D3D11_BLEND_DESC();
				descriptor.RenderTarget->BlendEnable		   = true;
				descriptor.RenderTarget->SrcBlend			   = D3D11_BLEND_SRC_ALPHA;
				descriptor.RenderTarget->DestBlend			   = D3D11_BLEND_INV_SRC_ALPHA;		// INV_SRC : Inverts data from source = 1 - Src.
				descriptor.RenderTarget->BlendOp			   = D3D11_BLEND_OP_ADD;
				descriptor.RenderTarget->SrcBlendAlpha		   = D3D11_BLEND_ZERO;
				descriptor.RenderTarget->DestBlendAlpha		   = D3D11_BLEND_ONE;
				descriptor.RenderTarget->BlendOpAlpha		   = D3D11_BLEND_OP_ADD;
				descriptor.RenderTarget->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

				ID3D11BlendState* blendState = nullptr;
				MUST(Device->CreateBlendState(&descriptor, &blendState));
				DeviceContext->OMSetBlendState(blendState, nullptr, D3D11_DEFAULT_SAMPLE_MASK);
				blendState->Release();
			}
			return;
		}

		case WM_DESTROY:
		{
			RenderTargetView->Release();

			Buffer::ConstantAlpha->Release();
			for (int i = 0; i < 2; ++i) { Buffer::Constant[i]->Release(); }
			Buffer::Vertex->Release();

			Device->Release();
			DeviceContext->Release();
			SwapChain->Release();
			return;
		}
		}
	}
}