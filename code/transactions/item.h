#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <vector>

class Item {
public:
    Item(const std::string& name, double price, const std::string& seller);

    const std::string& getName() const;
    double getPrice() const;
    const std::string& getSeller() const;
    bool isAvailable() const;
    void setPrice(double price);
    void setAvailability(bool available);
    void setBuyer(const std::string& buyer);
    const std::string& getBuyer() const;

private:
    std::string name_;
    double price_;
    std::string seller_;
    bool available_;
    std::string buyer_;
};

class ItemsFileManager {
public:
    ItemsFileManager(const std::string& filename);

    std::vector<Item> getItems() const;
    bool addItem(const Item& item);
    bool updateItem(const Item& item);
    bool removeItem(const std::string& name);

private:
    std::string filename_;
};

#endif // ITEM_H
