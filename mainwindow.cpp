#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileManager.h"
#include <string.h>
#include <iostream>
#include <QMessageBox>
#include <QScreen>
#include <json.hpp>
#include <qmenu.h>
#include <QFormLayout>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QLineEdit>
#include <QFrame>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QDebug>
#include <QScrollArea>
#include <QObject>
#include <QSpacerItem>
#include <QTextEdit>
#include <QGridLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QDate>
#include <QCoreApplication>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , CurrentUser(nullptr)
{
    ui->setupUi(this);

    // 只保留这一行：确保显示登录页
    ui->QstackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::createMenus()
{
    if (ui->toolButton->menu()) {
        delete ui->toolButton->menu();
    }
    if (ui->toolButton_2->menu()) {
        delete ui->toolButton_2->menu();
    }

    QMenu *menu = new QMenu(this);
    QAction *actHome = menu->addAction("首页");
    QAction *actInfo = menu->addAction("个人信息");
    QAction *actMyAssociation = menu->addAction("我的社团");
    QAction *actSet  = menu->addAction("设置");
    QAction *actUserManage = nullptr;
    if (CurrentUser && CurrentUser->getPermission() == "Admin") {
        actUserManage = menu->addAction("用户管理");
    }
    menu->addSeparator();
    menu->addAction("退出", this, &MainWindow::close);

    ui->toolButton->setMenu(menu);
    ui->toolButton->setPopupMode(QToolButton::InstantPopup);
    ui->toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    ui->toolButton->setText("☰");
    connect(actInfo, &QAction::triggered, this, [=](){
        ui->QstackedWidget->setCurrentIndex(2);
    });
    connect(actMyAssociation, &QAction::triggered, this, [=](){
        ui->QstackedWidget->setCurrentIndex(1);
    });
    connect(actHome, &QAction::triggered, this, [=](){
        ui->QstackedWidget->setCurrentIndex(1);
    });
    connect(actSet, &QAction::triggered, this, [=](){
        ui->QstackedWidget->setCurrentIndex(1);
    });
    if (actUserManage) {
        connect(actUserManage, &QAction::triggered, this, [=](){
            ui->QstackedWidget->setCurrentIndex(1);
        });
    }

    QMenu *menu_association = new QMenu(this);
    menu_association->addAction("搜索");
    if (CurrentUser && (CurrentUser->getPermission() == "Admin" || CurrentUser->getPermission() == "Principal")) {
        menu_association->addAction("社团管理");
    }
    ui->toolButton_2->setMenu(menu_association);
    ui->toolButton_2->setPopupMode(QToolButton::InstantPopup);
    ui->toolButton_2->setToolButtonStyle(Qt::ToolButtonTextOnly);
}*/
void MainWindow::setupPage2UI(QWidget *page)
{
    // ===== 页面主布局 =====
    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ✅ 80% 透明背景
    page->setStyleSheet("background: rgba(255, 255, 255, 0.8);");

    // ===== 顶部工具栏 =====
    QWidget *toolbar = new QWidget(page);
    toolbar->setFixedHeight(56);
    toolbar->setStyleSheet("background: rgba(255, 255, 255, 0.9); border-bottom: 1px solid #e4e7ed;");

    QHBoxLayout *toolLayout = new QHBoxLayout(toolbar);
    toolLayout->setContentsMargins(20, 0, 20, 0);
    toolLayout->setSpacing(16);

    // --- 汉堡菜单（≡ 橙色）---
    QPushButton *menuBtn = new QPushButton("≡", toolbar);
    menuBtn->setFixedSize(40, 40);
    menuBtn->setStyleSheet(R"(
        QPushButton {
            font-size: 22px;
            background: transparent;
            border: none;
            color: #FF9800;
        }
        QPushButton:hover {
            color: #FFB74D;
        }
    )");

    QMenu *mainMenu = new QMenu(menuBtn);
    QAction *actInfo = mainMenu->addAction("个人信息");
    QAction *actMyAssociation = mainMenu->addAction("我的社团");
    QAction *actSet = mainMenu->addAction("设置");
    QAction *actUserManage = nullptr;
    mainMenu->addSeparator();
    mainMenu->addAction("退出", this, &MainWindow::close);
    connect(actInfo, &QAction::triggered, this, [=](){
        ui->QstackedWidget->setCurrentIndex(2);
    });
    connect(actMyAssociation, &QAction::triggered, this, [=](){
        ui->QstackedWidget->setCurrentIndex(3);
    });
    connect(actSet, &QAction::triggered, this, [=](){
        QMessageBox::warning(this,"提示","设置功能正在开发中，敬请期待！");
        ui->QstackedWidget->setCurrentIndex(1);
    });
    // ✅ 强制菜单文字为黑色
    mainMenu->setStyleSheet(R"(
        QMenu {
            background: white;
            border: 1px solid #e4e7ed;
            border-radius: 6px;
            padding: 6px 0;
        }
        QMenu::item {
            color: black;
            font-size: 14px;
            padding: 8px 24px;
        }
        QMenu::item:selected {
            background: #f5f7fa;
            color: #409EFF;
        }
    )");

    menuBtn->setMenu(mainMenu);

    // --- 社团下拉菜单 ---
    QPushButton *clubMenuBtn = new QPushButton("社团 ▾", toolbar);
    clubMenuBtn->setFixedSize(100, 36);
    clubMenuBtn->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            font-size: 14px;
            color: black;
        }
        QPushButton:hover {
            color: #409EFF;
            border-color: #409EFF;
        }
    )");

    QMenu *clubMenu = new QMenu(clubMenuBtn);
    QAction *actClubManage = clubMenu->addAction("社团管理");
    QAction *actClubActivity = clubMenu->addAction("社团活动");
    connect(actClubManage, &QAction::triggered, this, [=](){
        if(!CurrentUser || CurrentUser->getPermission() == "User") {
            QMessageBox::warning(this,"提示","您没有权限访问社团管理界面！");
            return;
        }
        else{
            ui->QstackedWidget->setCurrentIndex(4);
        }
    });
    connect(actClubActivity, &QAction::triggered, this, [=](){
        if(!CurrentUser || CurrentUser->getPermission() == "User") {
            QMessageBox::warning(this,"提示","您没有权限访问社团活动界面！");
            return;
        }
        else{
            ui->QstackedWidget->setCurrentIndex(5);
        }
    });
    // ✅ 社团菜单文字也是黑色
    clubMenu->setStyleSheet(mainMenu->styleSheet());

    clubMenuBtn->setMenu(clubMenu);

    // --- 弹簧 ---
    toolLayout->addWidget(menuBtn);
    toolLayout->addWidget(clubMenuBtn);
    toolLayout->addStretch();

    // --- 搜索框 ---
    QLineEdit *searchEdit = new QLineEdit(toolbar);
    searchEdit->setPlaceholderText("搜索...");
    searchEdit->setFixedSize(200, 36);
    searchEdit->setStyleSheet(R"(
        QLineEdit {
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            padding: 0 10px;
            font-size: 14px;
            color: black;
        }
        QLineEdit:focus {
            border-color: #409EFF;
        }
    )");

    // --- 管理员按钮 ---
    QPushButton *adminBtn = new QPushButton("管理员", toolbar);
    adminBtn->setFixedSize(90, 36);
    adminBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #409EFF;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #66b1ff;
        }
    )");
    toolLayout->addWidget(searchEdit);
    toolLayout->addWidget(adminBtn);

    mainLayout->addWidget(toolbar);
    connect(adminBtn, &QPushButton::clicked, this, [=](){
        if(!CurrentUser || CurrentUser->getPermission() != "Admin") {
            QMessageBox::warning(this,"提示","您没有权限访问用户管理界面！");
            return;
        }
        else{
            ui->QstackedWidget->setCurrentIndex(6);
        }
    });
    // ===== 中心内容 =====
    QWidget *center = new QWidget(page);
    QVBoxLayout *centerLayout = new QVBoxLayout(center);
    centerLayout->setContentsMargins(60, 40, 60, 40);
    centerLayout->setSpacing(24);

    QLabel *title = new QLabel("近期社团活动", center);
    title->setStyleSheet("font-size:26px; font-weight:bold; color:black;");
    title->setAlignment(Qt::AlignCenter);

    // 公告栏 - 显示用户所在社团的活动
    QTextEdit *noticeBoard = new QTextEdit(center);
    noticeBoard->setReadOnly(true);
    noticeBoard->setMinimumHeight(400);
    noticeBoard->setStyleSheet(R"(
        QTextEdit {
            background: white;
            border: 1px solid #e4e7ed;
            border-radius: 8px;
            padding: 20px;
            font-size: 15px;
            color: black;
        }
    )");

    // 读取用户所在社团的活动
    QString activityHtml = "<p style='font-size:15px; color:black;'>";
    
    if (CurrentUser) {
        // 获取用户加入的社团
        std::vector<JAI> assocList = getMemberAssociationsInfo(CurrentUser->getId());
        
        if (assocList.empty()) {
            activityHtml += "您暂未加入任何社团，暂无活动安排。</p>";
        } else {
            activityHtml += "以下是您所在社团的活动安排：<br><br>";
            
            // 读取活动配置文件
            QString activityFilePath = QCoreApplication::applicationDirPath() + "/activity_config.json";
            json activityRoot;
            std::ifstream activityFile(activityFilePath.toStdString());
            if (activityFile.is_open()) {
                activityFile >> activityRoot;
                activityFile.close();
            }
            
            // 遍历用户的社团，查找相关活动
            for (const auto& jai : assocList) {
                // AssociationName 是向量，取第一个元素
                if (jai.AssociationName.empty()) continue;
                std::string assocName = jai.AssociationName[0];
                activityHtml += "<strong>" + QString::fromStdString(assocName) + "</strong><br>";
                
                bool hasActivity = false;
                for (auto& item : activityRoot.items()) {
                    if (item.value().contains("Association") && 
                        item.value()["Association"].get<std::string>() == assocName) {
                        std::string time = item.value()["Time"].get<std::string>();
                        std::string message = item.value()["Message"].get<std::string>();
                        activityHtml += "• " + QString::fromStdString(time) + "：" + QString::fromStdString(message) + "<br>";
                        hasActivity = true;
                    }
                }
                
                if (!hasActivity) {
                    activityHtml += "暂无活动安排<br>";
                }
                activityHtml += "<br>";
            }
        }
    } else {
        activityHtml += "请先登录以查看社团活动。</p>";
    }
    
    activityHtml += "</p>";
    noticeBoard->setHtml(activityHtml);

    centerLayout->addWidget(title);
    centerLayout->addWidget(noticeBoard);

    mainLayout->addWidget(center);
}

