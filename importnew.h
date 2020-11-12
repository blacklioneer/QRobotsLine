#ifndef IMPORTNEW_H
#define IMPORTNEW_H

#include <QDialog>

namespace Ui {
class ImportNew;
}

class ImportNew : public QDialog
{
    Q_OBJECT

public:
    explicit ImportNew(QWidget *parent = nullptr);
    ~ImportNew();

private slots:

    void on_buttonBox_accepted();


    void on_toolButton_clicked();

private:
    Ui::ImportNew *ui;
};

#endif // IMPORTNEW_H
