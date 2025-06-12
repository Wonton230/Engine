#include "Renderer.hpp"
#include <math.h>
#include <cassert>
#include <crtdbg.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#if defined(ENGINE_DEBUG_RENDER)
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

#if defined(OPAQUE)
#undef OPAQUE
#endif

#include "ThirdParty/stb/stb_image.h"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Renderer/ConstantBuffer.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Core/Image.hpp"


const char* DefaultShaderByteCode =
R"(
cbuffer CameraConstants : register(b2)
{
	float4x4 WorldToCameraTransform;
	float4x4 CameraToRenderTransform;
	float4x4 RenderToClipTransform;
};

cbuffer CameraConstants : register(b3)
{
	float4x4 ModelToWorldTransform;
	float4 ModelColor;
};

Texture2D diffuseTexture : register(t0);
SamplerState diffuseSampler : register(s0);

struct vs_input_t
{
	float3 modelSpacePosition : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

struct v2p_t
{
	float4 clipSpacePosition : SV_Position;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

v2p_t VertexMain(vs_input_t input)
{
	float4 modelSpacePosition = float4(input.modelSpacePosition, 1);
	float4 worldSpacePosition = mul(ModelToWorldTransform, modelSpacePosition);
	float4 cameraSpacePosition = mul(WorldToCameraTransform, worldSpacePosition);
	float4 renderSpacePosition = mul(CameraToRenderTransform, cameraSpacePosition);
	float4 clipSpacePosition = mul(RenderToClipTransform, renderSpacePosition);

	v2p_t v2p;
	v2p.clipSpacePosition = clipSpacePosition;
	v2p.color = input.color;
	v2p.uv = input.uv;
	return v2p;
}

float4 PixelMain(v2p_t input) : SV_Target0
{
	float4 textureColor = diffuseTexture.Sample(diffuseSampler, input.uv);
	float4 vertexColor = input.color;
	float4 color = textureColor * vertexColor * ModelColor;
	clip(color.a - 0.01f);
	return float4(color);
}
)";

Renderer::Renderer()
{
	m_renderConfig = RenderConfig();
}

Renderer::Renderer(RenderConfig renderConfig)
{
	m_renderConfig = renderConfig;
}

Renderer::~Renderer()
{

}

void Renderer::Startup() 
{
	//Create debug module
#if defined(ENGINE_DEBUG_RENDER)
	m_dxgiDebugModule = (void*)::LoadLibraryA("dxgidebug.dll");
	if (m_dxgiDebugModule == nullptr)
	{
		ERROR_AND_DIE("Could not load dxgidebug.dll");
	}
	typedef HRESULT(WINAPI* GetDebugModuleCB)(REFIID, void**);
	((GetDebugModuleCB)::GetProcAddress((HMODULE)m_dxgiDebugModule, "DXGIGetDebugInterface"))
		(__uuidof(IDXGIDebug), &m_dxgiDebug);
	if (m_dxgiDebug == nullptr)
	{
		ERROR_AND_DIE("Could not load debug module.");
	}
#endif

	InitializeDeviceContext();//Create Swap chain and device, also set device context
	InitializeRenderTargetView();//Get Back buffer and Create Render target view

	//Create and Bind Shader from source
	m_defaultShader = CreateDefaultShader();
	BindShader(CreateOrGetShader("Default"));
	//Create Vertex Buffer
	m_immediateVBO = CreateVertexBuffer(1, sizeof(Vertex_PCU));
	//Create Constant Buffer
	m_cameraCBO = CreateConstantBuffer(sizeof(CameraConstants)); //Must be a multiple of 16
	m_modelCBO = CreateConstantBuffer(sizeof(ModelConstants));
	//Create Default Texture
	m_defaultTexture = CreateTextureFromImage(Image(IntVec2(2, 2), Rgba8(255, 255, 255, 255)));
	
	InitializeBlendModes();//Create Blend Modes. Default to OPAQUE
	InitializeSamplerStates();//Create Sampler States. Default to POINT_CLAMP
	InitializeRasterizerStates();//Create and Set Rasterizer State
	InitializeDepthModes();//Create and Set Depth Modes
}

void Renderer::BeginFrame()
{
	//Set render target
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilDSV);
}