void MainWindow::on_pushButton_clicked()
{
    QString username = ui->lineEdit_2->text().trimmed();
    QString password = ui->lineEdit_3->text().trimmed();
    std::string u = username.toStdString();
    std::string p = password.toStdString();

    bool isSuccess = login_verification(u,p);
    if (isSuccess) {
        json outRoot;
        json *p = findJsonObject(0, u, outRoot);
        if (p == nullptr) {
            QMessageBox::warning(this, "登录失败", "账号或密码错误，请重新输入！");
            ui->lineEdit_3->clear();
            return;
        }

        auto toStringVector = [](const json &value) {
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
        };

        std::vector<std::string> addTimeVec;
        std::vector<std::string> associationNameVec;
        if (p->contains("Add_Time")) {
            addTimeVec = toStringVector((*p)["Add_Time"]);
        }
        if (p->contains("Association_Name")) {
            associationNameVec = toStringVector((*p)["Association_Name"]);
        }

        std::string permission = "User";
        if (p->contains("Permission") && (*p)["Permission"].is_string()) {
            permission = (*p)["Permission"].get<std::string>();
        }

        // 添加字段检查
        if (!p->contains("Name") || !(*p)["Name"].is_string()) {
            QMessageBox::warning(this, "登录失败", "用户数据缺少 Name 字段！");
            return;
        }
        if (!p->contains("Id") || !(*p)["Id"].is_string()) {
            QMessageBox::warning(this, "登录失败", "用户数据缺少 Id 字段！");
            return;
        }
        if (!p->contains("Password") || !(*p)["Password"].is_string()) {
            QMessageBox::warning(this, "登录失败", "用户数据缺少 Password 字段！");
            return;
        }

        qDebug() << "Creating user object...";
        qDebug() << "Name:" << QString::fromStdString((*p)["Name"].get<std::string>());
        qDebug() << "Id:" << QString::fromStdString((*p)["Id"].get<std::string>());
        qDebug() << "Permission:" << QString::fromStdString(permission);

        if (permission == "User") {
            CurrentUser = new User((*p)["Name"], (*p)["Id"], addTimeVec, associationNameVec, (*p)["Password"]);
        } else if (permission == "Principal") {
            CurrentUser = new Principal((*p)["Name"], (*p)["Id"], addTimeVec, associationNameVec, (*p)["Password"]);
        } else if (permission == "Admin"){
            CurrentUser = new Admin((*p)["Name"], (*p)["Id"], addTimeVec, associationNameVec, (*p)["Password"]);
        } else {
            QMessageBox::warning(this, "登录失败", "未知的权限类型！");
            return;
        }

        if (!CurrentUser) {
            QMessageBox::warning(this, "登录失败", "创建用户对象失败！");
            return;
        }

        qDebug() << "User object created successfully";

        // createMenus();
        qDebug() << "Setting up page 2...";
        setupPage2UI(ui->page_2);
        qDebug() << "Setting up page 3...";
        setupPage3UI(ui->page_3);
        qDebug() << "Setting up page 4...";
        setupPage4UI(ui->page_4);
        qDebug() << "Setting up page 5...";
        setupPage5UI(ui->page_5);
        qDebug() << "Setting up page 6...";
        setupPage6UI(ui->page_6);
        qDebug() << "Setting up page 7...";
        setupPage7UI(ui->page_7);
        qDebug() << "Setting current index to 1...";
        ui->QstackedWidget->setCurrentIndex(1);

        qDebug() << "Login successful!";

    } else {
        QMessageBox::warning(this, "登录失败", "账号或密码错误，请重新输入！");
        ui->lineEdit_3->clear();
    }
}
void MainWindow::setupPage3UI(QWidget *page)
{
    // ===== 页面根布局 =====
    QVBoxLayout *rootLayout = qobject_cast<QVBoxLayout*>(page->layout());
    if (!rootLayout) {
        rootLayout = new QVBoxLayout(page);
    }

    rootLayout->setContentsMargins(40, 20, 40, 40);
    rootLayout->setSpacing(20);

    // ===== 返回按钮 =====
    QPushButton *backBtn = new QPushButton("← 返回", page);
    backBtn->setFixedSize(80, 32);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #606266;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            color: #409EFF;
            border-color: #409EFF;
        }
    )");

    QObject::connect(backBtn, &QPushButton::clicked, this, [=]() {
        qDebug() << "返回上一页";
        ui->QstackedWidget->setCurrentIndex(1);
        // stackedWidget->setCurrentIndex(0);
    });

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(backBtn);
    topLayout->addStretch();

    rootLayout->insertLayout(0, topLayout);
    // ===== 卡片 =====
    QWidget *card = new QWidget(page);
    card->setObjectName("profileCard");
    card->setFixedWidth(620);
    card->setMinimumHeight(580);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(40, 40, 40, 40);
    cardLayout->setSpacing(22);

    // ===== 标题 =====
    QLabel *title = new QLabel("个人信息", card);
    title->setStyleSheet("font-size:22px; font-weight:bold; color:black;");
    title->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(title);

    // ===== 表单 =====
    QFormLayout *form = new QFormLayout();
    form->setSpacing(18);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // 姓名
    QLineEdit *nameEdit = new QLineEdit(card);
    nameEdit->setText(CurrentUser ? QString::fromStdString(CurrentUser->getName()) : "");
    nameEdit->setReadOnly(true);
    nameEdit->setPlaceholderText("请输入姓名");

    // 学号
    QLineEdit *sidEdit = new QLineEdit(card);
    sidEdit->setText(CurrentUser ? QString::fromStdString(CurrentUser->getId()) : "");
    sidEdit->setReadOnly(true);
    sidEdit->setPlaceholderText("请输入学号");

    // 密码
    QLineEdit *pwdEdit = new QLineEdit(card);
    pwdEdit->setText(CurrentUser ? QString::fromStdString(CurrentUser->getPassword()) : "");
    pwdEdit->setEchoMode(QLineEdit::Password);
    pwdEdit->setPlaceholderText("请输入密码");

    // 显示密码复选框
    QCheckBox *showPwd = new QCheckBox("显示密码", card);

    // ✅ Qt 6 正确连接方式
    QObject::connect(showPwd, &QCheckBox::checkStateChanged,
                     pwdEdit, [=](Qt::CheckState state) {
                         pwdEdit->setEchoMode(state == Qt::Checked
                                                  ? QLineEdit::Normal
                                                  : QLineEdit::Password);
                     });

    // 密码行布局
    QHBoxLayout *pwdLayout = new QHBoxLayout();
    pwdLayout->addWidget(pwdEdit);
    pwdLayout->addStretch();
    pwdLayout->addWidget(showPwd);

    form->addRow("姓　名：", nameEdit);
    form->addRow("学　号：", sidEdit);
    form->addRow("密　码：", pwdLayout);

    cardLayout->addLayout(form);

    // ===== 分隔线 =====
    QFrame *sep = new QFrame(card);
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color:#e4e7ed;");
    cardLayout->addWidget(sep);

    // ===== 社团表格 =====
    QLabel *tblTitle = new QLabel("我的社团", card);
    tblTitle->setStyleSheet("font-size:15px; font-weight:600; color:black;");
    cardLayout->addWidget(tblTitle);

    QTableWidget *table = new QTableWidget(0, 2, card);
    table->setObjectName("clubTable");
    table->setHorizontalHeaderLabels({"社团名称", "加入时间"});
    table->verticalHeader()->setVisible(false);

    // ✅ 列宽自适应（关键修复）
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    // ✅ 滚动条
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setMinimumHeight(180);
    table->setMaximumHeight(220);

    // 示例数据
    QStringList clubs;
    QStringList times;
    if (CurrentUser) {
        // 转换社团名称列表
        for (const auto& name : CurrentUser->getAssociationNames()) {
          clubs.append(QString::fromStdString(name));
        }
        // 转换时间列表，修正方法名
        for (const auto& t : CurrentUser->getAddTime()) {
            times.append(QString::fromStdString(t));
        }
    }

    for (int i = 0; i < clubs.size(); ++i) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(clubs[i]));
        table->setItem(row, 1, new QTableWidgetItem(times[i]));
    }

    cardLayout->addWidget(table);

    // ===== 修改按钮 =====
    QPushButton *modifyBtn = new QPushButton("修 改", card);
    modifyBtn->setFixedSize(140, 38);
    modifyBtn->setCursor(Qt::PointingHandCursor);

    QObject::connect(modifyBtn, &QPushButton::clicked, this, [=]() {
        QString userpassword = pwdEdit->text().trimmed();
        std::string pwd = userpassword.toStdString();
        CurrentUser->setPassword(pwd);
        write_user_json_config(CurrentUser,CurrentUser->getPermission());
        qDebug() << "点击了修改按钮";
    });

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(modifyBtn);
    btnLayout->addStretch();
    cardLayout->addLayout(btnLayout);

    // ===== 页面居中 =====
    rootLayout->addStretch();
    rootLayout->addWidget(card, 0, Qt::AlignHCenter);
    rootLayout->addStretch();

    // ===== 终极QSS（确保所有文字黑色）=====
    page->setStyleSheet(R"(
        QWidget#profileCard {
            background: white;
            border-radius: 12px;
            border: 1px solid #e4e7ed;
        }

        QLabel {
            color: black;
            font-size: 14px;
        }

        QLineEdit {
            background-color: white;
            color: black;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            padding: 8px 12px;
            font-size: 14px;
            min-height: 20px;
        }

        QLineEdit:focus {
            border-color: #409EFF;
            outline: none;
        }

        QCheckBox {
            color: black;
            font-size: 13px;
        }

        QTableWidget#clubTable {
            background: white;
            border: 1px solid #ebeef5;
            border-radius: 6px;
            font-size: 13px;
            color: black;
            gridline-color: #ebeef5;
        }

        QHeaderView::section {
            background: #fafafa;
            padding: 10px 12px;
            border-bottom: 1px solid #ebeef5;
            font-weight: bold;
            color: black;
        }

        QScrollBar:vertical {
            width: 8px;
            background: transparent;
        }

        QScrollBar::handle:vertical {
            background: #c0c4cc;
            border-radius: 4px;
        }

        QPushButton {
            background-color: #409EFF;
            color: white;
            border: none;
            border-radius: 6px;
            font-size: 14px;
            font-weight: 500;
        }

        QPushButton:hover {
            background-color: #66b1ff;
        }

        QPushButton:pressed {
            background-color: #337ecc;
        }
    )");

}
void MainWindow::setupPage4UI(QWidget *page)
{
    // ===== 页面根布局 =====
    QVBoxLayout *rootLayout = qobject_cast<QVBoxLayout*>(page->layout());
    if (!rootLayout) {
        rootLayout = new QVBoxLayout(page);
    }
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // ✅ 80% 透明背景
    page->setStyleSheet("background: rgba(255, 255, 255, 0.8);");

    // ===== 返回按钮（左上角）=====
    QPushButton *backBtn = new QPushButton("← 返回", page);
    backBtn->setFixedSize(80, 32);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #606266;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            color: #409EFF;
            border-color: #409EFF;
        }
    )");

    QObject::connect(backBtn, &QPushButton::clicked, this, [=]() {
        qDebug() << "返回上一页";
        ui->QstackedWidget->setCurrentIndex(1);
        // stackedWidget->setCurrentIndex(0);
    });

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(backBtn);
    topLayout->addStretch();
    rootLayout->addLayout(topLayout);

    // ===== 标题 =====
    QLabel *title = new QLabel("社团信息", page);
    title->setStyleSheet("font-size:24px; font-weight:bold; color:black; margin:20px 0;");
    title->setAlignment(Qt::AlignCenter);
    rootLayout->addWidget(title);

    // ===== 滚动区域 =====
    QScrollArea *scrollArea = new QScrollArea(page);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            background: transparent;
            border: none;
        }
        QScrollBar:vertical {
            width: 8px;
            background: transparent;
        }
        QScrollBar::handle:vertical {
            background: #c0c4cc;
            border-radius: 4px;
        }
    )");

    QWidget *container = new QWidget(scrollArea);
    container->setStyleSheet("background: transparent;");

    QVBoxLayout *textLayout = new QVBoxLayout(container);
    textLayout->setContentsMargins(40, 20, 40, 40);
    textLayout->setSpacing(30);

    struct ClubInfo {
        QString name;
        QString createTime;
        QString purpose;
        QStringList members;
        QStringList principals;
        QVector<QPair<QString, QString>> activities; // 活动时间和内容
    };

    QVector<ClubInfo> clubs;

    // 读取活动配置
    json activityRoot;
    QString activityFilePath = QCoreApplication::applicationDirPath() + "/activity_config.json";
    std::ifstream activityFile(activityFilePath.toStdString());
    if (activityFile.is_open()) {
        try {
            activityFile >> activityRoot;
        } catch (...) {
            activityRoot = json::object();
        }
        activityFile.close();
    }

    if (CurrentUser) {
        std::vector<std::string> userAssociations = CurrentUser->getAssociationNames();
        
        for (const auto &assocName : userAssociations) {
            Association *assoc = Association_parsing(assocName);
            if (!assoc) {
                continue;
            }

            ClubInfo club;
            club.name = QString::fromStdString(assoc->getName());
            club.createTime = QString::fromStdString(assoc->getFoundingTime());
            club.purpose = QString::fromStdString(assoc->getAssociationDescription());

            std::vector<Student> members = assoc->getMembers();
            for (const auto &member : members) {
                QString memberInfo = QString::fromStdString(member.Name) + " (" + QString::fromStdString(member.Id) + ")";
                club.members.append(memberInfo);
            }

            std::vector<Student> principalVec = assoc->getPrincipal();
            for (const auto &principal : principalVec) {
                QString principalInfo = QString::fromStdString(principal.Name) + " (" + QString::fromStdString(principal.Id) + ")";
                club.principals.append(principalInfo);
            }

            // 获取该社团的活动
            for (const auto& item : activityRoot.items()) {
                if (item.value().contains("Association") && 
                    item.value()["Association"].get<std::string>() == assocName) {
                    QString time = QString::fromStdString(item.value()["Time"].get<std::string>());
                    QString content = QString::fromStdString(item.value()["Message"].get<std::string>());
                    club.activities.append(qMakePair(time, content));
                }
            }

            clubs.append(club);
            delete assoc;
        }
    }

    for (int i = 0; i < clubs.size(); ++i) {
        const ClubInfo &club = clubs[i];
        
        // 声明 separator
        QFrame *separator = nullptr;
        if (i < clubs.size() - 1) {
            separator = new QFrame(container);
            separator->setFrameShape(QFrame::HLine);
            separator->setStyleSheet("border-top: 1px solid #e4e7ed; margin:10px 0;");
        }
        
        QLabel *nameLabel = new QLabel(club.name, container);
        nameLabel->setStyleSheet("font-size:20px; font-weight:bold; color:black; margin-top:10px;");
        
        QString membersStr;
        if (!club.members.isEmpty()) {
            membersStr = club.members.join("、");
        } else {
            membersStr = "暂无成员";
        }
        
        QString principalsStr;
        if (!club.principals.isEmpty()) {
            principalsStr = "社长：" + club.principals.join("、");
        } else {
            principalsStr = "暂无社长";
        }

        QString infoText = QString(
            "创立时间：%1\n\n"
            "社团宗旨：%2\n\n"
            "%3\n\n"
            "社团成员：%4"
        ).arg(club.createTime, club.purpose, principalsStr, membersStr);
        
        QLabel *infoLabel = new QLabel(infoText, container);
        infoLabel->setStyleSheet("font-size:15px; color:black; line-height:1.6;");
        infoLabel->setWordWrap(true);
        
        textLayout->addWidget(nameLabel);
        textLayout->addWidget(infoLabel);
        
        // 显示活动信息（在社团信息之后）
        if (!club.activities.isEmpty()) {
            QLabel *activityTitle = new QLabel("社团活动：", container);
            activityTitle->setStyleSheet("font-size:15px; font-weight:bold; color:black; margin-top:15px;");
            textLayout->addWidget(activityTitle);
            
            for (const auto& activity : club.activities) {
                QString activityText = QString("时间：%1\n内容：%2").arg(activity.first, activity.second);
                QLabel *activityLabel = new QLabel(activityText, container);
                activityLabel->setStyleSheet("font-size:14px; color:#606266; line-height:1.6; padding:8px 12px; background:#f5f7fa; border-radius:4px; margin-top:8px;");
                activityLabel->setWordWrap(true);
                textLayout->addWidget(activityLabel);
            }
        }
        
        if (separator) {
            textLayout->addWidget(separator);
        }
    }

    if (clubs.isEmpty()) {
        QLabel *emptyLabel = new QLabel("您暂未加入任何社团", container);
        emptyLabel->setStyleSheet("font-size:16px; color:#909399; margin:50px 0;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        textLayout->addWidget(emptyLabel);
    }

    textLayout->addStretch();
    scrollArea->setWidget(container);
    rootLayout->addWidget(scrollArea);
}
QFrame* MainWindow::createClubCard(const QString &name, const QString &purpose, 
                                  const QString &time, const QStringList &members, 
                                  const QStringList &principals, double funds,
                                  const std::vector<Equipment>& equipments,
                                  const std::vector<Transaction>& transactions,
                                  QWidget *parent)
{
    QFrame *card = new QFrame(parent);
    card->setStyleSheet(R"(
        QFrame {
            background: white;
            border: 1px solid #e4e7ed;
            border-radius: 8px;
            padding: 10px;
        }
    )");

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(16);

    // ===== 社团基本信息 =====
    QFormLayout *form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->setSpacing(12);

    // 社团名称
    QLabel *nameLabel = new QLabel("社团名称：", card);
    nameLabel->setStyleSheet("color:black; font-size:14px;");
    
    QLineEdit *nameEdit = new QLineEdit(name, card);
    nameEdit->setReadOnly(true);
    nameEdit->setStyleSheet(R"(
        QLineEdit {
            color: black;
            font-size: 14px;
            padding: 6px 10px;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            background: #f5f7fa;
        }
    )");

    // 社团宗旨
    QLabel *purposeLabel = new QLabel("社团宗旨：", card);
    purposeLabel->setStyleSheet("color:black; font-size:14px;");
    
    QLineEdit *purposeEdit = new QLineEdit(purpose, card);
    purposeEdit->setStyleSheet(R"(
        QLineEdit {
            color: black;
            font-size: 14px;
            padding: 6px 10px;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            background: white;
        }
    )");

    // 成立时间
    QLabel *timeLabel = new QLabel("成立时间：", card);
    timeLabel->setStyleSheet("color:black; font-size:14px;");
    
    QLineEdit *timeEdit = new QLineEdit(time, card);
    timeEdit->setStyleSheet(R"(
        QLineEdit {
            color: black;
            font-size: 14px;
            padding: 6px 10px;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            background: white;
        }
    )");
    
    // 活动资金
    QLabel *fundsLabel = new QLabel("活动资金：", card);
    fundsLabel->setStyleSheet("color:black; font-size:14px;");
    
    QLineEdit *fundsEdit = new QLineEdit(QString::number(funds, 'f', 2) + " 元", card);
    fundsEdit->setReadOnly(true);
    fundsEdit->setStyleSheet(R"(
        QLineEdit {
            color: #E6A23C;
            font-size: 14px;
            font-weight: bold;
            padding: 6px 10px;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            background: #fef9e7;
        }
    )");

    form->addRow(nameLabel, nameEdit);
    form->addRow(purposeLabel, purposeEdit);
    form->addRow(timeLabel, timeEdit);
    form->addRow(fundsLabel, fundsEdit);

    cardLayout->addLayout(form);

    // ===== 成员列表区域 =====
    QHBoxLayout *memberHeader = new QHBoxLayout();
    
    QLabel *memberTitle = new QLabel("社团成员", card);
    memberTitle->setStyleSheet("font-size:16px; font-weight:bold; color:black; margin-top:10px;");

    // 修改按钮
    QPushButton *modifyBtn = new QPushButton("修改", card);
    modifyBtn->setFixedSize(70, 32);
    modifyBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #E6A23C;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #ebb563;
        }
    )");

    memberHeader->addWidget(memberTitle);
    memberHeader->addStretch();
    memberHeader->addWidget(modifyBtn);

    QObject::connect(modifyBtn, &QPushButton::clicked, this, [=]() {
        QString newPurpose = purposeEdit->text().trimmed();
        QString newTime = timeEdit->text().trimmed();
        
        json assocRoot;
        json *assocObj = findJsonObject(1, name.toStdString(), assocRoot);
        
        if (assocObj != nullptr) {
            (*assocObj)["Association_Description"] = newPurpose.toStdString();
            (*assocObj)["Founding_Time"] = newTime.toStdString();
            
            QString assocFilePath = QCoreApplication::applicationDirPath() + "/association_config.json";
            std::ofstream assocFile(assocFilePath.toStdString());
            if (assocFile.is_open()) {
                assocFile << assocRoot.dump(4) << std::endl;
                assocFile.close();
            }
            
            QMessageBox informationBox;
            informationBox.setWindowTitle("提示");
            informationBox.setText("社团信息已修改！");
            informationBox.setStyleSheet(R"(
                QMessageBox {
                    background-color: black;
                    color: white;
                }
                QLabel {
                    color: white;
                    font-size: 14px;
                    background: transparent;
                    min-width: 200px;
                    padding: 10px;
                }
                QPushButton {
                    background-color: #409EFF;
                    color: white;
                    border: none;
                    border-radius: 4px;
                    padding: 6px 20px;
                    min-width: 80px;
                    font-size: 14px;
                }
                QPushButton:hover {
                    background-color: #66b1ff;
                }
            )");
            informationBox.exec();
            
            QLayout *oldLayout = ui->page_5->layout();
            if (oldLayout) {
                QLayoutItem *item;
                while ((item = oldLayout->takeAt(0)) != nullptr) {
                    if (item->widget()) {
                        delete item->widget();
                    }
                    delete item;
                }
                delete oldLayout;
            }
            setupPage5UI(ui->page_5);
        }
    });

    cardLayout->addLayout(memberHeader);

    // 成员列表容器（带滚动条）
    QScrollArea *memberScroll = new QScrollArea(card);
    memberScroll->setWidgetResizable(true);
    memberScroll->setFixedHeight(150);
    memberScroll->setStyleSheet(R"(
        QScrollArea {
            border: 1px solid #e4e7ed;
            border-radius: 4px;
            background: white;
        }
        QScrollBar:vertical {
            width: 8px;
            background: transparent;
        }
        QScrollBar::handle:vertical {
            background: #c0c4cc;
            border-radius: 4px;
        }
    )");

    QWidget *memberContainer = new QWidget(memberScroll);
    QVBoxLayout *memberLayout = new QVBoxLayout(memberContainer);
    memberLayout->setContentsMargins(10, 10, 10, 10);
    memberLayout->setSpacing(8);

    for (const QString &member : members) {
        QWidget *memberRow = new QWidget(memberContainer);
        QHBoxLayout *rowLayout = new QHBoxLayout(memberRow);
        rowLayout->setContentsMargins(0, 0, 0, 0);

        QLabel *memberLabel = new QLabel(member, memberRow);
        memberLabel->setStyleSheet("font-size:14px; color:black;");

        QPushButton *promoteBtn = new QPushButton("晋升", memberRow);
        promoteBtn->setFixedSize(50, 28);
        promoteBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #409EFF;
                color: white;
                border: none;
                border-radius: 4px;
                font-size: 12px;
            }
            QPushButton:hover {
                background-color: #66b1ff;
            }
        )");

        QPushButton *deleteBtn = new QPushButton("删除", memberRow);
        deleteBtn->setFixedSize(50, 28);
        deleteBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #F56C6C;
                color: white;
                border: none;
                border-radius: 4px;
                font-size: 12px;
            }
            QPushButton:hover {
                background-color: #f78989;
            }
        )");

        QString memberInfo = member;
        QString currentAssocName = name;
        
        QObject::connect(promoteBtn, &QPushButton::clicked, this, [=]() {
            QStringList parts = memberInfo.split(" (");
            QString memberName = parts[0];
            QString memberId = parts[1].chopped(1);
            
            if (!CurrentUser || CurrentUser->getPermission() != "Admin") {
                QMessageBox::warning(parent, "提示", "只有管理员才能执行晋升操作！");
                return;
            }
            
            QMessageBox::StandardButton reply = QMessageBox::question(
                parent, "确认晋升",
                QString("确定要将 %1 晋升为社长吗？").arg(memberName),
                QMessageBox::Yes | QMessageBox::No
            );
            
            if (reply == QMessageBox::Yes) {
                promoteToPrincipal(currentAssocName, memberId, memberName);
                QMessageBox::information(parent, "成功", QString("%1 已晋升为社长！").arg(memberName));
                
                QLayout *oldLayout = ui->page_5->layout();
                if (oldLayout) {
                    QLayoutItem *item;
                    while ((item = oldLayout->takeAt(0)) != nullptr) {
                        if (item->widget()) {
                            delete item->widget();
                        }
                        delete item;
                    }
                    delete oldLayout;
                }
                setupPage5UI(ui->page_5);
            }
        });

        QObject::connect(deleteBtn, &QPushButton::clicked, this, [=]() {
            QStringList parts = memberInfo.split(" (");
            QString memberName = parts[0];
            QString memberId = parts[1].chopped(1);
            
            QMessageBox::StandardButton reply = QMessageBox::question(
                parent, "确认删除",
                QString("确定要删除成员 %1 吗？").arg(memberName),
                QMessageBox::Yes | QMessageBox::No
            );
            
            if (reply == QMessageBox::Yes) {
                removeMemberFromAssociation(currentAssocName, memberId, memberName);
                QMessageBox::information(parent, "成功", "成员已删除！");
                
                QLayout *oldLayout = ui->page_5->layout();
                if (oldLayout) {
                    QLayoutItem *item;
                    while ((item = oldLayout->takeAt(0)) != nullptr) {
                        if (item->widget()) {
                            delete item->widget();
                        }
                        delete item;
                    }
                    delete oldLayout;
                }
                setupPage5UI(ui->page_5);
            }
        });

        rowLayout->addWidget(memberLabel);
        rowLayout->addStretch();
        rowLayout->addWidget(promoteBtn);
        rowLayout->addWidget(deleteBtn);

        memberLayout->addWidget(memberRow);
    }

    memberLayout->addStretch();
    memberScroll->setWidget(memberContainer);
    cardLayout->addWidget(memberScroll);

    // ===== 社长列表区域 =====
    QHBoxLayout *principalHeader = new QHBoxLayout();
    
    QLabel *principalTitle = new QLabel("社团社长", card);
    principalTitle->setStyleSheet("font-size:16px; font-weight:bold; color:black; margin-top:10px;");

    principalHeader->addWidget(principalTitle);
    principalHeader->addStretch();

    cardLayout->addLayout(principalHeader);

    // 社长列表容器（带滚动条）
    QScrollArea *principalScroll = new QScrollArea(card);
    principalScroll->setWidgetResizable(true);
    principalScroll->setFixedHeight(150);
    principalScroll->setStyleSheet(R"(
        QScrollArea {
            border: 1px solid #e4e7ed;
            border-radius: 4px;
            background: white;
        }
        QScrollBar:vertical {
            width: 8px;
            background: transparent;
        }
        QScrollBar::handle:vertical {
            background: #c0c4cc;
            border-radius: 4px;
        }
    )");

    QWidget *principalContainer = new QWidget(principalScroll);
    QVBoxLayout *principalLayout = new QVBoxLayout(principalContainer);
    principalLayout->setContentsMargins(10, 10, 10, 10);
    principalLayout->setSpacing(8);

    for (const QString &principal : principals) {
        QWidget *principalRow = new QWidget(principalContainer);
        QHBoxLayout *rowLayout = new QHBoxLayout(principalRow);
        rowLayout->setContentsMargins(0, 0, 0, 0);

        QLabel *principalLabel = new QLabel(principal, principalRow);
        principalLabel->setStyleSheet("font-size:14px; color:black;");

        QPushButton *deletePrincipalBtn = new QPushButton("删除", principalRow);
        deletePrincipalBtn->setFixedSize(50, 28);
        deletePrincipalBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #F56C6C;
                color: white;
                border: none;
                border-radius: 4px;
                font-size: 12px;
            }
            QPushButton:hover {
                background-color: #f78989;
            }
        )");

        QString principalInfo = principal;
        QString currentAssocName = name;
        QObject::connect(deletePrincipalBtn, &QPushButton::clicked, this, [=]() {
            if (!CurrentUser || CurrentUser->getPermission() != "Admin") {
                QMessageBox::warning(parent, "提示", "只有管理员才能删除社长！");
                return;
            }
            
            QStringList parts = principalInfo.split(" (");
            QString principalName = parts[0];
            QString principalId = parts[1].chopped(1);
            
            QMessageBox::StandardButton reply = QMessageBox::question(
                parent, "确认删除",
                QString("确定要删除社长 %1 吗？").arg(principalName),
                QMessageBox::Yes | QMessageBox::No
            );
            
            if (reply == QMessageBox::Yes) {
                removePrincipalFromAssociation(currentAssocName, principalId, principalName);
                QMessageBox::information(parent, "成功", "社长已删除！");
                
                QLayout *oldLayout = ui->page_5->layout();
                if (oldLayout) {
                    QLayoutItem *item;
                    while ((item = oldLayout->takeAt(0)) != nullptr) {
                        if (item->widget()) {
                            delete item->widget();
                        }
                        delete item;
                    }
                    delete oldLayout;
                }
                setupPage5UI(ui->page_5);
            }
        });

        rowLayout->addWidget(principalLabel);
        rowLayout->addStretch();
        rowLayout->addWidget(deletePrincipalBtn);

        principalLayout->addWidget(principalRow);
    }

    principalLayout->addStretch();
    principalScroll->setWidget(principalContainer);
    cardLayout->addWidget(principalScroll);

    // ===== 设备列表区域 =====
    QHBoxLayout *equipmentHeader = new QHBoxLayout();
    
    QLabel *equipmentTitle = new QLabel("社团设备", card);
    equipmentTitle->setStyleSheet("font-size:16px; font-weight:bold; color:black; margin-top:10px;");

    QPushButton *addEquipmentBtn = new QPushButton("新增设备", card);
    addEquipmentBtn->setFixedSize(80, 28);
    addEquipmentBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #67C23A;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 12px;
        }
        QPushButton:hover {
            background-color: #85ce61;
        }
    )");

    equipmentHeader->addWidget(equipmentTitle);
    equipmentHeader->addStretch();
    equipmentHeader->addWidget(addEquipmentBtn);

    cardLayout->addLayout(equipmentHeader);

    // 设备列表容器（带滚动条）
    QScrollArea *equipmentScroll = new QScrollArea(card);
    equipmentScroll->setWidgetResizable(true);
    equipmentScroll->setFixedHeight(100);
    equipmentScroll->setStyleSheet(R"(
        QScrollArea {
            border: 1px solid #e4e7ed;
            border-radius: 4px;
            background: white;
        }
        QScrollBar:vertical {
            width: 8px;
            background: transparent;
        }
        QScrollBar::handle:vertical {
            background: #c0c4cc;
            border-radius: 4px;
        }
    )");

    QWidget *equipmentContainer = new QWidget(equipmentScroll);
    QVBoxLayout *equipmentLayout = new QVBoxLayout(equipmentContainer);
    equipmentLayout->setContentsMargins(10, 10, 10, 10);
    equipmentLayout->setSpacing(8);

    if (equipments.empty()) {
        QLabel *emptyEqLabel = new QLabel("暂无设备", equipmentContainer);
        emptyEqLabel->setStyleSheet("font-size:14px; color:#909399;");
        equipmentLayout->addWidget(emptyEqLabel);
    } else {
        int eqIndex = 0;
        for (const auto &eq : equipments) {
            QWidget *eqRow = new QWidget(equipmentContainer);
            QHBoxLayout *eqRowLayout = new QHBoxLayout(eqRow);
            eqRowLayout->setContentsMargins(0, 0, 0, 0);

            QString eqInfo = QString::fromStdString(eq.name) + " (" + QString::fromStdString(eq.addTime) + ")";
            if (!eq.reason.empty()) {
                eqInfo += " - " + QString::fromStdString(eq.reason);
            }
            
            QLabel *eqLabel = new QLabel(eqInfo, eqRow);
            eqLabel->setStyleSheet("font-size:14px; color:black;");

            QPushButton *deleteEqBtn = new QPushButton("删除", eqRow);
            deleteEqBtn->setFixedSize(50, 28);
            deleteEqBtn->setStyleSheet(R"(
                QPushButton {
                    background-color: #F56C6C;
                    color: white;
                    border: none;
                    border-radius: 4px;
                    font-size: 12px;
                }
                QPushButton:hover {
                    background-color: #f78989;
                }
            )");

            int currentIndex = eqIndex;
            QString assocName = name;
            QObject::connect(deleteEqBtn, &QPushButton::clicked, this, [=]() {
                QMessageBox::StandardButton reply = QMessageBox::question(
                    parent, "确认删除",
                    QString("确定要删除设备 %1 吗？").arg(QString::fromStdString(equipments[currentIndex].name)),
                    QMessageBox::Yes | QMessageBox::No
                );
                
                if (reply == QMessageBox::Yes) {
                    removeEquipmentFromAssociation(assocName, currentIndex);
                    QMessageBox::information(parent, "成功", "设备已删除！");
                    
                    QLayout *oldLayout = ui->page_5->layout();
                    if (oldLayout) {
                        QLayoutItem *item;
                        while ((item = oldLayout->takeAt(0)) != nullptr) {
                            if (item->widget()) delete item->widget();
                            delete item;
                        }
                        delete oldLayout;
                    }
                    setupPage5UI(ui->page_5);
                }
            });

            eqRowLayout->addWidget(eqLabel);
            eqRowLayout->addStretch();
            eqRowLayout->addWidget(deleteEqBtn);

            equipmentLayout->addWidget(eqRow);
            eqIndex++;
        }
    }

    equipmentLayout->addStretch();
    equipmentScroll->setWidget(equipmentContainer);
    cardLayout->addWidget(equipmentScroll);

    // ===== 资金操作按钮区域 =====
    QHBoxLayout *fundsBtnLayout = new QHBoxLayout();
    fundsBtnLayout->setSpacing(12);

    QPushButton *addFundsBtn = new QPushButton("新增入账", card);
    addFundsBtn->setFixedSize(80, 32);
    addFundsBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #67C23A;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #85ce61;
        }
    )");

    QPushButton *addExpenseBtn = new QPushButton("新增支出", card);
    addExpenseBtn->setFixedSize(80, 32);
    addExpenseBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #F56C6C;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #f78989;
        }
    )");

    QPushButton *billsBtn = new QPushButton("账单", card);
    billsBtn->setFixedSize(60, 32);
    billsBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #409EFF;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #66b1ff;
        }
    )");

    fundsBtnLayout->addStretch();
    fundsBtnLayout->addWidget(addFundsBtn);
    fundsBtnLayout->addWidget(addExpenseBtn);
    fundsBtnLayout->addWidget(billsBtn);
    cardLayout->addLayout(fundsBtnLayout);

    // 连接设备和资金操作按钮
    QString assocName = name;
    QObject::connect(addEquipmentBtn, &QPushButton::clicked, this, [=]() {
        showAddEquipmentDialog(card, assocName);
    });

    QObject::connect(addFundsBtn, &QPushButton::clicked, this, [=]() {
        showAddFundsDialog(card, assocName, "income");
    });

    QObject::connect(addExpenseBtn, &QPushButton::clicked, this, [=]() {
        showAddFundsDialog(card, assocName, "expense");
    });

    QObject::connect(billsBtn, &QPushButton::clicked, this, [=]() {
        showBillsDialog(card, assocName, transactions, funds);
    });

    QPushButton *addMemberBtn = new QPushButton("新增成员", card);
    addMemberBtn->setFixedSize(100, 32);
    addMemberBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #67C23A;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #85ce61;
        }
    )");

    QString currentAssocName = name;
    QObject::connect(addMemberBtn, &QPushButton::clicked, card, [=]() {
        showAddMemberDialog(card, currentAssocName);
    });

    QHBoxLayout *addBtnLayout = new QHBoxLayout();
    addBtnLayout->addStretch();
    addBtnLayout->addWidget(addMemberBtn);
    cardLayout->addLayout(addBtnLayout);

    return card;
}

