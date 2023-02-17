#ifndef DELETE_H
#define DELETE_H

/**
 * Delete class
*/
class Delete : public Transaction {
    public:
        void executeTransaction(string name, string accountType, int balance);
};

#endif