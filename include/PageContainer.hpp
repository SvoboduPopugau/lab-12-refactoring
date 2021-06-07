//
// Created by vladislav on 07.06.2021.
//

// Copyright 2020 Your Name <your_email>

#ifndef PAGECONTAINER_H_
#define PAGECONTAINER_H_

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
#include "Log.hpp"
#include "StatSender.hpp"
#include "UsedMemory.hpp"
#include "header.hpp"

constexpr size_t kMinLines = 10;


class PageContainer {
 public:
  [[nodiscard]] const Item& ByIndex(size_t i) const;

  [[nodiscard]] const Item& ById(const std::string& id) const;

  void RawDataLoad(std::istream& io);

  void Load(std::istream& io, float threshold);

  void Reload(float threshold);

  PageContainer(const Log& log, UsedMemory* memory_counter)
      : log_(&log), memory_counter_(memory_counter), stat_sender_(new StatSender(log)){}

 private:
  const Log* log_;
  std::unique_ptr<UsedMemory> memory_counter_;
  std::unique_ptr<StatSender> stat_sender_;
  std::vector<Item> data_;
  std::vector<std::string> raw_data_;
};


#endif  // PAGECONTAINER_H_
