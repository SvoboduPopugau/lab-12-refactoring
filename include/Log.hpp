//
// Created by vladislav on 06.06.2021.
//

// Copyright 2020 Your Name <your_email>

#ifndef LOG_HPP_
#define LOG_HPP_

#include <string>
#include <iostream>

class Log {
 public:
  static Log& Instance();
  void Write(const std::string_view& message) const;
  void WriteDebug(const std::string& message) const;
  void setLevel(bool level);

 private:
  explicit Log() = default;
  Log(Log& tmp) = delete;
  Log& operator=(Log& tmp) = delete;
  ~Log() = default;

 private:
  bool level_ = false;
  mutable std::ostream* out_ = &std::cout;
};

#endif  // LOG_HPP_