void Renderer::EndFrame()
{
	if (m_renderConfig.m_window)
	{
		//Present
		HRESULT hr;
		hr = m_swapChain->Present(0, 0);
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			ERROR_AND_DIE("Device has been lost, application will close.");
		}
	}
}

void Renderer::Shutdown()
{
	//Release Loaded Shaders
	for (int i = 0; i < static_cast<int>(m_loadedShaders.size()); i++)
	{
		if (m_loadedShaders[i] != nullptr)
		{
			delete m_loadedShaders[i];
		}
	}
	m_loadedShaders.clear();

	//for (int i = 0; i < static_cast<int>(m_loadedFonts.size()); i++)
	//{
	//	if (m_loadedFonts[i] != nullptr)
	//	{
	//		delete m_loadedFonts[i];
	//	}
	//}
	m_loadedFonts.clear();

	//Release Loaded Textures
	for (int i = 0; i < static_cast<int>(m_loadedTextures.size()); i++)
	{
		if (m_loadedTextures[i] != nullptr)
		{
			delete m_loadedTextures[i];
		}
	}
	m_loadedTextures.clear();

	//Release Blend States
	for (int i = 0; i < static_cast<int>(BlendMode::COUNT); i++)
	{
		if (m_blendStates[i] != nullptr)
		{
			DX_SAFE_RELEASE(m_blendStates[i]);
		}
	}

	//Release Sampler States
	for (int i = 0; i < static_cast<int>(SamplerMode::COUNT); i++)
	{
		if (m_samplerStates[i] != nullptr)
		{
			DX_SAFE_RELEASE(m_samplerStates[i]);
		}
	}

	//Release Rasterizer states
	for (int i = 0; i < static_cast<int>(RasterizerMode::COUNT); i++)
	{
		if (m_rasterizerStates[i] != nullptr)
		{
			DX_SAFE_RELEASE(m_rasterizerStates[i]);
		}
	}

	for (int i = 0; i < static_cast<int>(DepthMode::COUNT); i++)
	{
		if (m_depthStencilStates[i] != nullptr)
		{
			DX_SAFE_RELEASE(m_depthStencilStates[i]);
		}
	}

	delete m_immediateVBO;
	delete m_modelCBO;
	m_modelCBO = nullptr;

	delete m_cameraCBO;
	m_cameraCBO = nullptr;

	DX_SAFE_RELEASE(m_depthStencilTexture);
	DX_SAFE_RELEASE(m_depthStencilDSV);
	DX_SAFE_RELEASE(m_renderTargetView);
	DX_SAFE_RELEASE(m_swapChain);
	DX_SAFE_RELEASE(m_deviceContext);
	DX_SAFE_RELEASE(m_device);

	//report error leaks and release debug module
#if defined(ENGINE_DEBUG_RENDER)
	((IDXGIDebug*)m_dxgiDebug)->ReportLiveObjects(
		DXGI_DEBUG_ALL,
		(DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)
	);

	((IDXGIDebug*)m_dxgiDebug)->Release();
	m_dxgiDebug = nullptr;

	::FreeLibrary((HMODULE)m_dxgiDebugModule);
	m_dxgiDebugModule = nullptr;
#endif
}

