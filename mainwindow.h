#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "importnew.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);    
    void InitTree();
    ~MainWindow();

private slots:    
    void slotTreeMenu(const QPoint &pos);
    void on_actionnew_import_triggered();
    void slotTreeMenuExpand(bool checked = false);
    void slotTreeMenuCollapse(bool checked = false);
    void slotTreeMenuAddType(bool checked = false);
    void slotTreeMenuRemoveType(bool checked = false);

    void on_fold_clicked();

private:
    Ui::MainWindow *ui;
    ImportNew *importnew;
    bool fold = false;  
    QStandardItemModel* mModel;
    std::map<std::string,QStandardItem*> itemmap;
};
#endif // MAINWINDOW_H
