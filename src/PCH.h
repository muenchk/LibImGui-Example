#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define IMGUI_DEFINE_MATH_OPERATORS

#pragma warning(push)


#define MANAGER(T) T::Manager::GetSingleton()


#include <codecvt>

#include <RE/Skyrim.h>
#include <REL/Relocation.h>
#include <SKSE/SKSE.h>

#include <boost/container/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/set.hpp>

//#include <LibImGuiUI/LibImGuiUI_API.h>
#include <LibImGuiUI/ImGuiAPI_ImGui.h>

#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif
#pragma warning(pop)

using namespace std::literals;

using namespace LibImGuiUIImGui;

namespace logger = SKSE::log;

namespace util
{
	using SKSE::stl::report_and_fail;
}

namespace stl
{
	using namespace SKSE::stl;

	template <typename T>
	constexpr auto write_thunk_call() noexcept
	{
		SKSE::AllocTrampoline(14);
		auto& trampoline{ SKSE::GetTrampoline() };
		T::func = trampoline.write_call<5>(T::address, T::Thunk);
	}

	template <typename TDest, typename TSource>
	constexpr auto write_vfunc() noexcept
	{
		REL::Relocation<std::uintptr_t> vtbl{ TDest::VTABLE[0] };
		TSource::func = vtbl.write_vfunc(TSource::idx, TSource::Thunk);
	}

	template <typename T>
	constexpr auto write_vfunc(const REL::VariantID variant_id) noexcept
	{
		REL::Relocation<std::uintptr_t> vtbl{ variant_id };
		T::func = vtbl.write_vfunc(T::idx, T::Thunk);
	}
}  // namespace stl

#define DLLEXPORT __declspec(dllexport)

#include "Plugin.h"
