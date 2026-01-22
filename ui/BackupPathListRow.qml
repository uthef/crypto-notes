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
                    onClicked: Qt.openUrlExternally("file:///" + row.path)

                    ToolTip.visible: hovered
                    ToolTip.text: "Open directory in file explorer"
                }

                RoundButton {
                    id: restoreFromFile
                    enabled: pathListView.buttonsEnabled
                    icon.source: "icons/restore.svg"
                    icon.width: 16
                    icon.height: 16
                    onClicked: {
                        fileDialog.currentFolder = "file:///" + row.path;
                        fileDialog.open();
                    }

                    ToolTip.visible: hovered
                    ToolTip.text: "Replace the current database file"
                }

                RoundButton {
                    id: changeButton
                    enabled: pathListView.buttonsEnabled
                    icon.source: "icons/edit.svg"
                    icon.width: 16
                    icon.height: 16
                    onClicked: folderDialog.openForChangingBackupPath(row.path)
                    ToolTip.visible: hovered
                    ToolTip.text: "Change directory"
                }

                RoundButton {
                    id: removeButton
                    enabled: pathListView.buttonsEnabled
                    Layout.rightMargin: 16
                    icon.source: "icons/bin.svg"
                    icon.width: 12
                    icon.height: 12
                    onClicked: {
                        appCtx.onBackupPathRemovalRequested(row.path);
                    }
                    ToolTip.visible: hovered
                    ToolTip.text: "Remove directory from the list"
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
            // if (!clickable) return;

            // noteListView.currentIndex = model.index
            // appCtx.onNoteRequested(model.index, false);
        }
    }

    Connections {
        target: pathListView
    }
}