#pragma once
#include "Entities.h"
#include "Statistic.h"
#include "Helpers.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

using namespace std;
using namespace Entities;
using namespace Statistics;
using namespace FileManager;

//Admin idareetmesi
// 
// 

namespace AdminPanel {
   
    bool AuthenticateAdminUser(const string& expectedUsername, const string& expectedPassword) {
        string inputUsername, inputPassword;
     
        cout << "İstifadəçi adı: ";
        cin >> inputUsername;
        
        if (inputUsername.empty()) {
            cout << "Xəta: İstifadəçi adı boş ola bilməz!" << endl;
            return false; 
        }

        cout << "Şifrə: ";
        cin >> inputPassword;

        if (inputPassword.empty()) {
            cout << "Xəta: Şifrə boş ola bilməz!" << endl;
            return false; 
        }

        if (inputUsername == expectedUsername && inputPassword == expectedPassword) {
            cout << "Giriş uğurlu!\n Davam etmək üçün 'Enter' düyməsinə basın" << endl;
            cin.ignore();
            cin.get();
            return true;
        }
        else {
            cout << "Yanlış istifadəçi adı və ya şifrə!" << endl;
            return false;
        }
    }




    class AdminController {
        Restaurant* restaurant;
        DataSet<Menu>* menuData;
        DataSet<Stock>* stockData;
        Statistic* statistics;
    public:
        AdminController(Restaurant* restaurant, DataSet<Menu>* menuData, DataSet<Stock>* stockData, Statistic* statistics)
            : restaurant(restaurant), menuData(menuData), stockData(stockData), statistics(statistics) {
            *menuData = FileHandler::LoadMenuData(); 
            *stockData = FileHandler::LoadStockData(); 
        }


      //Admin terefinden menyunun idaretmesi
        void AddMenuItem() {
            string name, description;
            vector<Stock> ingredients;
            double price;
            int ingredientCount;

            cout << "Yemək adı: ";
            cin.ignore();
            getline(cin, name);

            cout << "Inqrediyentlərin sayı: ";
            cin >> ingredientCount;

            DisplayStock(); 

            for (int i = 0; i < ingredientCount; i++) {
                int id, quantity;
                cout << "Inqrediyent ID-sini daxil edin: ";
                cin >> id;

                try {
                    Stock ingredient = stockData->GetItems().at(id - 1);

                    cout << "Inqrediyentin miqdarını daxil edin (kq): ";
                    cin >> quantity;

                    ingredient.SetQuantity(quantity);
                    ingredients.push_back(ingredient);
                }
                catch (const out_of_range&) {
                    cout << "Yanlış ID! Yenidən cəhd edin." << endl;
                    i--;
                }
            }

            cout << "Yeməyin qiyməti: ";
            cin >> price;

            cout << "Yemək haqqında qısa təsviri daxil edin: ";
            cin.ignore();
            getline(cin, description);

            Menu newMenu(name, ingredients, price, description);
            menuData->Add(newMenu);
            FileHandler::SaveMenuData(*menuData);
            
            Logger::LogMessage("Yeni yemək əlavə edildi: " + name);
            cout << "Yemək menyuya əlavə edildi!" << endl;
        }

        //Boyuk ve kicik herfe tesirli olmasi ucun
        string toLower(const string& str) {
            string lowerStr = str;
            transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
            return lowerStr;
        }

