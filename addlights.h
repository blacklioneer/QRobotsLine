#ifndef ADDLIGHTS_H
#define ADDLIGHTS_H

#include <QDialog>

namespace Ui {
class AddLights;
}

class AddLights : public QDialog
{
    Q_OBJECT

public:
    explicit AddLights(QWidget *parent = nullptr);
    ~AddLights();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddLights *ui;
};

#endif // ADDLIGHTS_H
