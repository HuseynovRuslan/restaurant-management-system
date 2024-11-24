#pragma once
#include <iostream>
#include <string>

using namespace std;

namespace Exceptions {

    class BaseException {
    protected:
        string message;
        string file;
        int line;
    public:
        BaseException(const string& message, const string& file, int line)
            : message(message), file(file), line(line) {}

       //Catchlar
        virtual void PrintMessage() const {
            cout << "===== Xəta =====" << endl;
            cout << "Mesaj: " << message << endl;
            cout << "Fayl: " << file << endl;
            cout << "Sətir: " << line << endl;
        }

        virtual string GetMessage() const {
            return "Xəta: " + message + " [" + file + ":" + to_string(line) + "]";
        }
    };

    class InvalidInputException : public BaseException {
    public:
        InvalidInputException(const string& message, const string& file, int line)
            : BaseException(message, file, line) {}

        void PrintMessage() const override {
            cout << "===== Yanlış Giriş =====" << endl;
            BaseException::PrintMessage();
        }
    };

    class OutOfStockException : public BaseException {
    public:
        OutOfStockException(const string& message, const string& file, int line)
            : BaseException(message, file, line) {}

        void PrintMessage() const override {
            cout << "===== Stokda Yoxdur =====" << endl;
            BaseException::PrintMessage();
        }
    };

    class FileNotFoundException : public BaseException {
    public:
        FileNotFoundException(const string& message, const string& file, int line)
            : BaseException(message, file, line) {}

        void PrintMessage() const override {
            cout << "===== Fayl Tapılmadı =====" << endl;
            BaseException::PrintMessage();
        }
    };

    class NotEnoughBudgetException : public BaseException {
    public:
        NotEnoughBudgetException(const string& message, const string& file, int line)
            : BaseException(message, file, line) {}

        void PrintMessage() const override {
            cout << "===== Büdcə Kifayət Deyil =====" << endl;
            BaseException::PrintMessage();
        }
    };
}
