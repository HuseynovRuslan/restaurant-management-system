#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Entities.h"
#include <ctime>

using namespace std;
using namespace Entities;

namespace FileManager {

    // Ekranı təmizləmək üçün funksiyanı tərtib etdik
    void ClearScreen() {
#if defined(_WIN32)
        system("cls");  // Windows üçün
#else
        system("clear");  // Linux/MacOS üçün
#endif
    }

    // Logger sinifi: Proqramda hərəkətləri loqlamaq üçün
    class Logger {
    public:
        static string logFileName;

        // Loq mesajlarını fayla yazmaq
        static void LogMessage(const string& message) {
            ofstream fout(logFileName, ios::app); // Faylı əlavə etmək üçün açır
            if (fout.is_open()) {
                fout << GetCurrentTime() << ": " << message << endl;
                fout.close(); // Faylı bağlayır
            }
            else {
                cerr << "Loq faylı açıla bilmir!" << endl;
            }
        }

    private:
        // Cari tarixi almaq üçün yardımçı funksiya
        static string GetCurrentTime() {
            time_t now = time(0);
            struct tm timeStruct;
            localtime_s(&timeStruct, &now); // Tarixi yerli vaxtda alır

            char buf[80];
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeStruct); // Tarixi formatlayır
            return string(buf);
        }
    };

    // Loq faylının adı təyin olunur
    string Logger::logFileName = "Logs.txt";

    // Fayl idarəetmə sinifi
    class FileHandler {
    public:
        static string stockFileName;
        static string userFileName; // User.txt faylı üçün fayl adı

        // Sklad məlumatlarını fayla yazan metod
        static void SaveStockData(const DataSet<Stock>& stockData) {
            ofstream fout(stockFileName, ios::trunc); // Faylı təzədən yazmağa açır
            if (fout.is_open()) {
                for (const auto& stock : stockData.GetItems()) {
                    fout << stock.GetName() << " "
                        << stock.GetPricePerUnit() << " "
                        << stock.GetQuantity() << endl;
                }
                fout.close(); // Faylı bağlayır
                Logger::LogMessage("Sklad məlumatları saxlanıldı.");
            }
            else {
                cerr << "Sklad faylı açıla bilmir!" << endl;
            }
        }

        // Sklad məlumatlarını fayldan oxumaq üçün metod
        static DataSet<Stock> LoadStockData() {
            DataSet<Stock> stockData;
            ifstream fin(stockFileName); // Faylı oxumağa açır
            if (fin.is_open()) {
                string name;
                double pricePerUnit;
                int quantity;

                while (fin >> name >> pricePerUnit >> quantity) {
                    Stock stock(name, pricePerUnit, quantity); // Sklad obyektini yaradır
                    stockData.Add(stock); // Verilənlər dəstinə əlavə edir
                }
                fin.close(); // Faylı bağlayır
                Logger::LogMessage("Sklad məlumatları oxundu.");
            }
            else {
                cerr << "Sklad faylı açıla bilmir!" << endl;
            }
            return stockData;
        }

        // İstifadəçi məlumatlarını fayla yazmaq
        static void SaveUserData(const DataSet<User>& userData) {
            ofstream fout(userFileName, ios::trunc); // Faylı təzədən yazmaq üçün açır
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

        // İstifadəçi məlumatlarını fayldan oxumaq
        static DataSet<User> LoadUserData() {
            DataSet<User> userData;
            ifstream fin(userFileName); // Faylı oxumağa açır
            if (fin.is_open()) {
                string line;
                while (getline(fin, line)) {
                    string username, password;

                    size_t pos = line.find("|");
                    username = line.substr(0, pos);
                    password = line.substr(pos + 1);

                    userData.Add(User(username, password)); // İstifadəçi obyektini əlavə et
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
            ofstream fout("Menu.txt", ios::trunc); // Faylı təzədən yazmaq üçün açır
            if (fout.is_open()) {
                for (const auto& menu : menuData.GetItems()) {
                    fout << menu.GetName() << "|"
                        << menu.GetPrice() << "|"
                        << menu.GetDescription() << "|";

                    // Inqrediyentləri yazır
                    for (const auto& ingredient : menu.GetIngredients()) {
                        fout << ingredient.GetName() << ","
                            << ingredient.GetQuantity() << ","
                            << ingredient.GetPricePerUnit() << ";";
                    }
                    fout << endl; // Yeni sətr
                }
                fout.close();
                Logger::LogMessage("Menu məlumatları fayla yazıldı.");
            }
            else {
                cerr << "Menu faylı açıla bilmir!" << endl;
            }
        }



        static void SaveBudget(double budget) {
            ofstream fout("Budget.txt", ios::trunc); // Faylı təzədən yazmaq üçün açır
            if (fout.is_open()) {
                fout << budget; // Büdcə məlumatını yazır
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
                fin >> budget; // Fayldan büdcə məlumatını oxuyur
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

                    // Menyu məlumatlarını parçala
                    size_t pos1 = line.find("|");
                    size_t pos2 = line.find("|", pos1 + 1);
                    size_t pos3 = line.find("|", pos2 + 1);

                    name = line.substr(0, pos1); // Adı
                    price = stod(line.substr(pos1 + 1, pos2 - pos1 - 1)); // Qiyməti
                    description = line.substr(pos2 + 1, pos3 - pos2 - 1); // Təsviri
                    ingredientsPart = line.substr(pos3 + 1); // Tərkibi

                    // Inqrediyentləri oxuyur
                    vector<Stock> ingredients;
                    size_t start = 0, end;
                    while ((end = ingredientsPart.find(";", start)) != string::npos) {
                        string ingredientStr = ingredientsPart.substr(start, end - start);
                        size_t comma1 = ingredientStr.find(",");
                        size_t comma2 = ingredientStr.find(",", comma1 + 1);

                        // Inqrediyent məlumatlarını parçala
                        if (comma1 != string::npos && comma2 != string::npos) {
                            string ingredientName = ingredientStr.substr(0, comma1);
                            double ingredientQuantity = stod(ingredientStr.substr(comma1 + 1, comma2 - comma1 - 1));
                            double ingredientPricePerUnit = stod(ingredientStr.substr(comma2 + 1));
                            ingredients.emplace_back(ingredientName, ingredientPricePerUnit, ingredientQuantity);
                        }

                        start = end + 1; // Növbəti inqrediyentə keçir
                    }

                    // Menyu obyektini əlavə et
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


       
    };

    // Fayl adları
    string FileHandler::stockFileName = "Stock.txt";
    string FileHandler::userFileName = "User.txt";
    static string statisticsFileName = "Statistics.txt";

}