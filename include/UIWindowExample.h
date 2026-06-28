#pragma once

using Window = LibImGuiUI::UserInterface::Window;

namespace Example
{
	/// <summary>
	/// Default settings window that shows with the UI registered Settings tabs
	/// </summary>
	class ExampleWindow : public Window
	{
	private:
		Window::KeyInformation _openKeys;
		Window::KeyInformation _closeKeys;

		std::chrono::steady_clock::time_point _opened;

	public:
		static ExampleWindow* GetSingleton();
		static std::shared_ptr<ExampleWindow> GetSingletonShared();

		void Init()
		{
			if (!_openKeys.keys) {
				_openKeys.numKeys = 2;
				_openKeys.keys = new uint32_t[_openKeys.numKeys];
				_openKeys.keys[0] = 0x12;  // E
				_openKeys.keys[1] = 0x2A;  // LShift
				_openKeys.alternateKeyEsacape = false;
			}

			if (!_closeKeys.keys) {
				_closeKeys.numKeys = 2;
				_closeKeys.keys = new uint32_t[_closeKeys.numKeys];
				_closeKeys.keys[0] = 0x12;  // E
				_closeKeys.keys[1] = 0x2A;  // LShift
				_closeKeys.alternateKeyEsacape = true;
			}
		}

		virtual void Draw() override;

		virtual bool Open() override;
		virtual bool Close() override;
		virtual bool CanOpen() override { return true; }

		virtual const char* GetName() override { return "Settings"; }
		virtual KeyInformation* GetCloseKeys() override;
		virtual KeyInformation* GetOpenKeys() override;

		void Register();
	};
}
