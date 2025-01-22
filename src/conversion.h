#pragma once
#include <string>
#include <vector>
#include <algorithm>

namespace Convert {
  // Forward declare instead of full implementation
  std::string TmToStr(const std::tm& _date);
  template <typename T>
  bool ContainsItemWithId(const std::vector<T>& vec, int id) {
    return std::any_of(vec.begin(), vec.end(), [id](const T& item) {
      return item.id == id;
      });
  }

  template <typename T>
  void PushBackIfUnique(std::vector<T>& vec, const T& item) {
    if (!ContainsItemWithId(vec, item.id)) {
      vec.push_back(item);
    }
  }
}