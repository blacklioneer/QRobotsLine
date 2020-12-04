#ifndef IMPORTNEW_H
#define IMPORTNEW_H

#include <QDialog>
#include "variables.h"
namespace Ui {
class ImportNew;
}

class ImportNew : public QDialog
{
    Q_OBJECT

public:
    explicit ImportNew(QWidget *parent = nullptr);
     ImportNew(ObjectData &it,QWidget *parent = nullptr);
    ~ImportNew();

private slots:

    void on_buttonBox_accepted();


    void on_toolButton_clicked();

    /** 父类型变化检测 */
    void on_comboBox_currentIndexChanged(const QString &arg1);

    /** 上级控件是否选定 */
    void on_checkBox_prior_stateChanged(int arg1);

private:
    Ui::ImportNew *ui;
    bool ischanging = false;
    /** 当前选定的模型对象值 */
    ObjectData &a_it ;
};

#endif // IMPORTNEW_H
