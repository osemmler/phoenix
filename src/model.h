#ifndef MODEL_H
#define MODEL_H

#include <QDialog>
#include <QAbstractItemModel>

namespace Ui {
class Model;
}

class Model : public QDialog
{
    Q_OBJECT

public:
    explicit Model(QWidget *parent = 0);
    ~Model();

    void setModel(QAbstractItemModel * model);

private:
    Ui::Model *ui;
};

#endif // MODEL_H
