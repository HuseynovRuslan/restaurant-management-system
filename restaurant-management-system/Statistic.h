#pragma once
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

namespace Statistics {
    //Tam deyil
    class Statistic {
    private:
        double dailyIncome;   
        double monthlyIncome;  
        double dailyExpense;   
        double monthlyExpense; 
        int dailyOrderCount;   
        int monthlyOrderCount; 

    public:
        Statistic()
            : dailyIncome(0.0), monthlyIncome(0.0),
            dailyExpense(0.0), monthlyExpense(0.0),
            dailyOrderCount(0), monthlyOrderCount(0) {}

        
        void AddIncome(double amount) {
            dailyIncome += amount;
            monthlyIncome += amount;
        }

        
        void AddExpense(double amount) {
            dailyExpense += amount;
            monthlyExpense += amount;
        }

        
        void IncrementOrderCount() {
            dailyOrderCount++;
            monthlyOrderCount++;
        }

        
        void ResetDailyStats() {
            dailyIncome = 0.0;
            dailyExpense = 0.0;
            dailyOrderCount = 0;
        }

        
        void ResetMonthlyStats() {
            monthlyIncome = 0.0;
            monthlyExpense = 0.0;
            monthlyOrderCount = 0;
        }

        void PrintDailyStats() const {
            cout << fixed << setprecision(2);
            cout << "========= Gündəlik Statistikalar =========" << endl;
            cout << "Gəlir: ₼" << dailyIncome << endl;
            cout << "Xərc: ₼" << dailyExpense << endl;
            cout << "Sifarişlərin sayı: " << dailyOrderCount << endl;
        }

       
        void PrintMonthlyStats() const {
            cout << fixed << setprecision(2);
            cout << "========= Aylıq Statistikalar =========" << endl;
            cout << "Gəlir: ₼" << monthlyIncome << endl;
            cout << "Xərc: ₼" << monthlyExpense << endl;
            cout << "Sifarişlərin sayı: " << monthlyOrderCount << endl;
        }

        string GetStatsLog() const {
            return "Gündəlik gəlir: ₼" + to_string(dailyIncome) +
                ", Gündəlik xərc: ₼" + to_string(dailyExpense) +
                ", Gündəlik sifarişlər: " + to_string(dailyOrderCount) +
                "\nAylıq gəlir: ₼" + to_string(monthlyIncome) +
                ", Aylıq xərc: ₼" + to_string(monthlyExpense) +
                ", Aylıq sifarişlər: " + to_string(monthlyOrderCount);
        }
    };

}
