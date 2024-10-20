#include "gui_main_window.h"
#include <thread>

namespace gui {
    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        ui.setupUi(this);
    }

    MainWindow::~MainWindow()
    {}
} // namespace gui
