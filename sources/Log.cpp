//
// Created by vladislav on 07.06.2021.
//
#include "Log.hpp"

void Log::Write(const std::string_view &message) const {
  *out_ << message << std::endl;
}
void Log::WriteDebug(const std::string &message) const {
  if (level_) *out_ << message << std::endl;
}
void Log::setLevel(bool level) {
  level_ = level;
}
Log &Log::Instance() {
    static Log res;
    return res;
  }
