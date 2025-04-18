#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Coffee {
public:
    virtual int getPrice() = 0;
    virtual string getRecipe() = 0;
    virtual ~Coffee() {}
};

class Espresso : public Coffee {
    int price;
    string recipe;
public:
    Espresso(int price, string recipe) : price(price), recipe(recipe) {}
    int getPrice() override {
        return price;
    }
    string getRecipe() override {
        return recipe;
    }
};

class Cappuccino : public Coffee {
    int price;
    string recipe;
public:
    Cappuccino(int price, string recipe) : price(price), recipe(recipe) {}
    int getPrice() override {
        return price;
    }
    string getRecipe() override {
        return recipe;
    }
};

class Payment {
public:
    virtual bool makePayment(int price) = 0;
    virtual ~Payment() {}
};

class CashPayment : public Payment {
public:
    bool makePayment(int price) override {
        cout << "Make your payment of " << price << " units.\n";
        cout << "Enter your money: ";
        int l;
        cin >> l;
        if (l < price) {
            cout << "Not enough money.\n";
            return false;
        }
        if (l > price) {
            cout << "Collect your change: " << (l - price) << " units.\n";
        }
        cout << "Payment completed.\n";
        return true;
    }
};

class CreditCard : public Payment {
public:
    bool makePayment(int price) override {
        cout << "Scan your card\n";
        cout << "Enter your pin: ";
        int pin;
        cin >> pin;
        cout << "Payment of " << price << " units completed using card.\n";
        return true;
    }
};

class CoffeeFactory {
public:
    static Coffee* addCoffee(string name, int price, string recipe) {
        if (name == "Espresso") return new Espresso(price, recipe);
        else return new Cappuccino(price, recipe);
    }
};

class CoffeeVendingMachine {
    vector<Coffee*> coffee;
    CoffeeVendingMachine() {}
    static CoffeeVendingMachine* instance;

public:
    static CoffeeVendingMachine* getInstance() {
        if (instance == nullptr) instance = new CoffeeVendingMachine();
        return instance;
    }

    void addCoffee(string name, int price, string recipe) {
        Coffee* newCoffee = CoffeeFactory::addCoffee(name, price, recipe);
        coffee.push_back(newCoffee);
        cout << "Coffee is added.\n";
    }

    void display() {
        int index = 1;
        for (auto it : coffee) {
            cout << index << ") " << it->getRecipe() << " - Price: " << it->getPrice() << endl;
            index++;
        }
    }

    void buyCoffee() {
        for (int i = 0; i < coffee.size(); ++i) {
            Coffee* c = coffee[i];
            cout << i + 1 << ") " << c->getRecipe() << " - Price: " << c->getPrice() << endl;
            cout << "Do you want to buy this coffee?\n1. Yes\n2. No\nChoice: ";
            int num;
            cin >> num;
            if (num == 1) {
                cout << "Select Payment Method\n1. Card\n2. Cash\nChoice: ";
                int num1;
                cin >> num1;

                Payment* p = nullptr;
                if (num1 == 1)  p = new CreditCard();
                else if (num1 == 2)  p = new CashPayment();
                else {
                    cout << "Invalid payment method.\n";
                    return;
                }

                if (p->makePayment(c->getPrice())) {
                    cout << "Dispensing your coffee...\n";
                    delete c;
                    coffee.erase(coffee.begin() + i);
                    delete p;
                    return;
                } else {
                    cout << "Payment failed. Try again.\n";
                }

                delete p;  
            }
        }
    }

    ~CoffeeVendingMachine() {
        for (auto c : coffee) delete c;
    }
};

CoffeeVendingMachine* CoffeeVendingMachine::instance = nullptr;

int main() {
    CoffeeVendingMachine* machine = CoffeeVendingMachine::getInstance();
    machine->addCoffee("Espresso", 100, "Espresso: Hot water + coffee beans");
    machine->addCoffee("Cappuccino", 150, "Cappuccino: Espresso + steamed milk + foam");

    machine->display();
    machine->buyCoffee();
    machine->buyCoffee();
    machine->buyCoffee(); 

    return 0;
}
