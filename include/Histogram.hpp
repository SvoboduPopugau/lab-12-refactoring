//
// Created by vladislav on 07.06.2021.
//

// Copyright 2020 Your Name <your_email>

#ifndef LAB_12_REFACTORING_HISTOGRAM_HPP
#define LAB_12_REFACTORING_HISTOGRAM_HPP

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
  void setScore(float score);
  void IncSkipped();
  [[nodiscard]] float getScore() const;
  [[nodiscard]] uint getSkipped() const;

 private:
  Histogram() = default;
  Histogram(Histogram& tmp) = delete;
  Histogram& operator = (Histogram& tmp) = delete;

 private:
  float score_ = 0;
  uint skipped_ = 0;
};

#endif  // LAB_12_REFACTORING_HISTOGRAM_HPP
