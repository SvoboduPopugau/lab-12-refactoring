// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_STATSENDER_HPP_
#define INCLUDE_STATSENDER_HPP_
#include <fstream>
#include <vector>

#include "Item.hpp"
#include "Log.hpp"

class StatSender {
 public:
  explicit StatSender(const Log& log) : log_(&log) {}
  void OnLoaded(const std::vector<Item>& new_items);

  void Skip(const Item& item);

  virtual ~StatSender() = default;

 private:
  virtual void AsyncSend(const std::vector<Item>& items, std::string_view path);

 private:
  const Log* log_;
  std::ofstream fstr{"network", std::ios::binary};
};

#endif  // INCLUDE_STATSENDER_HPP_
