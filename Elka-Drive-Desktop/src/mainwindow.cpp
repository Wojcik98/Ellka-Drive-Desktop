#include <QStandardItem>
#include <QStringListModel>
#include <QUrl>
#include <algorithm>

#include "include/mainwindow.h"
#include "include/view.h"
#include "include/controller.h"
#include "include/model.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : View(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->fileList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->fileList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fileList->setModel(&fileListModel);

    ui->fileList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->fileList, &QListView::customContextMenuRequested, [this](QPoint pos) {
        QModelIndex *index = new QModelIndex(ui->fileList->indexAt(pos));
        auto type = index->data(Model::TYPE_ROLE);
        if (type == Model::ItemType::GROUP) {
            return;
        }

        QMenu *menu = new QMenu(this);

        auto download = new QAction(QIcon(":/icons/cloud_download"), "Download", this);
        connect(download, &QAction::triggered, controller, [this, index]() {
            controller->requestDownload(*index);
            delete index;
        });
        menu->addAction(download);

        auto del = new QAction(QIcon(":/icons/delete.svg"), "Delete", this);
        connect(del, &QAction::triggered, controller, [this, index]() {
            controller->requestDelete(*index);
            delete index;
        });
        menu->addAction(del);

        menu->popup(ui->fileList->viewport()->mapToGlobal(pos));
    });

    ui->buttonsLayout->addWidget(&groupsWidget, 0, 0);
    ui->buttonsLayout->addWidget(&filesWidget, 0, 0);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::settingsButtonClicked() {
    auto selectionModel = ui->fileList->selectionModel();
    auto list = selectionModel->selectedIndexes();

    if (list.length() > 0) {
        auto index = list.first();
        emit openGroupSettings(index);
    }
}

void MainWindow::setController(Controller *controller) {
    this->controller = controller;
    connect(
        ui->fileList,
        &QAbstractItemView::doubleClicked,
        controller,
        &Controller::fileDoubleClicked
    );
    connect(
        ui->fileList,
        &QAbstractItemView::clicked,
        controller,
        &Controller::fileClicked
    );
    connect(
        ui->backButton,
        &QPushButton::clicked,
        controller,
        &Controller::goBack
    );
    connect(
        ui->refreshButton,
        &QPushButton::clicked,
        controller,
        &Controller::refresh
    );
}

bool MainWindow::filesSort(QStandardItem *a, QStandardItem *b) {
    return a->data(Model::TYPE_ROLE).toInt() < b->data(Model::TYPE_ROLE).toInt();
}

void MainWindow::setFileList(QList<QStandardItem*> files) {
    // TODO delete
    std::sort(files.begin(), files.end(), filesSort);
    fileListModel.clear();
    fileListModel.appendColumn(files);
}
