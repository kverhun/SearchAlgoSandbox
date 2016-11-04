#include <iostream>

#include <QApplication>
#include <QtWidgets>

int main(int i_argc, char** i_argv)
{
    std::cout << "Start application..." << std::endl;

    QApplication app(i_argc, i_argv);
    
    QMainWindow main_wnd;
    main_wnd.show();

    auto return_code = app.exec();
    std::cout << "Terminating application. Exit code: " << return_code << std::endl;
    
    return return_code;
}