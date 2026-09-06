#include "UISettingsExample.h"

using namespace LibImGuiUI;

#define UI_CHANGED_VALUE(condition, block)                                                \
	{                                                                                     \
		ImGui::PushStyleColor(ImGuiCol_Text, condition ? _colourNormal : _colourChanged); \
		block;                                                                            \
		ImGui::PopStyleColor();                                                           \
	}

void HelpMarkerFirst(const char* description)
{
	if (strlen(description) == 0) {
		ImGui::TextDisabled("   ");
		ImGui::SameLine();
		return;
	}
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip()) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(description);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::SameLine();
}

namespace Example
{
	void SettingsTab::GetSettings()
	{
		_genericSettings = Settings::genericSettings;
	}
	void SettingsTab::SetSettings()
	{
		Settings::genericSettings = _genericSettings;
	}
	void SettingsTab::GetDefaultSettings()
	{
		// only do this here for general settings
		_genericSettings = Settings::GenericSettings();
	}

	void SettingsTab::Draw()
	{
		auto settingsTab = SettingsTab::GetSingleton();
		ImGui::PushItemWidth(300);

		ImGui::TextWrapped("This tab allows you to change all settings for your mod. Settings are saved once the window is closed");

		//----- Generic Settings -----
		ImGui::SeparatorText("Diary Settings");
		ImGui::Separator();

		HelpMarkerFirst("Enable something that is yet to be known");
		UI_CHANGED_VALUE(Settings::genericSettings.enableSomething == _genericSettings.enableSomething,
			ImGui::Checkbox("Enable Something", &_genericSettings.enableSomething));

		HelpMarkerFirst("Slide something that has absolutely no impact");
		UI_CHANGED_VALUE(Settings::genericSettings.sliderSomething == _genericSettings.sliderSomething,
			ImGui::SliderInt("Slide Something", &_genericSettings.sliderSomething, 1, 100));


		ImGui::PopItemWidth();

		//----- Buttons -----
		ImGui::Separator();

		if (ImGui::Button("Load Default Settings")) {
			settingsTab->GetDefaultSettings();
		}
		if (ImGui::BeginItemTooltip()) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Loads the default values for all settings");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		ImGui::SameLine();
		if (ImGui::Button("Discard Settings")) {
			settingsTab->GetSettings();
		}
		if (ImGui::BeginItemTooltip()) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("Discards changes");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	const char* SettingsTab::GetName()
	{
		return "Example Settings";
	}

	void SettingsTab::OnWindowOpen()
	{
		GetSettings();
		_colourChanged = LibImGuiUI::LibImGuiUI_APIv1::instance->GetUserColour("SelectionChanged");
		_colourNormal = ImGui::GetStyleColorVec4(ImGuiCol_Text);
	}

	void SettingsTab::OnWindowClose()
	{
		SetSettings();
	}

	SettingsTab* SettingsTab::GetSingleton()
	{
		static SettingsTab* tab = new SettingsTab();
		return tab;
	}

	util::shared_ptr<SettingsTab> SettingsTab::GetSingletonShared()
	{
		static util::shared_ptr<SettingsTab> sharedtab = util::create_shared<SettingsTab>(GetSingleton());
		return sharedtab;
	}

	void SettingsTab::Register()
	{
		auto sett = GetSingleton();
		LibImGuiUI::LibImGuiUI_APIv1::instance->RegisterTab(GetSingletonShared());
	}
}
