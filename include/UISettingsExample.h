#pragma once

#include "Settings.h"

using Tab = LibImGuiUI::UserInterface::Tab;

namespace Example
{
	class SettingsTab : public Tab
	{
	private:
		ImVec4 _colourChanged;
		ImVec4 _colourNormal;

	public:
		Settings::GenericSettings _genericSettings;

	public:
		void GetSettings();
		void SetSettings();
		void GetDefaultSettings();

		static util::shared_ptr<SettingsTab> GetSingletonShared();
		static SettingsTab* GetSingleton();

	public:
		virtual void Draw() override;
		virtual const char* GetName() override;

		virtual TabType GetType() override
		{
			return Tab::TabType::SettingsTab;
		}
		virtual void OnWindowOpen() override;
		virtual void OnWindowClose() override;

		static void Register();
	};
}
