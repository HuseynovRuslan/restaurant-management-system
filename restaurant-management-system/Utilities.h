#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Models.h"
#include <ctime>

using namespace std;
using namespace Models;

namespace FileManager {

  
    void ClearScreen() {

        system("cls"); 

    }

  
    class Logger {
    public:
        static string logFileName;

       
        static void LogMessage(const string& message) {
            ofstream fout(logFileName, ios::app); 
            if (fout.is_open()) {
                fout << GetCurrentTime() << ": " << message << endl;
                fout.close(); 
            }
            else {
                cerr << "Loq faylı açıla bilmir!" << endl;
            }
        }

    private:
       
        static string GetCurrentTime() {
            time_t now = time(0);
            struct tm timeStruct;
            localtime_s(&timeStruct, &now);

            char buf[80];
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeStruct); 
            return string(buf);
        }
    };

    
    string Logger::logFileName = "Logs.txt";

   
    class FileHandler {
    public:
        static string stockFileName;
        static string userFileName; 

       
static void SaveStockData(const DataSet<Stock>& stockData) {
    ofstream fout(stockFileName, ios::trunc); 
    if (fout.is_open()) {
        for (const auto& stock : stockData.GetItems()) {
            fout << stock.GetName() << " "
                 << stock.GetPricePerUnit() << " "
                 << stock.GetQuantity() << endl; 
        }
        fout.close(); 
        Logger::LogMessage("Sklad məlumatları fayla yazıldı.");
    }
    else {
        cerr << "Sklad faylı açıla bilmir!" << endl;
    }
}


        
        static DataSet<Stock> LoadStockData() {
            DataSet<Stock> stockData;
            ifstream fin(stockFileName); 
            if (fin.is_open()) {
                string name;
                double pricePerUnit;
                int quantity;

                while (fin >> name >> pricePerUnit >> quantity) {
                    Stock stock(name, pricePerUnit, quantity);
                    stockData.Add(stock); 
                }
                fin.close(); 
                Logger::LogMessage("Sklad məlumatları oxundu.");
            }
            else {
                cerr << "Sklad faylı açıla bilmir!" << endl;
            }
            return stockData;
        }

       
        static void SaveUserData(const DataSet<User>& userData) {
            ofstream fout(userFileName, ios::trunc); 
            if (fout.is_open()) {
                for (const auto& user : userData.GetItems()) {
                    fout << user.GetUsername() << "|"
                        << user.GetPassword() << endl;
                }
                fout.close();
                Logger::LogMessage("İstifadəçi məlumatları fayla yazıldı.");
            }
            else {
                cerr << "User faylı açıla bilmir!" << endl;
            }
        }

        
        static DataSet<User> LoadUserData() {
            DataSet<User> userData;
            ifstream fin(userFileName); 
            if (fin.is_open()) {
                string line;
                while (getline(fin, line)) {
                    string username, password;

                    size_t pos = line.find("|");
                    username = line.substr(0, pos);
                    password = line.substr(pos + 1);

                    userData.Add(User(username, password)); 
                }
                fin.close();
                Logger::LogMessage("İstifadəçi məlumatları fayldan oxundu.");
            }
            else {
                cerr << "User faylı açıla bilmir!" << endl;
            }
            return userData;
        }

        static void SaveMenuData(const DataSet<Menu>& menuData) {
            ofstream fout("Menu.txt", ios::trunc); 
            if (fout.is_open()) {
                for (const auto& menu : menuData.GetItems()) {
                    fout << menu.GetName() << "|"
                        << menu.GetPrice() << "|"
                        << menu.GetDescription() << "|";

               
                    for (const auto& ingredient : menu.GetIngredients()) {
                        fout << ingredient.GetName() << ","
                            << ingredient.GetQuantity() << ","
                            << ingredient.GetPricePerUnit() << ";";
                    }
                    fout << endl; 
                }
                fout.close();
                Logger::LogMessage("Menu məlumatları fayla yazıldı.");
            }
            else {
                cerr << "Menu faylı açıla bilmir!" << endl;
            }
        }



        static void SaveBudget(double budget) {
            ofstream fout("Budget.txt", ios::trunc); 
            if (fout.is_open()) {
                fout << budget; 
                fout.close();
                Logger::LogMessage("Büdcə fayla yazıldı: ₼" + to_string(budget));
            }
            else {
                cerr << "Budget.txt faylı açıla bilmir!" << endl;
            }
        }
        static double LoadBudget() {
            ifstream fin("Budget.txt");
            double budget = 0.0;
            if (fin.is_open()) {
                fin >> budget; 
                fin.close();
                Logger::LogMessage("Büdcə fayldan oxundu: ₼" + to_string(budget));
            }
            else {
                cerr << "Budget.txt faylı açıla bilmir! Default olaraq ₼0.0 təyin olunur." << endl;
            }
            return budget;
        }



        static DataSet<Menu> LoadMenuData() {
            DataSet<Menu> menuData;
            ifstream fin("Menu.txt");
            if (fin.is_open()) {
                string line;
                while (getline(fin, line)) {
                    string name, description, ingredientsPart;
                    double price;

                    
                    size_t pos1 = line.find("|");
                    size_t pos2 = line.find("|", pos1 + 1);
                    size_t pos3 = line.find("|", pos2 + 1);

                    name = line.substr(0, pos1); 
                    price = stod(line.substr(pos1 + 1, pos2 - pos1 - 1));
                    description = line.substr(pos2 + 1, pos3 - pos2 - 1);
                    ingredientsPart = line.substr(pos3 + 1); 

                    
                    vector<Stock> ingredients;
                    size_t start = 0, end;
                    while ((end = ingredientsPart.find(";", start)) != string::npos) {
                        string ingredientStr = ingredientsPart.substr(start, end - start);
                        size_t comma1 = ingredientStr.find(",");
                        size_t comma2 = ingredientStr.find(",", comma1 + 1);

                        if (comma1 != string::npos && comma2 != string::npos) {
                            string ingredientName = ingredientStr.substr(0, comma1);
                            double ingredientQuantity = stod(ingredientStr.substr(comma1 + 1, comma2 - comma1 - 1));
                            double ingredientPricePerUnit = stod(ingredientStr.substr(comma2 + 1));
                            ingredients.emplace_back(ingredientName, ingredientPricePerUnit, ingredientQuantity);
                        }

                        start = end + 1; 
                    }

                    
                    menuData.Add(Menu(name, ingredients, price, description));
                }
                fin.close();
                Logger::LogMessage("Menu məlumatları fayldan oxundu.");
            }
            else {
                cerr << "Menu faylı açıla bilmir!" << endl;
            }
            return menuData;
        }

        
        static void SaveBasketData(const vector<pair<Menu, int>>& basket) {
            ofstream fout("Basket.txt", ios::trunc);
            if (fout.is_open()) {
                for (const auto& item : basket) {
                    fout << item.first.GetName() << "|"
                        << item.first.GetPrice() << "|"
                        << item.second << "|";

                   
                    for (const auto& ingredient : item.first.GetIngredients()) {
                        fout << ingredient.GetName() << ","
                            << ingredient.GetQuantity() << ","
                            << ingredient.GetPricePerUnit() << ";";
                    }
                    fout << endl; 
                }
                fout.close();
                Logger::LogMessage("Səbət məlumatları Basket.txt faylına yazıldı.");
            }
            else {
                cerr << "Basket.txt faylı açıla bilmir!" << endl;
            }
        }

        
        static vector<pair<Menu, int>> LoadBasketData(const DataSet<Menu>& menuData) {
            vector<pair<Menu, int>> basket;
            ifstream fin("Basket.txt");
            if (fin.is_open()) {
                string line;
                while (getline(fin, line)) {
                    string name, ingredientsPart;
                    double price;
                    int quantity;

                    
                    size_t pos1 = line.find("|");
                    size_t pos2 = line.find("|", pos1 + 1);
                    size_t pos3 = line.find("|", pos2 + 1);

                    name = line.substr(0, pos1); 
                    price = stod(line.substr(pos1 + 1, pos2 - pos1 - 1));
                    quantity = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1)); 
                    ingredientsPart = line.substr(pos3 + 1);

                    
                    auto it = find_if(menuData.GetItems().begin(), menuData.GetItems().end(),
                        [&name](const Menu& menu) { return menu.GetName() == name; });

                    if (it != menuData.GetItems().end()) {
                        basket.emplace_back(*it, quantity); 
                    }
                }
                fin.close();
                Logger::LogMessage("Səbət məlumatları Basket.txt faylından oxundu.");
            }
            else {
                cerr << "Basket.txt faylı açıla bilmir!" << endl;
            }
            return basket;
        }

       
    };

    
    string FileHandler::stockFileName = "Stock.txt";
    string FileHandler::userFileName = "User.txt";
    static string statisticsFileName = "Statistics.txt";

}