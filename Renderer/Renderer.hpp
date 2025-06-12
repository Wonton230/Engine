#pragma once
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Core/Vertex_PCUTBN.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Game/EngineBuildPreferences.hpp"
#include <vector>

class Texture;
class BitmapFont;
class Shader;
class Image;
class VertexBuffer;
class ConstantBuffer;
class IndexBuffer;
struct IntVec2;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11RasterizerState;
struct ID3D11BlendState;
struct ID3D11SamplerState;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;

#define DX_SAFE_RELEASE(dxObject)	{	if ((dxObject) != nullptr)	{		(dxObject)->Release();		(dxObject) = nullptr;	} }

#if defined(OPAQUE)
#undef OPAQUE
#endif

struct CameraConstants
{
	Mat44 WorldToCameraTransform;
	Mat44 CameraToRenderTransform;
	Mat44 RenderToClipTransform;
};
static const int k_cameraConstantsSlot = 2;

struct ModelConstants
{
	Mat44 ModelToWorldTransform;
	float ModelColor[4];
};
static const int k_modelConstantSlot = 3;

struct PointLight
{
	Vec3	LightPosition;
	float	Intensity;
	float	LightColor[4];
};

struct LightConstants
{
	Vec3  SunDirection = Vec3(2.f, 1.f, -1.f);
	float SunIntensity = .85f;
	float AmbientIntensity = .35f;
	Vec3  ByteKill = Vec3(2.f, 1.f, -1.f);
	int   NumPointLights = 0;
	Vec3  ByteKill2 = Vec3(2.f, 1.f, -1.f);
	PointLight PointLightList[64];
};

static const int k_indexLightingSlot = 1;

enum class BlendMode
{
	ALPHA,
	ADDITIVE,
	OPAQUE,
	COUNT
};

enum class SamplerMode
{
	POINT_CLAMP,
	BILINEAR_WRAP,
	COUNT
};

enum class RasterizerMode
{
	SOLID_CULL_NONE,
	SOLID_CULL_BACK,
	WIREFRAME_CULL_NONE,
	WIREFRAME_CULL_BACK,
	COUNT
};

enum class DepthMode
{
	DISABLED,
	READ_ONLY_ALWAYS,
	READ_ONLY_LESS_EQUAL,
	READ_WRITE_LESS_EQUAL,
	COUNT
};

enum class VertexType
{
	PCU,
	PCUTBN,
	COUNT
};

struct RenderConfig
{
	Window* m_window;
};

class Renderer
{
public:
	Renderer();
	Renderer(RenderConfig renderConfig);
	~Renderer();
	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	//Cameras
	void ClearScreen(const Rgba8& clearColor);
	void BeginCamera(const Camera& camera);
	void EndCamera(const Camera& camera);

	//Drawing
	void DrawVertexArray(int numVertexes, const Vertex_PCU* vertexes);
	void DrawVertexArray(std::vector<Vertex_PCU> const& verts);
	void DrawIndexArray(std::vector<Vertex_PCU> const& verts, std::vector<unsigned int> indexes);
	void DrawVertexBuffer(VertexBuffer* vbo, unsigned int vertexCount);
	void DrawIndexBuffer(VertexBuffer* vbo, IndexBuffer* ibo, unsigned int indexCount);
	void DrawLitIndexBuffer(VertexBuffer* vbo, IndexBuffer* ibo, ConstantBuffer* cbo, unsigned int indexCount);

	//Image Methods
	Image* CreateImageFromFile(char const* imageFilePath);

