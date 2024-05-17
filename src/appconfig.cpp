#include "appconfig.h"
#include "qdir.h"

using namespace cryptonotes;

const char* DB_PATH_KEY = "dbPath";

AppConfig::AppConfig() :
    _path(QDir::homePath().append("/.config/cryptonotes/")),
    _settings(_path + "config.ini", QSettings::IniFormat) { }

void AppConfig::setDbPath(QString path) {
    _settings.setValue(DB_PATH_KEY, path);
}

QString AppConfig::dbPath() const {
    auto path = _settings.value(DB_PATH_KEY).toString();
    return path.isEmpty() ? defaultDbPath() : path;
}

QString AppConfig::defaultDbPath() const {
    return _path + "notes.edb";
}

QString AppConfig::fileName() const {
    return _settings.fileName();
}
