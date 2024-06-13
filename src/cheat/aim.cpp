#include "./aim.h"

void aim::triggerbot() {
	while (gui::shouldRun) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		ViewMatrix vm = process.Read<ViewMatrix>(client + dwViewMatrix);
		if (!config::cfg.aimbot.triggerBot || !process.InForeground() || !GetAsyncKeyState(config::cfg.aimbot.triggerBotKey))
			continue;
		uintptr_t lPawn = process.Read<uintptr_t>(client + dwLocalPlayerPawn);
		const auto lTeam = process.Read<uint8_t>(lPawn + m_iTeamNum);
		uintptr_t pClippingWeapon = process.Read<uintptr_t>(lPawn + m_pClippingWeapon);
		uint16_t wepDefIndex = process.Read<uint16_t>(pClippingWeapon + m_AttributeManager + m_Item + m_iItemDefinitionIndex);
		if (wepDefIndex == 42 || wepDefIndex == 59)
			continue;
		int32_t entIdx = process.Read<int32_t>(lPawn + m_iIDEntIndex);
		uintptr_t entEntry = process.Read<uintptr_t>(process.Read<uintptr_t>(client + dwEntityList) + 0x8 * (entIdx >> 9) + 16);
		uintptr_t pPawn = process.Read<uintptr_t>(entEntry + 120 * (entIdx & 0x1FF));
		if (pPawn == 0)
			continue;
		const auto pHealth = process.Read<int32_t>(pPawn + m_iHealth);
		const auto pTeam = process.Read<uint8_t>(pPawn + m_iTeamNum);
		if (pHealth <= 0 || (lTeam == pTeam && !config::cfg.aimbot.triggerBotTargetsTeam))
			continue;
		if (config::cfg.aimbot.triggerMagnet) {
			uintptr_t gameSceneNode = process.Read<uintptr_t>(pPawn + m_pGameSceneNode);
			auto boneAddress = process.Read<uintptr_t>(gameSceneNode + 0x170 + 0x80) + (config::cfg.aimbot.selectedHitbox * 32);
			auto bonePosition = process.Read<Vector>(boneAddress);
			ImVec2 screenPos;
			w2s(bonePosition, screenPos, vm);
			mouse_event(MOUSEEVENTF_MOVE, screenPos.x - ImGui::GetIO().DisplaySize.x * 0.5, screenPos.y - ImGui::GetIO().DisplaySize.y * 0.5, 0, 0);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(config::cfg.aimbot.triggerBotDelay));
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
}