	//Texture Methods
	Texture* CreateOrGetTextureFromFile(char const* imageFilePath);
	Texture* CreateTextureFromFile(char const* imageFilePath);
	Texture* CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, uint8_t* texelData);
	Texture* CreateTextureFromImage(const Image& image);
	Texture* GetTextureFromFileName(char const* imageFilePath);
	void	 BindTexture(Texture* texture);

	//Font Methods
	BitmapFont* CreateOrGetBitmapFont(const char* bitmapFontFilePathWithNoExtension);
	BitmapFont* GetBitmapFontForFileName(const char* bitmapFontFilePathWithNoExtension);
	BitmapFont* CreateBitmapFontFromFile(const char* bitmapFontFilePathWithNoExtension);

	//Blend Methods
	void SetBlendMode(BlendMode blendMode);

	//Sampler Methods
	void SetSamplerMode(SamplerMode samplerMode);

	//Depth Methods
	void SetDepthMode(DepthMode depthMode);

	//Rasterizer Methods
	void SetRasterizerMode(RasterizerMode rasMode);

	void SetStatesIfChanged();
	void SetModelConstants(const Mat44& modelTowWorldTransform = Mat44(), const Rgba8& modelColor = Rgba8::WHITE);

	//Shader Methods
	Shader* CreateOrGetShader(char const* shaderName, VertexType vType = VertexType::PCU);
	Shader* CreateDefaultShader();
	Shader* CreateShader(char const* shaderName, VertexType vType = VertexType::PCU);
	Shader* CreateShader(char const* shaderName, char const* shaderSource, VertexType vType = VertexType::PCU);
	bool CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target);
	void BindShader(Shader* shader);

	//Buffer Methods
	VertexBuffer* CreateVertexBuffer(const unsigned int size, unsigned int stride);
	void CopyCPUToGPU(const void* data, unsigned int size, VertexBuffer* vbo);
	void BindVertexBuffer(VertexBuffer* vbo);
	ConstantBuffer* CreateConstantBuffer(const unsigned int size);
	void CopyCPUToGPU(const void* data, unsigned int size, ConstantBuffer* cbo);
	void BindConstantBuffer(int slot, ConstantBuffer* cbo);
	IndexBuffer* CreateIndexBuffer(const unsigned int size);
	void CopyCPUToGPU(const void* data, unsigned int size, IndexBuffer* ibo);
	void BindIndexBuffer(IndexBuffer* ibo);

private:
	//Initializer Helpers for Startup()
	void InitializeDeviceContext();
	void InitializeRenderTargetView();
	void InitializeBlendModes();
	void InitializeSamplerStates();
	void InitializeRasterizerStates();
	void InitializeDepthModes();

	RenderConfig m_renderConfig;
	std::vector<Texture*> m_loadedTextures;
	std::vector<BitmapFont*> m_loadedFonts;
	std::vector<Shader*> m_loadedShaders;

	const Texture* m_defaultTexture = nullptr;
	Shader* m_currentShader = nullptr;
	Shader* m_defaultShader = nullptr;
	BlendMode m_desiredBlendMode = BlendMode::ALPHA;
	BlendMode m_currentBlendMode = BlendMode::ALPHA;
	RasterizerMode m_desiredRasterizerMode = RasterizerMode::SOLID_CULL_NONE;
	DepthMode m_desiredDepthMode = DepthMode::READ_WRITE_LESS_EQUAL;
	SamplerMode m_desiredSamplerMode = SamplerMode::POINT_CLAMP;

protected:
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11RasterizerState* m_rasterizerState = nullptr;
	ID3D11RasterizerState* m_rasterizerStates[(int)RasterizerMode::COUNT] = {};
	VertexBuffer* m_immediateVBO = nullptr;
	ConstantBuffer* m_cameraCBO = nullptr;
	ConstantBuffer* m_modelCBO = nullptr;
	ID3D11BlendState* m_blendState = nullptr;
	ID3D11BlendState* m_blendStates[(int)(BlendMode::COUNT)] = {};
	ID3D11SamplerState* m_samplerState = nullptr;
	ID3D11SamplerState* m_samplerStates[(int)SamplerMode::COUNT] = {};
	ID3D11Texture2D* m_depthStencilTexture = nullptr;
	ID3D11DepthStencilView* m_depthStencilDSV = nullptr;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	ID3D11DepthStencilState* m_depthStencilStates[(int)DepthMode::COUNT] = {};

#if defined(ENGINE_DEBUG_RENDER)
	void* m_dxgiDebug = nullptr;
	void* m_dxgiDebugModule = nullptr;
#endif
};