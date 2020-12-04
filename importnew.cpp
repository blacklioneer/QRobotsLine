#include "importnew.h"
#include "ui_importnew.h"
#include <QFileDialog>
#include <iostream>

using namespace std;

ImportNew::ImportNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportNew),
    ischanging(false),
    a_it(InitObject)
{
    ui->setupUi(this);
    ui->checkBox_prior->setChecked(false);
    ui->comboBox_prior->setEnabled(false);
    for(auto i:g_parent_type){
            ui->comboBox->addItem(QString::fromStdString(i));
    }

}

ImportNew::ImportNew(ObjectData &it,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportNew),
    ischanging(true),
    a_it(it)
{
    ui->setupUi(this);
    ui->comboBox_prior->clear(); //先清空
    ui->comboBox_prior->setEnabled(false);
    ui->checkBox_prior->setChecked(false); //设置为未选中
    for(auto i:g_parent_type){
            ui->comboBox->addItem(QString::fromStdString(i));
    } //种类组件添加子单元
    ui->comboBox->setCurrentText(QString::fromStdString(it.parent_type));
    if(!it.higher_control.empty()) //如果不为空
    {
        ui->checkBox_prior->setChecked(true); //设置为选中
        for(auto i:g_objects){  //当前组件添加所有项
            if((i.parent_type == it.parent_type) && (i.name != it.name))  //如果父类为该对象
            {
                ui->comboBox_prior->addItem(QString::fromStdString(i.name));
            }
        }
        ui->comboBox_prior->setCurrentText(QString::fromStdString(it.higher_control));
    }
    ui->lineEdit_name->setText(QString::fromStdString(it.name));
    ui->lineEdit_name->setDisabled(true); // 名字不允许更改
    ui->lineEdit_path->setText(QString::fromStdString(it.path));
    ui->position_X->setValue(it.primary_position.x());
    ui->position_Y->setValue(it.primary_position.y());
    ui->position_Z->setValue(it.primary_position.z());
    ui->angel_X->setValue(it.primary_angle_x);
    ui->angel_Y->setValue(it.primary_angle_y);
    ui->angel_Z->setValue(it.primary_angle_z);
}

ImportNew::~ImportNew()
{
    delete ui;
}


void ImportNew::on_buttonBox_accepted()
{
    if(ischanging)
    {
        a_it.path = ui->lineEdit_path->text().toStdString();
//        a_it.name = ui->lineEdit_name->text().toStdString();
        a_it.parent_type = ui->comboBox->currentText().toStdString();
        a_it.primary_position.setX(ui->position_X->text().toFloat());
        a_it.primary_position.setY(ui->position_Y->text().toFloat());
        a_it.primary_position.setZ(ui->position_Z->text().toFloat());
        a_it.parent_type = ui->comboBox->currentText().toStdString();
        if((ui->checkBox_prior->isChecked()) && (!ui->comboBox_prior->currentText().toStdString().empty())) // 上一级控制单元写入
        {
            a_it.higher_control = ui->comboBox_prior->currentText().toStdString();
        }
        else
        {
            a_it.higher_control = "";
        }
        a_it.primary_angle_x = ui->angel_X->text().toFloat();
        a_it.primary_angle_y = ui->angel_Y->text().toFloat();
        a_it.primary_angle_z = ui->angel_Z->text().toFloat();
    }
    else
    {
        ObjectData bufferdata;
        if (std::find_if(g_objects.begin(),g_objects.end(),OjFind(ui->lineEdit_name->text().toUtf8().toStdString()))==g_objects.end()){
            bufferdata.path = ui->lineEdit_path->text().toStdString();
            bufferdata.name = ui->lineEdit_name->text().toStdString();
            bufferdata.parent_type = ui->comboBox->currentText().toStdString();
            if((ui->checkBox_prior->isChecked()) && (!ui->comboBox_prior->currentText().toStdString().empty())) // 上一级控制单元写入
            {
                bufferdata.higher_control = ui->comboBox_prior->currentText().toStdString();
            }
            else
            {
                bufferdata.higher_control = "";
            }
            bufferdata.primary_position.setX(ui->position_X->text().toFloat());
            bufferdata.primary_position.setY(ui->position_Y->text().toFloat());
            bufferdata.primary_position.setZ(ui->position_Z->text().toFloat());
            bufferdata.primary_angle_x = ui->angel_X->text().toFloat();
            bufferdata.primary_angle_y = ui->angel_Y->text().toFloat();
            bufferdata.primary_angle_z = ui->angel_Z->text().toFloat();

            g_objects.emplace_back(bufferdata);
        }
    }
}


void ImportNew::on_toolButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,QStringLiteral("打开文件"),"./savePages");
    ui->lineEdit_path->setText(path);
}

void ImportNew::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_prior->clear(); //先清空
    if(ischanging)
        //若为修改模式
    {
        for(auto i:g_objects){
            if((i.parent_type == arg1.toStdString())&&(i.name != a_it.name))  //如果父类为该对象
            {
                ui->comboBox_prior->addItem(QString::fromStdString(i.name));
            }
        }
    }
    else
        //若为新建模式
    {
        for(auto i:g_objects){
            if(i.parent_type == arg1.toStdString())  //如果父类为该对象
            {
                ui->comboBox_prior->addItem(QString::fromStdString(i.name));
            }
        }
    }

}

void ImportNew::on_checkBox_prior_stateChanged(int arg1)
{
    switch(arg1)
    {
        case Qt::Checked:
            ui->comboBox_prior->setEnabled(true);
            break;
        case Qt::Unchecked:
            ui->comboBox_prior->setEnabled(false);
            break;
        default:
            break;
    }
}
