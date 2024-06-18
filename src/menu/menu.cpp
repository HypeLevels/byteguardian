#include "menu.h"

void menu::drawMenu() {
    ImGui::SetNextWindowSize(ImVec2(812, 614));
    ImGui::PushStyleColor(ImGuiCol_WindowShadow, ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.f));
    ImGui::Begin("byteguardian", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
    ImGui::PopStyleColor();
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
    ImGui::Text("A");
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ImGui::CalcTextSize("byteguardian").y) / 4);
    ImGui::Text("byteguardian");
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - ImGui::CalcTextSize(gui::espPreview ? ICON_FA_SQUARE_MINUS : ICON_FA_SQUARE_PLUS).x - 20, ImGui::GetCursorPosY() + 2));
	ImGui::Text(gui::espPreview ? ICON_FA_SQUARE_MINUS : ICON_FA_SQUARE_PLUS);
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		gui::espPreview = !gui::espPreview;
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
        ImGui::SetTooltip(gui::espPreview ? "Disable ESP Preview" : "Enable ESP Preview", ImGui::GetStyle().HoverDelayShort);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
    ImGui::Separator();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);

    ImGui::BeginChild("esp", ImVec2(390, 270), true);
    ImGui::SeparatorText("ESP " ICON_FA_PAINTBRUSH);
    menu::drawEsp();
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 395, ImGui::GetCursorPosY() - 276));

    ImGui::BeginChild("aim", ImVec2(390, 270), true);
    ImGui::SeparatorText("AIMBOT " ICON_FA_SKULL);
    menu::drawAim();
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY()));

    ImGui::BeginChild("misc", ImVec2(390, 270), true);
    ImGui::SeparatorText("MISCELLANEOUS " ICON_FA_ROCKET);
    menu::drawMisc();
    ImGui::EndChild();

    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 395, ImGui::GetCursorPosY() - 276));

    ImGui::BeginChild("config", ImVec2(390, 270), true);
    ImGui::SeparatorText("CONFIG " ICON_FA_SD_CARD);
    menu::drawConfig();
    ImGui::EndChild();
    ImGui::End();
	if (gui::espPreview) {
		menu::drawEspPreview();
	}
}

