#include "AboutWindow.hpp"


AboutWindow::AboutWindow(QWidget* parent) : QWidget(parent, Qt::Window) {
    ui.setupUi(this);

    setWindowTitle("About ChuckStation3");
    hide();
}
