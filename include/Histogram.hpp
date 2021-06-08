//
// Created by vladislav on 07.06.2021.
//

// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_HISTOGRAM_HPP_
#define INCLUDE_HISTOGRAM_HPP_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

class Histogram {
 public:
  static Histogram& Instance();
  void ResetSkipped();
  void setAvg(float score);
  void IncSkipped();
  [[nodiscard]] float getAvg() const;
  [[nodiscard]] uint getSkipped() const;

 private:
  Histogram() = default;
  Histogram(Histogram& tmp) = delete;
  Histogram& operator=(Histogram& tmp) = delete;

 private:
  float avg_ = 0;
  uint skipped_ = 0;
};

#endif  // INCLUDE_HISTOGRAM_HPP_
