#include <QtDebug>
#include <QPushButton>

#include "include/controller.h"

Controller::Controller(QApplication *app) : app(app) {
    connect(
        this,
        &Controller::closeApp,
        app,
        &QApplication::quit,
        Qt::QueuedConnection
    );
}

void Controller::setModel(Model *model) {
    this->model = model;

    connect(
        model,
        &Model::loginStatus,
        this,
        &Controller::loginSuccess
    );
    connect(
        model,
        &Model::registerStatus,
        this,
        &Controller::registerSuccess
    );
    connect(
        model,
        &Model::groupsReceived,
        this,
        &Controller::groupsReceived
    );
    connect(
        model,
        &Model::pathReceived,
        this,
        &Controller::pathReceived
    );
}

void Controller::setView(View *view) {
    this->view = view;
}

void Controller::checkLogin() {
    if (model->isLogged()) {
        return;
    }
    loginDialog = new LoginDialog();

    connect(
        loginDialog,
        &LoginDialog::tryLogin,
        this,
        &Controller::slotTryUserLogin
    );
    connect(
        this,
        &Controller::loginSuccess,
        loginDialog,
        &LoginDialog::slotLoginResponse
    );
    connect(
        loginDialog,
        &LoginDialog::rejected,
        this,
        &Controller::loginDialogClosed
    );
    connect(
        loginDialog,
        &LoginDialog::openRegister,
        this,
        &Controller::openRegister
    );

    loginDialog->exec();
}

void Controller::slotTryUserLogin(QString& user, QString& password) {
    model->requestLogin(user, password);
}

void Controller::slotTryRegister(QString& user, QString& password) {
    model->requestRegister(user, password);
}

void Controller::loginDialogClosed() {
    loginDialog->deleteLater();

    if (!model->isLogged()) {
        emit closeApp();
    }

    qDebug("groups");
    model->requestGroups();
}

void Controller::registerDialogClosed() {
    registerDialog->deleteLater();
}

void Controller::openRegister() {
    registerDialog = new RegisterDialog();
    // TODO delete

    connect(
        registerDialog,
        &RegisterDialog::tryLogin,
        this,
        &Controller::slotTryRegister
    );
    connect(
        this,
        &Controller::registerSuccess,
        registerDialog,
        &RegisterDialog::slotRegisterResponse
    );
    connect(
        registerDialog,
        &RegisterDialog::rejected,
        this,
        &Controller::registerDialogClosed
    );

    registerDialog->exec();
}

void Controller::groupsReceived(QList<QStandardItem*> groups) {
    view->setFileList(groups);
    view->setGroupsButtonsVisible();
    view->setSettingsButtonEnabled(false);
}

void Controller::pathReceived(QList<QStandardItem*> path) {
    view->setFileList(path);
    view->setFilesButtonsVisible();
}

void Controller::requestDelete(const QModelIndex &index) {
    auto filename = index.data().toString();
    model->requestDelete(path.join("/") + filename);
}

void Controller::requestDownload(const QModelIndex &index) {
    auto id = index.data(Model::ID_ROLE).toInt();
    qDebug() << "Download " << id;
    model->requestDownload(QString::number((id)));
}

void Controller::fileDoubleClicked(const QModelIndex &index) {
    auto filename = index.data().toString();
    auto type = index.data(Model::TYPE_ROLE).toString();
    qDebug() << "clicked: " << filename;

    if (type == "group") {
        auto id = index.data(Model::ID_ROLE).toString();
        path.append(id);
        qDebug() << "get " << path.join("/");
        model->requestPath(path.join("/"));
    } else if (type == "folder") {
        path.append(filename);
        qDebug() << "get " << path.join("/");
        model->requestPath(path.join("/"));
    } else {
        requestDownload(index);
    }
}

void Controller::fileClicked(const QModelIndex&) {
    view->setSettingsButtonEnabled(true);
}

void Controller::goBack() {
    qDebug() << "Go back!";
    if (path.length() > 1) {
        path.pop_back();
        qDebug() << "get " << path.join("/");
        model->requestPath(path.join("/"));
    } else if (path.length() == 1) {
        path.pop_back();
        qDebug() << "get groups";
        model->requestGroups();
    }
}
