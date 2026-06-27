#pragma once
#include <string>
#include <vector>

struct Equipment {
    std::string name;
    std::string addTime;
    std::string reason;
    Equipment(std::string n, std::string t, std::string r) : name(n), addTime(t), reason(r) {}
    Equipment() = default;
};

struct Transaction {
    std::string type;
    double amount;
    std::string time;
    std::string reason;
    Transaction(std::string t, double a, std::string ti, std::string r) : type(t), amount(a), time(ti), reason(r) {}
    Transaction() = default;
};

struct Student{
    std::string Id;
    std::string Name;
    Student(std::string n, std::string i) : Name(n), Id(i) {}
    Student() = default;
};

class Association{
    public:
        Association(std::string name,std::string founding_time,std::string association_description,
                    std::vector<Student> members = {},std::vector<Student> principal = {},
                    double funds = 0.0, std::vector<Equipment> equipments = {}, 
                    std::vector<Transaction> transactions = {}):
            Name(name),Founding_Time(founding_time),Association_Description(association_description),
            m_members(members),Principal(principal),Funds(funds),Equipments(equipments),Transactions(transactions){};
        
        std::string getName() const;
        std::string getFoundingTime() const;
        std::string getAssociationDescription() const;
        std::vector<Student> getPrincipal() const;
        std::vector<Student> getMembers() const;
        double getFunds() const;
        std::vector<Equipment> getEquipments() const;
        std::vector<Transaction> getTransactions() const;
        
        void setFunds(double funds);
        void addEquipment(const Equipment& eq);
        void removeEquipment(int index);
        void addTransaction(const Transaction& trans);
        
    private:
        std::string Name;
        std::string Founding_Time;
        std::string Association_Description;
        std::vector<Student> m_members;
        std::vector<Student> Principal;
        double Funds;
        std::vector<Equipment> Equipments;
        std::vector<Transaction> Transactions;
};