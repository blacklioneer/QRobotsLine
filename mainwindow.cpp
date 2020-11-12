#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "variables.h"
//#include<algorithm>
#include <QFileDialog>
#include <QInputDialog>
#include <iostream>
#include <QDebug>
#include <QMessageBox>
#include <map>
#define ROLE_MARK Qt::UserRole + 1
#define MARK_FOLDER 2 //这是文件夹标记
#define MARK_ITEM 3 //条目标记


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    InitTree();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::InitTree()
{
    //1，QTreeView常用设置项
    QTreeView* t = ui->treeView;
    t->setEditTriggers(QTreeView::NoEditTriggers);			//单元格不能编辑
    t->setSelectionBehavior(QTreeView::SelectRows);			//一次选中整行
    t->setSelectionMode(QTreeView::SingleSelection);        //单选，配合上面的整行就是一次选单行
    t->setAlternatingRowColors(true);                       //每间隔一行颜色不一样，当有qss时该属性无效
    t->setFocusPolicy(Qt::NoFocus);                         //去掉鼠标移到单元格上时的虚线框

    //2，列头相关设置
    t->header()->setHighlightSections(true);                //列头点击时字体变粗，去掉该效果
    t->header()->setDefaultAlignment(Qt::AlignCenter);      //列头文字默认居中对齐
    t->header()->setDefaultSectionSize(100);                //默认列宽100
    t->header()->setStretchLastSection(true);               //最后一列自适应宽度
    t->header()->setSortIndicator(0,Qt::AscendingOrder);    //按第1列升序排序
    //3，构造Model
            //设置列头
            QStringList headers;
            headers << QStringLiteral("标签")
                    << QStringLiteral("待定");
            mModel = new QStandardItemModel(ui->treeView);
            mModel->setHorizontalHeaderLabels(headers);
            //设置数据//遍历父类容器，配置树节点主控件
            for(vector<string>::iterator it = g_parent_type.begin(); it!=g_parent_type.end();it++){

                QStandardItem* itemMain = new QStandardItem(QString::fromLocal8Bit((*it).c_str()));
                itemMain->setData(MARK_FOLDER,ROLE_MARK);
                itemmap[(*it)] = itemMain; //数据map将名字与 树控件单元进行匹配创建字典
                mModel->appendRow(itemMain);

            }
            //添加点光源 Light1231313
            QStandardItem* itemLight = new QStandardItem(QStringLiteral("光源"));
            itemLight->setData(MARK_FOLDER,ROLE_MARK);
            itemmap["Light"] = itemLight; //数据map将名字与 树控件单元进行匹配创建字典
            mModel->appendRow(itemLight);
            ////遍历物体容器，配置树节点主控件
            for(vector<ObjectData>::iterator it = g_objects.begin(); it!=g_objects.end();it++){

                QList<QStandardItem*> items;
                QStandardItem* itemName = new QStandardItem(QString::fromLocal8Bit((*it).name.c_str()));
                QStandardItem* itemHigher = new QStandardItem(QString::fromLocal8Bit((*it).higher_control.c_str()));
                QStandardItem* itemPath = new QStandardItem(QString::fromLocal8Bit((*it).path.c_str()));
                items << itemName << itemHigher << itemPath;
                items[0]->setData(MARK_ITEM,ROLE_MARK);
                //判断是否为光源
                if((*it).isLight){
                     itemmap["Light"]->appendRow(items);
                }
                else{
                     itemmap[(*it).parent_type]->appendRow(items);    //二级节点挂在一级的第1列节点上
                }


            }
//            for(int i=0;i<5;i++)
//            {
//                //一级节点：年级，只设第1列的数据，第2、3列将显示为空白
//                QStandardItem* itemGrade = new QStandardItem(QStringLiteral("%1年级").arg(i+1));
//                itemGrade->setData(MARK_FOLDER,ROLE_MARK);
//                mModel->appendRow(itemGrade);       //一级节点挂在model上

//                for(int j=0;j<3;j++)
//                {
//                    //二级节点：班级、姓名、分数
//                    QList<QStandardItem*> items;
//                    QStandardItem* itemClass = new QStandardItem(QStringLiteral("%1班").arg(j+1));
//                    QStandardItem* itemName = new QStandardItem("Tom");
//                    QStandardItem* itemScore = new QStandardItem("100");
//                    items << itemClass << itemName << itemScore;
//                    items[0]->setData(MARK_ITEM,ROLE_MARK);
//                    itemGrade->appendRow(items);    //二级节点挂在一级的第1列节点上
//                }
//            }

    //4，应用model
    t->setModel(mModel);


    //5, 信号槽，右键菜单
    t->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(t, &QTreeView::customContextMenuRequested, this, &MainWindow::slotTreeMenu);
}


