#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace FQW {

class Logger
{
public:
	Logger() = delete;

	static void Initialize(spdlog::level::level_enum logLevel = spdlog::level::level_enum::trace);
	static std::shared_ptr<spdlog::logger>& GetInstance();

private:
	static std::shared_ptr<spdlog::logger> s_Instance;
};

#define FQW_TRACE(...)		FQW::Logger::GetInstance()->trace(__VA_ARGS__)
#define FQW_INFO(...)		FQW::Logger::GetInstance()->info(__VA_ARGS__)
#define FQW_WARN(...)		FQW::Logger::GetInstance()->warn(__VA_ARGS__)
#define FQW_ERROR(...)		FQW::Logger::GetInstance()->error(__VA_ARGS__)
#define FQW_CRITICAL(...)	FQW::Logger::GetInstance()->critical(__VA_ARGS__)

}