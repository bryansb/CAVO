#include "../../../../../header_files/header.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    MainImageGUI migui;
    migui.init();
    
    return app.exec();
}
