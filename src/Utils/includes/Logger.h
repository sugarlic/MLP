#ifndef LOGGER_H
#define LOGGER_H
#include <QTextEdit>
#include <mutex>
#include <queue>
#include <string>
#define BIT(x) static_cast<int>(x)
namespace s21 {
class Logger {
 public:
  Logger() = default;
  Logger(const std::string& name_space) : name_space_(name_space){};
  ~Logger() = default;
  enum class LogLevel {
    kBasic = 1,
    kInfo = 1 << 1,
    kWarning = 1 << 2,
    kError = 1 << 3,
    kCritical = 1 << 4
  };
  void Log(const std::string& message, LogLevel level = LogLevel::kBasic) const;
  void SetNameSpace(const std::string& name_space);
  static bool IsLogLevelActive(LogLevel level) {
    std::lock_guard<std::mutex> lock(log_level_mutex_);
    return BIT(active_log_level_) & BIT(level);
  }
  static void ActivateLogLevel(LogLevel level) {
    if (IsLogLevelActive(level)) return;
    std::lock_guard<std::mutex> lock(log_level_mutex_);
    active_log_level_ =
        static_cast<LogLevel>(BIT(active_log_level_) | BIT(level));
  }
  static void DeactivateLogLevel(LogLevel level) {
    if (!IsLogLevelActive(level)) return;
    std::lock_guard<std::mutex> lock(log_level_mutex_);
    active_log_level_ =
        static_cast<LogLevel>(BIT(active_log_level_) & ~BIT(level));
  }
  void SetOutputer(QTextEdit* widget);

 private:
  const size_t max_queue_size_ = 512;
  static std::queue<std::string> log_queue_;
  static std::mutex log_level_mutex_;
  static std::mutex log_queue_mutex_;
  static std::mutex output_mutex_;
  static LogLevel active_log_level_;
  static QTextEdit* output_;
  void TryLogging() const;
  std::string name_space_ = "";
  std::string CreateMessage(const std::string& message, LogLevel level) const;
};

}  // namespace s21
#endif  // LOGGER_H
