#include "../../../../../header_files/main_cavo_gui.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    MainCavoGUI cavo;
    cavo.init();
    
    return app.exec();
}