        void AddStockItem() {
            string name;
            double pricePerUnit;
            int quantity;

            cout << "Inqrediyent adı: ";
            cin.ignore();
            getline(cin, name);

            string lowerName = toLower(name);


     

            if (lowerName.empty()) {
                cout << "Xəta: Inqrediyent adı boş ola bilməz!" << endl;
                return;
            }

            cout << "Bir kq üçün qiymət: ";
            cin >> pricePerUnit;

            if (pricePerUnit <= 0) {
                cout << "Xəta: Qiymət 0 və ya mənfi ola bilməz!" << endl;
                return;
            }

            cout << "Sayı: ";
            cin >> quantity;

            if (quantity <= 0) {
                cout << "Xəta: Say 0 və ya mənfi ola bilməz!" << endl;
                return;
            }
            //Eyni mehsuldan varsa ancaq sayi artsin.
            for (auto& stock : stockData->GetItems()) {
                if (toLower(stock.GetName()) == lowerName && stock.GetPricePerUnit() == pricePerUnit) {
                    stock.AddQuantity(quantity);
                    cout << "Məhsul əlavə edildi!" << endl;
                    Logger::LogMessage("Eyni inqrediyent: " + stock.GetName() + ", Yeni say: " + to_string(stock.GetQuantity()));
                    FileHandler::SaveStockData(*stockData);
                    return;
                }
            }



            double totalCost = pricePerUnit * quantity;

            try {
                restaurant->DeductFromBudget(totalCost);
                Stock newStock(name, pricePerUnit, quantity);
                FileHandler::SaveStockData(*stockData);
                FileHandler::SaveBudget(restaurant->GetBudget());

                stockData->Add(newStock);
                statistics->AddExpense(totalCost);
                Logger::LogMessage("Yeni inqrediyent əlavə edildi: " + name);
                cout << "Yeni inqrediyent uğurla əlavə edildi!" << endl;
            }
            catch (const runtime_error& e) {
                cout << "Xəta: " << e.what() << endl;
            }
        }

        void RemoveStockItem() {
            cout << "========= Inqrediyent Silmə =========" << endl;

            
            if (stockData->GetItems().empty()) {
                cout << "Sklad boşdur! Silinə biləcək inqrediyent yoxdur." << endl;
                return;
            }

            DisplayStock();

            int id;
            cout << "Silinmək istədiyiniz inqrediyentin ID-sini daxil edin: ";
            cin >> id;

           
            if (id <= 0 || id > stockData->GetItems().size()) {
                cout << "Yanlış ID daxil edildi! Yenidən cəhd edin." << endl;
                return;
            }

            
            try {
                string removedItemName = stockData->GetItems().at(id - 1).GetName();
                stockData->GetItems().erase(stockData->GetItems().begin() + (id - 1));
                Logger::LogMessage("Inqrediyent silindi: " + removedItemName);
                FileHandler::SaveStockData(*stockData);
                cout << "Inqrediyent uğurla silindi: " << removedItemName << endl;
            }
            catch (const out_of_range&) {
                cout << "Silinmə zamanı xəta baş verdi! Yanlış ID." << endl;
            }
        }
        void UpdateStockQuantity() {
            cout << "========= Inqrediyentin Sayını Dəyişdir =========" << endl;

            
            if (stockData->GetItems().empty()) {
                cout << "Sklad boşdur! Sayını dəyişdirə biləcəyiniz inqrediyent yoxdur." << endl;
                return;
            }

            
            DisplayStock();

            int id, newQuantity;
            cout << "Dəyişdirmək istədiyiniz inqrediyentin ID-sini daxil edin: ";
            cin >> id;

            
            if (id <= 0 || id > stockData->GetItems().size()) {
                cout << "Yanlış ID daxil edildi! Yenidən cəhd edin." << endl;
                return;
            }

            cout << "Yeni miqdarı daxil edin: ";
            cin >> newQuantity;
            if (newQuantity < 0) {
                cout << "Xəta: Miqdar mənfi ola bilməz!" << endl;
                return;
            }

            
            try {
                Stock& selectedStock = stockData->GetItems().at(id - 1);
                int oldQuantity = selectedStock.GetQuantity();
                selectedStock.SetQuantity(newQuantity);
                Logger::LogMessage("Inqrediyentin miqdarı dəyişdirildi: " + selectedStock.GetName() +
                    " Köhnə Say: " + to_string(oldQuantity) +
                    ", Yeni Say: " + to_string(newQuantity));
                FileHandler::SaveStockData(*stockData);
                cout << "Inqrediyentin miqdarı uğurla dəyişdirildi: " << selectedStock.GetName() << endl;
            }
            catch (const out_of_range&) {
                cout << "Dəyişiklik zamanı xəta baş verdi! Yanlış ID." << endl;
            }
        }