void Renderer::ClearScreen(const Rgba8& clearColor)
{
	//Clear screen
	float colorAsFloats[4];
	clearColor.GetAsFloats(colorAsFloats);
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, colorAsFloats);
	m_deviceContext->ClearDepthStencilView(m_depthStencilDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::BeginCamera(const Camera& camera)
{
	CameraConstants cam;
	
	float min;
	float max;
	if (camera.IsOrthoMode())
	{
		min = 0.f;
 		max = 0.f;
	}
	else
	{
		min = 0.f;
		max = 1.f;
	}
	cam.WorldToCameraTransform = camera.GetWorldToCameraTransform();
	cam.CameraToRenderTransform = camera.GetCameraToRenderTransform();
 	cam.RenderToClipTransform = camera.GetRenderToClipTransform();

	CopyCPUToGPU(&cam, sizeof(CameraConstants), m_cameraCBO);
	BindConstantBuffer(k_cameraConstantsSlot, m_cameraCBO);

	SetModelConstants();

	//Set viewport
	float viewToClient = (float)m_renderConfig.m_window->GetClientDimensions().x / G_SCREEN_SIZE_X;

	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = camera.GetViewPort().m_mins.x * viewToClient;
	viewport.TopLeftY = (float)m_renderConfig.m_window->GetClientDimensions().y - camera.GetViewPort().m_maxs.y * viewToClient;
	viewport.Width = camera.GetViewPort().GetDimensions().x * viewToClient;
	viewport.Height = camera.GetViewPort().GetDimensions().y * viewToClient;
	viewport.MinDepth = min;
	viewport.MaxDepth = max;
	m_deviceContext->RSSetViewports(1, &viewport);
}

void Renderer::EndCamera(const Camera& camera)
{
	camera;
}

void Renderer::DrawVertexArray(int numVertexes, const Vertex_PCU* vertexes)
{
	CopyCPUToGPU(vertexes, numVertexes, m_immediateVBO);
	DrawVertexBuffer(m_immediateVBO, numVertexes);
}

void Renderer::DrawVertexArray(std::vector<Vertex_PCU> const& verts)
{
	if(!verts.empty())
	{
		UINT numVerts = static_cast<unsigned int>(verts.size());
		const Vertex_PCU* addressOfFirstElement = &verts.front();

		CopyCPUToGPU(addressOfFirstElement, numVerts, m_immediateVBO);
		DrawVertexBuffer(m_immediateVBO, numVerts);
	}
}

void Renderer::DrawIndexArray(std::vector<Vertex_PCU> const& verts, std::vector<unsigned int> indexes)
{
	if (!verts.empty())
	{
		UINT numVerts = static_cast<unsigned int>(verts.size());
		const Vertex_PCU* addressOfFirstElement = &verts.front();
		CopyCPUToGPU(addressOfFirstElement, numVerts, m_immediateVBO);

		UINT numIndexes = static_cast<unsigned int>(indexes.size());
		const unsigned int* addressOfIndex = &indexes.front();
		IndexBuffer* iBuffer = CreateIndexBuffer(numIndexes);
		CopyCPUToGPU(addressOfIndex, numIndexes, iBuffer);
		DrawIndexBuffer(m_immediateVBO, iBuffer, numIndexes);
		delete iBuffer;
	}
}

void Renderer::DrawVertexBuffer(VertexBuffer* vbo, unsigned int vertexCount)
{
	SetStatesIfChanged();
	BindVertexBuffer(vbo);
	m_deviceContext->Draw(vertexCount, 0);
}

void Renderer::DrawIndexBuffer(VertexBuffer* vbo, IndexBuffer* ibo, unsigned int indexCount)
{
	SetStatesIfChanged();
	BindVertexBuffer(vbo);
	BindIndexBuffer(ibo);
	m_deviceContext->DrawIndexed(indexCount, 0, 0);
}

void Renderer::DrawLitIndexBuffer(VertexBuffer* vbo, IndexBuffer* ibo, ConstantBuffer* cbo, unsigned int indexCount)
{
	SetStatesIfChanged();
	BindVertexBuffer(vbo);
	BindIndexBuffer(ibo);
	BindConstantBuffer(k_indexLightingSlot, cbo);
	m_deviceContext->DrawIndexed(indexCount, 0, 0);
}

Image* Renderer::CreateImageFromFile(char const* imageFilePath)
{
	Image* newImage = new Image(imageFilePath);
	return newImage;
}

Texture* Renderer::CreateOrGetTextureFromFile(char const* imageFilePath)
{
	Texture* existingTexture = GetTextureFromFileName(imageFilePath);
	if (existingTexture)
	{
		return existingTexture;
	}

	Texture* newTexture = CreateTextureFromFile(imageFilePath);
	return newTexture;
}

Texture* Renderer::CreateTextureFromFile(char const* imageFilePath)
{
	//IntVec2 dimensions = IntVec2(0, 0); // This will be filled in for us to indicate image width & height
	//int bytesPerTexel = 0;					// ...and how many color components the image had (e.g. 3=RGB=24bit, 4=RGBA=32bit)

	//// Load (and decompress) the image RGB(A) bytes from a file on disk into a memory buffer (array of bytes)
	//stbi_set_flip_vertically_on_load(1); // We prefer uvTexCoords has origin (0,0) at BOTTOM LEFT
	//unsigned char* texelData = stbi_load(imageFilePath, &dimensions.x, &dimensions.y, &bytesPerTexel, 0);

	//// Check if the load was successful
	//GUARANTEE_OR_DIE(texelData, Stringf("Failed to load image \"%s\"", imageFilePath));

	Texture* newTexture = CreateTextureFromImage(*CreateImageFromFile(imageFilePath));

	//// Free the raw image texel data now that we've sent a copy of it down to the GPU to be stored in video memory
	//stbi_image_free(texelData);

	return newTexture;
}

Texture* Renderer::CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, uint8_t* texelData)
{
	GUARANTEE_OR_DIE(texelData, Stringf("CreateTextureFromData failed for \"%s\" - texelData was null!", name));
	GUARANTEE_OR_DIE(bytesPerTexel >= 3 && bytesPerTexel <= 4, Stringf("CreateTextureFromData failed for \"%s\" - unsupported BPP=%i (must be 3 or 4)", name, bytesPerTexel));
	GUARANTEE_OR_DIE(dimensions.x > 0 && dimensions.y > 0, Stringf("CreateTextureFromData failed for \"%s\" - illegal texture dimensions (%i x %i)", name, dimensions.x, dimensions.y));

	Texture* newTexture = new Texture();
	newTexture->m_name = name;
	newTexture->m_dimensions = dimensions;

	m_loadedTextures.push_back(newTexture);
	return newTexture;
}