void menu::drawEsp() {
    ImGui::Checkbox("Enable ESP", &config::cfg.esp.enableEsp);
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_FA_GEAR).x - 10);
    ImGui::Text(ICON_FA_GEAR);
    ImGui::OpenPopupOnItemClick("enableesp", ImGuiPopupFlags_MouseButtonLeft);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
        ImGui::SetTooltip("Click for options", ImGui::GetStyle().HoverDelayShort);
    if (ImGui::BeginPopup("enableesp", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::SeparatorText("ESP Options");
        ImGui::Checkbox("Target Team", &config::cfg.esp.displayTeam);
        ImGui::EndPopup();
    }
    ImGui::Checkbox("Bounding Box", &config::cfg.esp.boundingBox);
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_FA_GEAR).x - 10);
    ImGui::Text(ICON_FA_GEAR);
    ImGui::OpenPopupOnItemClick("bbox", ImGuiPopupFlags_MouseButtonLeft);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
        ImGui::SetTooltip("Click for options", ImGui::GetStyle().HoverDelayShort);
    if (ImGui::BeginPopup("bbox", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::SeparatorText("Bounding Box Options");
        ImGui::Checkbox("Glow", &config::cfg.esp.boundingBoxGlow);
        ImGui::ColorEdit4("Glow Color", config::cfg.esp.bboxGlow, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview);
        ImGui::EndPopup();
    }
    ImGui::Checkbox("Health", &config::cfg.esp.health);
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_FA_GEAR).x - 10);
    ImGui::Text(ICON_FA_GEAR);
    ImGui::OpenPopupOnItemClick("health", ImGuiPopupFlags_MouseButtonLeft);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
        ImGui::SetTooltip("Click for options", ImGui::GetStyle().HoverDelayShort);
    if (ImGui::BeginPopup("health", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::SeparatorText("Health Options");
        ImGui::Checkbox("Glow", &config::cfg.esp.healthGlow);
        ImGui::EndPopup();
    }
    ImGui::Checkbox("Gun", &config::cfg.esp.gun);
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_FA_GEAR).x - 10);
    ImGui::Text(ICON_FA_GEAR);
    ImGui::OpenPopupOnItemClick("gun", ImGuiPopupFlags_MouseButtonLeft);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
        ImGui::SetTooltip("Click for options", ImGui::GetStyle().HoverDelayShort);
    if (ImGui::BeginPopup("gun", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::SeparatorText("Gun Options");
        ImGui::ColorEdit4("Color", config::cfg.esp.gunColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview);
        ImGui::ColorEdit4("Outline Color", config::cfg.esp.gunOutline, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview);
        ImGui::EndPopup();
    }
    ImGui::Checkbox("Name", &config::cfg.esp.name);
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_FA_GEAR).x - 10);
    ImGui::Text(ICON_FA_GEAR);
    ImGui::OpenPopupOnItemClick("name", ImGuiPopupFlags_MouseButtonLeft);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
        ImGui::SetTooltip("Click for options", ImGui::GetStyle().HoverDelayShort);
    if (ImGui::BeginPopup("name", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::SeparatorText("Name Options");
        ImGui::ColorEdit4("Color", config::cfg.esp.nameColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview);
        ImGui::ColorEdit4("Outline Color", config::cfg.esp.nameOutline, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview);
        ImGui::EndPopup();
    }
    ImGui::Checkbox("Skeleton", &config::cfg.esp.skeleton);
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_FA_GEAR).x - 10);
    ImGui::Text(ICON_FA_GEAR);
    ImGui::OpenPopupOnItemClick("skeleton", ImGuiPopupFlags_MouseButtonLeft);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
        ImGui::SetTooltip("Click for options", ImGui::GetStyle().HoverDelayShort);
    if (ImGui::BeginPopup("skeleton", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::SeparatorText("Skeleton Options");
        ImGui::ColorEdit4("Color", config::cfg.esp.skeletonColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview);
        ImGui::EndPopup();
    }
}

