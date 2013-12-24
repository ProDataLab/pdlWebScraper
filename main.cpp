#include <QCoreApplication>

#include <QTimer>
#include <QtDebug>
#include <QString>
#include <QFileInfo>
#include <stdio.h>
#include "downloadcache.h"

const char usage[] =
        "Usage:\n"
        "======\n\n"
        "%1 [options] <URL>\n\n"
        "  URL:                The url path of the web page to download\n\n"
        "  Options:\n"
        "  ========\n\n"
        "    -u --username <value>:    Username if authentication is required\n"
        "    -p --password <value>:    Password if authentication is required\n"
        "    -s --to_stdout:           Print to stdout\n\n";


int main(int argc, char** argv) {


    QCoreApplication a(argc, argv);

    QString usageStr(usage);

    if (a.arguments().length() < 2) {
        QFileInfo fi(a.arguments().at(0));
        fprintf(stderr,"%s\n", usageStr.arg(QString("./") + fi.fileName()).toLocal8Bit().constData());
        exit(1);
    }

    DownloadCache dc;

    if (a.arguments().length() > 2) {
        for (int i=1; i < a.arguments().length(); ++i) {

            QString arg(a.arguments().at(i));

            if (!QString::compare(arg, QString("-u"))) {
                dc.setUserName(a.arguments().at(++i));
            }
            else if (!QString::compare(arg, QString("-p"))) {
                dc.setPassword(a.arguments().at(++i));
            }
            else if (!QString::compare(arg, QString("-s"))) {
                dc.setPrintToStdout(true);
            }
        }
    }

    dc.setUrlPath(a.arguments().at(a.arguments().length()-1));

    QTimer::singleShot(0, &dc, SLOT(onRequestDownload()));

    return a.exec();
}
