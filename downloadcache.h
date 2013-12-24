#ifndef DOWNLOADCACHE_H
#define DOWNLOADCACHE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QSslError>
#include <QList>
#include <QByteArray>

class QNetworkReply;
class QAuthenticator;

class DownloadCache : public QObject
{
    Q_OBJECT
public:
    explicit DownloadCache(QObject *parent = 0);


    void toStdout(const QString & urlPath);
    void toStdout(const QStringList & urlPathList);

    void setUrlPath(const QString & _urlPath) { urlPath = _urlPath; }
    void setUserName(const QString & _userName) { userName = _userName; }
    void setPassword(const QString & _password) { password = _password; }
    void setPrintToStdout(bool flag) { printToStdout = flag; }

signals:

public slots:
    void onRequestDownload();
    void onRequestDownload(const QString & urlPath);
    void onRequestCancel();

private slots:
    void onRequestReadyRead();
    void onRequestFinished();
    void onRequestAuthenticationRequired(QNetworkReply* _reply, QAuthenticator* authenticator);
    void onSslErrors(QNetworkReply* _reply, QList<QSslError> errorList);

private:
    bool requestCanceled;
    bool requestFinished;
    bool printToStdout;
    QString urlPath;
    QMap<QString, QByteArray> cache;
    QNetworkReply* reply;
    QNetworkAccessManager mgr;
    QUrl url;
    QByteArray buffer;
    QString userName;
    QString password;
};

#endif // DOWNLOADCACHE_H