void MainWindow::showAddMemberDialog(QWidget *parent, const QString &associationName)
{
    QDialog *dialog = new QDialog(parent);
    dialog->setWindowTitle("新增成员");
    dialog->setFixedSize(350, 200);
    dialog->setStyleSheet("QDialog { background-color: white; } QLabel { color: black; font-size: 14px; }");

    QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);
    dialogLayout->setContentsMargins(20, 20, 20, 20);
    dialogLayout->setSpacing(16);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    formLayout->setSpacing(12);

    QLineEdit *idInput = new QLineEdit(dialog);
    idInput->setPlaceholderText("请输入学号");
    idInput->setStyleSheet(R"(
        QLineEdit {
            color: black;
            font-size: 14px;
            padding: 6px 10px;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            background: white;
        }
        QLineEdit::placeholder {
            color: #c0c4cc;
        }
    )");

    formLayout->addRow("学号：", idInput);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
    
    buttonBox->button(QDialogButtonBox::Ok)->setText("确认");
    buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    
    buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet(
        "QPushButton { background-color: #409EFF; color: white; padding: 6px 16px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #66b1ff; }"
    );
    buttonBox->button(QDialogButtonBox::Cancel)->setStyleSheet(
        "QPushButton { background-color: #909399; color: white; padding: 6px 16px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #a6a9ad; }"
    );

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, dialog, [=]() {
        QString newId = idInput->text().trimmed();
        
        if (newId.isEmpty()) {
            QMessageBox::warning(dialog, "提示", "请输入学号！");
            return;
        }

        json userRoot;
        json *userObj = findJsonObject(0, newId.toStdString(), userRoot);
        
        if (userObj == nullptr) {
            QMessageBox::warning(dialog, "提示", "不存在此人，请确认学号是否正确！");
            return;
        }

        std::string userName = (*userObj)["Name"].get<std::string>();
        
        if ((*userObj).contains("Association_Name")) {
            for (const auto &assoc : (*userObj)["Association_Name"]) {
                if (assoc.get<std::string>() == associationName.toStdString()) {
                    QMessageBox::warning(dialog, "提示", "该用户已在此社团中！");
                    return;
                }
            }
        }

        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString timestamp = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

        if (!(*userObj).contains("Association_Name")) {
            (*userObj)["Association_Name"] = json::array();
        }
        (*userObj)["Association_Name"].push_back(associationName.toStdString());

        if (!(*userObj).contains("Add_Time")) {
            (*userObj)["Add_Time"] = json::array();
        }
        (*userObj)["Add_Time"].push_back(timestamp.toStdString());

        QString userFilePath = QCoreApplication::applicationDirPath() + "/user_config.json";
        std::ofstream userFile(userFilePath.toStdString());
        if (userFile.is_open()) {
            userFile << userRoot.dump(4) << std::endl;
            userFile.close();
        }

        json assocRoot;
        json *assocObj = findJsonObject(1, associationName.toStdString(), assocRoot);
        if (assocObj != nullptr) {
            if (!(*assocObj).contains("Members")) {
                (*assocObj)["Members"] = json::array();
            }
            json newMember;
            newMember["Id"] = newId.toStdString();
            newMember["Name"] = userName;
            (*assocObj)["Members"].push_back(newMember);

            QString assocFilePath = QCoreApplication::applicationDirPath() + "/association_config.json";
            std::ofstream assocFile(assocFilePath.toStdString());
            if (assocFile.is_open()) {
                assocFile << assocRoot.dump(4) << std::endl;
                assocFile.close();
            }
        }

        QMessageBox::information(dialog, "成功", QString("已将 %1 添加到社团！").arg(QString::fromStdString(userName)));
        dialog->accept();

        QLayout *oldLayout = ui->page_5->layout();
        if (oldLayout) {
            QLayoutItem *item;
            while ((item = oldLayout->takeAt(0)) != nullptr) {
                if (item->widget()) {
                    delete item->widget();
                }
                delete item;
            }
            delete oldLayout;
        }
        setupPage5UI(ui->page_5);
    });

    QObject::connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    dialogLayout->addLayout(formLayout);
    dialogLayout->addWidget(buttonBox);
    dialog->exec();
}

