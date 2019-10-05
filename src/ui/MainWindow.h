/*
 * Schroedinger - Scienza (c) 2019
 * Licensed under the LGPL 2.1; see the included LICENSE for details
 */

#pragma once
#include <QGridLayout>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    explicit MainWindow();
    ~MainWindow() = default;

  private:
    void setupLayout();
    QGridLayout *m_layout = nullptr;
};
