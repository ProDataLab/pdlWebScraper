#include "downloadcache.h"

#include <stdio.h>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVariant>
#include <QtDebug>
#include <QAuthenticator>
#include <QSsl>
#include <QSslError>
#include <QCoreApplication>

DownloadCache::DownloadCache(QObject *parent) :
    QObject(parent), requestCanceled(false), requestFinished(true),
    printToStdout(false)
{

    connect(&mgr, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
            this, SLOT(onRequestAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    connect(&mgr, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            this, SLOT(onSslErrors(QNetworkReply*,QList<QSslError>)));
}

void DownloadCache::toStdout(const QString &urlPath)
{
    printf("%s\n", cache[urlPath].constData());
    fflush(stdout);
}

void DownloadCache::toStdout(const QStringList &urlPathList)
{
    Q_UNUSED(urlPathList);
}

void DownloadCache::onRequestDownload()
{
    url = QUrl(urlPath);
    reply = mgr.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(onRequestReadyRead()));
}


void DownloadCache::onRequestDownload(const QString &urlPath)
{
    this->urlPath = urlPath;
    onRequestDownload();
}

void DownloadCache::onRequestCancel()
{
    requestCanceled = true;
    reply->abort();
}

void DownloadCache::onRequestReadyRead()
{
    buffer += reply->readAll();
}

void DownloadCache::onRequestFinished()
{
    if (requestCanceled) {
        buffer.clear();
        reply->deleteLater();
        return;
    }

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (reply->error()) {
        buffer.clear();
        qWarning() << "Download of" << url.path() << "failed";
    }
    else if (!redirectionTarget.isNull()) {
        buffer.clear();
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        url = newUrl;
        reply->deleteLater();
        onRequestDownload(url.path());
        return;
    }
    else {                         // all is well
        cache[url.path()] = buffer;
        buffer.clear();
    }

    reply->deleteLater();
    reply = 0;
    if (printToStdout)
        toStdout(url.path());
    qApp->quit();
}

void DownloadCache::onRequestAuthenticationRequired(QNetworkReply *_reply, QAuthenticator *authenticator)
{
    Q_UNUSED(_reply);
    if (userName.isEmpty() || password.isEmpty()) {
        qFatal("\n\nUsername and password needed");
    }
    authenticator->setUser(userName);
    authenticator->setPassword(password);
}

void DownloadCache::onSslErrors(QNetworkReply *_reply, QList<QSslError> errorList)
{

    QString errorString;
    foreach (const QSslError & error, errorList) {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }
    _reply->ignoreSslErrors();
}
