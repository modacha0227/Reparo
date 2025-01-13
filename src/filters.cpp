#include "filters.h"
#include "imguidatechooser.h"
#include <iostream>
#include "conversion.h"
#include "RoLocalization.h"

DateFilter::DateFilter() 
  : directions{
    { 1, "Before"},
    { 2, "After"},
    { 3, "Between"}
  }
  {
  ImGui::SetDateZero(&from);
  ImGui::SetDateZero(&to);
}

bool DateFilter::Render() {
  bool _state = false;
  ImGui::PushItemWidth(250);
  if (ImGui::BeginCombo("##Relation", directions[selected].c_str())) {
    for (auto& pair : directions) {
      if (ImGui::Selectable(pair.second.c_str())) {
        selected = pair.first;
        _state = true;
      }
    }
    ImGui::EndCombo();
  }
  ImGui::SameLine();
  if (ImGui::DateChooser("##From", from)) {
    _state = true;
    std::cout << Convert::TmToStr(from) << std::endl;
  }
  ImGui::SameLine();
  ImGui::BeginDisabled(selected != 3);
  if (ImGui::DateChooser("##To", to)) {
    _state = true;
    std::cout << Convert::TmToStr(to) << std::endl;
  }
  ImGui::EndDisabled();
  ImGui::PopItemWidth();
  return _state;
}

std::string DateFilter::GetForSQL() {
  std::string _string_for_sql;
  switch (selected) {
    case 1:
      _string_for_sql = " < '" + Convert::TmToStr(from) + "'";
      break;
    case 2:
      _string_for_sql = " > '" + Convert::TmToStr(from) + "'";
      break;
    case 3:
      _string_for_sql = " BETWEEN '" + Convert::TmToStr(from) + "' AND '" + Convert::TmToStr(to) + "'";
      break;
    default:
      _string_for_sql = "";
      break;
  }
  return _string_for_sql;
}

ItemPicker::ItemPicker()
  : supplier_sku(_("Supplier SKU"), 0, TFFlags_HasPopup | TFFlags_NeverAnError, "DISTINCT supplier_sku", "purchase_invoice_items", "supplier_sku")
  , own_sku_field(_("Own SKU"), 0, TFFlags_HasPopup | TFFlags_EmptyIsError | TFFlags_AllowDbPresence)
  , name_field(_("Item's name"), 0, TFFlags_HasPopup | TFFlags_HasLenValidator | TFFlags_EmptyIsError, "DISTINCT name", "parts", "name")
{
}

void ItemPicker::Render() {
  own_sku_field.Feedback();
  name_field.FeedbackEx();
  ImGui::NewLine();

  bool refresh = false;

  if (supplier_sku.Render()) {
    if (supplier_sku.Get().empty()) { return; } 
    Part _part = Database::Select<Part>("p.*").From("purchase_invoice_items pii")
      .InnerJoin("parts p")
      .On("pii.part_id = p.id")
      .Where("supplier_sku ")
      .Like(supplier_sku.Get())
      .One();
    if (!_part) { return; }
    RefreshFields(_part);
    refresh = true;
  }

  if (own_sku_field.Render()) {
    if (own_sku_field.Get().empty()) { return; }
    Part _part = own_sku_field.GetFromDb();
    if (!_part) { return; }
    RefreshFields(_part);
    refresh = true;
  }

  name_field.Render();

  if (ImGui::InputInt(_("Quantity"), &quantity))
    refresh = true;
  if (ImGui::InputDouble(_("Purchase price ex. VAT"), &price, 0.0, 0.0, "%.2f"))
    refresh = true;
  if (ImGui::InputDouble(_("% VAT rate"), &vat, 0.0, 0.0, "%.2f"))
    refresh = true;

  ImGui::InputDouble(_("Total net"), &total_net, 0.0, 0.0, "%.2f");
  ImGui::InputDouble(_("Total"), &total, 0.0, 0.0, "%.2f");

  if (refresh) {
    total_net = quantity * price;
    total = total_net + (total_net * vat / 100);
  }
  Validate();
}

void ItemPicker::RefreshFields(Part& _part) {
  own_sku_field.FillBuffer(_part.own_sku);
  name_field.FillBuffer(_part.name);
  price = _part.purch_price;
  vat = _part.vat;
  quantity = 1;
}

InvoiceItem ItemPicker::GetPart() {
  InvoiceItem _part;
  _part.part = own_sku_field.GetFromDb();
  _part.name = name_field.Get();
  _part.supplier_sku = supplier_sku.Get();
  _part.own_sku = own_sku_field.Get();
  _part.quantity = quantity;
  _part.price.amount = price;
  _part.price.vat_rate = vat;
  _part.total_net = total_net;
  _part.total = total;
  std::cout << _part.part.ToString() << std::endl;
  return _part;
}

void ItemPicker::Validate() {
  error = own_sku_field.error || name_field.error;
}