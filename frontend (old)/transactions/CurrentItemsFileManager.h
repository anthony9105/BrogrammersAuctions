#ifndef CURRENTITEMSFILEMANAGER_H
#define CURRENTITEMSFILEMANAGER_H

#include <string>
#include <vector>
#include "item.h"

class CurrentItemsFileManager {
public:
    // Constructor
    CurrentItemsFileManager();

    // Destructor
    ~CurrentItemsFileManager();

    // Function to read the list of current items from a file or database
    std::vector<Item> readItems();

    // Function to write the list of current items to a file or database
    void writeItems(std::vector<Item> items);

    // Function to add an item to the list of current items
    void addItem(Item item);

    // Function to remove an item from the list of current items
    void removeItem(Item item);
};

#endif // CURRENTITEMSFILEMANAGER_H