Texture* Renderer::CreateTextureFromImage(const Image& image)
{
	Texture* newTexture = new Texture();
	newTexture->m_dimensions = image.GetDimensions();
	newTexture->m_name = image.GetImageFilePath();

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = image.GetDimensions().x;
	textureDesc.Height = image.GetDimensions().y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA textureData;
	textureData.pSysMem = image.GetRawData();
	textureData.SysMemPitch = 4 * image.GetDimensions().x;

	HRESULT hr;
	hr = m_device->CreateTexture2D(&textureDesc, &textureData, &newTexture->m_texture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("CreateTextureFromImage failed for image: \"%s\".", image.GetImageFilePath().c_str()));
	}

	hr = m_device->CreateShaderResourceView(newTexture->m_texture, NULL, &newTexture->m_shaderResourceView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("CreateShaderresourceView failed for image: \"%s\".", image.GetImageFilePath().c_str()));
	}

	m_loadedTextures.push_back(newTexture);
	return newTexture;
}

Texture* Renderer::GetTextureFromFileName(char const* imageFilePath)
{
	for (int i = 0; i < m_loadedTextures.size(); i++)
	{
		if (m_loadedTextures[i]->m_name == imageFilePath)
		{
			return m_loadedTextures[i];
		}
	}
	return nullptr;
}

void Renderer::BindTexture(Texture* texture)
{
	if (texture)
	{
		m_deviceContext->PSSetShaderResources(0, 1, &texture->m_shaderResourceView);
	}
	else
	{
		m_deviceContext->PSSetShaderResources(0, 1, &m_defaultTexture->m_shaderResourceView);
	}
}

BitmapFont* Renderer::CreateOrGetBitmapFont(const char* bitmapFontFilePathWithNoExtension)
{
	BitmapFont* existingFont = GetBitmapFontForFileName(bitmapFontFilePathWithNoExtension); // You need to write this
	if (existingFont)
	{
		return existingFont;
	}

	BitmapFont* newFont = CreateBitmapFontFromFile(bitmapFontFilePathWithNoExtension);
	return newFont;
}

