#include <bits/stdc++.h>
using namespace std;

// ========================= Product Class =========================
class Product {
    string name;
    string company;
    int price;

public:
    Product(string name, string company, int price) : name(name), company(company), price(price) {}
    string getName() { return name; }
    string getCompany() { return company; }
    int getPrice() { return price; }
};

// ========================= User Class =========================
class User {
    string name;
    string email;
    vector<Product*> history;
    vector<Product*> cart;

public:
    User(string name, string email) : name(name), email(email) {}
    string getName() { return name; }
    void addToHistory(Product* p) { history.push_back(p); }
    void addToCart(Product* p) { cart.push_back(p); }
};

// ========================= Factory Pattern =========================
class ProductFactory {
public:
    static Product* createProduct(string name, string company, int price) {
        return new Product(name, company, price);
    }
};

class UserFactory {
public:
    static User* createUser(string name, string email) {
        return new User(name, email);
    }
};

// ========================= Strategy Pattern =========================
class SearchStrategy {
public:
    virtual void search(vector<Product*>& products, string query) = 0;
};

class NameSearch : public SearchStrategy {
public:
    void search(vector<Product*>& products, string query) override {
        for (auto p : products) {
            if (p->getName() == query) {
                cout << p->getName() << " " << p->getCompany() << " " << p->getPrice() << endl;
            }
        }
    }
};
class Payment{
    public:
    virtual void makePayment()=0;
};
class MakePaymentByCard:public Payment{
    public:
    void makePayment() override{
        cout<<"We make a payment by Card"<<endl;
        
    }

};
class MakePaymentByGpay:public Payment{
    public:
    void makePayment() override{
        cout<<"We make a payment by Gpay"<<endl;
    }

};

// ========================= Singleton Pattern =========================
class OnlineShoppingService {
    static OnlineShoppingService* instance;
    vector<Product*> products;
    vector<User*> users;
   // Payment*p;
    OnlineShoppingService() {}

public:
    static OnlineShoppingService* getInstance() {
        if (!instance)
            instance = new OnlineShoppingService();
        return instance;
    }

    void addProduct(string name, string company, int price) {
        Product* p = ProductFactory::createProduct(name, company, price);
        products.push_back(p);
        cout << "Product added." << endl;
    }

    void registerUser(string name, string email) {
        User* u = UserFactory::createUser(name, email);
        users.push_back(u);
        cout << "User registered." << endl;
    }

    void searchProduct(SearchStrategy* strategy, string query) {
        strategy->search(products, query);
    }

    void addToCart(string userName, string productName) {
        User* u = findUser(userName);
        Product* p = findProduct(productName);
        if (!u || !p) return;
        u->addToCart(p);
        cout << "Added to cart." << endl;
    }

    void buyProduct(string userName, string productName,Payment*py) {
        User* u = findUser(userName);
        Product* p = findProduct(productName);
        if (!u || !p) return;
        py->makePayment();
        u->addToHistory(p);
        cout << "Product purchased." << endl;
    }

private:
    User* findUser(string name) {
        for (auto u : users)
            if (u->getName() == name)
                return u;
        cout << "User not found." << endl;
        return nullptr;
    }

    Product* findProduct(string name) {
        for (auto p : products)
            if (p->getName() == name)
                return p;
        cout << "Product not found." << endl;
        return nullptr;
    }
};

OnlineShoppingService* OnlineShoppingService::instance = nullptr;

// ========================= Main Function =========================
int main() {
    OnlineShoppingService* service = OnlineShoppingService::getInstance();

    service->registerUser("Alice", "alice@example.com");
    service->addProduct("Phone", "Samsung", 500);
    service->addProduct("Laptop", "Dell", 1200);
    Payment*py=new MakePaymentByGpay();

    NameSearch nameSearch;
    service->searchProduct(&nameSearch, "Phone");

    service->addToCart("Alice", "Phone");
    service->buyProduct("Alice", "Phone",py);

    return 0;
}
