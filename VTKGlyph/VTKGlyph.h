#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VTKGlyph.h"

#include <QVTKOpenGLNativeWidget.h>

class VTKGlyph : public QMainWindow
{
    Q_OBJECT

public:
    VTKGlyph(QWidget *parent = nullptr);
    ~VTKGlyph();

private:
    Ui::VTKGlyphClass ui;

    QVTKOpenGLNativeWidget* _pVTKWidget = Q_NULLPTR;
};
