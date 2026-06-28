#pragma once

namespace LibImGuiUI
{
	namespace UserInterface
	{
		class Tab
		{
		public:
			enum class TabType
			{
				SettingsTab,    // window mainly used for settings
				RuleEditorTab,  // window used for rule editors
				ProfilingTab,   // window used to display performance metrics, uses small font to fit in content
				MiscTab,        // currently unused
			};
			/// <summary>
			/// draws the ui elememts
			/// </summary>
			virtual void Draw() = 0;
			/// <summary>
			/// returns the name of the tab
			/// </summary>
			/// <returns></returns>
			virtual const char* GetName() = 0;

			/// <summary>
			/// returns the type of the tab (i.e. the window it should associated with)
			/// </summary>
			/// <returns></returns>
			virtual TabType GetType() = 0;

			/// <summary>
			/// event called when the parent window is opened
			/// </summary>
			virtual void OnWindowOpen() = 0;
			/// <summary>
			/// event called when the parent window is closed
			/// </summary>
			virtual void OnWindowClose() = 0;

			virtual void AfterDraw() {};
		};
	}
}
