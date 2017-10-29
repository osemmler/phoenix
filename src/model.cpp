#include "model.h"
#include "ui_model.h"

Model::Model(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Model)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    ui->tableView->verticalHeader()->setVisible(false);

    connect(ui->pushButtonBack, &QPushButton::clicked, this, &Model::accept);
}

Model::~Model()
{
    delete ui;
}

void Model::setModel(QAbstractItemModel *model)
{
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
}
