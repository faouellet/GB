#include "mainwindow.h"

#include "debugwindow.h"
#include "renderwidget.h"

#include <QtWidgets>
#include <QGridLayout>

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent)
{
    CreateMenus();
    setWindowTitle("YAGBE");

    m_renderWidget = std::make_unique<RenderWidget>(this);
    setCentralWidget(m_renderWidget.get());
}

MainWindow::~MainWindow() = default;

void MainWindow::CreateMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("&Open..."), this, SLOT(Open()), QKeySequence::Open);
    fileMenu->addAction(tr("E&xit"), this, SLOT(close()), QKeySequence::Quit);

    QMenu* emulationMenu = menuBar()->addMenu(tr("&Emulation"));
    emulationMenu->addAction("Play", this, SLOT(Play()));

    QMenu* toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction("Open Debug Window", this, SLOT(OpenDebugWindow()));

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, SLOT(About()));
}

void MainWindow::About()
{
    QMessageBox::about(this, tr("About YAGBE"),
                       tr("Yet Another Game Boy Emulator"));
}

void MainWindow::Open()
{
    QString filename = QFileDialog::getOpenFileName(this);
    if(!m_emu.LoadCartridge(filename.toStdString()))
    {
        QMessageBox::critical(this, tr("Error"),
                              tr("Problem loading ROM file"));
    }
}

void MainWindow::OpenDebugWindow()
{
    m_debugWindow.reset(new DebugWindow);
    m_debugWindow->show();
}

void MainWindow::Play()
{
    m_emu.Play();
}