void menu::drawConfig() {
    if (ImGui::BeginListBox("##cfgselector", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
    {
        if (config::cfg.cfgSelector.configs.size() == 0) {
            ImGui::Text("No Config Files Found.");
        }
        for (size_t i = 0; i < config::cfg.cfgSelector.configs.size(); i++)
        {
            const bool is_selected = (config::cfg.cfgSelector.selectedConfig == i);
            if (ImGui::Selectable(config::cfg.cfgSelector.configs[i].c_str(), is_selected))
                config::cfg.cfgSelector.selectedConfig = i;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
    auto textSize = ImGui::CalcTextSize("Are you sure you want to save your config?");
    auto textSize2 = ImGui::CalcTextSize("It will overwrite the previous one.");
    ImGui::SetNextWindowSize(ImVec2(textSize.x + 25, 112));
    ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x / 2) - 195, (ImGui::GetIO().DisplaySize.y / 2) - 56));
    if (ImGui::BeginPopupModal("savedialog", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
    {
        ImGui::Text("Are you sure you want to save your config?");
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - textSize2.x)/2);
        ImGui::Text("It will overwrite the previous one.");
        ImGui::Separator();
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 200)/2);
        if (ImGui::Button("Save " ICON_FA_FILE_EXPORT, ImVec2(100, 0))) {
            config::saveConfig();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel" ICON_FA_XMARK, ImVec2(100, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
    static char cfg[128] = "";
    ImGui::Text("New Config name");
    ImGui::PushItemWidth(ImGui::GetWindowWidth() - 25);
    ImGui::InputText("##cfgName", cfg, IM_ARRAYSIZE(cfg));
    ImGui::PopItemWidth();

    if (ImGui::Button("Refresh " ICON_FA_ARROWS_ROTATE)) {
        config::refreshConfigs();
    }
    ImGui::SameLine();
    if (ImGui::Button("Save " ICON_FA_FILE_EXPORT)) {
        if (config::cfg.cfgSelector.selectedConfig != -1) {
            ImGui::OpenPopup("savedialog");
        }
        else {
            ImGui::InsertNotification({ ImGuiToastType::Error, 3000, "No config selected." });
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Load " ICON_FA_FILE_IMPORT)) {
        if (config::cfg.cfgSelector.selectedConfig != -1) {
            config::loadConfig();
        }
        else {
            ImGui::InsertNotification({ ImGuiToastType::Error, 3000, "No config selected." });
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Create " ICON_FA_FILE)) {
        if (std::string(cfg).length() > 0) {
            config::createConfig(cfg);
        }
        else {
            ImGui::InsertNotification({ ImGuiToastType::Error, 3000, "Please enter a name for the config." });
        }
    }
}

void menu::drawAim() {
    ImGui::Checkbox("Enable Triggerbot", &config::cfg.aimbot.triggerBot);
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_FA_GEAR).x - 10);
    ImGui::Text(ICON_FA_GEAR);
    ImGui::OpenPopupOnItemClick("enabletrigger", ImGuiPopupFlags_MouseButtonLeft);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
        ImGui::SetTooltip("Click for options", ImGui::GetStyle().HoverDelayShort);
    if (ImGui::BeginPopup("enabletrigger", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::SeparatorText("Triggerbot Options");
        ImGui::Checkbox("Target Team", &config::cfg.aimbot.triggerBotTargetsTeam);
        ImGui::Checkbox("Magnetic", &config::cfg.aimbot.triggerMagnet);
        ImGui::SliderInt("Delay", &config::cfg.aimbot.triggerBotDelay, 1, 200);
        ImGui::EndPopup();
    }
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
    ImGui::Text("Triggerbot Key");
	ImGui::SameLine(); ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5); ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ImGui::GetKeyNameExtended(ImGuiKey(config::cfg.aimbot.triggerBotKey))).x - 20);
    ImGui::Hotkey(&config::cfg.aimbot.triggerBotKey);
    ImGui::Text("Hitbox Selector");
    if (ImGui::BeginListBox("##hbSelector", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
    {
        int i = 0;
        for (auto& bone : esp::boneMap2)
        {
            const bool is_selected = (config::cfg.aimbot.selectedMenu == i);
            if (ImGui::Selectable(bone.first.c_str(), is_selected)) {
                config::cfg.aimbot.selectedMenu = i;
                config::cfg.aimbot.selectedHitbox = bone.second;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();

            i++;
        }
        ImGui::EndListBox();
    }
}

void menu::drawMisc() {
    ImGui::Checkbox("Enable Bunnyhop", &config::cfg.misc.bhop);
    ImGui::Checkbox("Enable Crosshair", &config::cfg.misc.crosshair);
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_FA_GEAR).x - 10);
    ImGui::Text(ICON_FA_GEAR);
    ImGui::OpenPopupOnItemClick("crosshair", ImGuiPopupFlags_MouseButtonLeft);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
        ImGui::SetTooltip("Click for options", ImGui::GetStyle().HoverDelayShort);
    if (ImGui::BeginPopup("crosshair", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::SeparatorText("Crosshair Options");
        ImGui::ColorEdit4("Color", config::cfg.misc.crosshairColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview);
        ImGui::EndPopup();
    }
}

void menu::drawEspPreview() {
	ImVec2 mainPos = ImGui::FindWindowByName("byteguardian")->Pos;
    ImVec2 mainSize = ImGui::FindWindowByName("byteguardian")->Size;
	ImGui::SetNextWindowPos(ImVec2(mainPos.x + mainSize.x + 25, mainPos.y + 92));
	ImGui::SetNextWindowSize(ImVec2(284, 430));
    ImGui::PushStyleColor(ImGuiCol_WindowShadow, ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.f));
    ImGui::Begin("esppreview", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
    ImGui::PopStyleColor();
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("ESP PREVIEW").x) / 2);
    ImGui::Text("ESP PREVIEW");
    ImGui::Separator();
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    ImVec2 characterPos = ImVec2(86, ImGui::GetCursorPosY() + 35);
    ImVec2 characterSize = ImVec2(112, 315);
    ImGui::SetCursorPos(characterPos);
    ImGui::Image((void*)gui::espCharacter, characterSize);
    characterPos = characterPos + ImGui::GetWindowPos() - ImVec2(5,5);
    characterSize = characterSize + ImVec2(10,10);

    if (config::cfg.esp.boundingBox) {
        ImGui::GetWindowDrawList()->AddOutlinedRect(characterPos, characterPos + characterSize, IM_COL32_WHITE, IM_COL32_BLACK);
        if (config::cfg.esp.boundingBoxGlow)
            ImGui::GetWindowDrawList()->AddShadowRect(characterPos, characterPos + characterSize, ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.bboxGlow), 50.f, ImVec2(0,0), ImDrawFlags_ShadowCutOutShapeBackground);
    }

    if (config::cfg.esp.name) {
        ImVec2 textSize = ImGui::CalcTextSizeFont(12.f, "Greymouth");
        ImVec2 textPos = ImVec2(characterPos.x + ((characterSize.x - textSize.x) / 2), characterPos.y - textSize.y - 2);

        ImGui::GetWindowDrawList()->AddOutlinedText(ImGui::GetIO().Fonts->Fonts[1], 12.f, textPos, ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.nameColor), ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.nameOutline), "Greymouth");
    }

    if (config::cfg.esp.health) {
        ImVec2 barPos = characterPos + ImVec2(-6, -1);
        ImVec2 textSize = ImGui::CalcTextSizeFont(12.f, "83");
        ImVec2 textPos = ImVec2(barPos.x - textSize.x, barPos.y + characterSize.y - (characterSize.y * 0.83) - 1);
        ImGui::GetWindowDrawList()->AddRectFilled(barPos, barPos + ImVec2(4, characterSize.y + 2), IM_COL32_BLACK);
        ImGui::GetWindowDrawList()->AddRectFilled(barPos + ImVec2(1, characterSize.y - (characterSize.y * 0.83) + 1), barPos + ImVec2(3, characterSize.y + 1), ImColor(0, 255, 0, 255));
        ImGui::GetWindowDrawList()->AddOutlinedText(ImGui::GetIO().Fonts->Fonts[1], 12.f, textPos, IM_COL32_WHITE, IM_COL32_BLACK, "83");
        if (config::cfg.esp.healthGlow)
            ImGui::GetWindowDrawList()->AddShadowRect(barPos + ImVec2(1,1), barPos + ImVec2(3, characterSize.y + 1), ImColor(0.f, 1.f, 0.f, 1.f), 50.f, ImVec2(0, 0), ImDrawFlags_ShadowCutOutShapeBackground);
    }

    if (config::cfg.esp.gun) {
        ImVec2 textSize = ImGui::CalcTextSizeFont(12.f, "Glock-18");
        ImVec2 textPos = ImVec2(characterPos.x + (characterSize.x - textSize.x) / 2, characterPos.y + characterSize.y + 2);
        ImGui::GetWindowDrawList()->AddOutlinedText(ImGui::GetIO().Fonts->Fonts[1], 12.f, textPos, ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.gunColor), ImGui::ColorConvertFloat4ToU32((ImVec4&)config::cfg.esp.gunOutline), "Glock-18");
    }
    ImGui::PopFont();
    ImGui::End();
}