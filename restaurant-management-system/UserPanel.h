#pragma once
#include "Entities.h"
#include "Statistic.h"
#include "Helpers.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace Entities;
using namespace Statistics;
using namespace FileManager;

namespace UserPanel {

    class UserController {
        DataSet<User>* userData;        
        DataSet<Menu>* menuData;        
        DataSet<Stock>* stockData;      
        DataSet<Order>* orderData;      
        Statistic* statistics;          
        Restaurant* restaurant;         
        vector<pair<Menu, int>> basket; 

    public:
        UserController(DataSet<User>* userData, DataSet<Menu>* menuData, DataSet<Stock>* stockData,
            Statistic* statistics, Restaurant* restaurant)
            : userData(userData), menuData(menuData), stockData(stockData), statistics(statistics), restaurant(restaurant) {}
        bool AuthenticateUser() {
            string inputUsername, inputPassword;
            FileManager::ClearScreen();
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

           
            for (const auto& user : userData->GetItems()) {
                if (user.GetUsername() == inputUsername) {
                    
                    if (user.GetPassword() == inputPassword) {
                        cout << "Giriş uğurlu!" << endl;
                        cout << "'Enter' düyməsinə basın.";
                        Logger::LogMessage("İstifadəçi uğurla daxil oldu: " + inputUsername);
                        return true;
                    }
                    else {
                        cout << "Xəta: Yanlış şifrə!" << endl;
                        Logger::LogMessage("Yanlış şifrə ilə giriş cəhdi: " + inputUsername);
                        return false;
                    }
                }
            }

           
            cout << "Xəta: İstifadəçi adı mövcud deyil!" << endl;
            Logger::LogMessage("Mövcud olmayan istifadəçi adı ilə giriş cəhdi: " + inputUsername);
            return false;
        }

        
        bool IsUsernameExists(const string& username) {
            for (const auto& user : userData->GetItems()) {
                if (user.GetUsername() == username) {
                    return true; 
                }
            }
            return false; 
        }

        void RegisterUser() {
            string username, password;

            cout << "Yeni istifadəçi adı: ";
            cin.ignore();
            getline(cin, username);

            
            if (username.empty()) {
                cout << "Xəta: İstifadəçi adı boş ola bilməz!" << endl;
                system("pause");
                FileManager::ClearScreen();
                RegisterUser();
                return;
            }

           
            if (IsUsernameExists(username)) { 
                cout << "Xəta: Bu istifadəçi adı artıq mövcuddur!" << endl;
                system("pause");
                FileManager::ClearScreen();
                RegisterUser();
                return;
            }

           
            if (username.length() < 3) {
                cout << "Xəta: İstifadəçi adı ən azı 3 simvol olmalıdır!" << endl;
                system("pause");
                FileManager::ClearScreen();
                RegisterUser();
                return;
            }

            cout << "Şifrə: ";
            getline(cin, password);

            
            if (password.empty()) {
                cout << "Xəta: Şifrə boş ola bilməz!" << endl;
                system("pause");
                FileManager::ClearScreen();
                RegisterUser();
                return;
            }

           
            if (password.length() < 6) {
                cout << "Xəta: Şifrə ən azı 6 simvol olmalıdır!" << endl;
                system("pause");
                FileManager::ClearScreen();
                RegisterUser();
                return;
            }

            
            User newUser(username, password);
            userData->Add(newUser);
            Logger::LogMessage("Yeni istifadəçi qeydiyyatdan keçdi: " + username);
            cout << "Qeydiyyat uğurla tamamlandı!'Enter' düyməsinə basın." << endl;
            cin.get(); 
        }


       
        // Menyu göstərir Userin
        void DisplayMenuWithBasketOptions() {
            FileManager::ClearScreen();

            cout << "========= Mövcud Yeməklər =========" << endl;

            if (menuData->GetItems().empty()) {
                cout << "Menyu boşdur!" << endl;
                cout << "Davam etmək üçün 'Enter' düyməsinə basın...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cin.get();
                return;
            }

            int index = 1;
            for (const auto& menu : menuData->GetItems()) {
                cout << index++ << ". Ad: " << menu.GetName()
                    << ", Qiymət: ₼" << menu.GetPrice() << endl;
                cout << "   Təsvir: " << menu.GetDescription() << endl;
                cout << "   Tərkibi: ";
                for (const auto& ingredient : menu.GetIngredients()) {
                    cout << ingredient.GetName() << " (" << ingredient.GetQuantity() << " kq), ";
                }
                cout << endl << "-------------------------------------" << endl;
            }

            cout << "\n1. Səbətə Yemək Əlavə Et" << endl;
            cout << "2. Səbətə Bax və Təsdiqlə" << endl;
            cout << "0. Əsas Menyuya Qayıt" << endl;
            cout << "Seçiminizi edin: ";

            int choice;
            cin >> choice;

            switch (choice) {
            case 1: {
                
                int menuId;
                cout << "Səbətə əlavə etmək üçün yeməyin ID-sini daxil edin: ";
                cin >> menuId;

                if (menuId <= 0 || menuId > menuData->GetItems().size()) {
                    cout << "Yanlış ID daxil edildi!" << endl;
                    break;
                }

                Menu selectedMenu = menuData->GetItems().at(menuId - 1);
                auto it = find_if(basket.begin(), basket.end(), [&selectedMenu](const pair<Menu, int>& item) {
                    return item.first.GetName() == selectedMenu.GetName();
                    });

                if (it != basket.end()) {
                    it->second++; 
                    cout << "Eyni yemək səbətdə mövcuddur. Miqdar artırıldı." << endl;
                }
                else {
                    basket.push_back({ selectedMenu, 1 }); 
                    cout << "Yemək səbətə əlavə edildi!" << endl;
                }
                break;
            }

            case 2: {
                
                if (basket.empty()) {
                    cout << "Səbət boşdur!" << endl;
                    break;
                }

                cout << "========= Səbətiniz =========" << endl;
                double totalPrice = 0.0;
                for (size_t i = 0; i < basket.size(); ++i) {
                    cout << i + 1 << ". " << basket[i].first.GetName()
                        << " (Qiymət: ₼" << basket[i].first.GetPrice()
                        << ", Miqdar: " << basket[i].second << ")" << endl;
                    totalPrice += basket[i].first.GetPrice() * basket[i].second;
                }

                cout << "Ümumi qiymət: ₼" << totalPrice << endl;

                char confirmChoice;
                cout << "Sifarişi təsdiqləmək istəyirsiniz? (y/n): ";
                cin >> confirmChoice;

                if (confirmChoice == 'y' || confirmChoice == 'Y') {
                    
                    restaurant->AddToBudget(totalPrice); 
                    statistics->AddIncome(totalPrice);   
                    basket.clear();                      
                    cout << "Sifariş təsdiqləndi! Ümumi məbləğ: ₼" << totalPrice << endl;
                }
                else {
                    cout << "Sifariş təsdiqlənmədi." << endl;
                }
                break;
            }

            case 0:
                
                cout << "Əsas Menyuya qayıdılır..." << endl;
                break;

            default:
                cout << "Yanlış seçim! Yenidən cəhd edin." << endl;
                break;
            }

            cout << "Davam etmək üçün 'Enter' düyməsinə basın...";
            cin.ignore(); 
            cin.get();
        }


        
        void ViewPastOrders() const {
        FileManager::ClearScreen();
        cout << "========= Keçmiş Zakazlar =========" << endl;

        if (orderData->GetItems().empty()) {
            cout << "Keçmiş zakazlar yoxdur!" << endl;
            cout << "Davam etmək üçün 'Enter' düyməsinə basın...";
            cin.ignore();
            cin.get();
            return;
        }

        int orderId = 1;
        for (const auto& order : orderData->GetItems()) {
            cout << "Zakaz ID: " << order.GetId() << endl;
            cout << "Ümumi məbləğ: ₼" << order.GetTotalPrice() << endl;
            cout << "Məhsullar:" << endl;

            for (const auto& item : order.GetItems()) {
                cout << " - " << item.GetName() << " (Qiymət: ₼" << item.GetPrice() << ")" << endl;
            }
            cout << "-------------------------------------" << endl;
        }

        cout << "Davam etmək üçün 'Enter' düyməsinə basın...";
        cin.ignore();
        cin.get();
    }



