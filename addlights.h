#ifndef ADDLIGHTS_H
#define ADDLIGHTS_H

#include <QDialog>
#include "variables.h"

namespace Ui {
class AddLights;
}

class AddLights : public QDialog
{
    Q_OBJECT

public:
    AddLights(QWidget *parent = nullptr);
    /** 光源数据修改构造函数 */
    AddLights(PointLights &it,QWidget *parent = nullptr);

    ~AddLights();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddLights *ui;
    /** 是否为修改界面 */
    bool ischanging = false;
    /** 当前选定的光源对象值 */
    PointLights &a_it ;
};

#endif // ADDLIGHTS_H
