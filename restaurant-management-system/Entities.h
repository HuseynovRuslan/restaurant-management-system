#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

namespace Entities {

    // Admin sinfi
    class Admin {
        string username;
        string password;
    public:
        Admin() : username(""), password("") {}
        Admin(const string& username, const string& password) {
            SetUsername(username);
            SetPassword(password);
        }

        string GetUsername() const { return username; }
        void SetUsername(const string& username) { this->username = username; }

        string GetPassword() const { return password; }
        void SetPassword(const string& password) { this->password = password; }
    };

    // User sinfi
    class User {
        string username;
        string password;
    public:
        User() : username(""), password("") {}
        User(const string& username, const string& password) {
            SetUsername(username);
            SetPassword(password);
        }

        string GetUsername() const { return username; }
        void SetUsername(const string& username) { this->username = username; }

        string GetPassword() const { return password; }
        void SetPassword(const string& password) { this->password = password; }
    };

    // Restoran sinfi
    class Restaurant {
        string name;
        string address;
        double budget;

    public:
        Restaurant() : name(""), address(""), budget(0.0) {}
        Restaurant(const string& name, const string& address, double budget) {
            SetName(name);
            SetAddress(address);
            SetBudget(budget);
        }

        string GetName() const { return name; }
        void SetName(const string& name) { this->name = name; }

        string GetAddress() const { return address; }
        void SetAddress(const string& address) { this->address = address; }

        double GetBudget() const { return budget; }
        void SetBudget(double budget) { this->budget = budget; }

        
        void AddToBudget(double amount) { budget += amount; }

        
        void DeductFromBudget(double amount) {
            if (budget >= amount) {
                budget -= amount;
            }
            else {
                throw runtime_error("Kifayət qədər büdcə yoxdur!");
            }
        }
    };






    // Stock sinfi
    class Stock {
        string name;
        double pricePerUnit;
        int quantity;
    public:
        Stock() : name(""), pricePerUnit(0.0), quantity(0) {}
        Stock(const string& name, double pricePerUnit, int quantity) {
            SetName(name);
            SetPricePerUnit(pricePerUnit);
            SetQuantity(quantity);
        }

        string GetName() const { return name; }
        void SetName(const string& name) { this->name = name; }

        double GetPricePerUnit() const { return pricePerUnit; }
        void SetPricePerUnit(double pricePerUnit) { this->pricePerUnit = pricePerUnit; }

        int GetQuantity() const { return quantity; }
        void SetQuantity(int quantity) { this->quantity = quantity; }

        void AddQuantity(int quantity) { this->quantity += quantity; }
        void DeductQuantity(int quantity) {
            if (this->quantity >= quantity) this->quantity -= quantity;
            else throw runtime_error("Insufficient stock!");
        }
    };

    // Menu sinfi
    class Menu {
        string name;
        vector<Stock> ingredients;
        double price;
        string description; 

    public:
        Menu() : name(""), price(0.0), description("") {}
        Menu(const string& name, const vector<Stock>& ingredients, double price, const string& description = "")
            : name(name), ingredients(ingredients), price(price), description(description) {}

        string GetName() const { return name; }
        void SetName(const string& name) { this->name = name; }

        vector<Stock> GetIngredients() const { return ingredients; }
        void SetIngredients(const vector<Stock>& ingredients) { this->ingredients = ingredients; }

        double GetPrice() const { return price; }
        void SetPrice(double price) { this->price = price; }

        string GetDescription() const { return description; } 
        void SetDescription(const string& desc) { description = desc; } 
    };


    // Order sinfi
    class Order {
        int id;
        vector<Menu> items;
        double totalPrice;
    public:
        Order() : id(0), totalPrice(0.0) {}
        Order(int id, const vector<Menu>& items) : id(id), items(items) {
            CalculateTotalPrice();
        }

        int GetId() const { return id; }
        void SetId(int id) { this->id = id; }

        vector<Menu> GetItems() const { return items; }
        void AddItem(const Menu& item) {
            items.push_back(item);
            CalculateTotalPrice();
        }

        double GetTotalPrice() const { return totalPrice; }
    private:
        void CalculateTotalPrice() {
            totalPrice = 0.0;
            for (const auto& item : items) {
                totalPrice += item.GetPrice();
            }
        }
    };

    // Şablon sinif: DataSet<T>
    template<typename T>
    class DataSet {
        vector<T> items;
    public:
        void Add(const T& item) { items.push_back(item); }
        void Remove(const T& item) {
            items.erase(remove(items.begin(), items.end(), item), items.end());
        }

        // Referans qaytaran GetItems funksiyası
        vector<T>& GetItems() { return items; } // Referans qaytarır
        const vector<T>& GetItems() const { return items; } // Sabit referans qaytarır

        size_t GetCount() const { return items.size(); }
    };


}
