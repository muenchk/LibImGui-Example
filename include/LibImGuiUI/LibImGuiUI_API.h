#pragma once

#include "LibImGuiUI/Tab.h"
#include "LibImGuiUI/Window.h"
#include "imgui.h"

enum class FontType
{
	Daedric,
	Dragon,
	Dwemer,
	Falmer,
	Mage,
	NotoSansJP,
	MainTitle,
	HeaderFont,
	ButtonFont,
	SettingsFont,
	SmallFont,
};

namespace LibImGuiUI
{
	enum class APIVersion
	{
		V1 = 1,
	};
	class LibImGuiUI_APIv1
	{
	private:
		/// <summary>
		/// Returns ImGui internal to initialize the callers ImGui
		/// </summary>
		/// <param name="allocFunc">The allocation function returned by ImGui::GetAllocatorFunctions</param>
		/// <param name="freeFunc">The deallocation function returned by ImGui::GetAllocatorFunctions</param>
		/// <param name="userData">The user data returned by ImGui::GetAllocatorFunctions</param>
		/// <param name="context">The currently active ImGui context</param>
		virtual void GetImGuiInternal(ImGuiMemAllocFunc* allocFunc, ImGuiMemFreeFunc* freeFunc, void** userData, ImGuiContext** context) = 0;
	public:
		static inline LibImGuiUI_APIv1* instance = nullptr;

		/// <summary>
		/// Returns the ImGui Font and scaled fontSize for the given fonttype
		/// </summary>
		/// <param name="fontType"></param>
		/// <returns></returns>
		virtual std::pair<ImFont*, float> GetFontByType(FontType fontType) = 0;
		/// <summary>
		/// Returns a user defined colour (defined in theme)
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		virtual ImVec4 GetUserColour(const char* name) = 0;
		/// <summary>
		/// Registers a tab for one of the default windows, Settings, RuleEditor, Performance Monitor
		/// </summary>
		/// <param name="tab"></param>
		virtual void RegisterTab(std::shared_ptr<UserInterface::Tab> tab) = 0;
		/// <summary>
		/// Registers a window
		/// </summary>
		/// <param name="window"></param>
		virtual void RegisterWindow(std::shared_ptr<UserInterface::Window> window) = 0;
		/// <summary>
		/// Unregisters a tab from one of the default windows. The tab will show up anymore when the window is openened
		/// </summary>
		/// <param name="tab"></param>
		virtual void UnregisterTab(std::shared_ptr<UserInterface::Tab> tab) = 0;
		/// <summary>
		/// Opens the given window [Window must be already be registered]
		/// </summary>
		/// <param name="window"></param>
		virtual void OpenWindow(std::shared_ptr<UserInterface::Window> window) = 0;
		/// <summary>
		/// Closes the given window [Window must be registered and open]
		/// </summary>
		/// <param name="window"></param>
		virtual void CloseWindow(std::shared_ptr<UserInterface::Window> window) = 0;
		/// <summary>
		/// Closes all open windows
		/// </summary>
		virtual void CloseAllWindows() = 0;

		/// <summary>
		/// Returns the number of currently open windows
		/// </summary>
		/// <returns></returns>
		virtual int GetNumberOfOpenWindows() = 0;
		/// <summary>
		/// Returns whether windows can currently be openened
		/// </summary>
		/// <returns></returns>
		virtual bool CanOpen() = 0;

		void InitImgui()
		{
			static bool loaded = false;
			if (!loaded) {
				loaded = true;
				ImGuiMemAllocFunc allocFunc;
				ImGuiMemFreeFunc freeFunc;
				void* userData;
				ImGuiContext* context;
				GetImGuiInternal(&allocFunc, &freeFunc, &userData, &context);
				ImGui::SetAllocatorFunctions(allocFunc, freeFunc, userData);
				ImGui::SetCurrentContext(context);
			}
		}
	};

	typedef void* (*_RequestPluginAPI)(APIVersion);

	[[nodiscard]] inline void* RequestPluginAPI(APIVersion version = APIVersion::V1)
	{
		std::string str = "LibImGuiUI.dll";
		std::wstring wstr = std::wstring(str.begin(), str.end());
		auto pluginHandle = GetModuleHandle(wstr.c_str());
		if (pluginHandle != 0) {
			_RequestPluginAPI requestAPIFunction = (_RequestPluginAPI)GetProcAddress(pluginHandle, "RequestPluginAPI");
			if (requestAPIFunction) {
				switch (version) {
				case APIVersion::V1:
					LibImGuiUI_APIv1::instance = (LibImGuiUI_APIv1*)requestAPIFunction(version);
					if (LibImGuiUI_APIv1::instance != nullptr)
						LibImGuiUI_APIv1::instance->InitImgui();
					return LibImGuiUI_APIv1::instance;
					break;
				}
			}
		}
		return nullptr;
	}
}
