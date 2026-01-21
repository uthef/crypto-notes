#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include "filters/mouseeventfilter.h"

using namespace cryptonotes;

MouseEventFilter::MouseEventFilter(QObject* parent) : QObject(parent) {
    
}

bool MouseEventFilter::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() != QEvent::MouseButtonRelease) return false;
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

    if (mouseEvent->button() != Qt::BackButton) return false;
    
    emit mouseBackButtonPressed();
    return false;
}

void MouseEventFilter::listenTo(QObject* obj) {
    if (!obj) return;

    obj->installEventFilter(this);
}