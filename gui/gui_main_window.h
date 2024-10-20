#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_gui_main_window.h"

#include "gui/gui_block.h"

namespace gui {
    class MainWindow :
        public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        virtual ~MainWindow() override;

    public:
        void addBlock(Block*);

    private:
        Ui::MainWindowClass ui;
    };

    inline void MainWindow::addBlock(Block* _pBlock) {
        if (!_pBlock) { return; }
        ui.tabWidgetBlocks->addTab(_pBlock, _pBlock->blockTitle());
    }
}
