//
// Created by vladislav on 07.06.2021.
//

// Copyright 2020 Your Name <your_email>

#ifndef USEDMEMORY_HPP_
#define USEDMEMORY_HPP_

#include <vector>

#include "Log.hpp"
#include "header.hpp"

class UsedMemory {
 public:
  explicit UsedMemory(const Log& log) : log_(&log) {}

  void OnDataLoad(const std::vector<Item>& old_items,
                  const std::vector<Item>& new_items);

  void OnRawDataLoad(const std::vector<std::string>& old_items,
                     const std::vector<std::string>& new_items);

  [[nodiscard]] size_t used() const;

 private:
  const Log* log_;
  size_t used_ = 0;
};


#endif  // USEDMEMORY_HPP_
