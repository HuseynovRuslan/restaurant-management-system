#pragma once
#include "AdminPanel.h"
#include "UserPanel.h"
#include "Entities.h"
#include "Helpers.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace AdminPanel;
using namespace UserPanel;
using namespace Entities;

//Menyular headeri

namespace Display {

    class DisplayHandler {
        Restaurant* restaurant;
        DataSet<Menu>* menuData;
        DataSet<Stock>* stockData;
        DataSet<User>* userData;
        Statistic* statistics;
    public:
        DisplayHandler(Restaurant* restaurant, DataSet<Menu>* menuData, DataSet<Stock>* stockData,
            DataSet<User>* userData, Statistic* statistics)
            : restaurant(restaurant), menuData(menuData), stockData(stockData), userData(userData), statistics(statistics) {}

        // Esas menyunun gosterilmesi
        void ShowMainMenu() {
            int choice;

            do {
                FileManager::ClearScreen();
                cout << "\n========= Əsas Menyu =========" << endl;
                cout << "1. Admin Panel" << endl;
                cout << "2. User Panel" << endl;
                cout << "0. Çıxış" << endl;
                cout << "Seçiminizi edin: ";
                cin >> choice;

                switch (choice) {
                case 1:
                    FileManager::ClearScreen();
                    ShowAdminMenu();
                    break;
                case 2:
                    FileManager::ClearScreen();
                    ShowUserSubmenu();
                    break;
                case 0:
                    cout << "Proqramdan çıxıldı..." << endl;
                    FileManager::ClearScreen();
                    break;
                default:
                    cout << "Yanlış seçim ! Yenidən cəhd edin." << endl;
                }
            } while (choice != 0);
        }


        // Admin menyusu
        void ShowAdminMenu() {
            AdminController adminController(restaurant, menuData, stockData, statistics);

            if (!AdminPanel::AuthenticateAdminUser("admin", "admin")) {
                cout << "Admin panelinə giriş uğursuz oldu." << endl;
                cin.ignore();
                cin.get();
                return;
            }


            int choice;
            do {
                FileManager::ClearScreen();
                cout << "\n========= Admin Panel =========" << endl;
                cout << "1. Yeni Yemək Əlavə Et" << endl;
                cout << "2. Yemək Sil" << endl;
                cout << "3. Yeməyi Dəyişdir" << endl;
                cout << "4. Yeni Inqrediyent Əlavə Et" << endl;
                cout << "5. Inqrediyent Sil" << endl;
                cout << "6. Inqrediyentin Sayını Dəyişdir" << endl;
                cout << "7. Skladı Göstər" << endl;
                cout << "8. Statistikaları Göstər(beta)" << endl;
                cout << "9. Büdcəni Göstər" << endl;
                cout << "10.Büdcəyə Məbləğ Əlavə Et" << endl;
                cout << "11.Büdcədən Məbləğ Çıxar" << endl;
                cout << "12.Menyuya Bax" << endl;
                cout << "0. Əsas Menyuya Qayıt" << endl;
                cout << "Seçiminizi edin: ";
                cin >> choice;

                switch (choice) {
                case 1:
                    FileManager::ClearScreen();
                    adminController.AddMenuItem();
                    cout << "\nDavam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();

                    break;

                case 2:
                    FileManager::ClearScreen();
                    adminController.RemoveMenuItem();
                    cout << "\nDavam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();
                    break;

                case 3:
                    FileManager::ClearScreen();
                    adminController.UpdateMenuItem();
                    cout << "\nDavam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();
                    break;

                case 4:
                    FileManager::ClearScreen();
                    adminController.AddStockItem();
                    cout << "\nDavam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();

                    break;
                case 5:
                    FileManager::ClearScreen();
                    adminController.RemoveStockItem();
                    cout << "\nDavam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();
                    break;

                case 6:
                    FileManager::ClearScreen();
                    adminController.UpdateStockQuantity();
                    cout << "\nDavam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();
                    break;


                case 7:
                    FileManager::ClearScreen();
                    adminController.DisplayStock();
                    cout << "\nDavam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();

                    break;
                case 8:
                    FileManager::ClearScreen();
                    adminController.DisplayStatistics();
                    cout << "\nDavam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();

                    break;
                case 9:
                    FileManager::ClearScreen();
                    adminController.DisplayBudget();
                    cout << "\nDavam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();
                    break;
                case 10:
                    FileManager::ClearScreen();
                    adminController.AddToBudget();
                    cout << "\nDavam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();
                    break;
                case 11:
                    FileManager::ClearScreen();
                    adminController.DeductFromBudget();
                    cout << "\nDavam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();
                    break;

                case 12:
                    FileManager::ClearScreen();
                    adminController.DisplayMenu();
                    cout << "\nDavam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();
                    break;
                case 0:
                    cout << "Əsas Menyuya qayıdılır..." << endl;
                    break;
                default:
                    cout << "Yanlış seçim! Yenidən cəhd edin." << endl;

                }

            } while (choice != 0);
        }

