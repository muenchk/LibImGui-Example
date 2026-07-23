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

		util::heap_allocator<void*> allocator;

	public:
		static ExampleWindow* GetSingleton();
		static util::shared_ptr<ExampleWindow> GetSingletonShared();

		void Init()
		{
			if (!_openKeys.keys) {
				_openKeys.numKeys = 2;
				_openKeys.keys = (uint32_t*)allocator.malloc(_openKeys.numKeys * sizeof(uint32_t));
				_openKeys.keys[0] = 0x12;  // E
				_openKeys.keys[1] = 0x2A;  // LShift
				_openKeys.alternateKeyEsacape = false;
			}

			if (!_closeKeys.keys) {
				_closeKeys.numKeys = 2;
				_closeKeys.keys = (uint32_t*)allocator.malloc(_closeKeys.numKeys * sizeof(uint32_t));
				_closeKeys.keys[0] = 0x12;  // E
				_closeKeys.keys[1] = 0x2A;  // LShift
				_closeKeys.alternateKeyEsacape = true;
			}
		}

		~ExampleWindow() 
		{
			allocator.dealloc(_openKeys.keys);
			allocator.dealloc(_closeKeys.keys);
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
