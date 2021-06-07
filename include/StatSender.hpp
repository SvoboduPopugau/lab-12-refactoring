//
// Created by vladislav on 07.06.2021.
//

// Copyright 2020 Your Name <your_email>

#ifndef STATSENDER_HPP_
#define STATSENDER_HPP_
#include <fstream>
#include <vector>

#include "Log.hpp"
#include "header.hpp"

class StatSender {
 public:
  explicit StatSender(const Log& log) : log_(&log) {}
  void OnLoaded(const std::vector<Item>& new_items);

  void Skip(const Item& item);

 private:
  void AsyncSend(const std::vector<Item>& items, std::string_view path);

 private:
  const Log* log_;
  std::ofstream fstr{"network", std::ios::binary};
};

#endif  // STATSENDER_HPP_