BitmapFont* Renderer::GetBitmapFontForFileName(const char* bitmapFontFilePathWithNoExtension)
{
	for (int i = 0; i < m_loadedFonts.size(); i++)
	{
		if (m_loadedFonts[i]->m_fontFilePathNameWithNoExtension == bitmapFontFilePathWithNoExtension)
		{
			return m_loadedFonts[i];
		}
	}
	return nullptr;
}

BitmapFont* Renderer::CreateBitmapFontFromFile(const char* bitmapFontFilePathWithNoExtension)
{
	Texture* fontTexture = CreateTextureFromFile(bitmapFontFilePathWithNoExtension);
	BitmapFont* newFont = new BitmapFont(bitmapFontFilePathWithNoExtension, *fontTexture);
	m_loadedFonts.push_back(newFont);
	return newFont;
}

void Renderer::SetBlendMode(BlendMode blendMode)
{
	if (blendMode == BlendMode::ALPHA		||
		blendMode == BlendMode::ADDITIVE	||
		blendMode == BlendMode::OPAQUE		)
	{
		m_desiredBlendMode = blendMode;
	}
	else
	{
		ERROR_AND_DIE(Stringf("Unknown / unsupported blend mode #%i", blendMode));
	}
}

void Renderer::SetRasterizerMode(RasterizerMode rasMode)
{
	if (rasMode == RasterizerMode::SOLID_CULL_BACK ||
		rasMode == RasterizerMode::SOLID_CULL_NONE ||
		rasMode == RasterizerMode::WIREFRAME_CULL_BACK ||
		rasMode == RasterizerMode::WIREFRAME_CULL_NONE)
	{
		m_desiredRasterizerMode = rasMode;
	}
	else
	{
		ERROR_AND_DIE(Stringf("Unknown / unsupported rasterizer mode #%i", rasMode));
	}
}

void Renderer::SetStatesIfChanged()
{
	if (m_blendStates[(int)m_desiredBlendMode] != m_blendState)
	{
		m_blendState = m_blendStates[(int)m_desiredBlendMode];
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		UINT sampleMask = 0xffffffff;
		m_deviceContext->OMSetBlendState(m_blendState, blendFactor, sampleMask);
	}

	if (m_samplerStates[(int)m_desiredSamplerMode] != m_samplerState)
	{
		m_deviceContext->PSSetSamplers(0, 1, &m_samplerStates[(int)m_desiredBlendMode]);
		m_samplerState = m_samplerStates[(int)m_desiredBlendMode];
	}

	if (m_rasterizerStates[(int)m_desiredRasterizerMode] != m_rasterizerState)
	{
		m_deviceContext->RSSetState(m_rasterizerStates[(int)m_desiredRasterizerMode]);
		m_rasterizerState = m_rasterizerStates[(int)m_desiredRasterizerMode];
	}

	if (m_depthStencilStates[(int)m_desiredDepthMode] != m_depthStencilState)
	{
		m_deviceContext->OMSetDepthStencilState(m_depthStencilStates[(int)m_desiredDepthMode], 0);
		m_depthStencilState = m_depthStencilStates[(int)m_desiredDepthMode];
	}
}

void Renderer::SetModelConstants(const Mat44& modelTowWorldTransform, const Rgba8& modelColor)
{
	ModelConstants mod;
	mod.ModelToWorldTransform = modelTowWorldTransform;
	float rgba[4];
	modelColor.GetAsFloats(rgba);
	mod.ModelColor[0] = rgba[0];
	mod.ModelColor[1] = rgba[1];
	mod.ModelColor[2] = rgba[2];
	mod.ModelColor[3] = rgba[3];

	CopyCPUToGPU(&mod, sizeof(ModelConstants), m_modelCBO);
	BindConstantBuffer(k_modelConstantSlot, m_modelCBO);
}