        // User menyusu
        void ShowUserSubmenu() {
            UserController userController(userData, menuData, stockData, statistics, restaurant);

            int choice;
            do {
                FileManager::ClearScreen();
                cout << "\n========= User Qeydiyyat Paneli =========" << endl;
                cout << "1. Giriş" << endl;
                cout << "2. Qeydiyyatdan Keç" << endl;
                cout << "0. Əsas Menyuya Qayıt" << endl;
                cout << "Seçiminizi edin: ";
                cin >> choice;

                switch (choice) {
                case 1:
                    if (userController.AuthenticateUser()) {
                        FileManager::ClearScreen();
                        ShowUserMenu();

                    }
                    else {
                        cout << "Giriş uğursuz oldu." << endl;
                        cout << "'Entere' basaraq geri qayıdın.";
                        cin.ignore();
                        cin.get();
                    }
                    break;
                case 2:
                    FileManager::ClearScreen();
                    userController.RegisterUser();
                    break;
                case 0:
                    cout << "Əsas Menyuya qayıdılır..." << endl;
                    break;
                default:
                    cout << "Yanlış seçim! Yenidən cəhd edin." << endl;
                }
            } while (choice != 0);
        }
        void ShowUserMenu() {
            UserController userController(userData, menuData, stockData, statistics, restaurant);

            int choice;
            do {
                FileManager::ClearScreen();
                cout << "\n========= User Panel =========" << endl;
                cout << "1. Yeməklərə və Səbətə Bax" << endl;
               /* cout << "2. Keçmiş Zakazlara Bax" << endl;*/
                cout << "0. Əsas Menyuya Qayıt" << endl;
                cout << "Seçiminizi edin: ";
                cin >> choice;

                switch (choice) {
                case 1:
                    
                    userController.DisplayMenuWithBasketOptions();
                    break;
                case 2:

                    userController.ViewPastOrders();
                    cout << "Davam etmək üçün 'Enter' düyməsinə basın...";
                    cin.ignore();
                    cin.get();
                    break;
                case 0:
                    cout << "Əsas Menyuya qayıdılır..." << endl;
                    break;
                default:
                    cout << "Yanlış seçim! Yenidən cəhd edin." << endl;
                }
            } while (choice != 0);
        }



        
        Menu GetMenuInput() {
            string name;
            vector<Stock> ingredients;
            double price;
            int ingredientCount;

            cout << "Yemək adı: ";
            cin.ignore();
            getline(cin, name);

            cout << "Inqrediyentlərin sayı: ";
            cin >> ingredientCount;

            for (int i = 0; i < ingredientCount; i++) {
                int id;
                cout << "Inqrediyent ID-sini daxil edin: ";
                cin >> id;

                try {
                    Stock ingredient = stockData->GetItems().at(id - 1);
                    ingredients.push_back(ingredient);
                }
                catch (const out_of_range&) {
                    cout << "ID mövcud deyil. Yenidən cəhd edin." << endl;
                    i--;
                }
            }

            cout << "Yeməyin qiyməti: ";
            cin >> price;

            return Menu(name, ingredients, price);
        }

        
        Stock GetStockInput() {
            string name;
            double pricePerUnit;
            int quantity;

            cout << "Inqrediyent adı: ";
            cin.ignore();
            getline(cin, name);

            cout << "Bir kq üçün qiymət: ";
            cin >> pricePerUnit;

            cout << "Sayı: ";
            cin >> quantity;

            return Stock(name, pricePerUnit, quantity);
        }
    };

}