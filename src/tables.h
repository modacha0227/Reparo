#pragma once
#include <vector>
#include "text_fields.h"
#include "models/simple_models.h"

enum TableCalls_ {
  TableCalls_None,
  TableCalls_Edit,
  TableCalls_Delete,
  TableCalls_Print,
};

typedef int TableCalls;

class RoTable {
public:
  static void AddressesInputs(std::vector<TextField>& first, std::vector<TextField>& second);
  static void Addresses(std::vector<std::string>& first, std::vector<std::string>& second);
  template <typename T>
  static TableCalls SimpleModel(std::vector<T>& models);
};

template<typename T>
inline TableCalls RoTable::SimpleModel(std::vector<T>& models) {
  if (ImGui::BeginTable("IDValue", 2)) {
    ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_NoHide);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_NoHide);
    ImGui::TableHeadersRow();
    ImGui::TableNextRow();
    for (auto& model : models) {
      ModelData data = model.Get<T>();
      static int selected = -1;
      const bool is_selected = (selected == data.ID());

      ImGui::TableNextColumn();
      char label[32];
      sprintf_s(label, "%d", data.ID());
      if (ImGui::Selectable(label, is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_DontClosePopups)) {
      }

      if (ImGui::BeginPopupContextItem()) {
        if (ImGui::Button("Edit record")) {
          return TableCalls_Edit;
        }
        if (ImGui::Button("Delete")) {
          return TableCalls_Delete;
        }
        ImGui::EndPopup();
      }
      ImGui::TableNextColumn();
      ImGui::Text("%s", data.ID());
    }
  ImGui::EndTable();
  }
  return TableCalls_None;
}
