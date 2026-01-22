#include "storage/appconfig.h"
#include "qdir.h"

using namespace cryptonotes;

const char* DB_PATH_KEY = "dbPath";
const char* BACKUP_PATHS_KEY = "backupPaths";
const char* WINDOW_WIDTH_KEY = "startupWindowWidth";
const char* WINDOW_HEIGHT_KEY = "startupWindowHeight";
const char* WINDOW_FULLSCREEN_KEY = "startupWindowMaximized";

AppConfig::AppConfig() :
    _path(QDir::homePath().append("/.config/cryptonotes/")),
    _settings(_path + "config.ini", QSettings::IniFormat) {
        _backupPaths = _settings.value(BACKUP_PATHS_KEY).toStringList().toList();
}

void AppConfig::setDbPath(QString path) {
    _settings.setValue(DB_PATH_KEY, path);
}

void AppConfig::updateBackupPaths(QStringList paths) {
    if (paths.isEmpty()) {
        _settings.remove(BACKUP_PATHS_KEY);
        return;
    }
    
    _settings.setValue(BACKUP_PATHS_KEY, paths);
}

QString AppConfig::dbPath() const {
    auto path = _settings.value(DB_PATH_KEY).toString();
    return path.isEmpty() ? defaultDbPath() : path;
}

QString AppConfig::defaultDbPath() const {
    return _path + "notes.edb";
}

QStringList AppConfig::backupPaths() const {
    return _backupPaths;
}

QString AppConfig::fileName() const {
    return _settings.fileName();
}

void AppConfig::setWindowSize(int width, int height) {
    _settings.setValue(WINDOW_WIDTH_KEY, width);
    _settings.setValue(WINDOW_HEIGHT_KEY, height);
}

int AppConfig::windowWidth() {
    return _settings.value(WINDOW_WIDTH_KEY, -1).toInt();
}

int AppConfig::windowHeight() {
    return _settings.value(WINDOW_HEIGHT_KEY, -1).toInt();
}

bool AppConfig::windowMaximized() {
    return _settings.value(WINDOW_FULLSCREEN_KEY, false).toBool();
}

void AppConfig::setWindowMaximized(bool value) {
    _settings.setValue(WINDOW_FULLSCREEN_KEY, value);
}