void Renderer::SetSamplerMode(SamplerMode samplerMode)
{
	if (samplerMode == SamplerMode::BILINEAR_WRAP ||
		samplerMode == SamplerMode::POINT_CLAMP)
	{
		m_desiredSamplerMode = samplerMode;
	}
	else
	{
		ERROR_AND_DIE(Stringf("Unknown / unsupported sampler mode #%i", samplerMode));
	}
}

void Renderer::SetDepthMode(DepthMode depthMode)
{
	switch (depthMode)
	{
	case DepthMode::DISABLED:
		
	case DepthMode::READ_ONLY_ALWAYS:

	case DepthMode::READ_ONLY_LESS_EQUAL:

	case DepthMode::READ_WRITE_LESS_EQUAL:
		m_desiredDepthMode = depthMode;
		break;
	default:
		ERROR_AND_DIE(Stringf("Unknown / unsupported depth mode #%i", depthMode));
	}
}

Shader* Renderer::CreateOrGetShader(char const* shaderName, VertexType vType)
{
	vType;
	for (int shaderIndex = 0; shaderIndex < static_cast<int>(m_loadedShaders.size()); shaderIndex++)
	{
		if (m_loadedShaders[shaderIndex] != nullptr && m_loadedShaders[shaderIndex]->GetName() == shaderName)
		{
			return m_loadedShaders[shaderIndex];
		}
	}
	return CreateShader(shaderName, vType);
}

Shader* Renderer::CreateDefaultShader()
{
	return CreateShader("Default", DefaultShaderByteCode, VertexType::PCU);
}

Shader* Renderer::CreateShader(char const* shaderName, VertexType vType)
{
	std::string fileName = shaderName;
	fileName += ".hlsl";
	std::string sourceByteCode;
	FileReadToString(sourceByteCode, "Data/Shaders/" + fileName);
	return CreateShader(shaderName, sourceByteCode.c_str(), vType);
}

Shader* Renderer::CreateShader(char const* shaderName, char const* shaderSource, VertexType vType)
{
	vType;
	std::vector<uint8_t> vertexShaderByteCode;
	std::vector<uint8_t> pixelShaderByteCode;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* inputLayoutForVertex = nullptr;
	HRESULT hr;
	//Create vertex shader
	if (!CompileShaderToByteCode(vertexShaderByteCode, shaderName, shaderSource, "VertexMain", "vs_5_0"))
	{
		ERROR_AND_DIE("Could not compile Vertex Shader");
	}
	hr = m_device->CreateVertexShader(
		vertexShaderByteCode.data(),
		vertexShaderByteCode.size(),
		NULL, &vertexShader
	);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create vertex shader."));
	}
	//Create pixel shader
	if (!CompileShaderToByteCode(pixelShaderByteCode, shaderName, shaderSource, "PixelMain", "ps_5_0"))
	{
		ERROR_AND_DIE("Could not compile Pixel Shader");
	}
	hr = m_device->CreatePixelShader(
		pixelShaderByteCode.data(),
		pixelShaderByteCode.size(),
		NULL, &pixelShader
	);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create pixel shader."));
	}
	//Create local array of input descriptions
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	//Create input layout
	UINT numElements = ARRAYSIZE(inputElementDesc);
	hr = m_device->CreateInputLayout(
		inputElementDesc, numElements,
		vertexShaderByteCode.data(),
		vertexShaderByteCode.size(),
		&inputLayoutForVertex
	);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create vertex layouts.");
	}

	ShaderConfig config = ShaderConfig();
	config.m_name = shaderName;
	Shader* makeShader = new Shader(config);
	makeShader->m_vertexShader = vertexShader;
	makeShader->m_pixelShader = pixelShader;
	makeShader->m_inputLayout = inputLayoutForVertex;

	m_loadedShaders.push_back(makeShader);
	return makeShader;
}

