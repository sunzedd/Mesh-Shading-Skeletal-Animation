#include "Logger.h"

namespace FQW {

std::shared_ptr<spdlog::logger> Logger::s_Instance;

void Logger::Initialize(spdlog::level::level_enum logLevel)
{
	s_Instance = spdlog::stdout_color_mt("FQW");
	s_Instance->set_pattern("%^[%T] %n: %v%$");
	s_Instance->set_level(logLevel);
}

std::shared_ptr<spdlog::logger>& Logger::GetInstance()
{
	return s_Instance;
}

} // namespace FQW