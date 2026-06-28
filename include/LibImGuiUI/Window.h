#pragma once

namespace LibImGuiUI
{
	namespace UserInterface
	{
		class Window
		{
		public:
			struct KeyInformation
			{
				/// <summary>
				/// unique keys that open the window
				/// </summary>
				uint32_t* keys = nullptr;
				/// <summary>
				/// number of keys
				/// </summary>
				size_t numKeys;

				/// <summary>
				/// whether in addition to the above keys, the action will also be performed if simply the ESC key is pressed
				/// </summary>
				bool alternateKeyEsacape = false;
			};

		protected:
			/// <summary>
			/// whether the window is currently open
			/// </summary>
			std::atomic<bool> _open = false;

		public:
			/// <summary>
			/// opens the window
			/// </summary>
			/// <returns>Whether the window was opened</returns>
			virtual bool Open()
			{
				_open = true;
				return _open;
			}
			/// <summary>
			/// closes the window
			/// </summary>
			/// <returns>Whether the window was closed</returns>
			virtual bool Close()
			{
				_open = false;
				return _open;
			}
			/// <summary>
			/// Returns wether the window can be openend (not if it is closed, but whether all conditions are fullfilled such that it may be opened)
			/// </summary>
			/// <returns></returns>
			virtual bool CanOpen()
			{
				return true;
			}

			/// <summary>
			/// draws the ui elements
			/// </summary>
			virtual void Draw() = 0;

			/// <summary>
			/// returns the name of the window
			/// </summary>
			/// <returns>The name of the window</returns>
			virtual const char* GetName() = 0;

			/// <summary>
			/// returns whether the window is currently open
			/// </summary>
			/// <returns>Whether the window is currently open</returns>
			virtual bool IsOpen() { return _open; }

			/// <summary>
			/// returns the KeyInformation for closing the window
			/// </summary>
			/// <returns></returns>
			virtual KeyInformation* GetCloseKeys() = 0;
			/// <summary>
			/// returns the KeyInformation for closing the window
			/// </summary>
			/// <returns></returns>
			virtual KeyInformation* GetAlternateCloseKeys()
			{
				return nullptr;
			}
			/// <summary>
			/// returns whether the window uses alernate closing keys
			/// </summary>
			/// <returns></returns>
			virtual bool HasAlternateCloseKeys() { return false; }
			/// <summary>
			/// returns the KeyInformation for opening the window
			/// </summary>
			/// <returns></returns>
			virtual KeyInformation* GetOpenKeys() = 0;
			/// <summary>
			/// returns the KeyInformation for opening the window
			/// </summary>
			/// <returns></returns>
			virtual KeyInformation* GetAlternateOpenKeys()
			{
				return nullptr;
			}
			/// <summary>
			/// retruns whether the window uses alternate open keys
			/// </summary>
			/// <returns></returns>
			virtual bool HasAlternateOpenKeys() { return false; }

			/// <summary>
			/// Returns whether this is a non-ui blocking side window
			/// </summary>
			/// <returns></returns>
			virtual bool IsSideWindow() { return false; }
		};
	}
}
