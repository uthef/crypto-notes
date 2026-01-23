import QtQuick

import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects

ColumnLayout {
    property string name

    RowLayout {
        Layout.topMargin: appWindow.margin
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.alignment: Qt.AlignHCenter

        Image {
            id: logo
            source: "icons/note.png"
            Layout.preferredHeight: 64
            Layout.preferredWidth: 64
            Layout.alignment: Qt.AlignVCenter
        }

        Text {
            Layout.leftMargin: appWindow.margin
            Layout.rightMargin: appWindow.margin
            color: Material.foreground
            font.pixelSize: 42
            Layout.alignment: Qt.AlignVCenter
            text: "Crypto Notes"
        }
    }

    Text {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.alignment: Qt.AlignHCenter
        text: `Version: ${appCtx.appVersion()}`
        color: Material.foreground
        font.pixelSize: 14
    }

    RowLayout {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.alignment: Qt.AlignHCenter

        Text {
            color: Material.foreground
            font.pixelSize: 14 
            text: "GitHub repository:"
        }

        Text {
            font.pixelSize: 14
            textFormat: Text.RichText
            color: Material.foreground
            text: "<a href='/'>https://github.com/uthef/crypto-notes</a>"
            onLinkActivated: Qt.openUrlExternally("https://github.com/uthef/crypto-notes")

            MouseArea {
                id: ghLinkMouseArea
                anchors.fill: parent
                acceptedButtons: Qt.NoButton
                cursorShape: Qt.PointingHandCursor
            }
        }
    }

    Text {
        Layout.topMargin: 16
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        color: Material.foreground
        Layout.fillWidth: true
        font.pixelSize: 32
        text: "First start"
    }

    ColumnLayout {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.bottomMargin: appWindow.margin

        Text {
            textFormat: Text.RichText
            color: Material.foreground
            Layout.fillWidth: true
            font.pixelSize: 14
            text: "At the start screen, if you haven't used this app previously, type any password you want to be set for data encryption."
        }
    }
    
    Text {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        color: Material.foreground
        Layout.fillWidth: true
        font.pixelSize: 32
        text: "Shortcuts"
    }

    Text {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.topMargin: 16
        color: Material.foreground
        Layout.fillWidth: true
        font.pixelSize: 18
        text: "Note list"
    }

    Pane {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Material.elevation: 3
        Layout.fillWidth: true

        background: Item {
            RectangularShadow {
                anchors.fill: paneBg
                offset.x: 4
                offset.y: 4
                radius: paneBg.radius
                blur: 10
                spread: 1
                color: Qt.darker(paneBg.color, 1.6)
            }

            Rectangle {
                anchors.fill: parent
                id: paneBg
                radius: 8
                color: Qt.darker(Material.backgroundColor, 0.8)
            }
        }

        ColumnLayout {
            Text {
                textFormat: Text.RichText
                color: Material.foreground
                Layout.fillWidth: true
                font.pixelSize: 14
                text: "<b>Alt+Shift+Delete</b> - remove a note from the list<br><b>Alt+Shift+S</b> - copy note summary<br><b>Alt+Shift+C</b> - copy note content"
            }
        }
    }

    Text {
        Layout.topMargin: 16
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        color: Material.foreground
        Layout.fillWidth: true
        font.pixelSize: 18
        text: "Editor"
    }

    Pane {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Material.elevation: 3
        Layout.fillWidth: true

        background: Item {
            RectangularShadow {
                anchors.fill: paneBg2
                offset.x: 4
                offset.y: 4
                radius: paneBg2.radius
                blur: 10
                spread: 1
                color: Qt.darker(paneBg2.color, 1.6)
            }

            Rectangle {
                anchors.fill: parent
                id: paneBg2
                radius: 8
                color: Qt.darker(Material.backgroundColor, 0.8)
            }
        }

        ColumnLayout {
            Text {
                textFormat: Text.RichText
                color: Material.foreground
                Layout.fillWidth: true
                font.pixelSize: 14
                text: "<b>Ctrl+G</b> - generate a secure password and insert it into the content field"
            }
        }
    }

    Item {
        Layout.fillHeight: true
    }

    TextField {
        id: textField
        visible: false
    }

    Rectangle {
        Layout.preferredHeight: 1
        Layout.fillWidth: true
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.bottomMargin: 0
        color: Material.dividerColor
    }
    
    Button {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.topMargin: appWindow.margin / 2
        Layout.bottomMargin: appWindow.margin

        text: "Go back"
        Layout.preferredHeight: textField.height + 10
        onClicked: appWindow.goBackRequest()
    }
}