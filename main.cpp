#include <QtWidgets/QApplication>

#include "engine/engine_terminal.h"
#include "gui/gui_terminal.h"

int main(int argc, char *argv[])
{
    // configurate the engine
    if (!engine::Terminal::build()) { return -1; }

    QApplication a(argc, argv);

    // create & configurate GUI
    gui::Terminal::build();
    QObject::connect(&a, &QApplication::aboutToQuit, gui::Terminal::terminal(), &gui::Terminal::deleteLater);

    // launch the data exchange & GUI update loop
    engine::Terminal::launch();
    gui::Terminal::launch();

    int appRes = a.exec();

    // stop the engine
    engine::Terminal::halt();

    return appRes;
}
