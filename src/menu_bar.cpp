#include "menu_bar.h"
#include "imgui.h"
#include "RoLocalization.h"
#include "window_manager.h"
#include <functional> // For std::function

namespace {
// Helper lambda to create a menu item and open a window
auto menuItemAction = [](const char* labelKey, const char* windowId) {
    if (ImGui::MenuItem(_(labelKey))) {
        WindowFactory::AddWindow(windowId);
    }
};

// Helper lambda for a simple menu with a single action
auto simpleMenuItemAction = [](const char* labelKey, const char* windowId) {
    if (ImGui::MenuItem(_(labelKey))) {
        WindowFactory::AddWindow(windowId);
    }
};

// Helper function for a submenu with multiple actions
void submenu(const char* labelKey, std::function<void()> contentCallback) {
    if (ImGui::BeginMenu(_(labelKey))) {
        contentCallback();
        ImGui::EndMenu();
    }
}

// Helper function for a submenu with simple menu item actions
void submenuItems(const char* labelKey, std::initializer_list<std::pair<const char*, const char*>> items) {
    submenu(labelKey, [&]() {
        for (const auto& item : items) {
            menuItemAction(item.first, item.second);
        }
    });
}

// Helper function for a submenu with simple menu item actions and a separator
void submenuItemsWithSeparator(const char* labelKey, std::initializer_list<std::pair<const char*, const char*>> items, const char* separatorTextKey) {
    submenu(labelKey, [&]() {
        for (const auto& item : items) {
            menuItemAction(item.first, item.second);
        }
        ImGui::SeparatorText(_(separatorTextKey));
        // Add any additional "Options" menu items here if needed
    });
}

} // namespace

void RoMenuBar::Render() {
    if (!ImGui::BeginMainMenuBar()) {
        return;
    }

    submenu(_("New"), [&]() {
        submenuItemsWithSeparator(_("Customer"), {{"New Customer", "customer"}}, "Options");
        submenuItemsWithSeparator(_("Repair"), {{"New Repair", "repair"}}, "Options");
        simpleMenuItemAction("Device", "device");
        simpleMenuItemAction("Device from Custom Devices", "custom_device");
        submenuItemsWithSeparator(_("Items"), {{"New Item", "parts"}}, "Options");
        submenuItemsWithSeparator(_("Stock Received"), {{"From Purchase Invoice", "purchase_invoice"}, {"Internal arrival", "internal_arrival"}}, "Options");
        simpleMenuItemAction("Supplier", "supplier");
    });

    submenuItems("Views", {
        {"Customers", "customers_view"},
        {"Repairs", "repairs_view"},
        {"Devices", "devices_view"},
        {"Items", "parts_view"},
    });

    AttributesView();

    ImGui::EndMainMenuBar();
}

void RoMenuBar::AttributesView() {
    submenu("Attributes", [&]() {
        menuItemAction("Repair Categories", "categories");
        menuItemAction("Brands", "brands");
        menuItemAction("Device Types", "device_types");
        menuItemAction("Repair States", "repair_states");
        menuItemAction("Qualities", "qualities");
        menuItemAction("Payment Methods", "payment_methods");
    });
}
