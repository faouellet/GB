#include "debugwindow.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

#include <memory>

DebugWindow::DebugWindow(QWidget* parent) 
    : QWidget(parent)
{
    auto grid = std::make_unique<QGridLayout>();
    grid->addWidget(CreateDisasmBox(), 0, 0, 2, 1);
    grid->addWidget(CreateCpuStateBox(), 0, 1);
    grid->addWidget(CreateMemoryDumpBox(), 1, 1);

    setLayout(grid.release());
}

QGroupBox* DebugWindow::CreateCpuStateBox() const
{
    auto layout = std::make_unique<QGridLayout>();

    const std::vector<std::string> labels{"A", "F", "B", "C", "D", "E", "H", "L", "PC", "SP"};

    for(int i = 0; i < 10; ++i)
    {
        auto label = std::make_unique<QLabel>(tr(labels[i].c_str()));
        auto lineEdit = std::make_unique<QLineEdit>();
        lineEdit->setReadOnly(true);

        const int rowIdx = i / 2;
        const bool isEven = (i % 2) == 0;
        const int labelColIdx = isEven ? i % 2 : (i % 2) + 1;
        const int textColIdx = isEven ? (i % 2) + 1 : (i % 2) + 2;

        layout->addWidget(label.release(), rowIdx, labelColIdx);
        layout->addWidget(lineEdit.release(), rowIdx, textColIdx);
    }

    auto cpuBox = std::make_unique<QGroupBox>(tr("CPU State"));
    cpuBox->setLayout(layout.release());

    return cpuBox.release();
}

QGroupBox* DebugWindow::CreateDisasmBox() const
{
    auto disasmText = std::make_unique<QTextEdit>();
    disasmText->setReadOnly(true);
    disasmText->setPlaceholderText("Lorem ipsum");

    auto layout = std::make_unique<QVBoxLayout>();
    layout->addWidget(disasmText.release());

    auto disasmBox = std::make_unique<QGroupBox>(tr("Program Disassembly"));
    disasmBox->setLayout(layout.release());

    return disasmBox.release();
}

QGroupBox* DebugWindow::CreateMemoryDumpBox() const
{   
    auto memText = std::make_unique<QTextEdit>();
    memText->setReadOnly(true);
    memText->setPlaceholderText("Lorem ipsum");

    auto layout = std::make_unique<QVBoxLayout>();
    layout->addWidget(memText.release());

    auto memBox = std::make_unique<QGroupBox>(tr("Memory Dump"));
    memBox->setLayout(layout.release());

    return memBox.release();
}
