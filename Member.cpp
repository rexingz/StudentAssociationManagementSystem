#include <vector>
#include <string>
#include "Member.h"
#include "FileManager.h"
#include "Association.h"
std::string Member :: getName() const{
    return this->Name;
}
std::string Member :: getPassword() const{
    return this->Password;
}
std::string Member :: getId() const{
    return this->Id;
}
std::vector<JAI> Member :: getJoinedAssociationInfo() const{
    return this->JoinedAssociations;
}
std::vector<std::string> Member::getAssociationNames() const
{
    std::vector<std::string> result;

    size_t totalSize = 0;
    for (const auto& info : JoinedAssociations) {
        totalSize += info.AssociationName.size();
    }
    result.reserve(totalSize);

    for (const auto& info : JoinedAssociations) {
        result.insert(
            result.end(),
            info.AssociationName.begin(),
            info.AssociationName.end()
        );
    }

    return result;
}

std::vector<std::string> Member::getAddTime() const
{
    std::vector<std::string> result;

    size_t totalSize = 0;
    for (const auto& info : JoinedAssociations) {
        totalSize += info.FoundingTime.size();
    }
    result.reserve(totalSize);

    for (const auto& info : JoinedAssociations) {
        result.insert(
            result.end(),
            info.FoundingTime.begin(),
            info.FoundingTime.end()
        );
    }

    return result;
}
std::string Member :: getPermission() const{
    return this->Permission;
}
void Principal::setAssociationInformation(std::string association_name,std::string Founding_Time,std::string Club_Description,std::vector<Student> members,std::string principal_name){
    std::vector<Student> principal_vector;
    Student principal_obj;
    principal_obj.Name = principal_name;
    principal_vector.push_back(principal_obj);
    Association *association = new Association(association_name,Founding_Time,Club_Description,members,principal_vector);
    write_association_json_config(association);
    delete association;
}
void Principal::setMemberInformation(Member *member, std::string AssociationName){
    write_user_json_config(member,"Principal");
}
void User::setMemberInformation(Member *member, std::string AssociationName){
    write_user_json_config(member,"User");
}
void Admin::setPermission(Member *member,std::string permission){
    write_user_json_config(member,"Admin");
}
void Admin::setMemberInformation(Member *member,std::string AssociationName){
    write_user_json_config(member,"Admin");
}
void Member::setMemberInformation(Member *member, std::string AssociationName){
    write_user_json_config(member,"User");
}
void Member::setPassword(const std::string &newPassword) {
    this->Password = newPassword;
}