void MainWindow::showAddEquipmentDialog(QWidget *parent, const QString &associationName)
{
    QDialog *dialog = new QDialog(parent);
    dialog->setWindowTitle("新增设备");
    dialog->setFixedSize(350, 250);
    dialog->setStyleSheet("QDialog { background-color: white; } QLabel { color: black; font-size: 14px; }");

    QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);
    dialogLayout->setContentsMargins(20, 20, 20, 20);
    dialogLayout->setSpacing(16);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    formLayout->setSpacing(12);

    QLineEdit *nameInput = new QLineEdit(dialog);
    nameInput->setPlaceholderText("请输入设备名称");
    nameInput->setStyleSheet(R"(
        QLineEdit {
            color: black;
            font-size: 14px;
            padding: 6px 10px;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            background: white;
        }
        QLineEdit::placeholder {
            color: #c0c4cc;
        }
    )");

    QLineEdit *reasonInput = new QLineEdit(dialog);
    reasonInput->setPlaceholderText("请输入添加原因（可选）");
    reasonInput->setStyleSheet(R"(
        QLineEdit {
            color: black;
            font-size: 14px;
            padding: 6px 10px;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            background: white;
        }
        QLineEdit::placeholder {
            color: #c0c4cc;
        }
    )");

    formLayout->addRow("设备名称：", nameInput);
    formLayout->addRow("添加原因：", reasonInput);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
    
    buttonBox->button(QDialogButtonBox::Ok)->setText("确认");
    buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    
    buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet(
        "QPushButton { background-color: #409EFF; color: white; padding: 6px 16px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #66b1ff; }"
    );
    buttonBox->button(QDialogButtonBox::Cancel)->setStyleSheet(
        "QPushButton { background-color: #909399; color: white; padding: 6px 16px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #a6a9ad; }"
    );

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, dialog, [=]() {
        QString eqName = nameInput->text().trimmed();
        QString reason = reasonInput->text().trimmed();
        
        if (eqName.isEmpty()) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("请输入设备名称！");
            msgBox.setStyleSheet(R"(
                QMessageBox { background-color: black; }
                QMessageBox QLabel { color: white; }
                QMessageBox QPushButton { background-color: #409EFF; color: white; border: none; border-radius: 4px; padding: 6px 16px; }
            )");
            msgBox.exec();
            return;
        }

        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString timestamp = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

        json assocRoot;
        json *assocObj = findJsonObject(1, associationName.toStdString(), assocRoot);
        
        if (assocObj != nullptr) {
            if (!(*assocObj).contains("Equipments")) {
                (*assocObj)["Equipments"] = json::array();
            }
            
            json newEquipment;
            newEquipment["name"] = eqName.toStdString();
            newEquipment["addTime"] = timestamp.toStdString();
            newEquipment["reason"] = reason.toStdString();
            (*assocObj)["Equipments"].push_back(newEquipment);

            QString assocFilePath = QCoreApplication::applicationDirPath() + "/association_config.json";
            std::ofstream assocFile(assocFilePath.toStdString());
            if (assocFile.is_open()) {
                assocFile << assocRoot.dump(4) << std::endl;
                assocFile.close();
                
                QMessageBox msgBox;
                msgBox.setWindowTitle("提示");
                msgBox.setText("设备添加成功！");
                msgBox.setStyleSheet(R"(
                    QMessageBox { background-color: black; }
                    QMessageBox QLabel { color: white; }
                    QMessageBox QPushButton { background-color: #409EFF; color: white; border: none; border-radius: 4px; padding: 6px 16px; }
                )");
                msgBox.exec();
                
                dialog->accept();
                
                QLayout *oldLayout = ui->page_5->layout();
                if (oldLayout) {
                    QLayoutItem *item;
                    while ((item = oldLayout->takeAt(0)) != nullptr) {
                        if (item->widget()) delete item->widget();
                        delete item;
                    }
                    delete oldLayout;
                }
                setupPage5UI(ui->page_5);
            }
        }
    });

    QObject::connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    dialogLayout->addLayout(formLayout);
    dialogLayout->addWidget(buttonBox);
    dialog->exec();
}

void MainWindow::removeEquipmentFromAssociation(const QString &associationName, int index)
{
    json assocRoot;
    json *assocObj = findJsonObject(1, associationName.toStdString(), assocRoot);
    
    if (assocObj != nullptr) {
        if ((*assocObj).contains("Equipments") && (*assocObj)["Equipments"].is_array()) {
            json newEqArray = json::array();
            int currentIndex = 0;
            for (const auto &eq : (*assocObj)["Equipments"]) {
                if (currentIndex != index) {
                    newEqArray.push_back(eq);
                }
                currentIndex++;
            }
            (*assocObj)["Equipments"] = newEqArray;

            QString assocFilePath = QCoreApplication::applicationDirPath() + "/association_config.json";
            std::ofstream assocFile(assocFilePath.toStdString());
            if (assocFile.is_open()) {
                assocFile << assocRoot.dump(4) << std::endl;
                assocFile.close();
            }
        }
    }
}

