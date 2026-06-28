## Example Plugin for creating a UI with [ImGui Window Framework](https://github.com/muenchk/LibImGuiUI)

The important part of the plugin are the 3 files in include/LibImGuiUI which define the API.
To get your own plugin running, you only need to define your own windows and tabs and register them in the MessageHandler

```cpp
void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
SKSE::MessagingInterface::kDataLoaded:
	LibImGuiUI::RequestPluginAPI(LibImGuiUI::APIVersion::V1);
	if (LibImGuiUI::LibImGuiUI_APIv1::instance) {
		Example::ExampleWindow::GetSingleton()->Register();
		Example::SettingsTab::GetSingleton()->Register();
	}
	break;
}
```

## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [The Elder Scrolls V: Skyrim Special Edition](https://store.steampowered.com/app/489830)
	* Add the environment variable `SkyrimOutputPath` to point to the folder where you want to store your compiled Plugins.
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
	* Desktop development with C++
* [CommonLibNG](https://github.com/CharmedBaryon/CommonLibSSE-NG)
	* add its path to the environment variable `CommonLibNGPath`

## Building for SSE / AE / VR
```
git clone https://github.com/muenchk/LibImGui-Example.git
cd LibImGui-Example
cmake --preset all 				
cmake --build build
```
