#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#define dwGameEntitySystem 0x1AD38B8
#define m_iszPlayerName 0x630
#define m_iMaxHealth 0x320
#define m_iHealth 0x324
#define m_pGameSceneNode 0x308
#define m_pCollision 0x318
#define m_vecOrigin  0x88
#define m_vecMins 0x40
#define m_vecMaxs 0x4C
#define dwViewMatrix 0x1A16A50
#define m_iTeamNum 0x3C3
#define dwLocalPlayerPawn 0x181A998
#define m_pClippingWeapon 0x12F0
#define m_AttributeManager 0x1090
#define m_Item 0x50
#define m_iItemDefinitionIndex 0x1BA
#define m_fFlags 0x3CC
#define m_iIDEntIndex 0x13A8
#define m_bIsScoped 0x2290
#define dwEntityList 0x19B4998
#include "../includes/imgui/imgui_internal.h"
#include "../includes/imgui/imgui.h"
#include "../includes/imgui/backends/imgui_impl_dx11.h"
#include "../includes/imgui/backends/imgui_impl_win32.h"
#include <d3d11.h>
#include <D3DX11tex.h>
#include <tchar.h>
#include <dwmapi.h>
#include <filesystem>
#include <thread>
#include <iostream>
#include <TlHelp32.h>
#include <string>
#include <Psapi.h>
#include <cstdint>
#include "../utils/logger.h"
#include "../utils/w2s.h"
#include "../assets/csCharacter.h"
#include "../includes/imgui/misc/fonts/smallest_pixel.h"
#include "../includes/imgui/misc/fonts/nunito.h"
#include "../includes/imgui/misc/fonts/fa_solid_900.h"
#include "../includes/imgui/misc/fonts/FontAwesome6.h"
#include "../includes/imgui/ImGuiNotify.h"
#include "../includes/imgui/misc/fonts/byteguardian.h"
#include "src/memify/handle_hijack.h"
#include "src/memify/memify.h"
inline memify process("cs2.exe");
inline uint64_t client = process.GetBase("client.dll");
#include "includes/yas/serialize.hpp"
#include "includes/yas/std_types.hpp"
#include "src/config/config.h"
#include "src/cheat/EntitySystem.h"
inline GameEntitySystem gameEntitySystem = process.Read<GameEntitySystem>(process.Read<uintptr_t>(client + dwGameEntitySystem));
#include "src/cheat/esp.h"
#include "src/cheat/misc.h"
#include "src/cheat/aim.h"
#include "src/menu/menu.h"
#include "src/gui/gui.h"