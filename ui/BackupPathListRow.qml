import QtQuick

import QtQuick.Layouts
import QtQuick.Controls.Material

Row {
    id: row

    property string path
    property bool clickable: false
    property int stateIndex: 0

    property Rectangle bg: Rectangle {
        color: "transparent"
    }

    function grabFocus() {
        openFolderButton.forceActiveFocus();
    }

    states: [
        State {
            name: "backup_unknown"
            PropertyChanges {
                row {
                    stateIndex: 0
                }
            }
        },
        State {
            name: "backup_error"
            PropertyChanges {
                row {
                    stateIndex: 1
                }
            }
        },
        State {
            name: "backup_success"
            PropertyChanges {
                row {
                    stateIndex: 2
                }
            }
        }
    ]

    AbstractButton {
        id: button
        width: pathListFrame.width
        focusPolicy: Qt.NoFocus
        focus: false
        display: "TextOnly"

        background: bg

        contentItem: ColumnLayout {
            spacing: 0

            RowLayout {
                Text {
                    text: path
                    elide: Text.ElideRight
                    font.bold: !clickable
                    Layout.fillWidth: true
                    wrapMode: "WordWrap"
                    color: clickable ? Material.foreground : Material.backgroundDimColor
                    padding: 16
                    clip: true
                    verticalAlignment: Text.AlignVCenter
                }

                StackLayout {
                    id: backupStatus
                    Layout.rightMargin: 7
                    Layout.fillHeight: false
                    Layout.fillWidth: false
                    Layout.alignment: Qt.AlignLeft
                    Layout.preferredHeight: 18
                    Layout.preferredWidth: 18
                    currentIndex: row.stateIndex

                    Item {

                    }

                    Image {
                        id: backupErrorImage
                        source: "icons/error.svg"
                        sourceSize.height: parent.height
                        sourceSize.width: parent.height
                    }

                    Image {
                        id: backupSuccessImage
                        source: "icons/green_check.svg"
                        sourceSize.height: parent.height
                        sourceSize.width: parent.height
                    }
                }

                RoundButton {
                    id: openFolderButton
                    enabled: pathListView.buttonsEnabled
                    icon.source: "icons/folder.svg"
                    icon.width: 16
                    icon.height: 16
                    focusPolicy: Qt.NoFocus
                    onClicked: {
                        pathListView.currentIndex = model.index;
                        Qt.openUrlExternally(appWindow.getPathWithFilePrefix(row.path));
                    }

                    ToolTip.visible: hovered | focus
                    ToolTip.text: qsTr("Open directory in file explorer")

                    Keys.onRightPressed: {
                        restoreFromFile.forceActiveFocus();
                    }

                }

                RoundButton {
                    id: restoreFromFile
                    enabled: pathListView.buttonsEnabled
                    icon.source: "icons/restore.svg"
                    icon.width: 16
                    icon.height: 16
                    focusPolicy: Qt.NoFocus
                    onClicked: {
                        pathListView.currentIndex = model.index;
                        fileDialog.currentFolder = appWindow.getPathWithFilePrefix(row.path);
                        fileDialog.open();
                    }

                    ToolTip.visible: hovered | focus
                    ToolTip.text: qsTr("Restore database from a backup file")

                    Keys.onLeftPressed: {
                        openFolderButton.forceActiveFocus();
                    }

                    Keys.onRightPressed: {
                        changeButton.forceActiveFocus();
                    }
                }

                RoundButton {
                    id: changeButton
                    enabled: pathListView.buttonsEnabled
                    icon.source: "icons/edit.svg"
                    icon.width: 16
                    icon.height: 16
                    focusPolicy: Qt.NoFocus
                    onClicked: {
                        pathListView.currentIndex = model.index;
                        folderDialog.openForChangingBackupPath(model.index, row.path)
                    }
                    ToolTip.visible: hovered | focus
                    ToolTip.text: qsTr("Change directory")

                    Keys.onLeftPressed: {
                        restoreFromFile.forceActiveFocus();
                    }

                    Keys.onRightPressed: {
                        removeButton.forceActiveFocus();
                    }
                }

                RoundButton {
                    id: removeButton
                    enabled: pathListView.buttonsEnabled
                    Layout.rightMargin: 16
                    icon.source: "icons/bin.svg"
                    icon.width: 12
                    icon.height: 12
                    focusPolicy: Qt.NoFocus
                    onClicked: {
                        pathListView.currentIndex = model.index;
                        appCtx.onBackupPathRemovalRequested(model.index);
                    }
                    ToolTip.visible: hovered | focus
                    ToolTip.text: qsTr("Remove directory from the list")

                    Keys.onLeftPressed: {
                        changeButton.forceActiveFocus();
                    }
                }
            }

            Rectangle {
                height: 1
                Layout.fillWidth: true
                color: Material.dividerColor
            }
        }

        SequentialAnimation {
            id: highlightAnim

            ColorAnimation {
                target: row.bg
                property: "color"
                to: Material.rippleColor
                duration: 150
            }
        }

        SequentialAnimation {
            id: fadeAnim

            ColorAnimation {
                target: row.bg
                property: "color"
                to: "transparent"
                duration: 250
            }
        }

        SequentialAnimation {
            id: focusAnim

            ColorAnimation {
                target: row.bg
                property: "color"
                to: Material.primaryColor
                duration: 200
            }
        }

        // onPressed: {
        //     if (clickable) highlightAnim.start()
        // }

        // onReleased: {
        //     if (clickable) fadeAnim.start()
        // }

        onHoveredChanged: hovered => {
            if (!hovered && clickable) fadeAnim.start()
        }

        onClicked: {
            if (!clickable) return;

            pathListView.currentIndex = model.index
            pathListView.forceActiveFocus();
        }
    }

    Connections {
        target: pathListView
    }
}
