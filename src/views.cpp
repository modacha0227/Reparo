#include "views.h"
#include "RoLocalization.h"
#include "conversion.h"
#include "database.h"

void CustomerView::DefaultRenderItem(const Customer& customer) {
  ImGui::TableNextColumn();
  ImGui::Text("%d", customer.id);

  ImGui::TableNextColumn();

  if (customer.HasRepairs()) {
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.7f, 0.3f, 0.3f, 0.65f));
  }

  ImGui::Selectable(customer.phone.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap);

  if (customer.HasRepairs()) {
    ImGui::PopStyleColor();
    ImGui::SetItemTooltip("Can't remove customer as long as there is a related repair.", ImGui::GetStyle().HoverDelayShort);
  }
  else {
    if (ImGui::BeginPopupContextItem()) {


      if (ImGui::Button(_("Remove customer?")))
        customer.RemoveModal();

      ImGui::EndPopup();
    }
  }

  ImGui::TableNextColumn();
  ImGui::Text("%s", customer.name.c_str());
  ImGui::TableNextColumn();
  ImGui::Text("%s", customer.surname.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", customer.email.c_str());

}

void CustomerView::LoadData(const std::string& _orderby, const int& _direction) {
  data = Database::Select<Customer>("c.*, COUNT(repairs.id) AS has_repairs")
    .From("customers c")
    .LeftJoin("repairs")
    .On("c.id = repairs.customer_id")
    .GroupBy("c.id")
    .OrderBy(_orderby, _direction)
    .All();
}

RepairView::RepairView(std::vector<Repair> _repairs)
  : BaseTableView<Repair>(
    "Repairs view",
    13,
    {
        {"id", "ID"},
        {"phone", "Cust. Phone"},
        {"name", "Cust. Name"},
        {"device", "Device"},
        {"category", "Category"},
        {"visible_desc", "Notes"},
        {"hidden_desc", "Hidden note"},
        {"price", "Price"},
        {"state", "State"},
        {"sn_imei", "SN / IMEI"},
        {"created_at", "Created at"},
        {"updated_at", "Updated at"},
        {"finished_at", "Finished at"}
    },
    std::move(_repairs)
  ),
  phone(
    _("Search by phone"),
    ImGuiInputTextFlags_CharsDecimal,
    TFFlags_HasPopup | TFFlags_NeverAnError | TFFlags_AllowDbPresence
  ),
  id_filter(
    _("Search by ID"),
    ImGuiInputTextFlags_CharsHexadecimal,
    TFFlags_HasPopup,
    "id",
    "repairs",
    "id"
  ),
  timelines{
      {"created_at", "Created at"},
      {"updated_at", "Updated at"},
      {"finished_at", "Finished at"}
  },
  timeline_combo(timelines),
  states(Database::Select<RepairState>().From().All())
  { }

void RepairView::DefaultRenderItem(const Repair& _repair) {
  ImGui::TableNextColumn();
  std::string _id_str = std::to_string(_repair.id);
  ImGui::Selectable(_id_str.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap);

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.customer.phone.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.customer.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.device.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.category.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.vis_note.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.hid_note.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", _repair.price);

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.repair_state.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.sn_imei.c_str());

  ImGui::TableNextColumn();
  std::string _created_at = Convert::TmToStr(_repair.created_at);
  ImGui::Text("%s", _created_at.c_str());

  ImGui::TableNextColumn();
  std::string _updated_at = Convert::TmToStr(_repair.updated_at);
  ImGui::Text("%s", _updated_at.c_str());

  ImGui::TableNextColumn();
  std::string _finished_at = Convert::TmToStr(_repair.finished_at);
  ImGui::Text("%s", _finished_at.c_str());

}

void RepairView::LoadData(const std::string& _orderby, const int& _direction) {
  data = Database::Select<Repair>("r.*, c.phone, c.name, rc.category, rs.state, ")
    .Coalesce(" (d.model, cd.model) AS model")
    .From("repairs r")
    .LeftJoin("customers c").On("c.id = r.customer_id")
    .LeftJoin("devices d").On("d.id = r.model_id")
    .LeftJoin("custom_devices cd").On("cd.id = r.cust_device_id")
    .LeftJoin("repair_categories rc").On("rc.id = r.category_id")
    .LeftJoin("repair_states rs").On("rs.id = r.repair_state_id")
    .Where("c.phone")
    .Like(phone.Get())
    .And("r.id")
    .Like(id_filter.Get())
    .And(state_query)
    .And(timeline_combo.Get().column)
    .Date(date.GetForSQL())
    .OrderBy(_orderby, _direction)
    .All();
}

void RepairView::Filters() {
  ImGui::SeparatorText(_("Filter by details"));
  if (phone.Render())
    LoadData();
  if (id_filter.Render()) {
    LoadData();
  }
  ImGui::SeparatorText(_("Filter by dates"));
  if (date.Render()) {
    LoadData();
  }
  if (timeline_combo.Render()) {
    LoadData();
  }
  ImGui::SeparatorText(_("Results"));
  if (states.Render()) {
    int _id = states.Get().id;
    if (_id <= 1) {
      state_query = "";
    }
    else {
      state_query = "r.repair_state_id =" + std::to_string(_id);
    }
    LoadData();
  }
}

InventoryView::InventoryView(std::vector<Part> _parts) 
  : BaseTableView<Part>(
    "Inventory view",
    15,
    {
        { "id", "ID"},
        { "name", "Name"},
        { "own_sku", "Own SKU"},
        { "quality", "Quality"},
        { "category", "Category"},
        { "sell_price", "Sell Price"},
        { "sell_price_ex_vat", "Sell Price ex.VAT"},
        { "color", "Color"},
        { "quantity", "Quantity"},
        { "purch_price", "Purchase price"},
        { "purch_price_ex_vat", "Purchase price ex.VAT"},
        { "location", "Location"},
        { "reserved_quantity", "Reserved Quantity"},
        { "created_at", "Created at" },
        { "updated_at", "Updated at" }
    },
    std::move(_parts)
  )
  {}

void InventoryView::DefaultRenderItem(const Part & _part) {
  ImGui::TableNextColumn();
  std::string _id_str = std::to_string(_part.id);
  ImGui::Selectable(_id_str.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap);

  ImGui::TableNextColumn();
  ImGui::Text("%s", _part.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _part.own_sku.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _part.quality.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _part.category.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", _part.sell_price);

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", _part.sell_price_ex_vat);

  ImGui::TableNextColumn();
  ImGui::Text("%s", _part.color.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%d", _part.quantity);

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", _part.purch_price);

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", _part.purch_price_ex_vat);

  ImGui::TableNextColumn();
  ImGui::Text("%s", _part.location.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%d", _part.reserved_quantity);

  ImGui::TableNextColumn();
  std::string _created_at = Convert::TmToStr(_part.created_at);
  ImGui::Text("%s", _created_at.c_str());

  ImGui::TableNextColumn();
  std::string _updated_at = Convert::TmToStr(_part.updated_at);
  ImGui::Text("%s", _updated_at.c_str());

}

void InventoryView::LoadData(const std::string& _orderby, const int& _direction) {
  data = Database::Select<Part>("p.*, q.quality , rc.category , c.color  ")
    .From("parts p")
    .LeftJoin("qualities q").On("q.id = p.quality_id")
    .LeftJoin("repair_categories rc").On("rc.id = p.category_id")
    .LeftJoin("colors c").On("c.id = p.color_id")
    .OrderBy(_orderby, _direction)
    .All();
}
