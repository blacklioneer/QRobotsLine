#include "addlights.h"
#include "ui_addlights.h"
#include "variables.h"

AddLights::AddLights(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLights)
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

AddLights::~AddLights()
{
    delete ui;
}

void AddLights::on_buttonBox_accepted()
{
    PointLights bufferdata;
    if (std::find_if(g_pointlights.begin(),g_pointlights.end(),PLFind(ui->lineEdit_name->text().toUtf8().toStdString()))==g_pointlights.end()){
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
