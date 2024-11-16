#pragma once 
#include "../../src/text_fields.h"
#include <concepts>

class Customer;
class Device;
class TextField;
class DeviceType;
class Brand;
class Color;
class Alias;

template<typename T>
class Builder;
template<typename T>
class ModelData;

// Add more concepts so Simple Models won't be able to use other setters and getters beside ID and Name
template<typename T>
concept IsDevice = std::is_convertible_v<T&, Device&>;

template<typename T>
concept IsCustomer = std::is_convertible_v<T&, Customer&>;

template<typename T>
class Builder {
public:
  Builder(T& model);

  Builder<T>& ID(const int& id);
  Builder<T>& LinkID(const int& id);
  Builder<T>& Model(const std::string& name);
  Builder<T>& Brand_(const Brand& brand);
  Builder<T>& Type(const DeviceType& type);
  Builder<T>& Name(const std::string& name);
  Builder<T>& Phone(const std::string& phone);
  Builder<T>& Surname(const std::string& surname);
  Builder<T>& Email(const std::string& email);
  Builder<T>& BillingAddresses(const std::vector<TextField>& addresses) requires IsCustomer<T>;
  Builder<T>& ShipAddresses(const std::vector<TextField>& addresses) requires IsCustomer<T>;
  Builder<T>& Colors(const std::vector<Color>& colors) requires IsDevice<T>;
  Builder<T>& Aliases(const std::vector<Alias>& aliases) requires IsDevice<T>;

private:
  T& model;
};

template<typename T>
inline Builder<T>::Builder(T& model) : model(model) {
}

template<typename T>
inline Builder<T>& Builder<T>::ID(const int& id) {
  model.id = id;
  return *this;
}

template<typename T>
inline Builder<T>& Builder<T>::LinkID(const int& id) {
  model.link_id = id;
  return *this;
}

template<typename T>
inline Builder<T>& Builder<T>::Model(const std::string& name) {
  model.name = name;
  return *this;
}

template<typename T>
inline Builder<T>& Builder<T>::Brand_(const Brand& brand) {
  model.brand = brand;
  return *this;
}

template<typename T>
inline Builder<T>& Builder<T>::Type(const DeviceType& type) {
  model.type = type;
  return *this;
}

template<typename T>
inline Builder<T>& Builder<T>::Name(const std::string& name) {
  model.name = name;
  return *this;
}

template<typename T>
inline Builder<T>& Builder<T>::Phone(const std::string& phone) {
  model.phone = phone;
  return *this;
}

template<typename T>
inline Builder<T>& Builder<T>::Surname(const std::string& surname) {
  model.surname = surname;
  return *this;
}

template<typename T>
inline Builder<T>& Builder<T>::Email(const std::string& email) {
  model.email = email;
  return *this;
}

template<typename T>
inline Builder<T>& Builder<T>::BillingAddresses(const std::vector<TextField>& addresses)
  requires IsCustomer<T> {
  for (int i = 0; i < addresses.size(); i++) {
        std::string line = addresses[i].Get();
        model.billing_addresses.emplace_back(line);
      }
  return *this;
}

template<typename T>
inline Builder<T>& Builder<T>::ShipAddresses(const std::vector<TextField>& addresses)
  requires IsCustomer<T> {
  for (int i = 0; i < addresses.size(); i++) {
    std::string line = addresses[i].Get();
    model.ship_addresses.emplace_back(line);
  }
  return *this;
}

template<typename T>
inline Builder<T>& Builder<T>::Colors(const std::vector<Color>& colors)
  requires IsDevice<T> {
  model.colors = colors;
  return *this;
}

template<typename T>
inline Builder<T>& Builder<T>::Aliases(const std::vector<Alias>& aliases)
  requires IsDevice<T> {
  model.aliases = aliases;
  return *this;
}


template<typename T>
class ModelData {
public:
  ModelData(const T& model);

  const int ID() const;
  const int LinkID() const;
  const std::string& Model() const;
  const Brand& Brand_() const; // _ added so it matches thebuilder
  const DeviceType& Type() const requires IsDevice<T>;
  const std::string& Name() const;
  const std::string& Phone() const;
  const std::string& Surname() const;
  const std::string& Email() const;
  const std::vector<std::string>& BillingAddresses() const requires IsCustomer<T>;
  const std::vector<std::string>& ShipAddresses() const requires IsCustomer<T>;
  const std::vector<Alias>& Aliases() const requires IsDevice<T>;
  const std::vector<Color>& Colors() const requires IsDevice<T>;
  const std::string_view& Table() const;
  const std::string& Column() const;
  const std::string& WindowTitle() const;

private:
  const T& model;
};

template<typename T>
inline ModelData<T>::ModelData(const T& model) : model(model) { }

template<typename T>
inline const int ModelData<T>::ID() const {
  return model.id;
}

template<typename T>
inline const int ModelData<T>::LinkID() const {
  return model.link_id;
}

template<typename T>
inline const std::string& ModelData<T>::Model() const {
  return model.name;
}

template<typename T>
inline const Brand& ModelData<T>::Brand_() const {
  return model.brand;
}

template<typename T>
inline const DeviceType& ModelData<T>::Type() const 
  requires IsDevice<T> {
    return model.type;
}

template<typename T>
inline const std::string& ModelData<T>::Name() const {
  return model.name;
}

template<typename T>
inline const std::string& ModelData<T>::Phone() const {
  return model.phone;
}

template<typename T>
inline const std::string& ModelData<T>::Surname() const {
  return model.surname;
}

template<typename T>
inline const std::string& ModelData<T>::Email() const {
  return model.email;
}

template<typename T>
inline const std::vector<std::string>& ModelData<T>::BillingAddresses() const
  requires IsCustomer<T> {
  return model.billing_addresses;
}

template<typename T>
inline const std::vector<std::string>& ModelData<T>::ShipAddresses() const
  requires IsCustomer<T> {
  return model.ship_addresses;
}

template<typename T>
inline const std::vector<Alias>& ModelData<T>::Aliases() const
  requires IsDevice<T> {
  return model.aliases;
}

template<typename T>
inline const std::vector<Color>& ModelData<T>::Colors() const
  requires IsDevice<T> {
  return model.colors;
}

template<typename T>
inline const std::string_view& ModelData<T>::Table() const {
  return model.table;
}

template<typename T>
inline const std::string& ModelData<T>::Column() const {
  return model.column;
}

template<typename T>
inline const std::string& ModelData<T>::WindowTitle() const {
  return model.window_title;
}