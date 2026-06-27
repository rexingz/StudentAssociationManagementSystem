#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qtimer>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QTableWidget>
#include "Member.h"
#include "Association.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:
    void on_pushButton_clicked();
    
private:
    void setupPage2UI(QWidget *page);
    void setupPage3UI(QWidget *page);
    void setupPage4UI(QWidget *page);
    void setupPage5UI(QWidget *page);
    void setupPage6UI(QWidget *page);
    QFrame* createClubCard(const QString &name, const QString &purpose, 
                          const QString &time, const QStringList &members, 
                          const QStringList &principals, double funds,
                          const std::vector<Equipment>& equipments,
                          const std::vector<Transaction>& transactions,
                          QWidget *parent);
    void showAddMemberDialog(QWidget *parent, const QString &associationName);
    void removeMemberFromAssociation(const QString &associationName, const QString &memberId, const QString &memberName);
    void promoteToPrincipal(const QString &associationName, const QString &memberId, const QString &memberName);
    void removePrincipalFromAssociation(const QString &associationName, const QString &principalId, const QString &principalName);
    void showAddEquipmentDialog(QWidget *parent, const QString &associationName);
    void removeEquipmentFromAssociation(const QString &associationName, int index);
    void showAddFundsDialog(QWidget *parent, const QString &associationName, const QString &type);
    void showBillsDialog(QWidget *parent, const QString &associationName, 
                         const std::vector<Transaction>& transactions, double currentFunds);
    void setupPage7UI(QWidget *page);
    QWidget* createUserManagePage(QWidget *parent);
    QWidget* createClubManagePage(QWidget *parent);
    void createMenus();
    Member* CurrentUser;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
