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