        void RemoveMenuItem() {
            cout << "========= Yemək Sil =========" << endl;

            
            if (menuData->GetItems().empty()) {
                cout << "Menyu boşdur! Silə biləcəyiniz yemək yoxdur." << endl;
                return;
            }

            
            cout << "Mövcud Yeməklər:" << endl;
            int index = 1;
            for (const auto& menu : menuData->GetItems()) {
                cout << index++ << ". " << menu.GetName() << " (Qiymət: ₼" << menu.GetPrice() << ")" << endl;
            }

            int id;
            cout << "Silmək istədiyiniz yeməyin ID-sini daxil edin: ";
            cin >> id;

            
            if (id <= 0 || id > menuData->GetItems().size()) {
                cout << "Yanlış ID daxil edildi! Yenidən cəhd edin." << endl;
                return;
            }

            try {
                Menu removedMenu = menuData->GetItems().at(id - 1); 
                menuData->GetItems().erase(menuData->GetItems().begin() + (id - 1)); 
                FileHandler::SaveMenuData(*menuData);
                Logger::LogMessage("Yemək silindi: " + removedMenu.GetName());
                cout << "Yemək uğurla silindi: " << removedMenu.GetName() << endl;
            }
            catch (const out_of_range&) {
                cout << "Silmə zamanı xəta baş verdi! Yanlış ID." << endl;
            }
        }
        void UpdateMenuItem() {
            cout << "========= Yeməyi Dəyişdir =========" << endl;

            if (menuData->GetItems().empty()) {
                cout << "Menyu boşdur! Dəyişdirə biləcəyiniz yemək yoxdur." << endl;
                return;
            }

            
            cout << "Mövcud Yeməklər:" << endl;
            int index = 1;
            for (const auto& menu : menuData->GetItems()) {
                cout << index++ << ". " << menu.GetName() << " (Qiymət: ₼" << menu.GetPrice() << ")" << endl;
            }

            int id;
            cout << "Dəyişdirmək istədiyiniz yeməyin ID-sini daxil edin: ";
            cin >> id;

            
            if (id <= 0 || id > menuData->GetItems().size()) {
                cout << "Yanlış ID daxil edildi! Yenidən cəhd edin." << endl;
                return;
            }

            
            Menu& selectedMenu = menuData->GetItems().at(id - 1);

            cout << "Yeni yemək adını daxil edin (mövcud ad: " << selectedMenu.GetName() << "): ";
            cin.ignore();
            string newName;
            getline(cin, newName);

            cout << "Yeni qiyməti daxil edin (mövcud qiymət: ₼" << selectedMenu.GetPrice() << "): ";
            double newPrice;
            cin >> newPrice;

            cout << "Yeni təsviri daxil edin (mövcud təsvir: " << selectedMenu.GetDescription() << "): ";
            cin.ignore();
            string newDescription;
            getline(cin, newDescription);

            
            cout << "Yeməyin yeni inqrediyentlərinin sayını daxil edin: ";
            int ingredientCount;
            cin >> ingredientCount;

            vector<Stock> newIngredients;
            DisplayStock(); 
            for (int i = 0; i < ingredientCount; i++) {
                int stockId, quantity;
                cout << "Inqrediyent ID-sini daxil edin: ";
                cin >> stockId;

                try {
                    Stock ingredient = stockData->GetItems().at(stockId - 1);

                    cout << "Inqrediyentin miqdarını daxil edin (kq): ";
                    cin >> quantity;

                    ingredient.SetQuantity(quantity);
                    newIngredients.push_back(ingredient);
                }
                catch (const out_of_range&) {
                    cout << "Yanlış ID! Yenidən cəhd edin." << endl;
                    i--;
                }
            }

            
            selectedMenu.SetName(newName.empty() ? selectedMenu.GetName() : newName);
            selectedMenu.SetPrice(newPrice > 0 ? newPrice : selectedMenu.GetPrice());
            selectedMenu.SetDescription(newDescription.empty() ? selectedMenu.GetDescription() : newDescription);
            selectedMenu.SetIngredients(newIngredients.empty() ? selectedMenu.GetIngredients() : newIngredients);
            FileHandler::SaveMenuData(*menuData);
            Logger::LogMessage("Yemək yeniləndi: " + selectedMenu.GetName());
            cout << "Yemək uğurla dəyişdirildi!" << endl;
        }

