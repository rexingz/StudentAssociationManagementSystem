#include "Member.h"
#include "Association.h"
#include "json.hpp"
#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <QFile>
#include <QDebug>
#include <unordered_map>
#include <string>
#include <QCoreApplication>

using json = nlohmann::json;

static QString getJsonFilePath(const std::string &fileName)
{
    return QCoreApplication::applicationDirPath() + "/" + QString::fromStdString(fileName);
}

static bool parseJsonFile(const QString &filePath, json &root)
{
    std::ifstream ifs(filePath.toStdString());
    if (!ifs.is_open()) {
        std::cerr << "Failed to open file: " << filePath.toStdString() << std::endl;
        return false;
    }

    try {
        root = json::parse(ifs);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    }
}

static std::vector<std::string> jsonToStringVector(const json &value)
{
    std::vector<std::string> result;
    if (value.is_array()) {
        for (const auto &item : value) {
            if (item.is_string()) {
                result.push_back(item.get<std::string>());
            }
        }
    } else if (value.is_string()) {
        result.push_back(value.get<std::string>());
    }
    return result;
}

static json* findJsonObjectRecursive(json& currentNode, const std::string& targetValue) {
    if (currentNode.is_object()) {
        if (currentNode.contains("Id") && currentNode["Id"] == targetValue) {
            return &currentNode;
        }
        if (currentNode.contains("Name") && currentNode["Name"] == targetValue) {
            return &currentNode;
        }
    }

    for (auto& [key, value] : currentNode.items()) {
        if (value.is_object()) {
            json* result = findJsonObjectRecursive(value, targetValue);
            if (result != nullptr) {
                return result;
            }
        }
    }
    return nullptr;
}
json* findJsonObject(int n, const std::string& targetValue, json& outRoot) {
    std::string fileName;
    if (n == 0) {
        fileName = "user_config.json";
    } else if (n == 1) {
        fileName = "association_config.json";
    } else if (n == 2) {
        fileName = "activity_config.json";
    } else {
        return nullptr;
    }

    const QString filePath = getJsonFilePath(fileName);
    if (!parseJsonFile(filePath, outRoot)) {
        return nullptr;
    }
    if (outRoot.is_object()) {
        if (outRoot.contains("Id") && outRoot["Id"] == targetValue) {
            return &outRoot;
        }
        if (outRoot.contains("Name") && outRoot["Name"] == targetValue) {
            return &outRoot;
        }
    }
    for (auto& item : outRoot.items()) {
        if (item.value().contains("Id") && item.value()["Id"] == targetValue) {
            return &item.value();
        }
        if (item.value().contains("Name") && item.value()["Name"] == targetValue) {
            return &item.value();
        }
        if (item.value().contains("AssociationName") && item.value()["AssociationName"] == targetValue) {
            return &item.value();
        }
        if (item.value().contains("Association_Name") && item.value()["Association_Name"] == targetValue) {
            return &item.value();
        }
        json* result = findJsonObjectRecursive(item.value(), targetValue);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

bool write_association_json_config(Association *association) {
    if (!association) return false;

    const QString filePath = getJsonFilePath("association_config.json");
    json root;
    std::ifstream ifs(filePath.toStdString());
    if (ifs.is_open()) {
        try {
            ifs >> root;
        } catch (...) {
            root = json::object();
        }
        ifs.close();
    } else {
        root = json::object();
    }
    json associationObj;
    associationObj["Name"] = association->getName();
    associationObj["Founding_Time"] = association->getFoundingTime();
    associationObj["Association_Description"] = association->getAssociationDescription();
    nlohmann::json membersArray = nlohmann::json::array();
    for (const auto& student : association->getMembers()) {
        nlohmann::json studentObj;
        studentObj["name"] = student.Name;
        studentObj["id"] = student.Id;
        membersArray.push_back(studentObj);
    }
    associationObj["Members"] = membersArray;

    nlohmann::json principalArray = nlohmann::json::array();
    for (const auto& student : association->getPrincipal()) {
        nlohmann::json studentObj;
        studentObj["name"] = student.Name;
        studentObj["id"] = student.Id;
        principalArray.push_back(studentObj);
    }
    associationObj["Principal"] = principalArray;
    json *existingObj = findJsonObject(1, association->getName(), root);
    if (existingObj != nullptr) {
        *existingObj = associationObj;
    } else {
        int maxIndex = 0;
        for (auto& [key, val] : root.items()) {
            if (key.rfind("Association_", 0) == 0) {
                int index = std::stoi(key.substr(12));
                if (index > maxIndex) maxIndex = index;
            }
        }
        std::string newKey = "Association_" + std::to_string(maxIndex + 1);
        root[newKey] = associationObj;
    }
    std::ofstream ofs(filePath.toStdString());
    if (!ofs.is_open()) return false;
    ofs << root.dump(4) << std::endl;
    ofs.close();

    return existingObj != nullptr;
}

bool write_user_json_config(Member *member, std::string permission) {
    if (!member) return false;

    const QString filePath = getJsonFilePath("user_config.json");
    json root;
    std::ifstream ifs(filePath.toStdString());
    if (ifs.is_open()) {
        try {
            ifs >> root;
        } catch (...) {
            root = json::object();
        }
        ifs.close();
    } else {
        root = json::object();
    }

    json userObj;
    userObj["Name"] = member->getName();
    userObj["Id"] = member->getId();
    userObj["Password"] = member->getPassword();
    userObj["Add_Time"] = member->getAddTime();
    userObj["Association_Name"] = member->getAssociationNames();
    userObj["Permission"] = member->getPermission();

    json *existingObj = findJsonObject(0, member->getId(), root);
    if (existingObj != nullptr) {
        *existingObj = userObj;
    } else {
        int maxIndex = 0;
        for (auto& [key, val] : root.items()) {
            if (key.rfind("User_", 0) == 0) {
                int index = std::stoi(key.substr(5));
                if (index > maxIndex) maxIndex = index;
            }
        }
        std::string newKey = "User_" + std::to_string(maxIndex + 1);
        root[newKey] = userObj;
    }

    std::ofstream ofs(filePath.toStdString());
    if (!ofs.is_open()) return false;
    ofs << root.dump(4) << std::endl;
    ofs.close();

    return true;
}
bool write_activity_json_config(ActivityMessage *activityMessage) {
    if (!activityMessage) {
        qDebug() << "write_activity_json_config: activityMessage is null";
        return false;
    }

    const QString filePath = getJsonFilePath("activity_config.json");
    qDebug() << "write_activity_json_config: filePath =" << filePath;
    
    json root;
    std::ifstream ifs(filePath.toStdString());
    if (ifs.is_open()) {
        try {
            ifs >> root;
            qDebug() << "write_activity_json_config: Successfully read file";
        } catch (...) {
            root = json::object();
            qDebug() << "write_activity_json_config: Error parsing JSON, creating empty object";
        }
        ifs.close();
    } else {
        root = json::object();
        qDebug() << "write_activity_json_config: File not found, creating empty object";
    }
    json activityObj;
    activityObj["Association"] = activityMessage->Association;
    activityObj["Time"] = activityMessage->Time;
    activityObj["Message"] = activityMessage->Message;
    int maxIndex = 0;
    for (auto& [key, val] : root.items()) {
        if (key.rfind("Activity_", 0) == 0) {
            int index = std::stoi(key.substr(9));
            if (index > maxIndex) maxIndex = index;
        }
    }
    std::string newKey = "Activity_" + std::to_string(maxIndex + 1);
    root[newKey] = activityObj;
    qDebug() << "write_activity_json_config: Created new activity with key:" << QString::fromStdString(newKey);
    std::ofstream ofs(filePath.toStdString());
    if (!ofs.is_open()) {
        qDebug() << "write_activity_json_config: Failed to open file for writing";
        return false;
    }
    ofs << root.dump(4) << std::endl;
    ofs.close();
    qDebug() << "write_activity_json_config: Successfully wrote to file";

    return true;
}
bool association_member_append_json_config(
    Association *association,
    Member *member
) {
    if (!association || !member) return false;

    json root;
    json *parentObj = findJsonObject(1, association->getName(), root);
    if (parentObj == nullptr) {
        std::cerr << "Association not found." << std::endl;
        return false;
    }

    json memberObject;
    memberObject["Name"] = member->getName();
    memberObject["Id"] = member->getId();

    (*parentObj)["Members"].push_back(memberObject);

    QString filePath = getJsonFilePath("association_config.json");
    std::ofstream ofs(filePath.toStdString());
    if (!ofs.is_open()) return false;
    ofs << root.dump(4) << std::endl;
    ofs.close();

    return true;
}

bool login_verification(const std::string &id, const std::string &password) {
    json root;
    json* userNode = findJsonObject(0, id, root);
    if(userNode != nullptr && (*userNode)["Password"] == password) {
        return true;
    }
    return false;
}

Member* Member_parsing(const std::string &id){
    json root;
    json *memberObj = findJsonObject(0, id, root);
    if (memberObj == nullptr) {
        std::cerr << "Member not found." << std::endl;
        return nullptr;
    }

    std::string name = (*memberObj)["Name"];
    std::vector<std::string> add_time;
    if ((*memberObj).contains("Add_Time")) {
        add_time = jsonToStringVector((*memberObj)["Add_Time"]);
    }
    std::string password;
    if ((*memberObj).contains("Password") && (*memberObj)["Password"].is_string()) {
        password = (*memberObj)["Password"].get<std::string>();
    }
    std::vector<std::string> association_names;
    if ((*memberObj).contains("Association_Name")) {
        association_names = jsonToStringVector((*memberObj)["Association_Name"]);
    }

    Member *member = new Member(name, id, add_time, association_names, password);
    if ((*memberObj).contains("Permission") && (*memberObj)["Permission"].is_string()) {
        member->setMemberInformation(member, (*memberObj)["Permission"].get<std::string>());
    }
    return member;
}

Association* Association_parsing(const std::string &name){
    json root;
    json *associationObj = findJsonObject(1, name, root);
    if (associationObj == nullptr) {
        std::cerr << "Association not found." << std::endl;
        return nullptr;
    }

    std::string founding_time = (*associationObj)["Founding_Time"].get<std::string>();
    std::string association_description = (*associationObj)["Association_Description"].get<std::string>();
    
    std::vector<Student> members;
    if ((*associationObj).contains("Members") && (*associationObj)["Members"].is_array()) {
        for (auto& student_json : (*associationObj)["Members"]) {
            Student member;
            member.Name = (student_json["Name"].get<std::string>());
            member.Id = (student_json["Id"].get<std::string>());
            members.push_back(member);
        }
    }

    std::vector<Student> principal;
    if ((*associationObj).contains("Principal") && (*associationObj)["Principal"].is_array()) {
        for (auto& student_json : (*associationObj)["Principal"]) {
            Student p;
            p.Name = (student_json["Name"].get<std::string>());
            p.Id = (student_json["Id"].get<std::string>());
            principal.push_back(p);
        }
    }
    
    double funds = 0.0;
    if ((*associationObj).contains("Funds") && (*associationObj)["Funds"].is_number()) {
        funds = (*associationObj)["Funds"].get<double>();
    }
    
    std::vector<Equipment> equipments;
    if ((*associationObj).contains("Equipments") && (*associationObj)["Equipments"].is_array()) {
        for (auto& eq_json : (*associationObj)["Equipments"]) {
            Equipment eq;
            eq.name = eq_json.contains("name") ? eq_json["name"].get<std::string>() : "";
            eq.addTime = eq_json.contains("addTime") ? eq_json["addTime"].get<std::string>() : "";
            eq.reason = eq_json.contains("reason") ? eq_json["reason"].get<std::string>() : "";
            equipments.push_back(eq);
        }
    }
    
    std::vector<Transaction> transactions;
    if ((*associationObj).contains("Transactions") && (*associationObj)["Transactions"].is_array()) {
        for (auto& trans_json : (*associationObj)["Transactions"]) {
            Transaction trans;
            trans.type = trans_json.contains("type") ? trans_json["type"].get<std::string>() : "";
            trans.amount = trans_json.contains("amount") ? trans_json["amount"].get<double>() : 0.0;
            trans.time = trans_json.contains("time") ? trans_json["time"].get<std::string>() : "";
            trans.reason = trans_json.contains("reason") ? trans_json["reason"].get<std::string>() : "";
            transactions.push_back(trans);
        }
    }

    Association *association = new Association(name, founding_time, association_description, members, principal, funds, equipments, transactions);
    return association;
}


std::vector<std::string> getAssociationMembers(const std::string &associationName) {
    json root;
    json *associationObj = findJsonObject(1, associationName, root);
    if (associationObj == nullptr) {
        std::cerr << "Association not found." << std::endl;
        return {};
    }

    std::vector<std::string> memberIds;
    if ((*associationObj).contains("Members") && (*associationObj)["Members"].is_array()) {
        for (const auto& member : (*associationObj)["Members"]) {
            if (member.contains("Id")) {
                memberIds.push_back(member["Id"]);
            }
        }
    }
    return memberIds;
}

std::vector<JAI> getMemberAssociationsInfo(const std::string &memberId) {
    json root;
    json *memberObj = findJsonObject(0, memberId, root);
    if (memberObj == nullptr) {
        std::cerr << "Member not found." << std::endl;
        return {};
    }

    std::vector<JAI> associationsInfo;
    if ((*memberObj).contains("Association_Name") && (*memberObj)["Association_Name"].is_array() &&
        (*memberObj).contains("Add_Time") && (*memberObj)["Add_Time"].is_array()) {
        for (size_t i = 0; i < (*memberObj)["Association_Name"].size(); ++i) {
            JAI info;
            if ((*memberObj)["Association_Name"][i].is_string()) {
                info.AssociationName.push_back((*memberObj)["Association_Name"][i].get<std::string>());
            }
            if ((*memberObj)["Add_Time"][i].is_string()) {
                info.FoundingTime.push_back((*memberObj)["Add_Time"][i].get<std::string>());
            }
            associationsInfo.push_back(info);
        }
    }
    return associationsInfo;
}

std::vector<std::string> getAssociationNamesList() {
    QString filePath = getJsonFilePath("association_config.json");
    json associations;
    if (!parseJsonFile(filePath, associations)) {
        return {};
    }
    std::vector<std::string> associationNames;
    for (const auto& [key, value] : associations.items()) {
        if (value.contains("Name") && value["Name"].is_string()) {
            associationNames.push_back(value["Name"].get<std::string>());
        }
    }
    return associationNames;
}
