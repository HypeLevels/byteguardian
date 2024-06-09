#include "./esp.h"
static std::map<int, std::string> weaponMap = {
    {1, "DEAGLE"},
    {2, "BERETTAS"},
    {3, "FIVESEVEN"},
    {4, "GLOCK"},
    {7, "AK47"},
    {8, "AUG"},
    {9, "AWP"},
    {10, "FAMAS"},
    {11, "G3SG1"},
    {13, "GALIL"},
    {14, "M249"},
    {16, "M4A1"},
    {17, "MAC10"},
    {19, "P90"},
    {23, "MP5SD"},
    {24, "UMP45"},
    {25, "XM1014"},
    {26, "BIZON"},
    {27, "MAG7"},
    {28, "NEGEV"},
    {29, "SAWEDOFF"},
    {30, "TEC9"},
    {31, "TASER"},
    {32, "P2000"},
    {33, "MP7"},
    {34, "MP9"},
    {35, "NOVA"},
    {36, "P250"},
    {37, "SHIELD"},
    {38, "SCAR20"},
    {39, "SG556"},
    {40, "SSG08"},
    {41, "KNIFEGG"},
    {42, "CT KNIFE"},
    {43, "FLASHBANG"},
    {44, "HE"},
    {45, "SMOKE"},
    {46, "MOLOTOV"},
    {47, "DECOY"},
    {48, "INCENDIARY"},
    {49, "C4"},
    {57, "HEALTHSHOT"},
    {59, "T KNIFE"},
    {60, "M4A1-S"},
    {61, "USP-S"},
    {63, "CZ75A"},
    {64, "REVOLVER"},
    {68, "TAGRENADE"},
    {69, "FISTS"},
    {70, "BREACHCHARGE"},
    {72, "TABLET"},
    {74, "MELEE"},
    {75, "AXE"},
    {76, "HAMMER"},
    {78, "SPANNER"},
    {80, "GHOST"},
    {81, "FIREBOMB"},
    {82, "DIVERSION"},
    {83, "FRAG GRENADE"},
    {84, "SNOWBALL"},
    {85, "BUMPMINE"},
    {500, "BAYONET"},
    {503, "CSS"},
    {505, "FLIP"},
    {506, "GUT"},
    {507, "KARAMBIT"},
    {508, "M9 BAYONET"},
    {509, "TACTICAL"},
    {512, "FALCHION"},
    {514, "BOWIE"},
    {515, "BUTTERFLY"},
    {516, "PUSH"},
    {517, "CORD"},
    {518, "CANIS"},
    {519, "URSUS"},
    {520, "GYPSY JACKKNIFE"},
    {521, "OUTDOOR"},
    {522, "STILETTO"},
    {523, "WIDOWMAKER"},
    {525, "SKELETON"},
    {526, "KUKRI"},
    {5027, "BLOODHOUND GLOVES"},
    {5028, "T GLOVES"},
    {5029, "CT GLOVES"},
    {5030, "SPORTY GLOVES"},
    {5031, "SLICK GLOVES"},
    {5032, "LEATHER HANDWRAPS"},
    {5033, "MOTORCYCLE GLOVES"},
    {5034, "SPECIALIST GLOVES"},
    {5035, "HYDRA GLOVES"}
};


void esp::runESP() {
	ViewMatrix vm = process.Read<ViewMatrix>(client + dwViewMatrix);
	uintptr_t lPawn = process.Read<uintptr_t>(client + dwLocalPlayerPawn);
	for (int32_t i = 1; i <= 512; i++) {
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
			ImVec2 textPos = ImVec2(characterPos.x + ((characterSize.x - textSize.x) / 2), characterPos.y - textSize.y);

			ImGui::GetBackgroundDrawList()->AddOutlinedText(ImGui::GetIO().Fonts->Fonts[1], 12.f, textPos, ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.nameColor), ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.nameOutline), pName);
		}

		if (config::cfg.esp.health) {
			ImVec2 barPos = characterPos + ImVec2(-6, -1);
			float factor = static_cast<float>(pHealth) / pMaxHealth;
			ImVec2 textSize = ImGui::CalcTextSizeFont(10.f, std::to_string(pHealth).c_str());
			ImVec2 textPos = ImVec2(barPos.x - textSize.x, barPos.y + characterSize.y - (characterSize.y * factor) - 1);
			ImGui::GetBackgroundDrawList()->AddRectFilled(barPos, barPos + ImVec2(4, characterSize.y + 2), IM_COL32_BLACK);
			ImGui::GetBackgroundDrawList()->AddRectFilled(barPos + ImVec2(1, characterSize.y - (characterSize.y * factor) + 1), barPos + ImVec2(3, characterSize.y + 1), ImColor((1.0f - (factor * 1.0f)), (factor * 1.0f), 0.f, 1.f));
			if (pHealth < 100)
				ImGui::GetBackgroundDrawList()->AddOutlinedText(ImGui::GetIO().Fonts->Fonts[1], 10.f, textPos, IM_COL32_WHITE, IM_COL32_BLACK, std::to_string(pHealth).c_str());
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
	}
}