#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Entities.h"
//Komekci funksiyalar
using namespace std;
using namespace Entities;
namespace FileManager {
    // Ekrani temizlemek ucun
    void ClearScreen() {
#if defined(_WIN32)
        system("cls");
#else
        system("clear");
#endif
    }
}

namespace FileManager {
    //Proqramda hereketlerin loqlari
    class Logger {
    public:
        static string logFileName;

        // Loq mesajlarını fayla yazır
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

        
        static void SaveStockData(const DataSet<Stock>& stockData) {
            ofstream fout(stockFileName, ios::trunc);
            if (fout.is_open()) {
                for (const auto& stock : stockData.GetItems()) {
                    fout << stock.GetName() << " "
                        << stock.GetPricePerUnit() << " "
                        << stock.GetQuantity() << endl;
                }
                fout.close();
                Logger::LogMessage("Sklad məlumatları saxlanıldı.");
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
    };

    string FileHandler::stockFileName = "Stock.txt";

}
