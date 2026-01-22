import QtQuick

import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

import cryptonotes

ApplicationWindow {
    id: appWindow
    width: 860
    height: 640
    visible: true
    title: "Crypto Notes App"
    minimumHeight: 640
    minimumWidth: 860

    Material.theme: Material.Dark
    Material.accent: Material.Blue

    property int margin: 16
    property string currentItem: stackView.currentItem.item.name
    property bool stackViewTransitionAllowed: false

    function visibilityChangeHandler(e) {
        if (e === Window.Maximized) {
            appCtx.onWindowVisibilityChanged(true);
            return;
        }

        if (e == Window.Windowed) appCtx.onWindowVisibilityChanged(false);
    }

    Component.onCompleted: {
        MouseEventFilter.listenTo(appWindow);

        if (appCtx.isWindowMaximized()) {
            visibility = Window.Maximized;
        }
        else {
            if (appCtx.windowWidth() > 0) width = appCtx.windowWidth();
            if (appCtx.windowHeight() > 0) height = appCtx.windowHeight(); 

            x = Screen.width / 2 - width / 2;
            y = Screen.height / 2 - height / 2;
        }

        this.visibilityChanged.connect(visibilityChangeHandler);
    }

    Connections {
        target: MouseEventFilter

        function onMouseBackButtonPressed() {
            if (stackView.currentItem.item.name === "list") goToAuthRequest();
            else goBackRequest();
        }
    }   

    Text {
        text: ""
        color: "white"
    }

    onWidthChanged: appCtx.onWindowWidthChanged(width)
    onHeightChanged: appCtx.onWindowHeightChanged(height)

    menuBar: MenuBar {
        Menu {
            title: "Database"

            MenuItem {
                id: menuItemConfigure
                text: "Configure…"
                onClicked: {
                    if (!appWindow.stackViewTransitionAllowed) return;

                    if (stackView.currentItem.item.name === "about") {
                        stackView.replace(settingsComp);
                        return;
                    }

                    if (stackView.currentItem.item.name !== "auth") return;

                    stackView.push(settingsComp);
                }
            }

            MenuItem {
                id: menuItemClose
                text: "Close"
                enabled: false
                onClicked: {
                    goToAuthRequest();
                }
            }
        }

        Menu {
            title: "Help"

            MenuItem {
                id: menuItemAbout
                text: "About"
                onClicked: {
                    if (!appWindow.stackViewTransitionAllowed) return;
                    if (stackView.currentItem.item.name === "about") return;

                    if (stackView.currentItem.item.name === "settings") {
                        stackView.replace(aboutComp);
                        return;
                    }

                    stackView.push(aboutComp);
                }
            }
        }
    }

    signal passwordReady(string password)
    signal editorRequested(int index, int id, string title, string summary, string content, string time);
    signal goBackRequest()
    signal goToAuthRequest();
    signal popupRequest(string label, string text, bool dialog)
    signal popupConfirmation(string label)

    StackView {
        id: stackView
        width: parent.width
        height: parent.height
        initialItem: authComp

        pushEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: appWindow.width
                to: 0
                easing.type: Easing.InSine
                duration: 300
            }
        }

        pushExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: -appWindow.width
                easing.type: Easing.InSine
                duration: 300
            }
        }

        popEnter: Transition {
            PropertyAnimation {
                property: "x"
                from: -appWindow.width
                to: 0
                easing.type: Easing.InSine
                duration: 300
            }
        }

        popExit: Transition {
            PropertyAnimation {
                property: "x"
                from: 0
                to: appWindow.width
                easing.type: Easing.InSine
                duration: 300
            }
        }
    }

    Component {
        id: authComp

        Loader {
            source: "Auth.qml"

            onLoaded: {
                item.name = "auth"
            }

            StackView.onActivating: appWindow.stackViewTransitionAllowed = false
            StackView.onActivated: appWindow.stackViewTransitionAllowed = true
            StackView.onDeactivating: appWindow.stackViewTransitionAllowed = false
            StackView.onDeactivated: appWindow.stackViewTransitionAllowed = true
        }
    }

    Component {
        id: listComp

        Loader {
            source: "List.qml"

            onLoaded: {
                item.name = "list"
            }

            StackView.onActivating: appWindow.stackViewTransitionAllowed = false
            StackView.onActivated: appWindow.stackViewTransitionAllowed = true
            StackView.onDeactivating: appWindow.stackViewTransitionAllowed = false
            StackView.onDeactivated: appWindow.stackViewTransitionAllowed = true
        }
    }

    Component {
        id: editorComp

        Loader {
            source: "Editor.qml"

            onLoaded: {
                item.name = "editor"
            }

            StackView.onActivating: appWindow.stackViewTransitionAllowed = false
            StackView.onActivated: appWindow.stackViewTransitionAllowed = true
            StackView.onDeactivating: appWindow.stackViewTransitionAllowed = false
            StackView.onDeactivated: appWindow.stackViewTransitionAllowed = true
        }
    }

    Component {
        id: settingsComp

        Loader {
            source: "Settings.qml"

            onLoaded: {
                item.name = "settings"
            }

            StackView.onActivating: appWindow.stackViewTransitionAllowed = false
            StackView.onActivated: appWindow.stackViewTransitionAllowed = true
            StackView.onDeactivating: appWindow.stackViewTransitionAllowed = false
            StackView.onDeactivated: appWindow.stackViewTransitionAllowed = true
        }
    }

    Component {
        id: aboutComp

        Loader {
            source: "About.qml"

            onLoaded: {
                item.name = "about"
            }

            StackView.onActivating: appWindow.stackViewTransitionAllowed = false
            StackView.onActivated: appWindow.stackViewTransitionAllowed = true
            StackView.onDeactivating: appWindow.stackViewTransitionAllowed = false
            StackView.onDeactivated: appWindow.stackViewTransitionAllowed = true
        }
    }

    onPasswordReady: (password) => {
        if (stackView.currentItem.item.name !== "auth") return;

        if (password.length === 0) {
            stackView.currentItem.item.error("Password cannot be blank");
            return;
        }

        appCtx.onPasswordValidated(password);
    }

    onPopupConfirmation: {
        if (popup.label !== "abort") return;

        goToAuthRequest();
    }

    onGoToAuthRequest: {
        if (!appWindow.stackViewTransitionAllowed) return;
        if (stackView.currentItem.item.name === "auth") return;

        appCtx.onDbDisconnectionRequest();

        while (stackView.depth > 1) {
            stackView.pop();
        }

        menuItemClose.enabled = false;
        menuItemConfigure.enabled = true;
    }

    onEditorRequested: (index, id, title, summary, content, time) => {
        if (stackView.currentItem.item.name !== "list") return;

        stackView.push(editorComp);

        stackView.currentItem.item.index = index;
        stackView.currentItem.item.id = id;
        stackView.currentItem.item.title = title;
        stackView.currentItem.item.summary = summary;
        stackView.currentItem.item.content = content;
        stackView.currentItem.item.time = time;
    }

    onGoBackRequest: {
        if (!appWindow.stackViewTransitionAllowed) return;

        if (popup.visible) {
            popup.close();
            return;
        }

        stackView.pop();
    }

    onPopupRequest: (label, text, dialog) => {
        if (popup.opened) return;

        popup.text = text;
        popup.label = label;
        popup.dialog = dialog;

        popup.open()
    }

    Connections {
        target: appCtx

        function onDbConnectionFail(msg, success) {
            stackView.currentItem.item.error(msg);
        }

        function onDbConnectionSuccess() {
            stackView.currentItem.item.erasePass();
            stackView.push(listComp);
            menuItemClose.enabled = true;
            menuItemConfigure.enabled = false;
        }

        function onAbort(msg) {
            popupRequest("abort", "Database connection error. " + msg, false);
        }

        function onError(msg) {
            popupRequest("error", "An error occurred. " + msg, false);
        }
    }

    Popup {
        id: popup
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        modal: true
        anchors.centerIn: parent
        focus: true
        height: 180

        property string text: "No text"
        property bool dialog: false
        property string label: ""
        property bool confirmed: false

        onClosed: {
            if (popup.confirmed) popupConfirmation(popup.label);

            popup.confirmed = false;
        }

        ColumnLayout {
            anchors.centerIn: parent

            Text {
                text: popup.text
                Layout.topMargin: 16
                Layout.maximumWidth: 300
                horizontalAlignment: Qt.AlignHCenter
                wrapMode: Text.WordWrap
                color: Material.foreground
            }

            RowLayout {
                Layout.topMargin: 8
                Layout.alignment: Qt.AlignHCenter
                Layout.margins: 0

                Button {
                    text: popup.dialog ? "Yes" : "Ok"
                    onClicked: {
                        popup.confirmed = true;
                        popup.close();
                    }
                }

                Button {
                    visible: popup.dialog
                    text: "No"
                    onClicked: popup.close()
                }
            }
        }
    }
}