void MainWindow::on_actionnew_import_triggered()
{
    importnew = new ImportNew(this);
    importnew->setModal(true);
    importnew->exec();

}

void MainWindow::on_fold_clicked()
{
    if (!fold){
        ui->treeView->setVisible(fold);
        ui->fold->setIcon(QIcon(":/textures/arrow_forward.png"));
    }
    else{
        ui->treeView->setVisible(fold);
        ui->fold->setIcon(QIcon(":/textures/arrow_back.png"));
    }
    fold = !fold;
}


void MainWindow::slotTreeMenu(const QPoint &pos)
{
    QString qss = "QMenu{color:#E8E8E8;background:#4D4D4D;margin:2px;}\
                QMenu::item{padding:3px 20px 3px 20px;}\
                QMenu::indicator{width:13px;height:13px;}\
                QMenu::item:selected{color:#E8E8E8;border:0px solid #575757;background:#1E90FF;}\
                QMenu::separator{height:1px;background:#757575;}";

    QMenu menu;
    menu.setStyleSheet(qss);    //可以给菜单设置样式

    QModelIndex curIndex = ui->treeView->indexAt(pos);      //当前点击的元素的index
    QModelIndex index = curIndex.sibling(curIndex.row(),0); //该行的第1列元素的index
    QVariant var = index.data(ROLE_MARK);

    if (var.isValid())
    {
        //可获取元素的文本、data,进行其他判断处理
        //QStandardItem* item = mModel->itemFromIndex(index);
        //QString text = item->text();
        //QVariant data = item->data(Qt::UserRole + 1);
        //...

        //添加一行菜单，进行展开
        if(MARK_FOLDER == var.toInt()){
        menu.addAction(QStringLiteral("展开"), this, SLOT(slotTreeMenuExpand(bool)));
        menu.addSeparator();    //添加一个分隔线
        menu.addAction(QStringLiteral("折叠"), this, SLOT(slotTreeMenuCollapse(bool)));
        menu.addSeparator();
        menu.addAction(QStringLiteral("添加"), this, SLOT(slotTreeMenuAddType(bool)));
        menu.addSeparator();
        menu.addAction(QStringLiteral("删除"), this, SLOT(slotTreeMenuRemoveType(bool)));
        }
        else if(MARK_ITEM == var.toInt()){
        menu.addAction(QStringLiteral("删除"), this, SLOT(slotTreeMenuRemoveType(bool)));
        }
    }
    else{
         menu.addAction(QStringLiteral("添加"), this, SLOT(slotTreeMenuAddType(bool)));
    }
//    else{
//        menu.addAction("新建大类",this ,SlOT(slotTreeMenuAddType(bool)));
//    }
    menu.exec(QCursor::pos());  //显示菜单
}


void MainWindow::slotTreeMenuExpand(bool checked)
{
    QModelIndex curIndex = ui->treeView->currentIndex();
    if(curIndex.isValid())
    {
        ui->treeView->expand(curIndex);

    }
}

