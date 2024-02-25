#include <Logger.h>

#include <cstdio>
#include <map>

namespace s21 {
QTextEdit* Logger::output_ = nullptr;
std::queue<std::string> Logger::log_queue_{};
std::mutex Logger::log_level_mutex_{};
std::mutex Logger::log_queue_mutex_{};
std::mutex Logger::output_mutex_{};

Logger::LogLevel Logger::active_log_level_ = static_cast<LogLevel>(
    BIT(LogLevel::kBasic) | BIT(LogLevel::kInfo) | BIT(LogLevel::kWarning) |
    BIT(LogLevel::kError) | BIT(LogLevel::kCritical));

void Logger::Log(const std::string& message, LogLevel level) const {
  if (!IsLogLevelActive(level)) return;
  std::lock_guard<std::mutex> lock(output_mutex_);
  std::lock_guard<std::mutex> lock_queue(log_queue_mutex_);
  if (log_queue_.size() + 1 > max_queue_size_) return;
  log_queue_.push(CreateMessage(message.c_str(), level));
  TryLogging();
}

void Logger::TryLogging() const {
  if (!output_) return;
  if (log_queue_.empty()) return;
  std::string str = log_queue_.front();
  output_->append(str.c_str());
  output_->moveCursor(QTextCursor::End);
  log_queue_.pop();
}
void Logger::SetNameSpace(const std::string& name_space) {
  name_space_ = name_space;
}

void Logger::SetOutputer(QTextEdit* widget) {
  std::lock_guard<std::mutex> lock(output_mutex_);
  std::lock_guard<std::mutex> lock_queue(log_queue_mutex_);
  output_ = widget;
  while (!log_queue_.empty() && output_) TryLogging();
}

std::string Logger::CreateMessage(const std::string& message,
                                  LogLevel level) const {
  static const std::map<s21::Logger::LogLevel, const char*> prefix_map = {
      {s21::Logger::LogLevel::kBasic, "[Basic]"},
      {s21::Logger::LogLevel::kInfo, "[Info]"},
      {s21::Logger::LogLevel::kWarning, "[Warning]"},
      {s21::Logger::LogLevel::kError, "[Error]"},
      {s21::Logger::LogLevel::kCritical, "[Critical]"}};

  auto namespace_str = name_space_ + "::";

  static size_t name_space_max_size_ = 1;
  name_space_max_size_ =
      std::max(name_space_max_size_, name_space_.length() + 2);

  size_t c_msg_size =
      message.length() + namespace_str.length() + name_space_max_size_ + 30;
  char* c_msg = new char[c_msg_size];

  std::sprintf(c_msg, "%-10s %-*s %s", prefix_map.at(level),
               (int)name_space_max_size_, namespace_str.c_str(),
               message.c_str());
  auto msg = std::string().assign(c_msg);
  delete[] c_msg;
  return msg;
}

}  // namespace s21
