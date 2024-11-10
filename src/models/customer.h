#pragma once
#include <string>
#include <vector>
#include "model_handler.h"

//class CustomerBuilder;
class TextField;

class Customer{
public:
  Customer();
  Customer& GetCustomer();
  void View();

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
  std::string phone;
  std::string name;
  std::string surname;
  std::string email;
  std::vector<std::string> billing_addresses;
  std::vector<std::string> ship_addresses;
};

template<typename T>
inline Builder<T> Customer::Set() {
  return Builder<T>(*this);
}

template<typename T>
inline ModelData<T> Customer::Get() const {
  return ModelData<T>(*this);
}