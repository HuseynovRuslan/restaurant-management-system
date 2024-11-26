#pragma once
#include "Models.h"
#include "Statistic.h"
#include "Utilities.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace Models;
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
            : userData(userData), menuData(menuData), stockData(stockData), statistics(statistics), restaurant(restaurant) {
            
            *userData = FileHandler::LoadUserData();
            *menuData = FileHandler::LoadMenuData(); 


        }
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
            FileHandler::SaveUserData(*userData);
            Logger::LogMessage("Yeni istifadəçi qeydiyyatdan keçdi: " + username);
            cout << "Qeydiyyat uğurla tamamlandı!'Enter' düyməsinə basın." << endl;
            cin.get(); 
        }


       
        // Menyu göstərir Userin
        void 
            MenuWithBasketOptions() {
            *menuData = FileHandler::LoadMenuData();
            *stockData = FileHandler::LoadStockData();
            basket = FileHandler::LoadBasketData(*menuData);
            FileManager::ClearScreen();

            cout << "========= Menyu =========" << endl;

            if (menuData->GetItems().empty()) {
                cout << "Menyu boşdur!" << endl;
                cout << "Davam etmək üçün 'Enter' düyməsinə basın...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                return;
            }

            int index = 1;
            for (const auto& menu : menuData->GetItems()) {
                bool allIngredientsAvailable = true;

                
                for (const auto& ingredient : menu.GetIngredients()) {
                    auto it = find_if(stockData->GetItems().begin(), stockData->GetItems().end(),
                        [&ingredient](const Stock& stock) {
                            return stock.GetName() == ingredient.GetName() && stock.GetQuantity() >= ingredient.GetQuantity();
                        });

                   
                    if (it == stockData->GetItems().end() || it->GetQuantity() < ingredient.GetQuantity()) {
                        allIngredientsAvailable = false;
                        break;
                    }
                }

                
                cout << index++ << ". Ad: " << menu.GetName()
                    << ", Qiymət: ₼" << menu.GetPrice() << endl;
                cout << "   Təsvir: " << menu.GetDescription() << endl;
                cout << "   Tərkibi: ";
                for (const auto& ingredient : menu.GetIngredients()) {
                    cout << ingredient.GetName() << " (" << ingredient.GetQuantity() << " qr), ";
                }

                
                if (allIngredientsAvailable) {
                    cout << "\n   Bu yemək mövcuddur." << endl;
                }
                else {
                    cout << "\n   Bu yeməkdən yoxdur." << endl;
                }

                cout << endl << "-------------------------------------" << endl;
            }

            
            cout << "\n1. Səbətə Yemək Əlavə Et" << endl;
            cout << "2. Səbətə Bax və Təsdiqlə" << endl;
            cout << "0. Çıxış" << endl;

            int choice;
            cout << "Seçiminizi edin: ";
            cin >> choice;

            switch (choice) {
            case 1:
                AddFoodToBasket();
                FileHandler::SaveBasketData(basket);
                break;
            case 2:
                ViewAndConfirmBasket();
                break;
            case 0:
                cout << "Çıxış edilir..." << endl;
                break;
            default:
                cout << "Yanlış seçim etdiniz. Yenidən cəhd edin!" << endl;
                break;
            }
            cout << "Davam etmək üçün 'Enter' düyməsinə basın...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
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

                
                for (const auto& ingredient : selectedMenu.GetIngredients()) {
                    auto it = find_if(stockData->GetItems().begin(), stockData->GetItems().end(),
                        [&ingredient](const Stock& stock) {
                            return stock.GetName() == ingredient.GetName();
                        });

                    if (it == stockData->GetItems().end()) {
                      
                        
                        cout << "\nBu yemək hal-hazırda mövcud deyil.\n" << endl;
                        return;
                    }
                    
                    if (it->GetQuantity() < ingredient.GetQuantity()) {
                        cout << "\nBu yemək keçici olaraq təmin edilə bilmir.\n" << endl;
                        return;
                    }
                }

              
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

   
        }




        void ConfirmOrder(double totalPrice) {
          
            for (const auto& basketItem : basket) {
                for (const auto& ingredient : basketItem.first.GetIngredients()) {
                    auto it = find_if(stockData->GetItems().begin(), stockData->GetItems().end(),
                        [&ingredient](const Stock& stock) {
                            return stock.GetName() == ingredient.GetName();
                        });

                    if (it == stockData->GetItems().end() || it->GetQuantity() < ingredient.GetQuantity() * basketItem.second) {
                        cout << "Xəta: '" << ingredient.GetName() << "' üçün kifayət qədər inqrediyent yoxdur. Sifariş ləğv edildi." << endl;
                        Logger::LogMessage("Sifariş uğursuz oldu: kifayət qədər inqrediyent yoxdur.");
                        return;
                    }
                }
            }

       
            restaurant->AddToBudget(totalPrice);
            statistics->AddIncome(totalPrice);

            
            for (const auto& basketItem : basket) {
                for (const auto& ingredient : basketItem.first.GetIngredients()) {
                    auto it = find_if(stockData->GetItems().begin(), stockData->GetItems().end(),
                        [&ingredient](Stock& stock) {
                            return stock.GetName() == ingredient.GetName();
                        });

                    if (it != stockData->GetItems().end()) {
                        it->SetQuantity(it->GetQuantity() - ingredient.GetQuantity() * basketItem.second);
                        
                    }
                }
            }

          
            FileHandler::SaveStockData(*stockData);

            
            basket.clear();

            cout << "Sifariş təsdiqləndi! Büdcəyə ₼" << totalPrice << " əlavə edildi." << endl;
            Logger::LogMessage("Sifariş uğurla təsdiqləndi. Məbləğ: ₼" + to_string(totalPrice));
        }

        void ViewAndConfirmBasket() {
            if (basket.empty()) {
                cout << "Səbət boşdur!" << endl;
                cout << "Davam etmək üçün 'Enter' düyməsinə basın...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                return;
            }

            double totalPrice = 0.0;

            while (true) {
               
                cout << "========= Səbətiniz =========" << endl;

                totalPrice = 0.0;
                for (size_t i = 0; i < basket.size(); ++i) {
                    cout << i + 1 << ". " << basket[i].first.GetName()
                        << " (Qiymət: ₼" << basket[i].first.GetPrice()
                        << ", Miqdar: " << basket[i].second << ")" << endl;
                    totalPrice += basket[i].first.GetPrice() * basket[i].second;
                }

                cout << "Ümumi qiymət: ₼" << totalPrice << endl;

                
                cout << "\nSeçim edin:\n";
                cout << "1. Yeməyi səbətdən çıxarmaq\n";
                cout << "2. Sifarişi təsdiqləmək\n";
                cout << "0. Çıxış\n";

                int choice;
                cout << "Seçiminizi edin: ";
                cin >> choice;

                if (choice == 1) {
                    
                    int removeIndex;
                    cout << "Səbətdən çıxarmaq istədiyiniz yeməyin nömrəsini daxil edin: ";
                    cin >> removeIndex;

                    if (removeIndex > 0 && removeIndex <= basket.size()) {
                        cout << basket[removeIndex - 1].first.GetName() << " səbətdən çıxarıldı." << endl;
                        basket.erase(basket.begin() + removeIndex - 1);
                    }
                    else {
                        cout << "Yanlış nömrə! Yemək çıxarıla bilmədi." << endl;
                    }
                }
                else if (choice == 2) {
                    
                    char confirm;
                    cout << "Sifarişi təsdiqləmək istəyirsiniz? (y/n): ";
                    cin >> confirm;

                    if (confirm == 'y' || confirm == 'Y') {
                        ConfirmOrder(totalPrice);
                        return;  
                    }
                    else {
                        cout << "Sifariş təsdiqlənmədi." << endl;
                    }
                }
                else if (choice == 0) {
                    
                    cout << "Çıxış edilir..." << endl;
                    return;
                }
                else {
                    cout << "Yanlış seçim! Yenidən cəhd edin." << endl;
                }

                cout << "Davam etmək üçün 'Enter' düyməsinə basın...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }
        }



    };

}
