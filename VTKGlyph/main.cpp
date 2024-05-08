#include "VTKGlyph.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VTKGlyph w;
    w.show();
    return a.exec();
}
