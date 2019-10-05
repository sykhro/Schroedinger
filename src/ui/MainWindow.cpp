/*
 * Schroedinger(c) 2019 Licensed under the GNU LGPL 2.1
 * See the included LICENSE for details
 */

#include "MainWindow.h"

MainWindow::MainWindow() : QMainWindow(nullptr) {
    setWindowTitle("Schroedinger");
    setUnifiedTitleAndToolBarOnMac(true);
    setContextMenuPolicy(Qt::NoContextMenu);

}

void MainWindow::setupLayout() {
}
