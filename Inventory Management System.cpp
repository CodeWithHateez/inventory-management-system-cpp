#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h> // For Windows-specific functions like Sleep
#include <limits>
using namespace std;

// ANSI escape codes for colors
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

// Function to clear the console screen
void clearScreen() {
    system("cls"); // For Windows
    // system("clear"); // For Linux/Mac
}

// Function to display a loading animation
void loadingAnimation() {
    cout << GREEN << "Loading ";
    for (int i = 0; i < 3; i++) {
        cout << ".";
        Sleep(500); // Pause for 500 milliseconds
    }
    cout << RESET << endl;
}

// Function to display a bordered message
void displayMessage(const string& message) {
    cout << CYAN << "==============================================" << RESET << endl;
    cout << GREEN << message << RESET << endl;
    cout << CYAN << "==============================================" << RESET << endl;
}

// Function to display the main menu
void displayMenu() {
    clearScreen();
    cout << CYAN << "==============================================" << RESET << endl;
    cout << YELLOW << "           INVENTORY MANAGEMENT SYSTEM       " << RESET << endl;
    cout << CYAN << "==============================================" << RESET << endl;
    cout << BLUE << "1. Add Product" << RESET << endl;
    cout << BLUE << "2. Remove Product" << RESET << endl;
    cout << BLUE << "3. Find Product" << RESET << endl;
    cout << BLUE << "4. Display All Products" << RESET << endl;
    cout << BLUE << "5. Filter Products by Category" << RESET << endl;
    cout << BLUE << "6. Filter Products by Price Range" << RESET << endl;
    cout << BLUE << "7. Search Product by Name (Binary Search)" << RESET << endl;
    cout << BLUE << "8. Save Inventory to File" << RESET << endl;
    cout << BLUE << "9. Load Inventory from File" << RESET << endl;
    cout << BLUE << "10. Edit Product" << RESET << endl;
    cout << BLUE << "11. Sort Products by Price (Bubble Sort)" << RESET << endl;
    cout << BLUE << "12. Sort Products by Name (Merge Sort)" << RESET << endl;
    cout << RED << "0. Quit" << RESET << endl;
    cout << CYAN << "==============================================" << RESET << endl;
}

// Product class
class Product {
private:
    int id;
    string name;
    string category;
    double price;
    int quantity;

public:
    Product(int id = 0, string name = "", string category = "", double price = 0.0, int quantity = 0)
        : id(id), name(name), category(category), price(price), quantity(quantity) {}

    int getId() { return id; }
    string getName() { return name; }
    string getCategory() { return category; }
    double getPrice() { return price; }
    int getQuantity() { return quantity; }

    void setName(string name) { this->name = name; }
    void setCategory(string category) { this->category = category; }
    void setPrice(double price) { this->price = price; }
    void setQuantity(int quantity) { this->quantity = quantity; }

    void display() {
        cout << CYAN << "==============================================" << RESET << endl;
        cout << GREEN << "ID: " << id << ", Name: " << name << ", Category: " << category
             << ", Price: $" << price << ", Quantity: " << quantity << RESET << endl;
        cout << CYAN << "==============================================" << RESET << endl;
    }
};

// HashTable class
class HashTable {
private:
    static int TABLE_SIZE;
    vector<Product> table[100];

public:
    HashTable() {
        TABLE_SIZE = 100;
    }

    int hashFunction(int id) {
        return id % TABLE_SIZE;
    }

    void addProduct(Product product) {
        int index = hashFunction(product.getId());
        table[index].push_back(product);
    }

    Product* findProduct(int id) {
        int index = hashFunction(id);
        for (int i = 0; i < table[index].size(); i++) {
            if (table[index][i].getId() == id) {
                return &table[index][i];
            }
        }
        return nullptr;
    }

    void removeProduct(int id) {
        int index = hashFunction(id);
        vector<Product>& products = table[index];
        for (int i = 0; i < products.size(); i++) {
            if (products[i].getId() == id) {
                products.erase(products.begin() + i);
                cout << "Product removed successfully." << endl;
                return;
            }
        }
        cout << "Product with ID " << id << " not found." << endl;
    }

    vector<Product> getAllProducts() {
        vector<Product> allProducts;
        for (int i = 0; i < TABLE_SIZE; i++) {
            for (int j = 0; j < table[i].size(); j++) {
                allProducts.push_back(table[i][j]);
            }
        }
        return allProducts;
    }
};

int HashTable::TABLE_SIZE = 100;

// Binary Search Tree for sorting and searching
class BST {
private:
    struct Node {
        Product product;
        Node* left;
        Node* right;
        Node(Product p) : product(p), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insert(Node* node, Product product) {
        if (node == nullptr) return new Node(product);
        if (product.getName() < node->product.getName()) {
            node->left = insert(node->left, product);
        } else {
            node->right = insert(node->right, product);
        }
        return node;
    }

