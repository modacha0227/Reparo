#pragma once
#include <unordered_map>
#include "text_fields.h"
#include "models/simple_models.h"
#include "database.h"
#include "combo.h"
#include "attributes.h"
#include "models/customer.h"
#include "models/supplier.h"

class StackModal;

class RoWindow {
public:
  virtual void Render() = 0;
  virtual ~RoWindow() = default;
  bool open = false;
};

class CustomerWin : public RoWindow {
public:
  CustomerWin();
  CustomerWin(TFFlags phone_flags);
  ~CustomerWin();
  void Init();
  void Render();
  void Debug();
  void Feedback();
  void Submit();
  void FillBuffersByPhone(Customer& autofill);
  void InputFields();
  void Addresses();
  void FieldsValidate();
  Customer CreateCustomer();
  Customer GetCustomer();
  bool error = true;
private:
  PhoneField phone;
  NameField name;
  SurnameField surname;
  EmailField email;
  std::vector<TextField> billing_address;
  std::vector<TextField> ship_address;

  //bool open = true;
};

class DeviceWin {
public:
  DeviceWin();
  DeviceWin(CustomDevice _custom);
  ~DeviceWin();
  void Init();
  void Render();
  void DeviceName();
  void FillDeviceByName(Device& autofill);

private:
  DeviceField name;
  RoCombo<Brand> brand_combo;
  RoCombo<DeviceType> type_combo;
  Attributes<Color> colors;
  Attributes<Alias> aliases;
  bool open = true;
};

class RepairWin {
public:
  RepairWin();
  void Render();
  void CustomerSection();
  void DeviceSection();
  void NotesSection();
  void FieldsValidate();
  void DeviceFeedback();
  void NotesFeedback();
  void PriceSection();
  void PriceFeedback();
  void Submit();
  void RepairValidated();
  Device CreateDevice();

private:
  CustomerWin customer_section;
  bool device_section_error = true;
  bool notes_section_error = true;
  bool price_section_error = true;
  bool error = true;
  double price = 0;
  bool price_can_be_zero;
  DeviceField device;
  SimpleModelField<Category> category;
  RelationalField<Color, DeviceField> color;
  TextField sn_imei;
  TextField vis_note;
  TextField hid_note;

  //bool open = true; // use later
};

class  PartsWin {
  public:
    PartsWin();
    void Render();
    void Feedback();
    void PriceSection(const std::string& _action, Price& _price);
    void QuantitySection();
    void CompatibleTablePicker();
    void CompatibleEntriesBox();
    template <typename T>
    void ListEntriesInBox(float& last_btn, float window, std::unordered_map<int, T>& entries);
    void Submit();
    void Filters();
    void LoadDevices();
    void FieldsValidate();
  private:
    // Main insert input fields
    SimpleModelField<Supplier> supplier;
    OwnSKUField own_sku_field;
    QueriedTextField name_field;
    SimpleModelField<Color> color;
    RoCombo<Quality> quality;
    RoCombo<Category> category;
    QueriedTextField location;
    Price purch_price;
    Price sell_price;
    int quantity = 0;
    // Filter field
    QueriedTextField device_filter;
    SimpleModelField<Brand> brand_filter;
    SimpleModelField<DeviceType> device_type_filter;
    std::vector<Device> devices;
    std::unordered_map<int, Device> cmptble_devices;
    std::unordered_map<int, Alias> cmptble_aliases;
    bool open = true;
    bool error = true;
};

class CustomDeviceWin {
public:
  CustomDeviceWin();
  void Render();

  bool error = true;
private:
  std::vector<CustomDevice> devices;
  bool open = true;
};

template <typename T>
class SimpleModelWin {
public:
  SimpleModelWin();
  void Render();
  void LoadData();

private:
  T model;
  TextField name;
  std::vector<T> values;
  bool open = false;
};

template<typename T>
inline SimpleModelWin<T>::SimpleModelWin() {
  LoadData();
}

template<typename T>
inline void SimpleModelWin<T>::LoadData() {
  std::string text = Database::Select<T>().From().GetSql();
  std::cout << text << std::endl;
  values = Database::Select<T>().From().All();
}

class WindowFactory {
public:
  static void AddWindow(const std::string& _window);
  static void Render();

private:
  static std::map<std::string, std::unique_ptr<RoWindow>> windows;
};