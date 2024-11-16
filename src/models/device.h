#pragma once
#include <string>
#include <vector>
#include "model_handler.h"
#include "simple_models.h"
#include "alias.h"

class Device {
public:
  Device();
  Device& GetDevice();
  template<typename T>
  Builder<T> Set();

  template<typename T>
  ModelData<T> Get() const;

private:
  template<typename T>
  friend class Builder;
  template<typename T>
  friend class ModelData;

  int id = -1;
  std::string name;
  Brand brand;
  DeviceType type;
  std::vector<Color> colors;
  std::vector<Alias> aliases;
  static constexpr std::string_view table = "devices";
};

template<typename T>
inline Builder<T> Device::Set() {
  return Builder<T>(*this);
}

template<typename T>
inline ModelData<T> Device::Get() const {
  return ModelData<T>(*this);
}