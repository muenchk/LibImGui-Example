#include "UIWindowExample.h"

using namespace LibImGuiUI;

namespace Example
{
	ExampleWindow* ExampleWindow::GetSingleton()
	{
		static ExampleWindow* window = new ExampleWindow();
		return window;
	}

	util::shared_ptr<ExampleWindow> ExampleWindow::GetSingletonShared()
	{
		static util::shared_ptr<ExampleWindow> sharedwindow = util::create_shared<ExampleWindow>(GetSingleton());
		return sharedwindow;
	}

	void ExampleWindow::Draw()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 12.0f, 12.0f });

		if (ImGui::Begin("Example Window", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)) {
			const float avail_w = ImGui::GetWindowWidth();
			auto [font, fontSize] = LibImGuiUI::LibImGuiUI_APIv1::instance->GetFontByType(FontType::MainTitle);
			ImGui::PushFont(font, fontSize);  

			ImVec2 textsize = ImGui::CalcTextSize("Awesome Example Window");
			auto cursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPos({ (avail_w - textsize.x) / 2.f, cursorPos.y });
			ImGui::Text("Awesome Example Window");

			ImGui::PopFont();  

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Text("Awesome Content");

			ImGui::End();
		}

		ImGui::PopStyleVar();
	}

	Window::KeyInformation* ExampleWindow::GetCloseKeys()
	{
		return &_closeKeys;
	}
	Window::KeyInformation* ExampleWindow::GetOpenKeys()
	{
		return &_openKeys;
	}

	void ExampleWindow::Register()
	{
		Init();
		LibImGuiUI::LibImGuiUI_APIv1::instance->RegisterWindow(GetSingletonShared());
	}

	bool ExampleWindow::Open()
	{
		if (_open)
			return false;
		// prevents window from opening too fast
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _opened) > std::chrono::milliseconds(500)) {
			Window::Open();

			_opened = std::chrono::steady_clock::now();
			return true;
		}
		return false;
	}

	bool ExampleWindow::Close()
	{
		if (!_open)
			return false;
		// prevents window from opening too fast
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _opened) > std::chrono::milliseconds(500)) {
			_opened = std::chrono::steady_clock::now();
			Window::Close();

			return true;
		}
		return false;
	}
}
