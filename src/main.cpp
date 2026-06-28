#include "UIWindowExample.h"
#include "UISettingsExample.h"

#include <string>

using namespace LibImGuiUI;

namespace
{
	void InitializeLog()
	{
		auto path = logger::log_directory();
		if (!path) {
			util::report_and_fail("Failed to find standard logging directory"sv);
		}

#ifndef NDEBUG
		auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
		auto spath = path.value();
		spath /= "Example";
		spath /= fmt::format("{}_skse.log"sv, Plugin::NAME);
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(spath.string(), true);
#endif

#ifndef NDEBUG
		const auto level = spdlog::level::trace;
#else
		const auto level = spdlog::level::info;
#endif
		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
		log->set_level(level);
		log->flush_on(level);

		spdlog::set_default_logger(std::move(log));
		//spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);
		spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);
	}
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v;

	v.PluginVersion(Plugin::VERSION);
	v.PluginName(Plugin::NAME);
	v.AuthorName("Some Author");

	v.UsesAddressLibrary();
	v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });
	v.UsesNoStructs();

	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	InitializeLog();
	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Plugin::NAME.data();
	a_info->version = Plugin::VERSION[0];

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (REL::Module::IsAE()) {
		if (ver < SKSE::RUNTIME_SSE_LATEST_AE) {
			logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
			return false;
		}
	} else if (REL::Module::IsSE()) {
		if (ver < SKSE::RUNTIME_SSE_1_5_97) {
			logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
			return false;
		}
	} else if (REL::Module::IsVR()) {
		if (ver < SKSE::RUNTIME_LATEST_VR) {
			logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
			return false;
		}
	}
	return true;
}

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kPreLoadGame:
		break;
	case SKSE::MessagingInterface::kPostLoadGame:
		break;
	case SKSE::MessagingInterface::kDataLoaded:
		LibImGuiUI::RequestPluginAPI(LibImGuiUI::APIVersion::V1);
		if (LibImGuiUI::LibImGuiUI_APIv1::instance) {
			Example::ExampleWindow::GetSingleton()->Register();
			Example::SettingsTab::GetSingleton()->Register();
		} else {
			SKSE::GetTaskInterface()->AddUITask([]() {
				RE::DebugMessageBox("Example Plugin\n You seem to be missing ImGui Window Framework which is required for the UI to work correctly.");
			});
		}
		break;
	case SKSE::MessagingInterface::kPostPostLoad:
		break;
	case SKSE::MessagingInterface::kPostLoad:
		break;
	case SKSE::MessagingInterface::kNewGame:
		break;
	case SKSE::MessagingInterface::kInputLoaded:
		break;
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	// find logging folder

	InitializeLog();

	SKSE::Init(a_skse);

	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		logger::error("[LOADPLUGIN] couldn't register listener");
		return false;
	}

	auto serialization = (SKSE::SerializationInterface*)a_skse->QueryInterface(SKSE::LoadInterface::kSerialization);
	if (!serialization) {
		logger::error("[LOADPLUGIN] couldn't get serialization interface");
		return false;
	}
	
	return true;
}
