#include "./misc.h"

void misc::bunnyhop() {
	while (gui::shouldRun) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (!config::cfg.misc.bhop || !process.InForeground())
			continue;
		uintptr_t lPawn = process.Read<uintptr_t>(client + dwLocalPlayerPawn);
		const auto fFlags = process.Read<int>(lPawn + m_fFlags);
		if (GetAsyncKeyState(VK_SPACE) & 0x8000 && fFlags & (1 << 0)) {
			mouse_event(MOUSEEVENTF_WHEEL, 0, 0, WHEEL_DELTA, 0);
		}
	}
}

void misc::crosshair() {
	uintptr_t lPawn = process.Read<uintptr_t>(client + dwLocalPlayerPawn);
	bool bIsScoped = process.Read<bool>(lPawn + m_bIsScoped);
	if (bIsScoped)
		return;
	uintptr_t pClippingWeapon = process.Read<uintptr_t>(lPawn + m_pClippingWeapon);
	uint16_t wepDefIndex = process.Read<uint16_t>(pClippingWeapon + m_AttributeManager + m_Item + m_iItemDefinitionIndex);
	if (wepDefIndex == 9 || wepDefIndex == 11 || wepDefIndex == 38 || wepDefIndex == 40) {
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - 10, ImGui::GetIO().DisplaySize.y / 2), ImVec2(ImGui::GetIO().DisplaySize.x / 2 + 10, ImGui::GetIO().DisplaySize.y / 2), ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.misc.crosshairColor), 2);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2 - 10), ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2 + 10), ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.misc.crosshairColor), 2);

	}
}