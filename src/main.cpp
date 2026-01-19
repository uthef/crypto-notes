#include <QDebug>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "appcontext.h"
#include <QtQml/qqmlregistration.h>
#include "mouseeventfilter.h"

using namespace cryptonotes;

static QObject* newMouseEventFilterSingleton(QQmlEngine* engine, QJSEngine* jsEngine) {
    return ((QObject*)new MouseEventFilter());
}

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);

    // qputenv("QT_QUICK_CONTROLS_STYLE", QByteArray("Material"));
    // qputenv("QT_QUICK_CONTROLS_MATERIAL_THEME", QByteArray("Dark"));
    // qputenv("QT_QUICK_CONTROLS_MATERIAL_ACCENT", QByteArray("Blue"));
    // qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", QByteArray("Dense"));

    AppContext context;
    qmlRegisterSingletonType<MouseEventFilter>("cryptonotes", 1, 0, "MouseEventFilter", newMouseEventFilterSingleton);

    app.setWindowIcon(QIcon(":/ui/icons/note.png"));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("appCtx", &context);
    engine.loadFromModule("cryptonotes", "Window");

    return app.exec();
}
