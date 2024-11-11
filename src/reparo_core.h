#pragma once
#include <functional>
#include <string>
#include "reparo_windows.h"

class Database;
class TableCreator;

class ReparoCore {
public:
  void Render();

private:
  CustomerWin customer_win;
  BrandWin    brand_win;

#ifdef DEBUG
public:
  static void RenderDebug();

private:  
  static void CreateDatabaseBtn();
  struct TableCreationInfo {
    std::string label;
    std::function<void()> create_function;
  };
  static std::vector<TableCreationInfo> table_config;
  static void InitializeTableBtns();
#endif // DEBUG

};