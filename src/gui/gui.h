#pragma once
#include "../include.h"

namespace gui {
	inline WNDCLASSEX wc;
	inline HWND hwnd;
	inline DXGI_SWAP_CHAIN_DESC sd{};
	inline ID3D11Device* device{};
	inline ID3D11DeviceContext* context{};
	inline IDXGISwapChain* swapchain{};
	inline ID3D11RenderTargetView* render_target_view{};
	inline D3D_FEATURE_LEVEL level{};
	inline ID3D11Texture2D* back_buffer{};
	inline ID3D11ShaderResourceView* espCharacter = nullptr;
	inline bool shouldRun = true;
	inline bool isOpen = true;
	inline bool espPreview = true;
	EXTERN_C DWORD PrepareForUIAccess();

	void setupWindow(HINSTANCE instance);
	void setupDX11(int nCmdShow);
	void setupImgui();
	void beginRender();
	void render();
	void endRender();
	void cleanUp();
}