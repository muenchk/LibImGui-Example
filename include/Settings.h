#pragma once

namespace Example
{
	class Settings
	{
	public:
		struct GenericSettings
		{
			bool enableSomething = true;
			int sliderSomething = 4;
		};
		static inline GenericSettings genericSettings;
	};
}
