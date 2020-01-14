#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QStandardItem>
#include <QLayout>
#include <QProgressBar>
#include <QTimer>

#include "include/controller.h"

class Controller;

class View : public QMainWindow {
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);
    virtual ~View();

    virtual void setController(Controller *controller) = 0;
    virtual void setFileList(const QList<QStandardItem*> &files, const QString &path) = 0;
    virtual void clearMsg() = 0;
    virtual void setChatModel(QStandardItemModel *model) = 0;
    void setGroupsView();
    void setFilesView();
    void anyItemSelected(bool enabled);
    virtual void showLogoutMsg() = 0;
    virtual void showForbiddenMsg() = 0;
    virtual void showDirectoryNotFound() = 0;
    virtual void showGroupExist() = 0;
    virtual void showUnknownErrorMsg() = 0;
    virtual void showFileOpenError(const QString &filename) = 0;
    virtual QString getNewGroupName(bool *ok) = 0;
    virtual QString getNewFolderName(bool *ok) = 0;
    virtual QStringList getUploadFileNames() = 0;
    virtual QString getUploadFolderName() = 0;
    virtual QString getSaveFilename(const QString &suggested) = 0;

protected:
    virtual void settingsButtonClicked() = 0;
    virtual void downloadButtonClicked() = 0;
    virtual void deleteButtonClicked() = 0;

    QStandardItemModel fileListModel;
    Controller *controller;
    QWidget groupsWidget;
    QWidget filesWidget;
    QPushButton *newButton;
    QPushButton *settingsButton;
    QPushButton *dirButton;
    QPushButton *uploadFileButton;
    QPushButton *uploadFolderButton;
    QPushButton *downloadButton;
    QPushButton *deleteButton;
    QPushButton *sendButton;
    QPushButton *backButton;

    QWidget *uploadWidget;
    QProgressBar *uploadProgress;
    QLabel *uploadFinishedLabel;
    QTimer uploadTimer;
    QWidget *downloadWidget;
    QProgressBar *downloadProgress;
    QLabel *downloadFinishedLabel;
    QTimer downloadTimer;

private:
    void setupBottomButtons();
    void setupUpload();
    void setupDownload();
    void connectSignals();

signals:
    void createNewGroup();
    void openGroupSettings(const QModelIndex &index);
    void requestDownload(const QModelIndex &index);
    void requestDelete(const QModelIndex &index);
    void createNewFolder();
    void uploadFile();
    void uploadFolder();
    void sendMsg(const QString &msg);

public slots:
    void setUploadProgress(const qint64 current, const qint64 total);
    void setDownloadProgress(const qint64 current, const qint64 total);
    void resetUploadProgress();
    void resetDownloadProgress();
};

#endif // VIEW_H
