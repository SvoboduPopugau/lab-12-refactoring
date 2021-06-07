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
  const Item& ByIndex(size_t i) const;

  const Item& ById(const std::string& id) const;

  void RawDataLoad(std::istream& io){
    std::vector<std::string> raw_data;

    if (!io)
      throw std::runtime_error("Stream is not exist");

    if (io.peek() == EOF)
      throw std::runtime_error("Stream is empty");

    log_->WriteDebug("Stream can be read");

    while (!io.eof()) {
      std::string line;
      line.clear();
      std::getline(io, line, '\n');
      raw_data.push_back(std::move(line));
    }

    if (raw_data.size() < kMinLines) {
      throw std::runtime_error("Too small input stream");
    }

    memory_counter_->OnRawDataLoad(raw_data_, raw_data);
    raw_data_ = std::move(raw_data);
  }

  void Load(std::istream& io, float threshold) {
    RawDataLoad(io);
    Histogram::Instance().ResetSkipped();

    std::vector<Item> data;
    std::set<std::string> ids;
    for (const auto& line : raw_data_) {
      std::stringstream stream(line);

      Item item;
      stream >> item.id >> item.name >> item.score;

      //Что это, блин?!
      if (auto&& [_, inserted] = ids.insert(item.id); !inserted) {
        throw std::runtime_error("already seen");
      }

      if (item.score > threshold) {
        data.push_back(std::move(item));
      } else {
        stat_sender_->Skip(item);
        Histogram::Instance().IncSkipped();
      }
    }

    if (data.size() < kMinLines) {
      throw std::runtime_error("oops");
    }

    memory_counter_->OnDataLoad(data_, data);
    stat_sender_->OnLoaded(data);
    data_ = std::move(data);
  }

  void Reload(float threshold) {
    std::vector<Item> data;
    std::set<std::string> ids;
    for (const auto& line : raw_data_) {
      std::stringstream stream(line);

      Item item;
      stream >> item.id >> item.name >> item.score;

      if (auto&& [_, inserted] = ids.insert(item.id); !inserted) {
        throw std::runtime_error("already seen");
      }

      if (item.score > threshold) {
        data.push_back(std::move(item));
      } else {
        stat_sender_.Skip(item);
      }
    }

    if (data.size() < kMinLines) {
      throw std::runtime_error("oops");
    }

    memory_counter_->OnDataLoad(data_, data);
    stat_sender_->OnLoaded(data);
    data_ = std::move(data);
  }

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
