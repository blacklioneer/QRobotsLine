#include "importnew.h"
#include "ui_importnew.h"
#include <QFileDialog>
#include <iostream>
#include "variables.h"
using namespace std;

ImportNew::ImportNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportNew)
{
    ui->setupUi(this);
    for(auto i:g_parent_type){
            ui->comboBox->addItem(QString::fromStdString(i));
    }

}

ImportNew::~ImportNew()
{
    delete ui;
}


void ImportNew::on_buttonBox_accepted()
{
    ObjectData bufferdata;
    if (std::find_if(g_objects.begin(),g_objects.end(),OjFind(ui->lineEdit_name->text().toUtf8().toStdString()))==g_objects.end()){
        bufferdata.path = ui->lineEdit_path->text().toStdString();
        bufferdata.name = ui->lineEdit_name->text().toStdString();
        bufferdata.parent_type = ui->comboBox->currentText().toStdString();
        bufferdata.primary_position.setX(ui->lineEdit_x->text().toFloat());
        bufferdata.primary_position.setY(ui->lineEdit_y->text().toFloat());
        bufferdata.primary_position.setZ(ui->lineEdit_z->text().toFloat());
        bufferdata.primary_angle_x = ui->lineEdit_xr->text().toFloat();
        bufferdata.primary_angle_y = ui->lineEdit_yr->text().toFloat();
        bufferdata.primary_angle_z = ui->lineEdit_zr->text().toFloat();

        g_objects.emplace_back(bufferdata);
    }


}


void ImportNew::on_toolButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,QStringLiteral("打开文件"),"./savePages");
    ui->lineEdit_path->setText(path);
}
