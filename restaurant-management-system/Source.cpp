#include <iostream>
#include "Entities.h"
#include "AdminPanel.h"
#include "UserPanel.h"
#include "Display.h"
#include "Helpers.h"
#include "Statistic.h"
#include "Exceptions.h"

using namespace std;
using namespace Entities;
using namespace AdminPanel;
using namespace UserPanel;
using namespace Display;
using namespace FileManager;
using namespace Statistics;

int main() {
    // Restoran, Menyu, Sklad, İstifadəçi və Statistika məlumatlarını hazırlamaq
    Restaurant restaurant("MC Dondalds", "N.Narimanov", 5000.0);
    DataSet<Menu> menuData;
    DataSet<Stock> stockData;
    DataSet<User> userData;
    Statistic statistics;
    DataSet<Order>* orderData = new DataSet<Order>();
    delete orderData;

    // Başlanğıc üçün bəzi məlumatlar əlavə etmək
    stockData.Add(Stock("Tomato", 0.5, 100));
    stockData.Add(Stock("Cheese", 1.0, 50));
    stockData.Add(Stock("Bread", 0.2, 200));

    vector<Stock> pizzaIngredients = { stockData.GetItems()[0], stockData.GetItems()[1], stockData.GetItems()[2] };
    menuData.Add(Menu("Pizza", pizzaIngredients, 10.0));

    // DisplayHandler obyekti
    DisplayHandler displayHandler(&restaurant, &menuData, &stockData, &userData, &statistics);

    try {
        // Əsas menyunu göstər
        displayHandler.ShowMainMenu();
    }
    catch (const Exceptions::BaseException& e) {
        e.PrintMessage();
    }
    catch (const exception& e) {
        cerr << "Xəta: " << e.what() << endl;
    }
    catch (...) {
        cerr << "Naməlum bir xəta baş verdi!" << endl;
    }

    cout << "Proqramdan çıxıldı..." << endl;
    return 0;
}
