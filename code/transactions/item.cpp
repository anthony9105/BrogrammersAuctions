#include "item.h"
#include <stdexcept>

Item::Item(const std::string& name, double price, const std::string& seller)
    : name_(name), price_(price), seller_(seller), available_(true) {}

const std::string& Item::getName() const {
    return name_;
}

double Item::getPrice() const {
    return price_;
}

const std::string& Item::getSeller() const {
    return seller_;
}

bool Item::isAvailable() const {
    return available_;
}

void Item::setPrice(double price) {
    if (price < 0) {
        throw std::invalid_argument("Price must be non-negative");
    }
    price_ = price;
}

void Item::setAvailability(bool available) {
    available_ = available;
}

void Item::setBuyer(const std::string& buyer) {
    buyer_ = buyer;
}

const std::string& Item::getBuyer() const {
    return buyer_;
}

ItemsFileManager::ItemsFileManager(const std::string& filename)
    : filename_(filename) {}

std::vector<Item> ItemsFileManager::getItems() const {
    std::vector<Item> items;
    // implementation to read items from file and populate vector
    return items;
}

bool ItemsFileManager::addItem(const Item& item) {
    // implementation to add item to file
    return true;
}

bool ItemsFileManager::updateItem(const Item& item) {
    // implementation to update item in file
    return true;
}

bool ItemsFileManager::removeItem(const std::string& name) {
    // implementation to remove item from file
    return true;
}