void MainWindow::showAddFundsDialog(QWidget *parent, const QString &associationName, const QString &type)
{
    QDialog *dialog = new QDialog(parent);
    dialog->setWindowTitle(type == "income" ? "新增入账" : "新增支出");
    dialog->setFixedSize(350, 250);
    dialog->setStyleSheet("QDialog { background-color: white; } QLabel { color: black; font-size: 14px; }");

    QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);
    dialogLayout->setContentsMargins(20, 20, 20, 20);
    dialogLayout->setSpacing(16);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    formLayout->setSpacing(12);

    QLineEdit *amountInput = new QLineEdit(dialog);
    amountInput->setPlaceholderText("请输入金额");
    amountInput->setStyleSheet(R"(
        QLineEdit {
            color: black;
            font-size: 14px;
            padding: 6px 10px;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            background: white;
        }
        QLineEdit::placeholder {
            color: #c0c4cc;
        }
    )");

    QLineEdit *reasonInput = new QLineEdit(dialog);
    reasonInput->setPlaceholderText("请输入原因");
    reasonInput->setStyleSheet(R"(
        QLineEdit {
            color: black;
            font-size: 14px;
            padding: 6px 10px;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            background: white;
        }
        QLineEdit::placeholder {
            color: #c0c4cc;
        }
    )");

    formLayout->addRow("金额：", amountInput);
    formLayout->addRow("原因：", reasonInput);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
    
    buttonBox->button(QDialogButtonBox::Ok)->setText("确认");
    buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
    
    buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet(
        "QPushButton { background-color: #409EFF; color: white; padding: 6px 16px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #66b1ff; }"
    );
    buttonBox->button(QDialogButtonBox::Cancel)->setStyleSheet(
        "QPushButton { background-color: #909399; color: white; padding: 6px 16px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #a6a9ad; }"
    );

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, dialog, [=]() {
        QString amountStr = amountInput->text().trimmed();
        QString reason = reasonInput->text().trimmed();
        
        if (amountStr.isEmpty()) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("请输入金额！");
            msgBox.setStyleSheet(R"(
                QMessageBox { background-color: black; }
                QMessageBox QLabel { color: white; }
                QMessageBox QPushButton { background-color: #409EFF; color: white; border: none; border-radius: 4px; padding: 6px 16px; }
            )");
            msgBox.exec();
            return;
        }

        bool ok;
        double amount = amountStr.toDouble(&ok);
        if (!ok || amount <= 0) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("请输入有效的金额！");
            msgBox.setStyleSheet(R"(
                QMessageBox { background-color: black; }
                QMessageBox QLabel { color: white; }
                QMessageBox QPushButton { background-color: #409EFF; color: white; border: none; border-radius: 4px; padding: 6px 16px; }
            )");
            msgBox.exec();
            return;
        }

        if (reason.isEmpty()) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("请输入原因！");
            msgBox.setStyleSheet(R"(
                QMessageBox { background-color: black; }
                QMessageBox QLabel { color: white; }
                QMessageBox QPushButton { background-color: #409EFF; color: white; border: none; border-radius: 4px; padding: 6px 16px; }
            )");
            msgBox.exec();
            return;
        }

        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString timestamp = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

        json assocRoot;
        json *assocObj = findJsonObject(1, associationName.toStdString(), assocRoot);
        
        if (assocObj != nullptr) {
            // 更新资金
            double currentFunds = 0.0;
            if ((*assocObj).contains("Funds") && (*assocObj)["Funds"].is_number()) {
                currentFunds = (*assocObj)["Funds"].get<double>();
            }
            
            if (type == "income") {
                currentFunds += amount;
            } else {
                if (currentFunds < amount) {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("提示");
                    msgBox.setText("余额不足！");
                    msgBox.setStyleSheet(R"(
                        QMessageBox { background-color: black; }
                        QMessageBox QLabel { color: white; }
                        QMessageBox QPushButton { background-color: #409EFF; color: white; border: none; border-radius: 4px; padding: 6px 16px; }
                    )");
                    msgBox.exec();
                    return;
                }
                currentFunds -= amount;
            }
            (*assocObj)["Funds"] = currentFunds;

            // 添加交易记录
            if (!(*assocObj).contains("Transactions")) {
                (*assocObj)["Transactions"] = json::array();
            }
            
            json newTransaction;
            newTransaction["type"] = type.toStdString();
            newTransaction["amount"] = amount;
            newTransaction["time"] = timestamp.toStdString();
            newTransaction["reason"] = reason.toStdString();
            (*assocObj)["Transactions"].push_back(newTransaction);

            QString assocFilePath = QCoreApplication::applicationDirPath() + "/association_config.json";
            std::ofstream assocFile(assocFilePath.toStdString());
            if (assocFile.is_open()) {
                assocFile << assocRoot.dump(4) << std::endl;
                assocFile.close();
                
                QMessageBox msgBox;
                msgBox.setWindowTitle("提示");
                msgBox.setText(type == "income" ? "入账成功！" : "支出成功！");
                msgBox.setStyleSheet(R"(
                    QMessageBox { background-color: black; }
                    QMessageBox QLabel { color: white; }
                    QMessageBox QPushButton { background-color: #409EFF; color: white; border: none; border-radius: 4px; padding: 6px 16px; }
                )");
                msgBox.exec();
                
                dialog->accept();
                
                QLayout *oldLayout = ui->page_5->layout();
                if (oldLayout) {
                    QLayoutItem *item;
                    while ((item = oldLayout->takeAt(0)) != nullptr) {
                        if (item->widget()) delete item->widget();
                        delete item;
                    }
                    delete oldLayout;
                }
                setupPage5UI(ui->page_5);
            }
        }
    });

    QObject::connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    dialogLayout->addLayout(formLayout);
    dialogLayout->addWidget(buttonBox);
    dialog->exec();
}

void MainWindow::showBillsDialog(QWidget *parent, const QString &associationName, 
                                  const std::vector<Transaction>& transactions, double currentFunds)
{
    QDialog *dialog = new QDialog(parent);
    dialog->setWindowTitle("账单 - " + associationName);
    dialog->setFixedSize(600, 400);
    dialog->setStyleSheet("QDialog { background-color: white; }");

    QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);
    dialogLayout->setContentsMargins(10, 10, 10, 10);
    dialogLayout->setSpacing(10);

    // 选项卡
    QTabWidget *tabWidget = new QTabWidget(dialog);
    tabWidget->setStyleSheet(R"(
        QTabWidget::tab-bar { left: 5px; }
        QTabBar::tab {
            background: #f5f7fa;
            border: 1px solid #e4e7ed;
            border-bottom: none;
            padding: 8px 20px;
            margin-right: 2px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
            font-size: 14px;
            color: #606266;
        }
        QTabBar::tab:selected {
            background: white;
            color: #409EFF;
            border-color: #409EFF;
        }
    )");

    // 资金选项卡
    QWidget *fundsTab = new QWidget(tabWidget);
    QVBoxLayout *fundsLayout = new QVBoxLayout(fundsTab);
    fundsLayout->setContentsMargins(10, 10, 10, 10);

    // 当前余额
    QLabel *balanceLabel = new QLabel(QString("当前余额：%1 元").arg(QString::number(currentFunds, 'f', 2)), fundsTab);
    balanceLabel->setStyleSheet("font-size:16px; font-weight:bold; color:#E6A23C; margin-bottom:10px;");
    fundsLayout->addWidget(balanceLabel);

    // 交易列表
    QScrollArea *fundsScroll = new QScrollArea(fundsTab);
    fundsScroll->setWidgetResizable(true);
    fundsScroll->setStyleSheet(R"(
        QScrollArea { border: 1px solid #e4e7ed; border-radius: 4px; }
        QScrollBar:vertical { width: 8px; background: transparent; }
        QScrollBar::handle:vertical { background: #c0c4cc; border-radius: 4px; }
    )");

    QWidget *fundsContainer = new QWidget(fundsScroll);
    QVBoxLayout *fundsListLayout = new QVBoxLayout(fundsContainer);
    fundsListLayout->setContentsMargins(5, 5, 5, 5);
    fundsListLayout->setSpacing(8);

    if (transactions.empty()) {
        QLabel *emptyLabel = new QLabel("暂无交易记录", fundsContainer);
        emptyLabel->setStyleSheet("font-size:14px; color:#909399; text-align:center; padding:20px;");
        fundsListLayout->addWidget(emptyLabel);
    } else {
        double totalIncome = 0.0;
        double totalExpense = 0.0;
        
        for (const auto &trans : transactions) {
            QWidget *transRow = new QWidget(fundsContainer);
            QHBoxLayout *rowLayout = new QHBoxLayout(transRow);
            rowLayout->setContentsMargins(0, 0, 0, 0);

            QString typeStr = trans.type == "income" ? "入账" : "支出";
            QString amountStr = trans.type == "income" ? 
                "+" + QString::number(trans.amount, 'f', 2) : 
                "-" + QString::number(trans.amount, 'f', 2);
            
            QString transInfo = QString("[%1] %2 - %3 (%4)").arg(
                QString::fromStdString(trans.time),
                typeStr,
                QString::fromStdString(trans.reason),
                amountStr
            );
            
            QLabel *transLabel = new QLabel(transInfo, transRow);
            transLabel->setStyleSheet(QString("font-size:14px; color:%1;").arg(
                trans.type == "income" ? "#67C23A" : "#F56C6C"
            ));

            rowLayout->addWidget(transLabel);
            fundsListLayout->addWidget(transRow);

            if (trans.type == "income") {
                totalIncome += trans.amount;
            } else {
                totalExpense += trans.amount;
            }
        }

        // 统计信息
        QFrame *statsFrame = new QFrame(fundsContainer);
        statsFrame->setStyleSheet("QFrame { border-top: 1px solid #e4e7ed; padding-top: 10px; }");
        QHBoxLayout *statsLayout = new QHBoxLayout(statsFrame);
        statsLayout->setSpacing(30);

        QLabel *incomeLabel = new QLabel(QString("总收入：%1 元").arg(QString::number(totalIncome, 'f', 2)), statsFrame);
        incomeLabel->setStyleSheet("font-size:14px; color:#67C23A;");

        QLabel *expenseLabel = new QLabel(QString("总支出：%1 元").arg(QString::number(totalExpense, 'f', 2)), statsFrame);
        expenseLabel->setStyleSheet("font-size:14px; color:#F56C6C;");

        statsLayout->addWidget(incomeLabel);
        statsLayout->addWidget(expenseLabel);
        fundsListLayout->addWidget(statsFrame);
    }

    fundsListLayout->addStretch();
    fundsScroll->setWidget(fundsContainer);
    fundsLayout->addWidget(fundsScroll);

    // 设备选项卡
    QWidget *equipmentTab = new QWidget(tabWidget);
    QVBoxLayout *equipmentLayout = new QVBoxLayout(equipmentTab);
    equipmentLayout->setContentsMargins(10, 10, 10, 10);

    QScrollArea *eqScroll = new QScrollArea(equipmentTab);
    eqScroll->setWidgetResizable(true);
    eqScroll->setStyleSheet(R"(
        QScrollArea { border: 1px solid #e4e7ed; border-radius: 4px; }
        QScrollBar:vertical { width: 8px; background: transparent; }
        QScrollBar::handle:vertical { background: #c0c4cc; border-radius: 4px; }
    )");

    QWidget *eqContainer = new QWidget(eqScroll);
    QVBoxLayout *eqListLayout = new QVBoxLayout(eqContainer);
    eqListLayout->setContentsMargins(5, 5, 5, 5);
    eqListLayout->setSpacing(8);

    // 读取设备信息
    json assocRoot;
    json *assocObj = findJsonObject(1, associationName.toStdString(), assocRoot);
    
    if (assocObj == nullptr || !(*assocObj).contains("Equipments") || !(*assocObj)["Equipments"].is_array() || 
        (*assocObj)["Equipments"].empty()) {
        QLabel *emptyEqLabel = new QLabel("暂无设备", eqContainer);
        emptyEqLabel->setStyleSheet("font-size:14px; color:#909399; text-align:center; padding:20px;");
        eqListLayout->addWidget(emptyEqLabel);
    } else {
        for (const auto &eq : (*assocObj)["Equipments"]) {
            QString eqName = eq.contains("name") ? QString::fromStdString(eq["name"].get<std::string>()) : "";
            QString eqTime = eq.contains("addTime") ? QString::fromStdString(eq["addTime"].get<std::string>()) : "";
            QString eqReason = eq.contains("reason") ? QString::fromStdString(eq["reason"].get<std::string>()) : "";
            
            QString eqInfo = eqName + " (" + eqTime + ")";
            if (!eqReason.isEmpty()) {
                eqInfo += " - " + eqReason;
            }
            
            QLabel *eqLabel = new QLabel(eqInfo, eqContainer);
            eqLabel->setStyleSheet("font-size:14px; color:black;");
            eqListLayout->addWidget(eqLabel);
        }
    }

    eqListLayout->addStretch();
    eqScroll->setWidget(eqContainer);
    equipmentLayout->addWidget(eqScroll);

    tabWidget->addTab(fundsTab, "资金");
    tabWidget->addTab(equipmentTab, "设备");

    dialogLayout->addWidget(tabWidget);

    QPushButton *closeBtn = new QPushButton("关闭", dialog);
    closeBtn->setFixedSize(80, 32);
    closeBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #909399;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #a6a9ad;
        }
    )");
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(closeBtn);
    dialogLayout->addLayout(btnLayout);

    QObject::connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::close);

    dialog->exec();
}

void MainWindow::removeMemberFromAssociation(const QString &associationName, const QString &memberId, const QString &memberName)
{
    json userRoot;
    json *userObj = findJsonObject(0, memberId.toStdString(), userRoot);
    
    if (userObj != nullptr) {
        if ((*userObj).contains("Association_Name") && (*userObj)["Association_Name"].is_array()) {
            json newAssocArray = json::array();
            for (const auto &assoc : (*userObj)["Association_Name"]) {
                if (assoc.get<std::string>() != associationName.toStdString()) {
                    newAssocArray.push_back(assoc);
                }
            }
            (*userObj)["Association_Name"] = newAssocArray;
        }
        
        if ((*userObj).contains("Add_Time") && (*userObj)["Add_Time"].is_array()) {
            json newTimeArray = json::array();
            if ((*userObj).contains("Association_Name") && (*userObj)["Association_Name"].is_array()) {
                size_t assocCount = (*userObj)["Association_Name"].size();
                for (size_t i = 0; i < assocCount && i < (*userObj)["Add_Time"].size(); ++i) {
                    newTimeArray.push_back((*userObj)["Add_Time"][i]);
                }
            }
            (*userObj)["Add_Time"] = newTimeArray;
        }

        QString userFilePath = QCoreApplication::applicationDirPath() + "/user_config.json";
        std::ofstream userFile(userFilePath.toStdString());
        if (userFile.is_open()) {
            userFile << userRoot.dump(4) << std::endl;
            userFile.close();
        }
    }

    json assocRoot;
    json *assocObj = findJsonObject(1, associationName.toStdString(), assocRoot);
    
    if (assocObj != nullptr) {
        if ((*assocObj).contains("Members") && (*assocObj)["Members"].is_array()) {
            json newMembersArray = json::array();
            for (const auto &member : (*assocObj)["Members"]) {
                if (member.contains("Id") && member["Id"].get<std::string>() != memberId.toStdString()) {
                    newMembersArray.push_back(member);
                }
            }
            (*assocObj)["Members"] = newMembersArray;
        }

        QString assocFilePath = QCoreApplication::applicationDirPath() + "/association_config.json";
        std::ofstream assocFile(assocFilePath.toStdString());
        if (assocFile.is_open()) {
            assocFile << assocRoot.dump(4) << std::endl;
            assocFile.close();
        }
    }
}

