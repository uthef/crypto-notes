#include <QDebug>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "appcontext.h"
#include <QtQml/qqmlregistration.h>
#include "filters/mouseeventfilter.h"
#include <QSharedMemory>
#include <QTranslator>

using namespace cryptonotes;

static QObject* newMouseEventFilterSingleton(QQmlEngine* engine, QJSEngine* jsEngine) {
    return ((QObject*)new MouseEventFilter());
}

int main(int argc, char** argv) {
    // qputenv("QT_QUICK_CONTROLS_STYLE", QByteArray("Material"));
    // qputenv("QT_QUICK_CONTROLS_MATERIAL_THEME", QByteArray("Dark"));
    // qputenv("QT_QUICK_CONTROLS_MATERIAL_ACCENT", QByteArray("Blue"));
    // qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", QByteArray("Dense"));

    QTranslator translator;
    QGuiApplication app(argc, argv);

    bool isAppAlreadyRunning = false;

    QSharedMemory sharedMemory;
    sharedMemory.setNativeKey(APP_UUID);

    if (!sharedMemory.attach()) {
        if (!sharedMemory.create(1)) return 1;
    }
    else {
        isAppAlreadyRunning = true;
    }

    AppConfig appConfig;
    QString lang = appConfig.language();
    AppContext context(&appConfig, isAppAlreadyRunning);

    if (lang != "en" && translator.load(QString(":/cryptonotes_").append(lang))) {
        app.installTranslator(&translator);
    }

    QObject::connect(&app, &QGuiApplication::aboutToQuit, &context, &AppContext::onAppAboutToQuit);
    qmlRegisterSingletonType<MouseEventFilter>("cryptonotes", 1, 0, "MouseEventFilter", newMouseEventFilterSingleton);

    app.setWindowIcon(QIcon(":/ui/icons/note.png"));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("appCtx", &context);
    engine.loadFromModule("cryptonotes", "Window");

    return app.exec();
}
