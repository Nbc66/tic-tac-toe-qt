#include <QApplication>
#include "MainWindow.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    MainWindow w;
    w.show();
    return app.exec();
}