void MainWindow::promoteToPrincipal(const QString &associationName, const QString &memberId, const QString &memberName)
{
    json assocRoot;
    json *assocObj = findJsonObject(1, associationName.toStdString(), assocRoot);
    
    if (assocObj != nullptr) {
        if (!(*assocObj).contains("Principal")) {
            (*assocObj)["Principal"] = json::array();
        }
        
        bool alreadyPrincipal = false;
        for (const auto &principal : (*assocObj)["Principal"]) {
            if (principal.contains("Id") && principal["Id"].get<std::string>() == memberId.toStdString()) {
                alreadyPrincipal = true;
                break;
            }
        }
        
        if (!alreadyPrincipal) {
            json newPrincipal;
            newPrincipal["Id"] = memberId.toStdString();
            newPrincipal["Name"] = memberName.toStdString();
            (*assocObj)["Principal"].push_back(newPrincipal);

            QString assocFilePath = QCoreApplication::applicationDirPath() + "/association_config.json";
            std::ofstream assocFile(assocFilePath.toStdString());
            if (assocFile.is_open()) {
                assocFile << assocRoot.dump(4) << std::endl;
                assocFile.close();
            }
        }
    }

    json userRoot;
    json *userObj = findJsonObject(0, memberId.toStdString(), userRoot);
    if (userObj != nullptr) {
        if ((*userObj).contains("Permission")) {
            std::string currentPermission = (*userObj)["Permission"].get<std::string>();
            if (currentPermission == "User") {
                (*userObj)["Permission"] = "Principal";
                
                QString userFilePath = QCoreApplication::applicationDirPath() + "/user_config.json";
                std::ofstream userFile(userFilePath.toStdString());
                if (userFile.is_open()) {
                    userFile << userRoot.dump(4) << std::endl;
                    userFile.close();
                }
            }
        }
    }
}

void MainWindow::removePrincipalFromAssociation(const QString &associationName, const QString &principalId, const QString &principalName)
{
    json assocRoot;
    json *assocObj = findJsonObject(1, associationName.toStdString(), assocRoot);
    
    if (assocObj != nullptr) {
        if ((*assocObj).contains("Principal") && (*assocObj)["Principal"].is_array()) {
            json newPrincipalArray = json::array();
            for (const auto &principal : (*assocObj)["Principal"]) {
                if (principal.contains("Id") && principal["Id"].get<std::string>() != principalId.toStdString()) {
                    newPrincipalArray.push_back(principal);
                }
            }
            (*assocObj)["Principal"] = newPrincipalArray;

            QString assocFilePath = QCoreApplication::applicationDirPath() + "/association_config.json";
            std::ofstream assocFile(assocFilePath.toStdString());
            if (assocFile.is_open()) {
                assocFile << assocRoot.dump(4) << std::endl;
                assocFile.close();
            }
        }
    }
}

