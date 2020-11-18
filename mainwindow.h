#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

#include "importnew.h"
#include "addlights.h"

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
    /** 删除大类和下面子模型
      * 或者删除单个模型 */
    void slotTreeMenuRemoveType(bool checked = false);
    /** 增加光源 */
    void slotTreeMenuAddLightType(bool checked = false);
    /** 增加模型 */
    void slotTreeMenuAddModelType(bool checked = false);
    void on_fold_clicked();

private:
    Ui::MainWindow *ui;
    ImportNew *importnew;
    AddLights *addlights;
    bool fold = false;  
    QStandardItemModel* mModel;
    /** 树控件第一级节点表 */
    std::map<std::string,QStandardItem*> itemFirstmap;
    /** 树控件灯光节点匹配 */
    std::map<std::string,QList<QStandardItem*>> itemLightmap;
    /** 树控件模型节点匹配 */
    std::map<std::string,QList<QStandardItem*>> itemModelmap;
};
#endif // MAINWINDOW_H
