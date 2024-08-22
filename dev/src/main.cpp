#include <iostream>
#include <QApplication>
#include <App/GuiNN.hpp>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GuiNN gui;

    app.exec();
    return 0;
}