    void inOrder(Node* node, vector<Product>& result) {
        if (node == nullptr) return;
        inOrder(node->left, result);
        result.push_back(node->product);
        inOrder(node->right, result);
    }

public:
    BST() : root(nullptr) {}

    void addProduct(Product product) {
        root = insert(root, product);
    }

    vector<Product> getSortedProducts() {
        vector<Product> sortedProducts;
        inOrder(root, sortedProducts);
        return sortedProducts;
    }
};

class Inventory {
private:
    HashTable hashTable;
    BST bst;
    int LOW_STOCK_THRESHOLD = 10;

public:
    void addProduct(Product product) {
        hashTable.addProduct(product);
        bst.addProduct(product);
        checkLowStock(product);
    }

    void removeProduct(int id) {
        hashTable.removeProduct(id);
    }

    Product* findProduct(int id) {
        return hashTable.findProduct(id);
    }

    void displayAllProducts() {
        vector<Product> products = hashTable.getAllProducts();
        if (products.empty()) {
            cout << "No products in inventory." << endl;
            return;
        }
        for (int i = 0; i < products.size(); i++) {
            products[i].display();
        }
    }

    void checkLowStock(Product product) {
        if (product.getQuantity() < LOW_STOCK_THRESHOLD) {
            cout << "Low stock alert! Product ID " << product.getId() << " (" << product.getName() << ") has only " << product.getQuantity() << " units left." << endl;
        }
    }

    void filterByCategory(string category) {
        vector<Product> products = hashTable.getAllProducts();
        bool found = false;
        for (int i = 0; i < products.size(); i++) {
            if (products[i].getCategory() == category) {
                products[i].display();
                found = true;
            }
        }
        if (!found) {
            cout << "No products found in category: " << category << endl;
        }
    }

    void filterByPriceRange(double minPrice, double maxPrice) {
        vector<Product> products = hashTable.getAllProducts();
        bool found = false;
        for (int i = 0; i < products.size(); i++) {
            if (products[i].getPrice() >= minPrice && products[i].getPrice() <= maxPrice) {
                products[i].display();
                found = true;
            }
        }
        if (!found) {
            cout << "No products found in price range: $" << minPrice << " - $" << maxPrice << endl;
        }
    }

    void binarySearchByName(string name) {
        vector<Product> sortedProducts = bst.getSortedProducts();
        int left = 0, right = sortedProducts.size() - 1;
        bool found = false;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (sortedProducts[mid].getName() == name) {
                sortedProducts[mid].display();
                found = true;
                break;
            }
            if (sortedProducts[mid].getName() < name) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        if (!found) {
            cout << "Product with name '" << name << "' not found." << endl;
        }
    }

    void saveToFile(string filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file " << filename << endl;
            return;
        }
        file << "ID\tNAME\tCATEGORY\tPRICE\tQUANTITY" << endl;
        vector<Product> products = hashTable.getAllProducts();
        for (int i = 0; i < products.size(); i++) {
            file << products[i].getId() << "\t" << products[i].getName() << "\t"
                 << products[i].getCategory() << "\t" << products[i].getPrice() << "\t"
                 << products[i].getQuantity() << endl;
        }
        file.close();
        cout << "Inventory saved to " << filename << endl;
    }

    void loadFromFile(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file " << filename << endl;
            return;
        }
        string line;
        getline(file, line); // Skip headings
        while (getline(file, line)) {
            stringstream ss(line);
            int id, quantity;
            string name, category;
            double price;

            ss >> id >> name >> category >> price >> quantity;

            Product* existingProduct = findProduct(id);
            if (existingProduct) {
                existingProduct->setName(name);
                existingProduct->setCategory(category);
                existingProduct->setPrice(price);
                existingProduct->setQuantity(quantity);
            } else {
                Product product(id, name, category, price, quantity);
                addProduct(product);
            }
        }
        file.close();
        cout << "Inventory loaded from " << filename << endl;
    }

    void updateProduct(int id, string name, string category, double price, int quantity) {
        Product* product = findProduct(id);
        if (product) {
            product->setName(name);
            product->setCategory(category);
            product->setPrice(price);
            product->setQuantity(quantity);
            cout << "Product updated successfully." << endl;
        } else {
            cout << "Product with ID " << id << " not found." << endl;
        }
    }

    void bubbleSortByPrice() {
        vector<Product> productList = hashTable.getAllProducts();
        int n = productList.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (productList[j].getPrice() > productList[j + 1].getPrice()) {
                    swap(productList[j], productList[j + 1]);
                }
            }
        }
        for (int i = 0; i < productList.size(); i++) {
            productList[i].display();
        }
    }

    void mergeSort(vector<Product>& products, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(products, left, mid);
            mergeSort(products, mid + 1, right);
            merge(products, left, mid, right);
        }
    }

    void merge(vector<Product>& products, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        vector<Product> leftProducts(n1), rightProducts(n2);

        for (int i = 0; i < n1; i++) {
            leftProducts[i] = products[left + i];
        }
        for (int j = 0; j < n2; j++) {
            rightProducts[j] = products[mid + 1 + j];
        }

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (leftProducts[i].getName() <= rightProducts[j].getName()) {
                products[k] = leftProducts[i];
                i++;
            } else {
                products[k] = rightProducts[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            products[k] = leftProducts[i];
            i++;
            k++;
        }

        while (j < n2) {
            products[k] = rightProducts[j];
            j++;
            k++;
        }
    }

    void sortProductsByName() {
        vector<Product> products = hashTable.getAllProducts();
        mergeSort(products, 0, products.size() - 1);
        for (int i = 0; i < products.size(); i++) {
            products[i].display();
        }
    }
};