bool Renderer::CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target)
{
	HRESULT hr;
	DWORD shaderFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#if defined(ENGINE_DEBUG_RENDER)
	shaderFlags = D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	shaderFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#endif
	ID3DBlob* shaderBlob = NULL;
	ID3DBlob* errorBlob = NULL;

	hr = D3DCompile(
		source, strlen(source),
		name, nullptr, nullptr,
		entryPoint, target, shaderFlags, 0,
		&shaderBlob, &errorBlob
	);
	if (SUCCEEDED(hr))
	{
		outByteCode.resize(shaderBlob->GetBufferSize());
		memcpy(
			outByteCode.data(),
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize()
		);
	}
	else
	{
		if (errorBlob != NULL)
		{
			DebuggerPrintf((char*)errorBlob->GetBufferPointer());
		}
		return false;
	}
	shaderBlob->Release();
	if (errorBlob != NULL)
	{
		errorBlob->Release();
	}
	return true;
}

void Renderer::BindShader(Shader* shader)
{
	if (shader == nullptr)
	{
		m_deviceContext->VSSetShader(m_defaultShader->m_vertexShader, nullptr, 0);
		m_deviceContext->PSSetShader(m_defaultShader->m_pixelShader, nullptr, 0);
		m_deviceContext->IASetInputLayout(m_defaultShader->m_inputLayout);
		m_currentShader = shader;
	}
	else
	{
		m_deviceContext->VSSetShader(shader->m_vertexShader, nullptr, 0);
		m_deviceContext->PSSetShader(shader->m_pixelShader, nullptr, 0);
		m_deviceContext->IASetInputLayout(shader->m_inputLayout);
		m_currentShader = shader;
	}
}

VertexBuffer* Renderer::CreateVertexBuffer(const unsigned int size, unsigned int stride)
{
	VertexBuffer* makeBuffer = new VertexBuffer(m_device, size, stride);
	return makeBuffer;
}

void Renderer::CopyCPUToGPU(const void* data, unsigned int size, VertexBuffer* vbo)
{
	if (vbo->GetSize() < size*vbo->GetStride())
	{
		vbo->Resize(size * vbo->GetStride());
	}

	//copy vertices
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(vbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size*vbo->GetStride());
	m_deviceContext->Unmap(vbo->m_buffer, 0);
}

void Renderer::BindVertexBuffer(VertexBuffer* vbo)
{
	UINT stride = vbo->GetStride();
	UINT startOffset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &vbo->m_buffer, &stride, &startOffset);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ConstantBuffer* Renderer::CreateConstantBuffer(const unsigned int size)
{
	ConstantBuffer* makeBuffer = new ConstantBuffer(size);

	//Create Constant Buffer
	HRESULT hr;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = m_device->CreateBuffer(&bufferDesc, nullptr, &makeBuffer->m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create constant buffer");
	}
	return makeBuffer;
}

void Renderer::CopyCPUToGPU(const void* data, unsigned int size, ConstantBuffer* cbo)
{
	//copy vertices
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(cbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap(cbo->m_buffer, 0);
}

void Renderer::BindConstantBuffer(int slot, ConstantBuffer* cbo)
{
	m_deviceContext->VSSetConstantBuffers(slot, 1, &cbo->m_buffer);
	m_deviceContext->PSSetConstantBuffers(slot, 1, &cbo->m_buffer);
}

IndexBuffer* Renderer::CreateIndexBuffer(const unsigned int size)
{
	//Create Index Buffer
	IndexBuffer* returnBuffer = new IndexBuffer(size);
	HRESULT hr;
	UINT vertexBufferSize = returnBuffer->m_size * returnBuffer->GetStride();
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = vertexBufferSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = m_device->CreateBuffer(&bufferDesc, nullptr, &returnBuffer->m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create index buffer");
	}
	return returnBuffer;
}

void Renderer::CopyCPUToGPU(const void* data, unsigned int size, IndexBuffer* ibo)
{
	//copy vertices
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(ibo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size * ibo->GetStride());
	m_deviceContext->Unmap(ibo->m_buffer, 0);
}

void Renderer::BindIndexBuffer(IndexBuffer* ibo)
{
	m_deviceContext->IASetIndexBuffer(ibo->m_buffer, DXGI_FORMAT_R32_UINT, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::InitializeDeviceContext()
{
	unsigned int deviceFlags = 0;
#if defined(ENGINE_DEBUG_RENDER)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = m_renderConfig.m_window->GetClientDimensions().x;
	swapChainDesc.BufferDesc.Height = m_renderConfig.m_window->GetClientDimensions().y;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = (HWND)m_renderConfig.m_window->GetHwnd();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags,
		nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc,
		&m_swapChain, &m_device, nullptr, &m_deviceContext);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create D3D 11 device and swap chain.");
	}
}

void Renderer::InitializeRenderTargetView()
{
	ID3D11Texture2D* backBuffer;
	HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not get chain buffer.");
	}
	hr = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create render target view for swap chain.");
	}
	backBuffer->Release();
}

