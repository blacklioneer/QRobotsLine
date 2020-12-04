#include "addlights.h"
#include "ui_addlights.h"

AddLights::AddLights(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLights),
    ischanging(false),
    a_it(InitLight)
{
    ui->setupUi(this);
    ui->position_X->setValue(1.0f);
    ui->position_Y->setValue(1.0f);
    ui->position_Z->setValue(1.0f);
    ui->ambient_X->setValue(0.05f);
    ui->ambient_Y->setValue(0.05f);
    ui->ambient_Z->setValue(0.05f);
    ui->diffuse_X->setValue(0.8f);
    ui->diffuse_Y->setValue(0.8f);
    ui->diffuse_Z->setValue(0.8f);
    ui->specular_X->setValue(1.0f);
    ui->specular_Y->setValue(1.0f);
    ui->specular_Z->setValue(1.0f);
    ui->linear->setValue(0.09f);
    ui->quadratic->setValue(0.032f);
}

AddLights::AddLights(PointLights &it,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLights),
    ischanging(true),
    a_it(it)
{
    ui->setupUi(this);
    ui->lineEdit_name->setText(QString::fromStdString(it.name));
    ui->lineEdit_name->setDisabled(true); // 名字不允许更改
    ui->position_X->setValue(it.position.x());
    ui->position_Y->setValue(it.position.y());
    ui->position_Z->setValue(it.position.z());
    ui->ambient_X->setValue(it.ambient.x());
    ui->ambient_Y->setValue(it.ambient.y());
    ui->ambient_Z->setValue(it.ambient.z());
    ui->diffuse_X->setValue(it.diffuse.x());
    ui->diffuse_Y->setValue(it.diffuse.y());
    ui->diffuse_Z->setValue(it.diffuse.z());
    ui->specular_X->setValue(it.specular.x());
    ui->specular_Y->setValue(it.specular.y());
    ui->specular_Z->setValue(it.specular.z());
    ui->linear->setValue(it.linear);
    ui->quadratic->setValue(it.quadratic);
}

AddLights::~AddLights()
{
    delete ui;
}

void AddLights::on_buttonBox_accepted()
{
    if(ischanging)
    {
        a_it.position.setX(ui->position_X->text().toFloat());
        a_it.position.setY(ui->position_Y->text().toFloat());
        a_it.position.setZ(ui->position_Z->text().toFloat());
        a_it.ambient.setX(ui->ambient_X->text().toFloat());
        a_it.ambient.setY(ui->ambient_Y->text().toFloat());
        a_it.ambient.setZ(ui->ambient_Z->text().toFloat());
        a_it.diffuse.setX(ui->diffuse_X->text().toFloat());
        a_it.diffuse.setY(ui->diffuse_Y->text().toFloat());
        a_it.diffuse.setZ(ui->diffuse_Z->text().toFloat());
        a_it.specular.setX(ui->specular_X->text().toFloat());
        a_it.specular.setY(ui->specular_Y->text().toFloat());
        a_it.specular.setZ(ui->specular_Z->text().toFloat());
//        a_it.constant = 1.0f;
        a_it.linear = ui->linear->text().toFloat();
        a_it.quadratic = ui->quadratic->text().toFloat();
    }
    else{
        PointLights bufferdata;
        if (std::find_if(g_pointlights.begin(),g_pointlights.end(),PLFind(ui->lineEdit_name->text().toUtf8().toStdString()))==g_pointlights.end())
        {
            bufferdata.name = ui->lineEdit_name->text().toStdString();
            bufferdata.position.setX(ui->position_X->text().toFloat());
            bufferdata.position.setY(ui->position_Y->text().toFloat());
            bufferdata.position.setZ(ui->position_Z->text().toFloat());
            bufferdata.ambient.setX(ui->ambient_X->text().toFloat());
            bufferdata.ambient.setY(ui->ambient_Y->text().toFloat());
            bufferdata.ambient.setZ(ui->ambient_Z->text().toFloat());
            bufferdata.diffuse.setX(ui->diffuse_X->text().toFloat());
            bufferdata.diffuse.setY(ui->diffuse_Y->text().toFloat());
            bufferdata.diffuse.setZ(ui->diffuse_Z->text().toFloat());
            bufferdata.specular.setX(ui->specular_X->text().toFloat());
            bufferdata.specular.setY(ui->specular_Y->text().toFloat());
            bufferdata.specular.setZ(ui->specular_Z->text().toFloat());
            bufferdata.constant = 1.0f;
            bufferdata.linear = ui->linear->text().toFloat();
            bufferdata.quadratic = ui->quadratic->text().toFloat();
            g_pointlights.emplace_back(bufferdata);
        }
    }
}