void MainWindow::setupPage5UI(QWidget *page)
{
    QVBoxLayout *rootLayout = qobject_cast<QVBoxLayout*>(page->layout());
    if (!rootLayout) {
        rootLayout = new QVBoxLayout(page);
    }
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    page->setStyleSheet("background: rgba(255, 255, 255, 0.8);");

    QPushButton *backBtn = new QPushButton("← 返回", page);
    backBtn->setFixedSize(80, 32);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: black;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            color: #409EFF;
            border-color: #409EFF;
        }
    )");

    QObject::connect(backBtn, &QPushButton::clicked, this, [=]() {
        ui->QstackedWidget->setCurrentIndex(1);
        qDebug() << "返回上一页";
    });

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(backBtn);
    topLayout->addStretch();
    rootLayout->addLayout(topLayout);

    QLabel *title = new QLabel("社团管理", page);
    title->setStyleSheet("font-size:24px; font-weight:bold; color:black; margin:20px 0;");
    title->setAlignment(Qt::AlignCenter);
    rootLayout->addWidget(title);

    QScrollArea *scrollArea = new QScrollArea(page);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(R"(
        QScrollArea {
            background: transparent;
            border: none;
        }
        QScrollBar:vertical {
            width: 8px;
            background: transparent;
        }
        QScrollBar::handle:vertical {
            background: #c0c4cc;
            border-radius: 4px;
        }
    )");

    QWidget *container = new QWidget(scrollArea);
    container->setStyleSheet("background: transparent;");

    QVBoxLayout *cardsLayout = new QVBoxLayout(container);
    cardsLayout->setContentsMargins(40, 20, 40, 40);
    cardsLayout->setSpacing(24);

    if (CurrentUser) {
        std::string currentUserId = CurrentUser->getId();
        std::string currentUserPermission = CurrentUser->getPermission();
        
        std::vector<std::string> allAssociations;
        
        if (currentUserPermission == "Admin") {
            QString assocFilePath = QCoreApplication::applicationDirPath() + "/association_config.json";
            std::ifstream assocFile(assocFilePath.toStdString());
            if (assocFile.is_open()) {
                json assocRoot;
                assocFile >> assocRoot;
                assocFile.close();
                
                for (auto& item : assocRoot.items()) {
                    if (item.value().contains("AssociationName")) {
                        allAssociations.push_back(item.value()["AssociationName"].get<std::string>());
                    }
                }
            }
        } else {
            allAssociations = CurrentUser->getAssociationNames();
        }

        for (const auto &assocName : allAssociations) {
            Association *assoc = Association_parsing(assocName);
            if (!assoc) {
                continue;
            }

            bool isPresident = false;
            std::vector<Student> principals = assoc->getPrincipal();
            for (const auto &p : principals) {
                if (p.Id == currentUserId) {
                    isPresident = true;
                    break;
                }
            }

            if (currentUserPermission != "Admin" && !isPresident) {
                delete assoc;
                continue;
            }

            QString qAssocName = QString::fromStdString(assoc->getName());
            QString qAssocDesc = QString::fromStdString(assoc->getAssociationDescription());
            QString qAssocTime = QString::fromStdString(assoc->getFoundingTime());
            double assocFunds = assoc->getFunds();
            
            QStringList membersList;
            std::vector<Student> members = assoc->getMembers();
            for (const auto &member : members) {
                QString memberInfo = QString::fromStdString(member.Name) + " (" + QString::fromStdString(member.Id) + ")";
                membersList.append(memberInfo);
            }

            QStringList principalsList;
            std::vector<Student> principalVec = assoc->getPrincipal();
            for (const auto &principal : principalVec) {
                QString principalInfo = QString::fromStdString(principal.Name) + " (" + QString::fromStdString(principal.Id) + ")";
                principalsList.append(principalInfo);
            }
            
            std::vector<Equipment> equipments = assoc->getEquipments();
            std::vector<Transaction> transactions = assoc->getTransactions();

            cardsLayout->addWidget(createClubCard(qAssocName, qAssocDesc, qAssocTime, membersList, principalsList, assocFunds, equipments, transactions, container));

            delete assoc;
        }
    }

    cardsLayout->addStretch();
    scrollArea->setWidget(container);
    rootLayout->addWidget(scrollArea);
}
void MainWindow::setupPage6UI(QWidget *page)
{
    QVBoxLayout *rootLayout = qobject_cast<QVBoxLayout*>(page->layout());
    if (!rootLayout) {
        rootLayout = new QVBoxLayout(page);
    }
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    page->setStyleSheet("background: rgba(255, 255, 255, 0.8);");

    // 获取当前用户信息（移到开头）
    QString currentUserId;
    QString currentUserPermission;
    
    if (CurrentUser) {
        currentUserId = QString::fromStdString(CurrentUser->getId());
        currentUserPermission = QString::fromStdString(CurrentUser->getPermission());
    }

    QWidget *topBar = new QWidget(page);
    topBar->setFixedHeight(56);
    topBar->setStyleSheet("background: rgba(255, 255, 255, 0.9); border-bottom: 1px solid #e4e7ed;");

    QHBoxLayout *topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(20, 0, 20, 0);
    topLayout->setSpacing(16);

    QPushButton *backBtn = new QPushButton("← 返回", topBar);
    backBtn->setFixedSize(80, 32);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: black;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            color: #409EFF;
            border-color: #409EFF;
        }
    )");

    QObject::connect(backBtn, &QPushButton::clicked, this, [=]() {
        ui->QstackedWidget->setCurrentIndex(1);
    });

    QLabel *title = new QLabel("社团活动管理", topBar);
    title->setStyleSheet("font-size:18px; font-weight:bold; color:black;");
    title->setAlignment(Qt::AlignCenter);

    topLayout->addWidget(backBtn);
    topLayout->addWidget(title);
    topLayout->addStretch();

    rootLayout->addWidget(topBar);

    QWidget *body = new QWidget(page);
    QHBoxLayout *bodyLayout = new QHBoxLayout(body);
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(0);

    QWidget *sidebar = new QWidget(body);
    sidebar->setFixedWidth(200);
    sidebar->setStyleSheet("background: rgba(255, 255, 255, 0.95); border-right: 1px solid #e4e7ed;");

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(0, 20, 0, 0);
    sidebarLayout->setSpacing(0);

    QPushButton *publishBtn = new QPushButton("发布活动", sidebar);
    publishBtn->setFixedHeight(48);
    publishBtn->setStyleSheet(R"(
        QPushButton {
            background: #409EFF;
            color: white;
            border: none;
            text-align: left;
            padding-left: 20px;
            font-size: 14px;
        }
        QPushButton:hover {
            background: #66b1ff;
        }
    )");

    QPushButton *manageBtn = new QPushButton("管理活动", sidebar);
    manageBtn->setFixedHeight(48);
    manageBtn->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #606266;
            border: none;
            text-align: left;
            padding-left: 20px;
            font-size: 14px;
        }
        QPushButton:hover {
            background: #f5f7fa;
            color: #409EFF;
        }
    )");

    sidebarLayout->addWidget(publishBtn);
    sidebarLayout->addWidget(manageBtn);
    sidebarLayout->addStretch();

    QStackedWidget *contentStack = new QStackedWidget(body);

    QWidget *publishPage = new QWidget(contentStack);
    QVBoxLayout *publishLayout = new QVBoxLayout(publishPage);
    publishLayout->setContentsMargins(40, 40, 40, 40);
    publishLayout->setSpacing(24);

    QScrollArea *publishScroll = new QScrollArea(publishPage);
    publishScroll->setWidgetResizable(true);
    publishScroll->setStyleSheet(R"(
        QScrollArea { background: transparent; border: none; }
        QScrollBar:vertical { width: 8px; background: transparent; }
        QScrollBar::handle:vertical { background: #c0c4cc; border-radius: 4px; }
    )");

    QWidget *publishContainer = new QWidget(publishScroll);
    QVBoxLayout *formLayout = new QVBoxLayout(publishContainer);
    formLayout->setContentsMargins(60, 0, 60, 0);
    formLayout->setSpacing(24);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->setSpacing(16);

    QLabel *nameLabel = new QLabel("社团名称：", publishContainer);
    nameLabel->setStyleSheet("font-size:14px; color:black;");
    nameLabel->setFixedWidth(80);

    QLineEdit *nameEdit = new QLineEdit(publishContainer);
    nameEdit->setPlaceholderText("请输入社团名称");
    nameEdit->setStyleSheet(R"(
        QLineEdit {
            color: black; font-size: 14px; padding: 8px 12px;
            border: 1px solid #dcdfe6; border-radius: 4px; background: white;
        }
    )");

    QPushButton *submitBtn = new QPushButton("发布", publishContainer);
    submitBtn->setFixedSize(80, 36);
    submitBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #67C23A; color: white; border: none;
            border-radius: 4px; font-size: 14px; font-weight: 500;
        }
        QPushButton:hover { background-color: #85ce61; }
    )");

    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameEdit);
    nameLayout->addWidget(submitBtn);

    formLayout->addLayout(nameLayout);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    timeLayout->setSpacing(16);

    QLabel *timeLabel = new QLabel("活动时间：", publishContainer);
    timeLabel->setStyleSheet("font-size:14px; color:black;");
    timeLabel->setFixedWidth(80);

    QLineEdit *timeEdit = new QLineEdit(publishContainer);
    timeEdit->setPlaceholderText("例如：2024-12-25 14:00");
    timeEdit->setStyleSheet(R"(
        QLineEdit {
            color: black; font-size: 14px; padding: 8px 12px;
            border: 1px solid #dcdfe6; border-radius: 4px; background: white;
        }
    )");

    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(timeEdit);
    timeLayout->addStretch();

    formLayout->addLayout(timeLayout);

    QLabel *contentLabel = new QLabel("活动内容：", publishContainer);
    contentLabel->setStyleSheet("font-size:14px; color:black;");

    QTextEdit *contentEdit = new QTextEdit(publishContainer);
    contentEdit->setPlaceholderText("请详细描述活动内容，包括活动流程、注意事项、参与要求等...");
    contentEdit->setMinimumHeight(300);
    contentEdit->setStyleSheet(R"(
        QTextEdit {
            color: black; font-size: 14px; padding: 12px;
            border: 1px solid #dcdfe6; border-radius: 6px;
            background: white; line-height: 1.6;
        }
    )");

    formLayout->addWidget(contentLabel);
    formLayout->addWidget(contentEdit);

    formLayout->addStretch();

    publishScroll->setWidget(publishContainer);
    publishLayout->addWidget(publishScroll);

    contentStack->addWidget(publishPage);

    QWidget *managePage = new QWidget(contentStack);
    QVBoxLayout *manageLayout = new QVBoxLayout(managePage);
    manageLayout->setContentsMargins(40, 40, 40, 40);
    manageLayout->setSpacing(20);

    QScrollArea *manageScroll = new QScrollArea(managePage);
    manageScroll->setWidgetResizable(true);
    manageScroll->setStyleSheet(R"(
        QScrollArea { background: transparent; border: none; }
        QScrollBar:vertical { width: 8px; background: transparent; }
        QScrollBar::handle:vertical { background: #c0c4cc; border-radius: 4px; }
    )");

    QWidget *manageContainer = new QWidget(manageScroll);
    QVBoxLayout *cardsLayout = new QVBoxLayout(manageContainer);
    cardsLayout->setContentsMargins(0, 0, 0, 0);
    cardsLayout->setSpacing(20);

    json activityRoot;
    QString activityFilePath = QCoreApplication::applicationDirPath() + "/activity_config.json";
    std::ifstream activityFile(activityFilePath.toStdString());
    if (activityFile.is_open()) {
        activityFile >> activityRoot;
        activityFile.close();
    }

    // 检查用户登录状态（已在开头获取用户信息）
    if (CurrentUser == nullptr) {
        QLabel *emptyLabel = new QLabel("请先登录", manageContainer);
        emptyLabel->setStyleSheet("font-size:16px; color:#909399; margin:50px 0;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        cardsLayout->addWidget(emptyLabel);
        cardsLayout->addStretch();
        manageScroll->setWidget(manageContainer);
        manageLayout->addWidget(manageScroll);
        contentStack->addWidget(managePage);
        bodyLayout->addWidget(sidebar);
        bodyLayout->addWidget(contentStack);
        rootLayout->addWidget(body);
        return;
    }

    // 使用非const迭代器遍历
    for (json::iterator it = activityRoot.begin(); it != activityRoot.end(); ++it) {
        std::string activityKey = it.key();
        json& itemValue = it.value();
        
        if (!itemValue.contains("Association") || !itemValue.contains("Time") || !itemValue.contains("Message")) {
            continue;
        }
        
        std::string assocName = itemValue["Association"].get<std::string>();
        std::string time = itemValue["Time"].get<std::string>();
        std::string message = itemValue["Message"].get<std::string>();

        bool canManage = false;

        if (currentUserPermission == "Admin") {
            canManage = true;
        } else {
            Association *assoc = Association_parsing(assocName);
            if (assoc) {
                std::vector<Student> principals = assoc->getPrincipal();
                for (const auto& p : principals) {
                    if (p.Id == currentUserId.toStdString()) {
                        canManage = true;
                        break;
                    }
                }
                delete assoc;
            }
        }

        if (!canManage) continue;

        QFrame *card = new QFrame(manageContainer);
        card->setStyleSheet(R"(
            QFrame { background: transparent; border: none; padding: 16px; }
        )");

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(0, 0, 0, 0);
        cardLayout->setSpacing(8);

        QHBoxLayout *cardHeader = new QHBoxLayout();
        cardHeader->setSpacing(12);

        QLabel *assocLabel = new QLabel(QString::fromStdString(assocName), card);
        assocLabel->setStyleSheet("font-size:18px; font-weight:bold; color:#409EFF;");

        QPushButton *editBtn = new QPushButton("修改", card);
        editBtn->setFixedSize(60, 28);
        editBtn->setStyleSheet(R"(
            QPushButton { background: #E6A23C; color: white; border: none; border-radius: 4px; font-size: 12px; }
            QPushButton:hover { background: #ebb563; }
        )");

        QPushButton *delBtn = new QPushButton("删除", card);
        delBtn->setFixedSize(60, 28);
        delBtn->setStyleSheet(R"(
            QPushButton { background: #F56C6C; color: white; border: none; border-radius: 4px; font-size: 12px; }
            QPushButton:hover { background: #f78989; }
        )");

        cardHeader->addWidget(assocLabel);
        cardHeader->addStretch();
        cardHeader->addWidget(editBtn);
        cardHeader->addWidget(delBtn);

        QLabel *timeInfo = new QLabel("📅 " + QString::fromStdString(time), card);
        timeInfo->setStyleSheet("font-size:14px; color:#606266;");

        // 活动内容直接显示，不加框
        QLabel *messageLabel = new QLabel(QString::fromStdString(message), card);
        messageLabel->setStyleSheet("font-size:14px; color:#303133; line-height:1.8; padding-top:4px;");
        messageLabel->setWordWrap(true);

        cardLayout->addLayout(cardHeader);
        cardLayout->addWidget(timeInfo);
        cardLayout->addWidget(messageLabel);

        cardsLayout->addWidget(card);

        // 删除按钮 - 使用值捕获，避免引用已销毁的局部变量
        QObject::connect(delBtn, &QPushButton::clicked, this, [this, activityKey, activityFilePath, page]() {
            // 设置样式的确认删除弹窗
            QMessageBox confirmBox;
            confirmBox.setWindowTitle("确认删除");
            confirmBox.setText("确定要删除这个活动吗？");
            confirmBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            confirmBox.setDefaultButton(QMessageBox::No);
            confirmBox.setStyleSheet(R"(
                QMessageBox { background-color: white; }
                QMessageBox QLabel { color: #303133; font-size: 14px; }
                QMessageBox QPushButton { 
                    background-color: #409EFF; 
                    color: white; 
                    border: none; 
                    border-radius: 4px; 
                    padding: 6px 16px; 
                    min-width: 80px;
                }
                QMessageBox QPushButton:hover { background-color: #66b1ff; }
            )");
            
            if (confirmBox.exec() != QMessageBox::Yes) {
                return;
            }

            // 重新读取最新的活动数据，避免引用已销毁的局部变量
            json activityRoot;
            std::ifstream inFile(activityFilePath.toStdString());
            if (!inFile.is_open()) {
                QMessageBox::warning(page, "提示", "读取文件失败！");
                return;
            }
            inFile >> activityRoot;
            inFile.close();

            // 删除活动
            activityRoot.erase(activityKey);
            
            std::ofstream outFile(activityFilePath.toStdString());
            bool writeSuccess = false;
            if (outFile.is_open()) {
                outFile << activityRoot.dump(4) << std::endl;
                outFile.close();
                writeSuccess = true;
            }

            if (writeSuccess) {
                QMessageBox msgBox;
                msgBox.setWindowTitle("提示");
                msgBox.setText("活动已删除！");
                msgBox.setStyleSheet(R"(
                    QMessageBox { background-color: white; }
                    QMessageBox QLabel { color: #303133; font-size: 14px; }
                    QMessageBox QPushButton { 
                        background-color: #409EFF; 
                        color: white; 
                        border: none; 
                        border-radius: 4px; 
                        padding: 6px 16px; 
                        min-width: 80px;
                    }
                    QMessageBox QPushButton:hover { background-color: #66b1ff; }
                )");
                msgBox.exec();
                
                // 使用 QTimer::singleShot 延迟刷新，确保弹窗完全关闭后再刷新
                QTimer::singleShot(100, this, [this]() {
                    // 刷新活动管理页面
                    QLayout *oldLayout = ui->page_6->layout();
                    if (oldLayout) {
                        QLayoutItem *item;
                        while ((item = oldLayout->takeAt(0)) != nullptr) {
                            if (item->widget()) delete item->widget();
                            delete item;
                        }
                        delete oldLayout;
                    }
                    setupPage6UI(ui->page_6);
                    
                    // 同时刷新主页面
                    QLayout *oldLayout2 = ui->page_2->layout();
                    if (oldLayout2) {
                        QLayoutItem *item;
                        while ((item = oldLayout2->takeAt(0)) != nullptr) {
                            if (item->widget()) delete item->widget();
                            delete item;
                        }
                        delete oldLayout2;
                    }
                    setupPage2UI(ui->page_2);
                });
            } else {
                QMessageBox msgBox;
                msgBox.setWindowTitle("提示");
                msgBox.setText("删除失败！");
                msgBox.setStyleSheet(R"(
                    QMessageBox { background-color: white; }
                    QMessageBox QLabel { color: #303133; font-size: 14px; }
                    QMessageBox QPushButton { 
                        background-color: #F56C6C; 
                        color: white; 
                        border: none; 
                        border-radius: 4px; 
                        padding: 6px 16px; 
                        min-width: 80px;
                    }
                    QMessageBox QPushButton:hover { background-color: #f78989; }
                )");
                msgBox.exec();
            }
        });

        // 捕获 activityKey 的副本，activityRoot 通过引用捕获
        QObject::connect(editBtn, &QPushButton::clicked, this, [this, activityKey, activityFilePath, time, message]() {
            QDialog *editDialog = new QDialog();
            editDialog->setWindowTitle("修改活动");
            editDialog->setFixedSize(400, 300);

            QVBoxLayout *dialogLayout = new QVBoxLayout(editDialog);
            dialogLayout->setContentsMargins(20, 20, 20, 20);
            dialogLayout->setSpacing(16);

            QLineEdit *timeInput = new QLineEdit(QString::fromStdString(time), editDialog);
            timeInput->setPlaceholderText("活动时间");
            timeInput->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid #dcdfe6; border-radius: 4px; }");

            QTextEdit *contentInput = new QTextEdit(QString::fromStdString(message), editDialog);
            contentInput->setPlaceholderText("活动内容");
            contentInput->setMinimumHeight(150);
            contentInput->setStyleSheet("QTextEdit { padding: 8px; border: 1px solid #dcdfe6; border-radius: 4px; }");

            QPushButton *saveBtn = new QPushButton("保存", editDialog);
            saveBtn->setFixedSize(80, 32);
            saveBtn->setStyleSheet("QPushButton { background: #409EFF; color: white; border: none; border-radius: 4px; }");

            dialogLayout->addWidget(new QLabel("活动时间："));
            dialogLayout->addWidget(timeInput);
            dialogLayout->addWidget(new QLabel("活动内容："));
            dialogLayout->addWidget(contentInput);
            dialogLayout->addWidget(saveBtn, 0, Qt::AlignRight);

            // 保存按钮使用值捕获
            QObject::connect(saveBtn, &QPushButton::clicked, this, [this, activityKey, activityFilePath, timeInput, contentInput, editDialog]() {
                // 获取输入内容
                QString newTime = timeInput->text().trimmed();
                QString newContent = contentInput->toPlainText().trimmed();

                if (newTime.isEmpty() || newContent.isEmpty()) {
                    QMessageBox::warning(editDialog, "提示", "请填写完整信息");
                    return;
                }

                // 读取最新的活动数据
                json activityRoot;
                std::ifstream inFile(activityFilePath.toStdString());
                if (inFile.is_open()) {
                    inFile >> activityRoot;
                    inFile.close();
                }

                // 更新活动信息
                activityRoot[activityKey]["Time"] = newTime.toStdString();
                activityRoot[activityKey]["Message"] = newContent.toStdString();

                // 写入文件
                std::ofstream outFile(activityFilePath.toStdString());
                bool writeSuccess = false;
                if (outFile.is_open()) {
                    outFile << activityRoot.dump(4) << std::endl;
                    outFile.close();
                    writeSuccess = true;
                }

                if (writeSuccess) {
                    QMessageBox::information(editDialog, "提示", "活动已修改！");
                    editDialog->accept(); // 使用 accept() 关闭对话框

                    // 使用 QTimer::singleShot 延迟刷新，确保对话框完全关闭后再刷新
                    QTimer::singleShot(100, this, [this]() {
                        QLayout *oldLayout = ui->page_6->layout();
                        if (oldLayout) {
                            QLayoutItem *item;
                            while ((item = oldLayout->takeAt(0)) != nullptr) {
                                if (item->widget()) delete item->widget();
                                delete item;
                            }
                            delete oldLayout;
                        }
                        setupPage6UI(ui->page_6);
                        
                        // 同时刷新主页面
                        QLayout *oldLayout2 = ui->page_2->layout();
                        if (oldLayout2) {
                            QLayoutItem *item;
                            while ((item = oldLayout2->takeAt(0)) != nullptr) {
                                if (item->widget()) delete item->widget();
                                delete item;
                            }
                            delete oldLayout2;
                        }
                        setupPage2UI(ui->page_2);
                    });
                } else {
                    QMessageBox::warning(editDialog, "提示", "修改失败！");
                }
            });

            editDialog->exec();
        });
    }

    if (cardsLayout->count() == 0) {
        QLabel *emptyLabel = new QLabel("暂无活动", manageContainer);
        emptyLabel->setStyleSheet("font-size:16px; color:#909399; margin:50px 0;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        cardsLayout->addWidget(emptyLabel);
    }

    cardsLayout->addStretch();

    manageScroll->setWidget(manageContainer);
    manageLayout->addWidget(manageScroll);

    contentStack->addWidget(managePage);

    bodyLayout->addWidget(sidebar);
    bodyLayout->addWidget(contentStack);

    rootLayout->addWidget(body);

    QObject::connect(publishBtn, &QPushButton::clicked, this, [=]() {
        contentStack->setCurrentIndex(0);
        publishBtn->setStyleSheet(R"(
            QPushButton { background: #409EFF; color: white; border: none; text-align: left; padding-left: 20px; font-size: 14px; }
            QPushButton:hover { background: #66b1ff; }
        )");
        manageBtn->setStyleSheet(R"(
            QPushButton { background: transparent; color: #606266; border: none; text-align: left; padding-left: 20px; font-size: 14px; }
            QPushButton:hover { background: #f5f7fa; color: #409EFF; }
        )");
    });

    QObject::connect(manageBtn, &QPushButton::clicked, this, [=]() {
        contentStack->setCurrentIndex(1);
        manageBtn->setStyleSheet(R"(
            QPushButton { background: #409EFF; color: white; border: none; text-align: left; padding-left: 20px; font-size: 14px; }
            QPushButton:hover { background: #66b1ff; }
        )");
        publishBtn->setStyleSheet(R"(
            QPushButton { background: transparent; color: #606266; border: none; text-align: left; padding-left: 20px; font-size: 14px; }
            QPushButton:hover { background: #f5f7fa; color: #409EFF; }
        )");
    });

    QObject::connect(submitBtn, &QPushButton::clicked, this, [=]() {
        QString assocName = nameEdit->text().trimmed();
        QString activityTime = timeEdit->text().trimmed();
        QString activityContent = contentEdit->toPlainText().trimmed();

        if (assocName.isEmpty()) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("请输入社团名称");
            msgBox.setStyleSheet(R"(
                QMessageBox { background-color: black; }
                QMessageBox QLabel { color: white; }
                QMessageBox QPushButton { background-color: #409EFF; color: white; border: none; border-radius: 4px; padding: 6px 16px; }
            )");
            msgBox.exec();
            return;
        }

        json assocRoot;
        json *assocObj = findJsonObject(1, assocName.toStdString(), assocRoot);
        if (!assocObj) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("不存在该社团");
            msgBox.setStyleSheet(R"(
                QMessageBox { background-color: black; }
                QMessageBox QLabel { color: white; }
                QMessageBox QPushButton { background-color: #409EFF; color: white; border: none; border-radius: 4px; padding: 6px 16px; }
            )");
            msgBox.exec();
            return;
        }

        bool isPrincipal = false;
        if (currentUserPermission == "Admin") {
            isPrincipal = true;
        } else {
            Association *assoc = Association_parsing(assocName.toStdString());
            if (assoc) {
                std::vector<Student> principals = assoc->getPrincipal();
                for (const auto& p : principals) {
                    if (p.Id == currentUserId.toStdString()) {
                        isPrincipal = true;
                        break;
                    }
                }
                delete assoc;
            }
        }

        if (!isPrincipal) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("无权限发布该社团活动");
            msgBox.setStyleSheet(R"(
                QMessageBox { background-color: black; }
                QMessageBox QLabel { color: white; }
                QMessageBox QPushButton { background-color: #409EFF; color: white; border: none; border-radius: 4px; padding: 6px 16px; }
            )");
            msgBox.exec();
            return;
        }

        ActivityMessage activityMsg;
        activityMsg.Association = assocName.toStdString();
        activityMsg.Time = activityTime.toStdString();
        activityMsg.Message = activityContent.toStdString();

        bool success = write_activity_json_config(&activityMsg);
        if (success) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("活动发布成功");
            msgBox.setStyleSheet(R"(
                QMessageBox { background-color: black; }
                QMessageBox QLabel { color: white; }
                QMessageBox QPushButton { background-color: #409EFF; color: white; border: none; border-radius: 4px; padding: 6px 16px; }
            )");
            msgBox.exec();
            
            nameEdit->clear();
            timeEdit->clear();
            contentEdit->clear();
            
            // 刷新页面6
            QLayout *oldLayout6 = ui->page_6->layout();
            if (oldLayout6) {
                QLayoutItem *item;
                while ((item = oldLayout6->takeAt(0)) != nullptr) {
                    if (item->widget()) delete item->widget();
                    delete item;
                }
                delete oldLayout6;
            }
            setupPage6UI(ui->page_6);
            
            // 刷新页面2（主页面）
            QLayout *oldLayout2 = ui->page_2->layout();
            if (oldLayout2) {
                QLayoutItem *item;
                while ((item = oldLayout2->takeAt(0)) != nullptr) {
                    if (item->widget()) delete item->widget();
                    delete item;
                }
                delete oldLayout2;
            }
            setupPage2UI(ui->page_2);
        } else {
            QMessageBox msgBox;
            msgBox.setWindowTitle("提示");
            msgBox.setText("活动发布失败");
            msgBox.setStyleSheet(R"(
                QMessageBox { background-color: black; }
                QMessageBox QLabel { color: white; }
                QMessageBox QPushButton { background-color: #409EFF; color: white; border: none; border-radius: 4px; padding: 6px 16px; }
            )");
            msgBox.exec();
        }
    });
}

void MainWindow::setupPage7UI(QWidget *page)
{
    // ===== 页面根布局 =====
    QVBoxLayout *rootLayout = qobject_cast<QVBoxLayout*>(page->layout());
    if (!rootLayout) {
        rootLayout = new QVBoxLayout(page);
    }
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // ✅ 80% 透明背景
    page->setStyleSheet("background: rgba(255, 255, 255, 0.8);");

    // ===== 顶部导航栏 =====
    QWidget *topBar = new QWidget(page);
    topBar->setFixedHeight(56);
    topBar->setStyleSheet("background: rgba(255, 255, 255, 0.9); border-bottom: 1px solid #e4e7ed;");

    QHBoxLayout *topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(20, 0, 20, 0);
    topLayout->setSpacing(16);

    // 返回按钮
    QPushButton *backBtn = new QPushButton("← 返回", topBar);
    backBtn->setFixedSize(80, 32);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: black;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            font-size: 13px;
        }
        QPushButton:hover {
            color: #409EFF;
            border-color: #409EFF;
        }
    )");

    QObject::connect(backBtn, &QPushButton::clicked, this, [=]() {
        ui->QstackedWidget->setCurrentIndex(1);
        qDebug() << "返回上一页";
        // stackedWidget->setCurrentIndex(0);
    });

    // 标题
    QLabel *title = new QLabel("系统管理", topBar);
    title->setStyleSheet("font-size:18px; font-weight:bold; color:black;");
    title->setAlignment(Qt::AlignCenter);

    topLayout->addWidget(backBtn);
    topLayout->addWidget(title);
    topLayout->addStretch();

    rootLayout->addWidget(topBar);

    // ===== 主体布局 =====
    QWidget *body = new QWidget(page);
    QHBoxLayout *bodyLayout = new QHBoxLayout(body);
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(0);

    // ===== 左侧选项栏 =====
    QWidget *sidebar = new QWidget(body);
    sidebar->setFixedWidth(200);
    sidebar->setStyleSheet("background: rgba(255, 255, 255, 0.95); border-right: 1px solid #e4e7ed;");

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(0, 20, 0, 0);
    sidebarLayout->setSpacing(0);

    // 用户管理按钮
    QPushButton *userBtn = new QPushButton("用户管理", sidebar);
    userBtn->setFixedHeight(50);
    userBtn->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            border: none;
            text-align: left;
            padding-left: 30px;
            font-size: 15px;
            color: #606266;
        }
        QPushButton:hover {
            color: #409EFF;
            background: #ecf5ff;
        }
        QPushButton:checked {
            color: #409EFF;
            background: #ecf5ff;
            border-left: 3px solid #409EFF;
        }
    )");
    userBtn->setCheckable(true);
    userBtn->setChecked(true);

    // 社团管理按钮
    QPushButton *clubBtn = new QPushButton("社团管理", sidebar);
    clubBtn->setFixedHeight(50);
    clubBtn->setStyleSheet(userBtn->styleSheet());
    clubBtn->setCheckable(true);

    sidebarLayout->addWidget(userBtn);
    sidebarLayout->addWidget(clubBtn);
    sidebarLayout->addStretch();

    // ===== 右侧内容区 =====
    QStackedWidget *stackedWidget = new QStackedWidget(body);
    stackedWidget->addWidget(createUserManagePage(stackedWidget));
    stackedWidget->addWidget(createClubManagePage(stackedWidget));

    // 按钮点击事件
    QObject::connect(userBtn, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(0);
        userBtn->setChecked(true);
        clubBtn->setChecked(false);
    });

    QObject::connect(clubBtn, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(1);
        clubBtn->setChecked(true);
        userBtn->setChecked(false);
    });

    bodyLayout->addWidget(sidebar);
    bodyLayout->addWidget(stackedWidget);
    rootLayout->addWidget(body);
}

// 创建用户管理页面
QWidget* MainWindow::createUserManagePage(QWidget *parent)
{
    QWidget *page = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(40, 30, 40, 30);
    layout->setSpacing(20);

    // 标题
    QLabel *title = new QLabel("用户管理", page);
    title->setStyleSheet("font-size:20px; font-weight:bold; color:black;");
    layout->addWidget(title);

    // 表单区域
    QFormLayout *form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->setSpacing(16);
    form->setContentsMargins(0, 10, 0, 10);

    // 设置标签固定宽度，确保对齐
    form->setHorizontalSpacing(20);

    // 姓名
    QLabel *nameLabel = new QLabel("姓　名：", page);
    nameLabel->setStyleSheet("color:black; font-size:14px;");
    nameLabel->setFixedWidth(100);
    
    QLineEdit *nameEdit = new QLineEdit(page);
    nameEdit->setPlaceholderText("请输入姓名");
    nameEdit->setStyleSheet(R"(
        QLineEdit {
            color: black;
            font-size: 14px;
            padding: 8px 12px;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            background: white;
        }
    )");
    form->addRow(nameLabel, nameEdit);

    // 学号
    QLabel *sidLabel = new QLabel("学　号：", page);
    sidLabel->setStyleSheet("color:black; font-size:14px;");
    sidLabel->setFixedWidth(100);
    
    QLineEdit *sidEdit = new QLineEdit(page);
    sidEdit->setPlaceholderText("请输入学号");
    sidEdit->setStyleSheet(nameEdit->styleSheet());
    form->addRow(sidLabel, sidEdit);

    // 密码
    QLabel *pwdLabel = new QLabel("密　码：", page);
    pwdLabel->setStyleSheet("color:black; font-size:14px;");
    pwdLabel->setFixedWidth(100);
    
    QLineEdit *pwdEdit = new QLineEdit(page);
    pwdEdit->setPlaceholderText("请输入密码");
    pwdEdit->setEchoMode(QLineEdit::Password);
    pwdEdit->setStyleSheet(nameEdit->styleSheet());
    form->addRow(pwdLabel, pwdEdit);

    // 权限下拉菜单
    QLabel *roleLabel = new QLabel("权　限：", page);
    roleLabel->setStyleSheet("color:black; font-size:14px;");
    roleLabel->setFixedWidth(100);
    
    QComboBox *roleCombo = new QComboBox(page);
    roleCombo->addItems({"用户", "社长", "管理员"});
    roleCombo->setStyleSheet(R"(
        QComboBox {
            color: black;
            font-size: 14px;
            padding: 8px 12px;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            background: white;
            min-width: 120px;
        }
        QComboBox::drop-down {
            border: none;
        }
        QComboBox QAbstractItemView {
            color: black;
            background: white;
            selection-background-color: #ecf5ff;
        }
    )");
    form->addRow(roleLabel, roleCombo);

    layout->addLayout(form);

    // 提示文字
    QLabel *tip = new QLabel("提示：若用户不存在则将新增用户", page);
    tip->setStyleSheet("font-size:13px; color:#909399; margin-top:10px;");
    layout->addWidget(tip);

    // 修改按钮
    QPushButton *modifyBtn = new QPushButton("修 改", page);
    modifyBtn->setFixedSize(120, 40);
    modifyBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #409EFF;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 15px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #66b1ff;
        }
    )");
    connect(modifyBtn, &QPushButton::clicked, this, [=]() {
        std::string name = nameEdit->text().trimmed().toStdString();
        std::string sid = sidEdit->text().trimmed().toStdString();
        std::string pwd = pwdEdit->text().toStdString();
        std::string role = roleCombo->currentText().toStdString();
        if (role == "用户") role = "User";
        else if (role == "社长") role = "Principal";
        else if (role == "管理员") role = "Admin";
        if (name.empty() || sid.empty() || pwd.empty()) {
            qDebug() << "请完整填写用户信息";
            return;
        }
        Member *newUser = new Member(name, sid,{},{},pwd, role);
        bool success = write_user_json_config(newUser,role);
        if (success) {
            QMessageBox::warning(this,"提示","写入用户信息成功！");
        }
        else {
            QMessageBox::information(this,"提示","用户信息修改失败！");
        }
    });
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(modifyBtn);
    btnLayout->addStretch();
    layout->addLayout(btnLayout);

    layout->addStretch();

    return page;
}

// 创建社团管理页面
QWidget* MainWindow::createClubManagePage(QWidget *parent)
{
    QWidget *page = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(40, 30, 40, 30);
    layout->setSpacing(20);

    // 标题
    QLabel *title = new QLabel("社团管理", page);
    title->setStyleSheet("font-size:20px; font-weight:bold; color:black;");
    layout->addWidget(title);

    // 表单区域
    QFormLayout *form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->setSpacing(16);
    form->setContentsMargins(0, 10, 0, 10);

    // 设置标签固定宽度，确保对齐（与用户管理页面一致）
    form->setHorizontalSpacing(20);

    // 社团名称
    QLabel *nameLabel = new QLabel("社团名称：", page);
    nameLabel->setStyleSheet("color:black; font-size:14px;");
    nameLabel->setFixedWidth(100);
    
    QLineEdit *nameEdit = new QLineEdit(page);
    nameEdit->setPlaceholderText("请输入社团名称");
    nameEdit->setStyleSheet(R"(
        QLineEdit {
            color: black;
            font-size: 14px;
            padding: 8px 12px;
            border: 1px solid #dcdfe6;
            border-radius: 4px;
            background: white;
        }
    )");
    form->addRow(nameLabel, nameEdit);
    QLabel *FoundtimeLabel = new QLabel("成立时间：", page);
    FoundtimeLabel->setStyleSheet("color:black; font-size:14px;");
    FoundtimeLabel->setFixedWidth(100);
    
    QLineEdit *FoundtimeEdit = new QLineEdit(page);
    FoundtimeEdit->setPlaceholderText("请输入成立时间");
    FoundtimeEdit->setStyleSheet(nameEdit->styleSheet());
    form->addRow(FoundtimeLabel, FoundtimeEdit);
    // 社团宗旨
    QLabel *purposeLabel = new QLabel("社团宗旨：", page);
    purposeLabel->setStyleSheet("color:black; font-size:14px;");
    purposeLabel->setFixedWidth(100);
    
    QLineEdit *purposeEdit = new QLineEdit(page);
    purposeEdit->setPlaceholderText("请输入社团宗旨");
    purposeEdit->setStyleSheet(nameEdit->styleSheet());
    form->addRow(purposeLabel, purposeEdit);

    // 社长
    QLabel *presidentLabel = new QLabel("社　长：", page);
    presidentLabel->setStyleSheet("color:black; font-size:14px;");
    presidentLabel->setFixedWidth(100);
    
    QLineEdit *presidentEdit = new QLineEdit(page);
    presidentEdit->setPlaceholderText("请输入社长姓名");
    presidentEdit->setStyleSheet(nameEdit->styleSheet());
    form->addRow(presidentLabel, presidentEdit);

    // ✅ 社长学号（新增）
    QLabel *presidentSidLabel = new QLabel("社长学号：", page);
    presidentSidLabel->setStyleSheet("color:black; font-size:14px;");
    presidentSidLabel->setFixedWidth(100);
    
    QLineEdit *presidentSidEdit = new QLineEdit(page);
    presidentSidEdit->setPlaceholderText("请输入社长学号");
    presidentSidEdit->setStyleSheet(nameEdit->styleSheet());
    form->addRow(presidentSidLabel, presidentSidEdit);

    layout->addLayout(form);

    // 提示文字
    QLabel *tip = new QLabel("提示：若社团不存在则将新增社团", page);
    tip->setStyleSheet("font-size:13px; color:#909399; margin-top:10px;");
    layout->addWidget(tip);

    // 修改按钮
    QPushButton *modifyBtn = new QPushButton("修 改", page);
    modifyBtn->setFixedSize(120, 40);
    modifyBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #409EFF;
            color: white;
            border: none;
            border-radius: 4px;
            font-size: 15px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #66b1ff;
        }
    )");
    connect(modifyBtn, &QPushButton::clicked, this, [=]() {
        std::string Association_name = nameEdit->text().trimmed().toStdString();
        std::string time = FoundtimeEdit->text().trimmed().toStdString();
        std::string Association_Description = purposeEdit->text().trimmed().toStdString();
        std::string president = presidentEdit->text().toStdString();
        std::string president_id = presidentSidEdit->text().trimmed().toStdString();

        if (Association_name.empty() || Association_Description.empty() || president.empty() || president_id.empty()) {
            qDebug() << "请完整填写社团信息";
            return;
        }
        Student *president_member = new Student(president, president_id);
        Association *newAssociation = new Association(Association_name, time, Association_Description, {}, { *president_member });
        bool success = write_association_json_config(newAssociation);
        if (!success) {
            QMessageBox::warning(this,"提示","写入社团信息成功！");
            return;
        }
        else {
            QMessageBox::information(this,"提示","社团信息修改失败！");
        }
    });
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(modifyBtn);
    btnLayout->addStretch();
    layout->addLayout(btnLayout);

    layout->addStretch();

    return page;
}