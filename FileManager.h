#pragma once
#include "Member.h"
#include <string>
#include <vector>
#include "json.hpp"
using json = nlohmann::json;
typedef struct activety_message{
    std::string Association;
    std::string Time;
    std::string Message;
}ActivityMessage;
class Member;
class Association;

json* findJsonObject(int n, const std::string& targetValue, json& outRoot); //查询json对象的函数，返回指向目标对象的指针，如果未找到则返回nullptr
bool write_user_json_config(Member *member,std::string permission); //将成员信息写入json配置文件
bool write_association_json_config(Association *association); //将社团信息写入json配置文件
bool association_member_append_json_config( //将成员信息追加到社团的json配置文件中
    Association *association,
    Member *member
);
bool write_activity_json_config(ActivityMessage *activityMessage); //将活动信息写入json配置文件
bool login_verification(const std::string &id, const std::string &password); //登录验证函数，根据成员ID和密码验证登录信息，返回验证结果
Member* Member_parsing(const std::string &id); //根据成员ID从json配置文件中解析出成员对象，返回指向成员对象的指针，如果未找到则返回nullptr
Association* Association_parsing(const std::string &name); //根据社团名称从json配置文件中解析出社团对象，返回指向社团对象的指针，如果未找到则返回nullptr
std::vector<std::string> getAssociationMembers(const std::string &associationName); //根据社团名称从json配置文件中获取成员ID列表，返回成员ID的字符串向量
std::vector<JAI> getMemberAssociationsInfo(const std::string &memberId); //根据成员ID从json配置文件中获取社团信息列表，返回社团信息的向量
std::vector<std::string> getAssociationNamesList(); //获取所有社团名称，返回社团名称的字符串向量