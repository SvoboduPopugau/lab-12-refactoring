//
// Created by vladislav on 07.06.2021.
//

// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_PAGECONTAINER_H_
#define INCLUDE_PAGECONTAINER_H_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "Histogram.hpp"
#include "Item.hpp"
#include "Log.hpp"
#include "StatSender.hpp"
#include "UsedMemory.hpp"

constexpr size_t kMinLines = 10;

class PageContainer {
 public:
  [[nodiscard]] const Item& ByIndex(size_t i) const;

  [[nodiscard]] const Item& ById(const std::string& id) const;

  void RawDataLoad(std::istream& io);

  void Load(std::istream& io, float threshold);

  void Reload(float threshold);
  void PrintTable() const;

  size_t GetRawDataSize();
  size_t GetDataSize();

  explicit PageContainer(UsedMemory* memory_counter)
      : memory_counter_(memory_counter),
        stat_sender_(new StatSender(Log::Instance())) {}

  PageContainer(UsedMemory* memory_counter, StatSender* stat_sender)
      : memory_counter_(memory_counter), stat_sender_(stat_sender) {}

 private:
  std::unique_ptr<UsedMemory> memory_counter_;
  std::unique_ptr<StatSender> stat_sender_;
  std::vector<Item> data_;
  std::vector<std::string> raw_data_;
};

#endif  // INCLUDE_PAGECONTAINER_H_
