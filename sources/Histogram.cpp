//
// Created by vladislav on 07.06.2021.
//

#include "Histogram.hpp"
Histogram& Histogram::Instance() {
  static Histogram res;
  return res;
}
float Histogram::getScore() const { return score_; }
uint Histogram::getSkipped() const { return skipped_; }
void Histogram::ResetSkippedt() { skipped_ = 0; }
void Histogram::setScore(float score) { score_ = score; }
void Histogram::IncSkipped() { ++skipped_; };
