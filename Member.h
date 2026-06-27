#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Association.h"
typedef struct JoinedAssociationInformation{
    std::vector<std::string> AssociationName;
    std::vector<std::string> FoundingTime;
}JAI;
class Member
{
    public:
        Member(std::string name,std::string id,std::vector<std::string> add_time = {},std::vector<std::string> association_names = {},std::string password="", std::string permission="User"):Name(name),Id(id),Password(password),Permission(permission){
            int x = add_time.size();
            int y = association_names.size();
            for(int i = 0; i < y; i++) {
                this->JoinedAssociations.push_back({{association_names[i]}, {add_time[i]}});
            }
        }
        std::string getName() const;
        std::string getId() const;
        std::vector<JAI> getJoinedAssociationInfo() const;
        std::vector<std::string> getAssociationNames() const;
        std::vector<std::string> getAddTime() const;
        std::string getPermission() const;
        std::string getPassword() const;
        void setPassword(const std::string &newPassword);
        virtual void setMemberInformation(Member *member, std::string AssociationName = ""); //虚函数，设置成员信息，AssociationName参数仅在设置社团成员信息时使用
        virtual void setAssociationInformation(std::string association_name,std::string Founding_Time,std::string Club_Description,std::vector<Student> members,std::string principal){}; //纯虚函数，设置社团信息
        virtual void setPermission(Member *member,std::string permission){}; //纯虚函数，设置成员权限
    private:
        std::string Name;
        std::string Id;
        std::string Permission;
        std::string Password;
        std::vector<JAI> JoinedAssociations;
};
class User : public Member
{
    public:
        User(std::string name,std::string id,std::vector<std::string> add_time = {},std::vector<std::string> association_names = {},std::string password = ""):Member(name,id,add_time,association_names,password,"User"){};
        void setMemberInformation(Member *member, std::string AssociationName = "")override; //重写虚函数，设置成员信息(仅修改个人信息及密码)，AssociationName参数仅在设置社团成员信息时使用
};
class Principal : public Member
{
    public:
        Principal(std::string name,std::string id,std::vector<std::string> add_time = {},std::vector<std::string> association_names = {},std::string password = "", std::string permission = "Principal"):Member(name,id,add_time,association_names,password,permission){};
        void setAssociationInformation(std::string association_name,std::string Founding_Time,std::string Club_Description,std::vector<Student> members,std::string principal);
        void setMemberInformation(Member *member, std::string AssociationName = "")override; //重写虚函数，设置成员信息(可移除移入社团成员信息)，AssociationName参数仅在设置社团成员信息时使用
};
class Admin : public Principal
{
    public:
        Admin(std::string name,std::string id,std::vector<std::string> add_time = {},std::vector<std::string> association_names = {},std::string password = ""):Principal(name,id,add_time,association_names,password,"Admin"){};
        void setPermission(Member *member,std::string permission);
        void setMemberInformation(Member *member,std::string AssociationName) override;//重写虚函数，设置成员信息(可设置成员权限)，AssociationName参数仅在设置社团成员信息时使用
};