        void AddFoodToBasket() {
            int menuId;
            cout << "Səbətə əlavə etmək üçün yeməyin ID-sini daxil edin: ";
            cin >> menuId;

            try {
                Menu selectedMenu = menuData->GetItems().at(menuId - 1); 

               
                auto it = find_if(basket.begin(), basket.end(), [&selectedMenu](const pair<Menu, int>& item) {
                    return item.first.GetName() == selectedMenu.GetName() && item.first.GetPrice() == selectedMenu.GetPrice();
                    });

                if (it != basket.end()) {
                    it->second++; 
                    cout << "Eyni yemək tapıldı. Miqdar artırıldı!" << endl;
                }
                else {
                    basket.push_back({ selectedMenu, 1 }); 
                    cout << "Yemək səbətə əlavə edildi!" << endl;
                }

                Logger::LogMessage("Səbətə yemək əlavə edildi: " + selectedMenu.GetName());
            }
            catch (const out_of_range&) {
                cout << "Yanlış ID! Yenidən cəhd edin." << endl;
            }

            cout << "Davam etmək üçün 'Enter' düyməsinə basın...";
            cin.ignore();
            cin.get();
        }



         void ConfirmOrder(double totalPrice) {
        restaurant->AddToBudget(totalPrice);
        statistics->AddIncome(totalPrice);
        
     
        vector<Menu> orderedItems;
        for (const auto& basketItem : basket) {
            for (int i = 0; i < basketItem.second; ++i) { 
                orderedItems.push_back(basketItem.first);
            }
        }
        Order newOrder(orderData->GetItems().size() + 1, orderedItems); 
        orderData->Add(newOrder); 
        
        Logger::LogMessage("Sifariş təsdiqləndi. Ümumi məbləğ: ₼" + to_string(totalPrice));
        cout << "Sifariş təsdiqləndi! Ümumi məbləğ: ₼" << totalPrice << endl;

        basket.clear(); 
    }

        void ViewAndConfirmBasket() {
            if (basket.empty()) {
                cout << "Səbət boşdur!" << endl;
                cout << "Davam etmək üçün 'Enter' düyməsinə basın...";
                cin.ignore();
                cin.get();
                return;
            }

            cout << "========= Səbətiniz =========" << endl;
            double totalPrice = 0.0;

            for (size_t i = 0; i < basket.size(); ++i) {
                cout << i + 1 << ". " << basket[i].first.GetName()
                    << " (Qiymət: ₼" << basket[i].first.GetPrice()
                    << ", Miqdar: " << basket[i].second << ")" << endl;
                totalPrice += basket[i].first.GetPrice() * basket[i].second;
            }

            cout << "Ümumi qiymət: ₼" << totalPrice << endl;

            char choice;
            cout << "Sifarişi təsdiqləmək istəyirsiniz? (y/n): ";
            cin >> choice;

            if (choice == 'y' || choice == 'Y') {
                ConfirmOrder(totalPrice);
                basket.clear();
            }
            else {
                cout << "Sifariş təsdiqlənmədi." << endl;
            }

            cout << "Davam etmək üçün 'Enter' düyməsinə basın...";
            cin.ignore();
            cin.get();
        }


    };

}
