//
// Created by vladislav on 07.06.2021.
//

#include "PageContainer.hpp"

const Item& PageContainer::ByIndex(size_t i) const {
  return data_[i];
}
const Item& PageContainer::ById(const std::string& id) const {
  auto it = std::find_if(std::begin(data_), std::end(data_),
                         [&id](const auto& i) { return id == i.id; });
  return *it;
}
void PageContainer::Load(std::istream& io, float threshold) {
  RawDataLoad(io);
  Histogram::Instance().ResetSkipped();

  std::vector<Item> data;
  std::set<std::string> ids;
  float sum = 0;
  size_t item_count = 0;
  for (const auto& line : raw_data_) {
    std::stringstream stream(line);

    Item item;
    stream >> item.id >> item.name >> item.score;

    if (auto&& [_, inserted] = ids.insert(item.id); !inserted) {
      throw std::runtime_error("already seen");
    }

    if (item.score > threshold) {
      data.push_back(std::move(item));
      ++item_count;
      sum += item.score;
    } else {
      stat_sender_->Skip(item);
      Histogram::Instance().IncSkipped();
    }
  }
  Histogram::Instance().setAvg(sum/ item_count);

  if (data.size() < kMinLines) {
    throw std::runtime_error("Correct items less than kMinLines");
  }

  memory_counter_->OnDataLoad(data_, data);
  stat_sender_->OnLoaded(data);
  data_ = std::move(data);
}
void PageContainer::RawDataLoad(std::istream& io) {
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
void PageContainer::Reload(float threshold) {
  Histogram::Instance().ResetSkipped();

  std::vector<Item> data;
  std::set<std::string> ids;
  float sum = 0;
  size_t item_count = 0;
  for (const auto& line : raw_data_) {
    std::stringstream stream(line);

    Item item;
    stream >> item.id >> item.name >> item.score;

    if (auto&& [_, inserted] = ids.insert(item.id); !inserted) {
      throw std::runtime_error("already seen");
    }

    if (item.score > threshold) {
      data.push_back(std::move(item));
      ++item_count;
      sum += item.score;
    } else {
      stat_sender_->Skip(item);
      Histogram::Instance().IncSkipped();
    }
  }
  Histogram::Instance().setAvg(sum/item_count);

  if (data.size() < kMinLines) {
    throw std::runtime_error("Correct items less than kMinLines");
  }

  memory_counter_->OnDataLoad(data_, data);
  stat_sender_->OnLoaded(data);
  data_ = std::move(data);
}
