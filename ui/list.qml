import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects

GridLayout {
    columns: 1
    rows: 3
    columnSpacing: 8
    rowSpacing: 8

    property string name

    Shortcut {
        sequence: "Alt+Shift+Delete"
        onActivated: {
            if (appWindow.currentItem !== "list" || !noteListView.activeFocus || noteListView.currentIndex === -1) return;
            appCtx.onNoteRemovalRequested(noteListView.currentIndex);
        }
    }

    Shortcut {
        sequence: "Alt+Shift+C"
        onActivated: {
            if (appWindow.currentItem !== "list" || noteListView.currentIndex === -1 || !noteListView.activeFocus) return;
            appCtx.onNoteRequested(noteListView.currentIndex, true);
        }
    }

    Shortcut {
        sequence: "Alt+Shift+S"
        onActivated: {
            if (appWindow.currentItem !== "list" || noteListView.currentIndex === -1 || !noteListView.activeFocus) return;
            appCtx.onCopyToClipboardRequest(noteListView.currentItem.data[0].summary);
        }
    }

    Connections {
        target: appCtx

        function onSearchProgress(finished) {
            noteListView.state = finished ? "normal" : "loading";
        }

        function onNoteDataAvailable(index, id, title, summary, content, time, shortcut) {
            if (shortcut) {
                appCtx.onCopyToClipboardRequest(content);
            }
            else appWindow.editorRequested(index, id, title, summary, content, time);
        }
    }

    RowLayout {
        Layout.row: 0
        Layout.column: 0
        Layout.fillWidth: true
        spacing: 10
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.topMargin: appWindow.margin

        Image {
            id: searchIcon
            Layout.preferredWidth: 32
            mipmap: true
            Layout.preferredHeight: 32;
            // color: Material.frameColor
            source: "icons/search_inactive.svg"
        }

        TextField {
            text: appCtx.searchQuery
            Layout.fillWidth: true
            placeholderText: "Search"
            onActiveFocusChanged: (f) => {
                if (f) searchIcon.source = "icons/search.svg";
                else searchIcon.source = "icons/search_inactive.svg";
            }
            maximumLength: 128
            onTextChanged: appCtx.onSearchRequest(text)
        }
    }

    Text {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.column: 0
        Layout.topMargin: 8
        Layout.row: 1
        color: Material.backgroundDimColor
        text: `Showing the latest ${appCtx.rowCount} note${appCtx.rowCount !== 1 ? "s" : "" }`
    }

    Frame {
        id: listFrame
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.bottomMargin: appWindow.margin
        Layout.alignment: Qt.AlignTop
        Layout.row: 2
        Layout.column: 0
        Layout.fillHeight: true
        padding: 1
        Layout.fillWidth: true
        clip: true

        ListView {
            id: noteListView
            anchors.fill: parent
            headerPositioning: ListView.OverlayHeader
            clip: true
            activeFocusOnTab: true
            highlightResizeVelocity: 10000
            model: appCtx.listModel
            delegate: delegate

            header: ListRow {
                title: "Title"
                summary: "Summary"
                time: "Last updated"

                bg: Rectangle {
                    color: Material.background
                }

                z: 2
            }

            ScrollBar.vertical: ScrollBar {
                id: vertScrollbar
                active: true
            }

            Keys.onSpacePressed: {
                if (noteListView.currentItem !== null)
                    appCtx.onNoteRequested(currentIndex, false);
            }

            onActiveFocusChanged: () => {
                listFrame.background.border.color = activeFocus ? Material.accentColor : Material.frameColor
            }

            highlight: Rectangle {
                color: Material.primary
                opacity: .7
            }

            states: [
                State {
                    name: "normal"
                    PropertyChanges {
                        target: noteListView
                        opacity: 1
                    }
                },
                State {
                    name: "loading"

                    PropertyChanges {
                        target: noteListView
                        opacity: .2
                    }
                }
            ]

            transitions: [
                Transition {
                    from: "*"
                    to: "*"

                    PropertyAnimation {
                        property: "opacity"
                        duration: 100
                    }
                }
            ]

            onStateChanged: state => enabled = !(noteListPreloader.running = (state === "loading"));
        }

        BusyIndicator {
            id: noteListPreloader
            Layout.row: 2
            running: false
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        RoundButton {
            Layout.row: 2
            anchors.left: parent.left
            anchors.leftMargin: 8
            icon.source: "icons/plus.svg"
            anchors.bottom: parent.bottom
            Layout.column: 0
            padding: 24
            icon.width: 24
            icon.height: 24

            onClicked: appWindow.editorRequested(-1, -1, "", "", "", "");
        }
    }

    Component {
        id: delegate

        Item {
            height: item.height
            width: listFrame.width

            ListRow {
                id: item
                clickable: true
                title: model.title
                summary: model.summary
                time: model.time
            }
        }
    }
}
