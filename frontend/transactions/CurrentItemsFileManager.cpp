#include "CurrentItemsFileManager.h"
#include <fstream>

///TODO: implement/fix this

CurrentItemsFileManager::CurrentItemsFileManager(const std::string& filename)
    : m_filename(filename)
{}

bool CurrentItemsFileManager::load(std::unordered_map<std::string, Item>& items)
{
    std::ifstream infile(m_filename);
    if (!infile) {
        return false;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string item_name;
        int item_id, price, quantity;
        if (!(iss >> item_name >> item_id >> price >> quantity)) {
            continue;
        }
        Item item(item_id, price, quantity);
        items[item_name] = item;
    }

    return true;
}

bool CurrentItemsFileManager::save(const std::unordered_map<std::string, Item>& items)
{
    std::ofstream outfile(m_filename);
    if (!outfile) {
        return false;
    }

    for (auto& [item_name, item] : items) {
        outfile << item_name << " " << item.get_id() << " " << item.get_price() << " " << item.get_quantity() << "\n";
    }

    return true;
}
