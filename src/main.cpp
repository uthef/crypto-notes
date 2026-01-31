#include <QDebug>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "appcontext.h"
#include <QtQml/qqmlregistration.h>
#include "filters/mouseeventfilter.h"
#include <QSharedMemory>
#include <QQuickStyle>

using namespace cryptonotes;

static QObject* newMouseEventFilterSingleton(QQmlEngine* engine, QJSEngine* jsEngine) {
    return ((QObject*)new MouseEventFilter());
}

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    bool isAppAlreadyRunning = false;

    QSharedMemory sharedMemory;
    sharedMemory.setNativeKey(APP_UUID);

    if (!sharedMemory.attach()) {
        if (!sharedMemory.create(1)) return 1;
    }
    else {
        isAppAlreadyRunning = true;
    }

    AppContext context(&app, isAppAlreadyRunning);

    QObject::connect(&app, &QGuiApplication::aboutToQuit, &context, &AppContext::onAppAboutToQuit);
    qmlRegisterSingletonType<MouseEventFilter>("cryptonotes", 1, 0, "MouseEventFilter", newMouseEventFilterSingleton);

    app.setWindowIcon(QIcon(":/ui/icons/note.png"));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("appCtx", &context);
    engine.rootContext()->setContextProperty("appEngine", &engine);
    engine.loadFromModule("cryptonotes", "Window");

    return app.exec();
}
