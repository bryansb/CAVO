#include "../../../../../header_files/main-cavo-gui.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    MainCavoGUI cavo;
    cavo.init();
    
    return app.exec();
}
