#include "./esp.h"

void esp::runESP() {
	ViewMatrix vm = process.Read<ViewMatrix>(client + dwViewMatrix);
	uintptr_t lPawn = process.Read<uintptr_t>(client + dwLocalPlayerPawn);
	for (int32_t i = 1; i <= 1024; i++) {
		std::map<std::string, ImVec2> boneScreenPos;
		const auto pController = gameEntitySystem.entity_list.get_player_controller(i);
		if (pController == 0)
			continue;

		char pName[128];
		process.ReadRaw(pController + m_iszPlayerName, pName, sizeof(pName));
		const uint64_t pPawn = gameEntitySystem.entity_list.get_player_pawn(pController);
		if (pPawn == 0 || pPawn == lPawn)
			continue;

		const auto pHealth = process.Read<int32_t>(pPawn + m_iHealth);
		const auto pMaxHealth = process.Read<int32_t>(pPawn + m_iMaxHealth);
		if (pHealth <= 0)
			continue;

		const auto pTeam = process.Read<uint8_t>(pPawn + m_iTeamNum);
		if (pTeam == process.Read<uint8_t>(lPawn + m_iTeamNum) && !config::cfg.esp.displayTeam)
			continue;

		uintptr_t gameSceneNode = process.Read<uintptr_t>(pPawn + m_pGameSceneNode);
		uintptr_t collision = process.Read<uintptr_t>(pPawn + m_pCollision);
		Vector vecOrigin = process.Read<Vector>(gameSceneNode + m_vecOrigin);
		Vector vecMins = process.Read<Vector>(collision + m_vecMins);
		Vector vecMaxs = process.Read<Vector>(collision + m_vecMaxs);
		vecMins += vecOrigin;
		vecMaxs += vecOrigin;
		Vector point_list[] = {
			Vector(vecMins.x, vecMins.y, vecMins.z), Vector(vecMins.x, vecMaxs.y, vecMins.z),
			Vector(vecMaxs.x, vecMaxs.y, vecMins.z), Vector(vecMaxs.x, vecMins.y, vecMins.z),
			Vector(vecMaxs.x, vecMaxs.y, vecMaxs.z), Vector(vecMins.x, vecMaxs.y, vecMaxs.z),
			Vector(vecMins.x, vecMins.y, vecMaxs.z), Vector(vecMaxs.x, vecMins.y, vecMaxs.z),
		};

		ImVec2 screen_pos[8]{ ImVec2(0,0) };

		float left = FLT_MAX, top = FLT_MAX, right = FLT_MIN, bottom = FLT_MIN;

		for (size_t i = 0; i < 8; i++)
		{
			if (!w2s(point_list[i], screen_pos[i], vm))
				continue;
			left = std::round(std::min<float>(left, screen_pos[i].x));
			top = std::round(std::min<float>(top, screen_pos[i].y));
			right = std::round(std::max<float>(right, screen_pos[i].x));
			bottom = std::round(std::max<float>(bottom, screen_pos[i].y));
		}

		if (left == FLT_MAX || top == FLT_MAX || right == FLT_MIN || bottom == FLT_MIN)
			continue;

		ImVec2 characterPos = ImVec2(left, top);
		ImVec2 characterSize = ImVec2(right - left, bottom - top);

		if (config::cfg.esp.boundingBox) {
			ImGui::GetBackgroundDrawList()->AddOutlinedRect(characterPos, characterPos + characterSize, IM_COL32_WHITE, IM_COL32_BLACK);
			if (config::cfg.esp.boundingBoxGlow)
				ImGui::GetBackgroundDrawList()->AddShadowRect(characterPos, characterPos + characterSize, ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.bboxGlow), 50.f, ImVec2(0, 0), ImDrawFlags_ShadowCutOutShapeBackground);
		}

		if (config::cfg.esp.name) {
			ImVec2 textSize = ImGui::CalcTextSizeFont(12.f, pName);
			ImVec2 textPos = ImVec2(characterPos.x + (characterSize.x - textSize.x) / 2, characterPos.y - textSize.y);

			ImGui::GetBackgroundDrawList()->AddOutlinedText(ImGui::GetIO().Fonts->Fonts[1], 12.f, textPos, ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.nameColor), ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.nameOutline), pName);
		}

		if (config::cfg.esp.health) {
			ImVec2 barPos = characterPos + ImVec2(-6, -1);
			float factor = static_cast<float>(pHealth) / pMaxHealth;
			ImVec2 textSize = ImGui::CalcTextSizeFont(12.f, std::to_string(pHealth).c_str());
			ImVec2 textPos = ImVec2(barPos.x - textSize.x, barPos.y + characterSize.y - (characterSize.y * factor) - 1);
			ImGui::GetBackgroundDrawList()->AddRectFilled(barPos, barPos + ImVec2(4, characterSize.y + 2), IM_COL32_BLACK);
			ImGui::GetBackgroundDrawList()->AddRectFilled(barPos + ImVec2(1, characterSize.y - (characterSize.y * factor) + 1), barPos + ImVec2(3, characterSize.y + 1), ImColor((1.0f - (factor * 1.0f)), (factor * 1.0f), 0.f, 1.f));
			if (pHealth < 100)
				ImGui::GetBackgroundDrawList()->AddOutlinedText(ImGui::GetIO().Fonts->Fonts[1], 12.f, textPos, IM_COL32_WHITE, IM_COL32_BLACK, std::to_string(pHealth).c_str());
			if (config::cfg.esp.healthGlow)
				ImGui::GetBackgroundDrawList()->AddShadowRect(barPos + ImVec2(1, 1), barPos + ImVec2(3, characterSize.y + 1), ImColor((1.0f - (factor * 1.0f)), (factor * 1.0f), 0.f, 1.f), 50.f, ImVec2(0, 0), ImDrawFlags_ShadowCutOutShapeBackground);
		}

		if (config::cfg.esp.gun) {
			uintptr_t pClippingWeapon = process.Read<uintptr_t>(pPawn + m_pClippingWeapon);
			uint16_t wepDefIndex = process.Read<uint16_t>(pClippingWeapon + m_AttributeManager + m_Item + m_iItemDefinitionIndex);
			ImVec2 textSize = ImGui::CalcTextSizeFont(12.f, weaponMap[wepDefIndex].c_str());
			ImVec2 textPos = ImVec2(characterPos.x + (characterSize.x - textSize.x) / 2, characterPos.y + characterSize.y + 2);
			ImGui::GetBackgroundDrawList()->AddOutlinedText(ImGui::GetIO().Fonts->Fonts[1], 12.f, textPos, ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.gunColor), ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.gunOutline), weaponMap[wepDefIndex].c_str());
		}

		if (config::cfg.esp.skeleton) {
				for (const auto& entry : boneMap) {
					auto boneAddress = process.Read<uintptr_t>(gameSceneNode + 0x170 + 0x80) + entry.second * 32;
					auto bonePosition = process.Read<Vector>(boneAddress);
					if (!w2s(bonePosition, boneScreenPos[entry.first], vm))
						continue;
				}

				for (const auto& connection : boneConnections) {
					const std::string& boneFrom = connection.first;
					const std::string& boneTo = connection.second;
					if (boneScreenPos[boneFrom] != ImVec2(0, 0) && boneScreenPos[boneTo] != ImVec2(0, 0)) {
						ImGui::GetBackgroundDrawList()->AddLine(boneScreenPos[boneFrom], boneScreenPos[boneTo], IM_COL32_BLACK, 2.f);
						ImGui::GetBackgroundDrawList()->AddLine(boneScreenPos[boneFrom], boneScreenPos[boneTo], ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.skeletonColor));
					}
				}
		}
	}
}