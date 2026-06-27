#include "Association.h"
#include <string>
#include <vector>

std::string Association::getName() const {
    return Name;
}
std::string Association::getFoundingTime() const {
    return Founding_Time;
}
std::string Association::getAssociationDescription() const {
    return Association_Description;
}
std::vector<Student> Association::getMembers() const {
    return m_members;
}
std::vector<Student> Association::getPrincipal() const {
    return Principal;
}
double Association::getFunds() const {
    return Funds;
}
std::vector<Equipment> Association::getEquipments() const {
    return Equipments;
}
std::vector<Transaction> Association::getTransactions() const {
    return Transactions;
}

void Association::setFunds(double funds) {
    Funds = funds;
}

void Association::addEquipment(const Equipment& eq) {
    Equipments.push_back(eq);
}

void Association::removeEquipment(int index) {
    if (index >= 0 && index < Equipments.size()) {
        Equipments.erase(Equipments.begin() + index);
    }
}

void Association::addTransaction(const Transaction& trans) {
    Transactions.push_back(trans);
}