void MainWindow::slotTreeMenuAddType(bool checked)
{
    QString text = QInputDialog::getText(this,QStringLiteral("新建父类型"),QStringLiteral("输入"));
    if (std::find_if(g_parent_type.begin(),g_parent_type.end(), [=](string const& obj){
                     return obj == text.toUtf8().toStdString();
                 })== g_parent_type.end()){    //如果输入的值不存在重复
        QStandardItem* itemtype = new QStandardItem(text);
        itemtype->setData(MARK_FOLDER,ROLE_MARK);//一级节点挂在model上
        itemmap[text.toStdString()] = itemtype; //map 添加
        mModel->appendRow(itemtype);
        g_parent_type.emplace_back(text.toUtf8().toStdString());
        for(vector<string>::iterator it = g_parent_type.begin(); it!=g_parent_type.end();it++){
            cout<<(*it)<< endl;
        }
    }
    else{
        QMessageBox::critical(NULL, QStringLiteral("错误"), text+QStringLiteral("已存在"), QMessageBox::Yes | QMessageBox::No,
                                                  QMessageBox::Yes);
    }


}

void MainWindow::slotTreeMenuRemoveType(bool checked)
{
    QModelIndex curIndex = ui->treeView->currentIndex();      //当前点击的元素的index
    QModelIndex index = curIndex.sibling(curIndex.row(),0); //该行的第1列元素的index
    QVariant var = index.data(ROLE_MARK);

    if (var.isValid())
    {

        if(MARK_FOLDER == var.toInt()){
            QMessageBox:: StandardButton result = QMessageBox::warning(NULL, QStringLiteral("警告"), QStringLiteral("确定删除该父类及相应模型吗！"), QMessageBox::Yes | QMessageBox::No,
                                                      QMessageBox::No);
            switch (result)
            {
            case QMessageBox::Yes:
            {
                QString t = ui->treeView->model()->itemData(index).values()[0].toString();
                std::string t1 = t.toUtf8().toStdString();
//                cout << t1<<endl;
//                for(vector<string>::iterator it  = g_parent_type.begin(); it != g_parent_type.end(); ++it) //删除父节点数据
//                        {
//                                if((*it) == t1){
//                                    g_parent_type.erase(it);
//                                }
//                       }
                auto print_type = std::remove_if(g_parent_type.begin(),g_parent_type.end(),[=](string const& obj)
                {return obj == t1;});
                if (print_type != g_parent_type.end()){
                    g_parent_type.erase(print_type);
                }


//                for(vector<string>::iterator it = g_parent_type.begin(); it!=g_parent_type.end();it++){
//                    cout<<(*it)<< endl;}
                for(vector<ObjectData>::iterator it  = g_objects.begin(); it != g_objects.end();it++ )  //删除子节点数据
                        {
                                if((*it).parent_type == t1){
                                    g_objects.erase(it);
                                }
                       }
                itemmap.erase(t.toUtf8().toStdString()); //清除Map函数数据。
                mModel->removeRow(index.row(),index.parent()); //清除树节点数据
                break;
            }
            case QMessageBox::No:
                break;
            default:
                break;
            }

        }
        else if(MARK_ITEM == var.toInt()){
            QMessageBox:: StandardButton result = QMessageBox::warning(NULL, QStringLiteral("警告"), QStringLiteral("确定删除该模型吗！"), QMessageBox::Yes | QMessageBox::No,
                                                      QMessageBox::No);
            switch (result)
            {
            case QMessageBox::Yes:
            {
                QString t = ui->treeView->model()->itemData(curIndex).values()[0].toString();
                std::string t1 = t.toUtf8().toStdString();
                for(vector<ObjectData>::iterator it  = g_objects.begin(); it != g_objects.end(); )  //删除子节点数据
                        {
                                if((*it).name == t1){
                                    g_objects.erase(it);
                                    mModel->removeRow(index.row(),index.parent()); //清除树节点数据
                                }
                       }
                for(vector<string>::iterator it = g_parent_type.begin(); it!=g_parent_type.end();it++){
                    cout<<(*it)<< endl;
                }


                //itemmap.erase(ui->treeView->get)
                break;
            }
            case QMessageBox::No:
                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::slotTreeMenuCollapse(bool checked)
{
    QModelIndex curIndex = ui->treeView->currentIndex();
    //QModelIndex index = curIndex.sibling(curIndex.row(),0); //同一行第一列元素的index
    if(curIndex.isValid())
    {
        ui->treeView->collapse(curIndex);
    }


}

//比较函数
