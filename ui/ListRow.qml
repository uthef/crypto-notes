import QtQuick

import QtQuick.Layouts
import QtQuick.Controls.Material

Row {
    id: row
    property string title;
    property string summary;
    property string time;
    property bool clickable: false

    property Rectangle bg: Rectangle {
        color: "transparent"
    }

    AbstractButton {
        id: button
        width: listFrame.width
        focusPolicy: "NoFocus"
        focus: false
        display: "TextOnly"

        background: bg

        contentItem: ColumnLayout {
            spacing: 0

            RowLayout {
                Text {
                    text: title
                    elide: Text.ElideRight
                    font.bold: !clickable
                    Layout.fillWidth: true
                    Layout.minimumWidth: 150
                    Layout.maximumWidth: 150
                    Layout.horizontalStretchFactor: 70
                    wrapMode: "WordWrap"
                    color: clickable ? Material.foreground : Material.backgroundDimColor
                    padding: 16
                    clip: true
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    text: summary
                    elide: Text.ElideRight
                    font.bold: !clickable
                    wrapMode: "WordWrap"
                    clip: true
                    Layout.fillWidth: true
                    Layout.horizontalStretchFactor: 100
                    Layout.minimumWidth: 200
                    color: clickable ? Material.foreground : Material.backgroundDimColor
                    padding: 16
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    text: time
                    elide: Text.ElideRight
                    font.bold: !clickable
                    wrapMode: "WordWrap"
                    horizontalAlignment: Qt.AlignRight
                    clip: true
                    Layout.maximumWidth: 120
                    Layout.fillWidth: true
                    Layout.horizontalStretchFactor: 30
                    Layout.minimumWidth: 80
                    color: clickable ? Material.foreground : Material.backgroundDimColor
                    padding: 16
                    verticalAlignment: Text.AlignVCenter
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

        onPressed: {
            if (clickable) highlightAnim.start()
        }

        onReleased: {
            if (clickable) fadeAnim.start()
        }

        onHoveredChanged: hovered => {
            if (!hovered && clickable) fadeAnim.start()
        }

        onClicked: {
            if (!clickable) return;

            noteListView.currentIndex = model.index
            appCtx.onNoteRequested(model.index, false);
        }
    }
}
