#ifndef APIBRIDGE_H
#define APIBRIDGE_H

#include <QNetworkReply>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include "response.h"

class APIBridge : public QObject {
Q_OBJECT

public:
    APIBridge();
    virtual ~APIBridge();
    virtual void requestLogin(QString user, QString password) = 0;
    virtual void requestRegister(QString user, QString password) = 0;
    virtual void requestGroups() = 0;
    virtual void requestNewGroup(QString groupName) = 0;
    virtual void requestPath(QString path) = 0;
    virtual void requestFileDownload(int id, QString path) = 0;
    virtual void requestDirectoryDownload(int id, QString path) = 0;
    virtual void requestGroupUsers(int groupId) = 0;
    virtual void requestGroupDelete(int groupId) = 0;
    virtual void requestAddUserToGroup(QString username, int groupId) = 0;
    virtual void requestRemoveUserFromGroup(QString username, int groupId) = 0;
    virtual void requestFileDelete(int id) = 0;
    virtual void requestDirectoryDelete(QString path) = 0;
    virtual void requestNewFolder(QString path) = 0;
    virtual void requestFileUpload(QString rootLocal, QString rootServer, QString path) = 0;

signals:
    void gotResponse(Response response);    // TODO reference?
    void responseError(QNetworkReply::NetworkError error);
    void downloadProgress(qint64 bytesSent, qint64 bytesTotal);
    void downloadEnded();
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void uploadEnded();
};

#endif // APIBRIDGE_H
