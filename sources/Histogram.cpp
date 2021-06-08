//
// Created by vladislav on 07.06.2021.
//

#include "Histogram.hpp"
Histogram& Histogram::Instance() {
  static Histogram res;
  return res;
}
float Histogram::getAvg() const { return avg_; }
uint Histogram::getSkipped() const { return skipped_; }
void Histogram::ResetSkipped() { skipped_ = 0; }
void Histogram::setAvg(float score) { avg_ = score; }
void Histogram::IncSkipped() { ++skipped_; }
