#include "core/emulator.h"
#include "ui/mainwindow.h"

//int main(int argc, char** argv)
//{
//    Emulator emu;
//
//    if(argc != 2)
//    {
//        std::cout << "Usage: gb [ROM file path]\n";
//        return 1;
//    }
//
//    const char* romFilePath = argv[1];
//    if(!emu.LoadCartridge(romFilePath))
//    {
//        std::cout << "Unable to load ROM: " << romFilePath << "\n";
//        return 1;
//    }
//
//    emu.Play();
//
//    return 0;
//}

#include <QApplication>

int main(int argc, char** argv)
{
    Emulator emu;

    QApplication app{argc, argv};

    MainWindow window;
    window.show();

    return app.exec();
}