void Renderer::InitializeBlendModes()
{
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = blendDesc.RenderTarget[0].SrcBlend;
	blendDesc.RenderTarget[0].DestBlendAlpha = blendDesc.RenderTarget[0].DestBlend;
	blendDesc.RenderTarget[0].BlendOpAlpha = blendDesc.RenderTarget[0].BlendOp;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT hr = m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)(BlendMode::OPAQUE)]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create blend state (Startup: OPAQUE)");
	}
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	hr = m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)(BlendMode::ALPHA)]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create blend state (Startup: ALPHA)");
	}
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	hr = m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)(BlendMode::ADDITIVE)]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create blend state (Startup: ADDITIVE)");
	}
	m_desiredBlendMode = BlendMode::ALPHA;
}

void Renderer::InitializeSamplerStates()
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerStates[(int)SamplerMode::POINT_CLAMP]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("SamplerState could not be created. Mode: POINT_CLAMP");
	}
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerStates[(int)SamplerMode::BILINEAR_WRAP]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("SamplerState could not be created. Mode: BILINEAR_WRAP");
	}
	m_deviceContext->PSSetSamplers(0, 1, &m_samplerStates[(int)SamplerMode::POINT_CLAMP]);
	m_samplerState = m_samplerStates[(int)SamplerMode::POINT_CLAMP];
	m_desiredSamplerMode = SamplerMode::POINT_CLAMP;
}

void Renderer::InitializeRasterizerStates()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.AntialiasedLineEnable = true;
	HRESULT hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_NONE]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create rasterizer state SOLID_CULL_NONE.");
	}
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_BACK]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create rasterizer state SOLID_CULL_BACK.");
	}
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStates[(int)RasterizerMode::WIREFRAME_CULL_NONE]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create rasterizer state WIREFRAME_CULL_NONE.");
	}
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStates[(int)RasterizerMode::WIREFRAME_CULL_BACK]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create rasterizer state WIREFRAME_CULL_BACK.");
	}
	m_deviceContext->RSSetState(m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_BACK]);
	m_desiredRasterizerMode = RasterizerMode::SOLID_CULL_BACK;
}

void Renderer::InitializeDepthModes()
{
	D3D11_TEXTURE2D_DESC depthTextureDesc = {};
	depthTextureDesc.Width = m_renderConfig.m_window->GetClientDimensions().x;
	depthTextureDesc.Height = m_renderConfig.m_window->GetClientDimensions().y;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.SampleDesc.Count = 1;
	HRESULT hr = m_device->CreateTexture2D(&depthTextureDesc, nullptr, &m_depthStencilTexture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create texture for depth stencil.");
	}
	hr = m_device->CreateDepthStencilView(m_depthStencilTexture, nullptr, &m_depthStencilDSV);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create depth stencil view.");
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStates[(int)DepthMode::DISABLED]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create depth stencil state for DISABLED");
	}
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStates[(int)DepthMode::READ_ONLY_ALWAYS]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create depth stencil state for READ_ONLY_ALWAYS");
	}
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStates[(int)DepthMode::READ_ONLY_LESS_EQUAL]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create depth stencil state for READ_ONLY_LESS_EQUAL");
	}
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStates[(int)DepthMode::READ_WRITE_LESS_EQUAL]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create depth stencil state for READ_WRITE_LESS_EQUAL");
	}
	m_desiredDepthMode = DepthMode::DISABLED;
}