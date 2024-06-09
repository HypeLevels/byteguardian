#pragma once
#include "../include.h"

namespace config {
	struct Misc {
		bool bhop;
		bool crosshair;
		float crosshairColor[4] = {255, 255, 255, 255};
	};
	struct Aimbot {
		bool triggerBot;
		int triggerBotKey;
		bool triggerBotTargetsTeam;
		int triggerBotDelay;
	};
	struct ESP {
		bool enableEsp;
		bool displayTeam;
		bool boundingBox;
		bool boundingBoxGlow;
		bool health;
		bool healthGlow;
		bool gun;
		bool name;
		bool skeleton;
		float bboxColor[4] = {255, 255, 255, 255};
		float bboxOutline[4] = {0, 0, 0, 255};
		float bboxGlow[4] = {255, 255, 255, 255};
		float gunColor[4] = {255, 255, 255, 255};
		float gunOutline[4] = { 0, 0, 0, 255 };
		float nameColor[4] = { 255, 255, 255, 255 };
		float nameOutline[4] = { 0, 0, 0, 255 };
		float skeletonColor[4] = { 255, 255, 255, 255 };
	};
	struct ConfigSelector {
		std::vector<std::string> configs;
		int selectedConfig = -1;
	};
	struct Config {
		Misc misc;
		Aimbot aimbot;
		ESP esp;
		ConfigSelector cfgSelector;

		Config()
			:misc(), aimbot(), esp(), cfgSelector() {}

		template<typename Ar>
		void serialize(Ar& ar) {
			ar& YAS_OBJECT(nullptr, misc.bhop, misc.crosshair, misc.crosshairColor, aimbot.triggerBot, aimbot.triggerBotKey, aimbot.triggerBotDelay, aimbot.triggerBotTargetsTeam,
				esp.enableEsp, esp.displayTeam, esp.boundingBox, esp.boundingBoxGlow, esp.health, esp.gun, esp.name, esp.skeleton,
				esp.bboxColor, esp.bboxOutline, esp.bboxGlow, esp.healthGlow, esp.gunColor, esp.gunOutline, esp.nameColor, esp.nameOutline, esp.skeletonColor);
		}
	};
	inline Config cfg;

	inline void saveConfig() {
		try {
			std::string filePath = "./" + cfg.cfgSelector.configs.at(cfg.cfgSelector.selectedConfig);
			yas::file_ostream ofs(filePath.c_str(), 1);
			yas::save<yas::file | yas::binary | yas::compacted>(ofs, cfg);
			ImGui::InsertNotification({ ImGuiToastType::Success, 3000, "Config saved." });
		}
		catch (...) {
			ImGui::InsertNotification({ ImGuiToastType::Error, 3000, "Failed to save config." });
		}
	}
	inline void loadConfig() {
		try {
			std::string filePath = "./" + cfg.cfgSelector.configs.at(cfg.cfgSelector.selectedConfig);
			yas::file_istream ifs(filePath.c_str());
			yas::load<yas::file | yas::binary | yas::compacted>(ifs, cfg);
			ImGui::InsertNotification({ ImGuiToastType::Success, 3000, "Config loaded." });
		}
		catch (...) {
			ImGui::InsertNotification({ ImGuiToastType::Error, 3000, "Failed to load config." });
		}
	}

	inline void refreshConfigs() {
		config::cfg.cfgSelector.configs.clear();
		config::cfg.cfgSelector.selectedConfig = -1;
		for (const auto& entry : std::filesystem::directory_iterator("./")) {
			if (entry.path().extension() == ".bg") {
				config::cfg.cfgSelector.configs.push_back(entry.path().filename().string());
			}
		}
	}

	inline void createConfig(const char* name) {
		std::string filePath = "./" + std::string(name) + ".bg";
		yas::file_ostream ofs(filePath.c_str(), 1);
		yas::save<yas::file | yas::binary | yas::compacted>(ofs, cfg);
		ImGui::InsertNotification({ ImGuiToastType::Success, 3000, "Config created." });
		refreshConfigs();
	}
}