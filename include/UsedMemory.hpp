// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_USEDMEMORY_HPP_
#define INCLUDE_USEDMEMORY_HPP_

#include <string>
#include <vector>

#include "Item.hpp"
#include "Log.hpp"

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

#endif  // INCLUDE_USEDMEMORY_HPP_
