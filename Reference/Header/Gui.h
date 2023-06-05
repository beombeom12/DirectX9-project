#pragma once

#include "Engine_Include.h"

namespace  gui
{
	bool open = true;

	bool setup = false;

	HWND window = nullptr;
	WNDCLASSEX windowClass = {};
	WNDPROC origianlWindowProcess = nullptr;

	LPDIRECT3DDEVICE9 device = nullptr;
	LPDIRECT3D9 d3d9 = nullptr;

	bool SetupWindowClass(const wchar_t* windowClassName) noexcept;
	void DesstroyWindowClass() noexcept;

	bool SetUpWindow(const wchar_t* windowName) noexcept;
	void DestroyWindow() noexcept;

	bool SetupDirectX() noexcept;
	void DestroyDirectX() noexcept;

	void SetUp();
	void SetUpMenu(LPDIRECT3DDEVICE9 device) noexcept;
	void Destroy() noexcept;

	void Render() noexcept;
}
