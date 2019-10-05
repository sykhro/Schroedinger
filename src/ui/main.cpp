/*
 * Schroedinger(c) 2019 Licensed under the GNU LGPL 2.1
 * See the included LICENSE for details
 */

#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QCoreApplication::setOrganizationName(QStringLiteral("Scienza"));
    QCoreApplication::setApplicationName(QStringLiteral("Schroedinger"));

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
