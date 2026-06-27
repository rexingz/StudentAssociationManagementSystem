[README.md](https://github.com/user-attachments/files/29412064/README.md)
# 学生社团管理系统

## 📋 项目简介

本项目是面向高校学生社团开发的桌面端管理系统，旨在系统化解决社团日常运营中的信息管理需求，提供直观、易用的图形化管理界面。

### 核心功能

| 模块 | 功能描述 |
|------|----------|
| **社团信息管理** | 添加、删除、修改社团基本信息（名称、成立时间、宗旨等），支持关键词查询 |
| **成员管理** | 记录成员信息，支持成员加入/退出、晋升负责人操作 |
| **活动管理** | 发布、修改、删除社团活动，主页面实时展示近期活动 |
| **资金管理** | 社团资金入账、支出记录，余额统计与交易历史查询 |
| **设备管理** | 设备新增、删除，记录设备添加时间与原因 |
| **账单管理** | 查看社团资金流水和设备清单的详细账单 |

## 🛠️ 技术栈

| 技术/工具 | 版本/说明 |
|-----------|-----------|
| 开发语言 | C++17 |
| GUI框架 | Qt 6.11.1 (MinGW 64-bit) |
| JSON处理 | nlohmann/json |
| 构建工具 | CMake |
| 开发环境 | Visual Studio Code |

## 📁 项目结构

```
StudentAssociationManagementSystem/
├── pic/                           # 资源图片文件夹
│   └── 01.png                     # UI素材图片
├── build/                         # CMake构建目录
├── release/                       # 发布版本目录
├── app.ico                        # 应用程序图标
├── app.rc                         # 资源配置文件
├── Association.cpp                # 社团类实现
├── Association.h                  # 社团类头文件
├── CMakeLists.txt                 # CMake构建配置
├── FileManager.cpp                # 文件读写模块实现
├── FileManager.h                  # 文件读写模块头文件
├── json.hpp                       # nlohmann json库头文件
├── main.cpp                       # 程序入口
├── mainwindow.cpp                 # 主窗口逻辑实现
├── mainwindow.h                   # 主窗口头文件
├── mainwindow.ui                  # 主窗口UI设计文件
├── Member.cpp                     # 成员类实现
├── Member.h                       # 成员类头文件
├── README.md                      # 项目说明文档
├── resource.qrc                   # Qt资源配置文件
└── StudentAssociationManagementSystem_zh_CN.ts  # 中文翻译文件
```

## 🚀 环境配置与运行

### 环境准备

1. 安装 **Qt 6.11.1**（需包含 MinGW 64-bit 编译套件）
2. 安装 **CMake 3.20+** 构建工具
3. 配置 VSCode 开发环境，安装以下插件：
   - C/C++ (Microsoft)
   - CMake Tools
   - Qt Creator

### 编译运行步骤

**方式一：使用命令行**
```bash
# 1. 创建构建目录并配置
cmake -B build/Desktop_Qt_6_11_1_MinGW_64_bit-Debug -G "MinGW Makefiles"

# 2. 编译项目
cmake --build build/Desktop_Qt_6_11_1_MinGW_64_bit-Debug

# 3. 运行程序
./build/Desktop_Qt_6_11_1_MinGW_64_bit-Debug/StudentAssociationManagementSystem.exe
```

**方式二：使用 Qt Creator**
1. 打开 Qt Creator，导入项目的 CMakeLists.txt
2. 选择 MinGW 64-bit 构建套件
3. 点击构建按钮编译项目
4. 点击运行按钮启动程序

### 直接运行

已编译的发布版本位于 `release/` 目录，可直接运行：
```bash
./release/StudentAssociationManagementSystem.exe
```

## 📊 数据存储

系统采用 JSON 文件存储数据，配置文件如下：

| 文件 | 用途 |
|------|------|
| `user_config.json` | 用户账户信息（学号、密码、权限） |
| `association_config.json` | 社团信息（名称、成员、资金、设备等） |
| `activity_config.json` | 社团活动信息（时间、内容、发布社团） |

## 🎯 使用说明

### 登录系统
- 默认管理员账号：`admin` / `admin`
- 普通用户可使用学生账号登录

### 功能模块

1. **主页面**：展示近期社团活动列表
2. **社团管理**：管理社团信息、成员、资金和设备
3. **活动管理**：发布、修改、删除社团活动
4. **用户管理**：管理系统用户权限

### 资金与设备管理

- **资金初始值**：每个社团初始资金为 0 元
- **设备初始值**：每个社团初始设备为空
- **账单查看**：点击社团卡片的"账单"按钮查看详细记录

## 👨‍💻 开发团队

## 📝 版本更新

### v1.0.0
- 完成基础登录/登出功能
- 实现社团信息管理模块
- 实现成员管理模块
- 实现活动管理模块

### v1.1.0
- 添加资金管理功能（入账、支出）
- 添加设备管理功能（新增、删除）
- 添加账单查看功能
- 优化主页面活动展示

## ⚠️ 注意事项

1. 首次运行请确保 `release/` 目录下存在配置文件
2. 运行时请勿修改或删除配置文件
3. 建议定期备份 `*_config.json` 文件
4. 程序运行需要 Qt 相关动态库支持

## 📄 许可证

本项目仅供学习交流使用，未经允许不得用于商业用途。