        //Gosterisler
        void DisplayStock() const {
            cout << "========= Sklad Məlumatları =========" << endl;

            if (stockData->GetItems().empty()) {
                cout << "Sklad boşdur!" << endl;
                return;
            }

            int id = 1;
            cout << left << setw(5) << "ID" << setw(20) << "Ad" << setw(10) << "Qiymət" << setw(10) << "Say (KQ)" << endl;
            cout << "------------------------------------------------" << endl;

            for (const auto& stock : stockData->GetItems()) {
                cout << left << setw(5) << id++
                    << setw(20) << stock.GetName()
                    << setw(10) << stock.GetPricePerUnit()
                    << setw(10) << stock.GetQuantity() << endl;
            }
        }


        void DisplayStatistics() const {
            statistics->PrintDailyStats();
            statistics->PrintMonthlyStats();
        }

        void DisplayBudget() const {
            cout << "Hazırkı büdcə: ₼ " << restaurant->GetBudget() << endl;
        }

        void AddToBudget() {
            double amount;
            cout << "Əlavə etmək istədiyiniz məbləği daxil edin: ";
            cin >> amount;

            if (amount <= 0) {
                cout << "Xəta: Məbləğ müsbət olmalıdır!" << endl;
                return;
            }

            restaurant->AddToBudget(amount);
            cout << "Büdcəyə ₼" << amount << " əlavə edildi." << endl;
            Logger::LogMessage("Büdcəyə əlavə edildi: ₼" + to_string(amount));
            FileHandler::SaveBudget(restaurant->GetBudget());
        }

        void DeductFromBudget() {
            double amount;
            cout << "Çıxarmaq istədiyiniz məbləği daxil edin: ";
            cin >> amount;

            if (amount <= 0) {
                cout << "Xəta: Məbləğ müsbət olmalıdır!" << endl;
                return;
            }

            try {
                restaurant->DeductFromBudget(amount);
                cout << "Büdcədən ₼" << amount << " çıxarıldı." << endl;
                Logger::LogMessage("Büdcədən çıxarıldı: ₼" + to_string(amount));
                FileHandler::SaveBudget(restaurant->GetBudget());
            }
            catch (const runtime_error& e) {
                cout << "Xəta: " << e.what() << endl;
            }
        }
        void DisplayMenu() const {
            cout << "========= Mövcud Yeməklər =========" << endl;
            if (menuData->GetItems().empty()) {
                cout << "Menyu boşdur!" << endl;
                return;
            }

            int index = 1;
            for (const auto& menuItem : menuData->GetItems()) {
                cout << index++ << ". Ad: " << menuItem.GetName()
                    << ", Qiymət: ₼" << menuItem.GetPrice() << endl;
            }

            
            int menuId;
            cout << "Ətraflı məlumat üçün yemək ID-sini daxil edin (0 - Çıxış): ";
            cin >> menuId;
            FileManager::ClearScreen();

            if (menuId > 0 && menuId <= menuData->GetItems().size()) {
                const auto& menuItem = menuData->GetItems().at(menuId - 1);

               
                cout << "\n========= Yemək Haqqında Ətraflı Məlumat =========" << endl;
                cout << "Ad: " << menuItem.GetName() << endl;
                cout << "Qiymət: ₼" << menuItem.GetPrice() << endl;
                cout << "Təsvir: " << menuItem.GetDescription() << endl; 
                cout << "Tərkibi:" << endl;

                for (const auto& ingredient : menuItem.GetIngredients()) {
                    cout << " - " << ingredient.GetName()
                        << " (Sayı: " << ingredient.GetQuantity() << " kq)" << endl;
                }
                cout << "===================================================" << endl;
            }
            else if (menuId != 0) {
                cout << "Yanlış ID daxil edildi!" << endl;
            }
        }



    };

}
