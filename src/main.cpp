#include <QDebug>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "appcontext.h"
#include <QtQml/qqmlregistration.h>

using namespace cryptonotes;

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);

    qputenv("QT_QUICK_CONTROLS_STYLE", QByteArray("Material"));
    qputenv("QT_QUICK_CONTROLS_MATERIAL_THEME", QByteArray("Dark"));
    qputenv("QT_QUICK_CONTROLS_MATERIAL_ACCENT", QByteArray("Blue"));
    qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", QByteArray("Dense"));

    AppContext context;

    app.setWindowIcon(QIcon(":/ui/icons/note.png"));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("appCtx", &context);
    engine.load("qrc:/ui/window.qml");

    return app.exec();
}
