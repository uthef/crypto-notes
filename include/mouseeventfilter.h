#ifndef MOUSE_EVENT_FILTER_H
#define MOUSE_EVENT_FILTER_H

#include <QObject>

namespace cryptonotes {
    class MouseEventFilter : public QObject {
    Q_OBJECT

    public:
        MouseEventFilter(QObject* parent = nullptr);
        bool eventFilter(QObject* obj, QEvent* event) override;
        Q_INVOKABLE void listenTo(QObject* obj);
    
    signals:
        void mouseBackButtonPressed();
    };
}

#endif