int main() {
    Inventory inventory;

    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        while (!(cin >> choice)) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << RED << "Invalid input. Please enter a number: " << RESET;
        }

        switch (choice) {
            case 1: {
                int id, quantity;
                string name, category;
                double price;

                cout << "Enter Product ID: ";
                while (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "Invalid input. Please enter an integer: " << RESET;
                }
                cin.ignore(); // Clear the newline character
                cout << "Enter Product Name: ";
                getline(cin, name);
                cout << "Enter Product Category: ";
                getline(cin, category);
                cout << "Enter Product Price: ";
                while (!(cin >> price)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "Invalid input. Please enter a number: " << RESET;
                }
                cout << "Enter Product Quantity: ";
                while (!(cin >> quantity)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "Invalid input. Please enter an integer: " << RESET;
                }

                Product newProduct(id, name, category, price, quantity);
                inventory.addProduct(newProduct);
                displayMessage("Product added successfully!");
                break;
            }
            case 2: {
                int id;
                cout << "Enter Product ID to remove: ";
                while (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "Invalid input. Please enter an integer: " << RESET;
                }
                inventory.removeProduct(id);
                inventory.saveToFile("inventory.txt");
                displayMessage("Product removed successfully!");
                break;
            }
            case 3: {
                int id;
                cout << "Enter Product ID to find: ";
                while (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "Invalid input. Please enter an integer: " << RESET;
                }
                Product* product = inventory.findProduct(id);
                if (product) {
                    product->display();
                } else {
                    displayMessage("Product with ID " + to_string(id) + " not found.");
                }
                break;
            }
            case 4:
                inventory.displayAllProducts();
                break;
            case 5: {
                string category;
                cout << "Enter category to filter: ";
                cin.ignore();
                getline(cin, category);
                inventory.filterByCategory(category);
                break;
            }
            case 6: {
                double minPrice, maxPrice;
                cout << "Enter minimum price: ";
                while (!(cin >> minPrice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "Invalid input. Please enter a number: " << RESET;
                }
                cout << "Enter maximum price: ";
                while (!(cin >> maxPrice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "Invalid input. Please enter a number: " << RESET;
                }
                inventory.filterByPriceRange(minPrice, maxPrice);
                break;
            }
            case 7: {
                string name;
                cout << "Enter product name to search: ";
                cin.ignore();
                getline(cin, name);
                inventory.binarySearchByName(name);
                break;
            }
            case 8:
                inventory.saveToFile("inventory.txt");
                displayMessage("Inventory saved to file!");
                break;
            case 9:
                inventory.loadFromFile("inventory.txt");
                displayMessage("Inventory loaded from file!");
                break;
            case 10: {
                int id, quantity;
                string name, category;
                double price;

                cout << "Enter Product ID to edit: ";
                while (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "Invalid input. Please enter an integer: " << RESET;
                }
                cin.ignore();
                cout << "Enter new Product Name: ";
                getline(cin, name);
                cout << "Enter new Product Category: ";
                getline(cin, category);
                cout << "Enter new Product Price: ";
                while (!(cin >> price)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "Invalid input. Please enter a number: " << RESET;
                }
                cout << "Enter new Product Quantity: ";
                while (!(cin >> quantity)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "Invalid input. Please enter an integer: " << RESET;
                }

                inventory.updateProduct(id, name, category, price, quantity);
                displayMessage("Product updated successfully!");
                break;
            }
            case 11:
                inventory.bubbleSortByPrice();
                break;
            case 12:
                inventory.sortProductsByName();
                break;
            case 0:
                displayMessage("Exiting the program...");
                break;
            default:
                displayMessage("Invalid choice!");
        }
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    } while (choice != 0);

    return 0;
}