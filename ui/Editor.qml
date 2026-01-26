import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts


ColumnLayout {
    id: root

    property int id
    property int index
    property string title
    property string summary
    property string content
    property string time

    property string name

    function insertPassword() {
        if (contentArea.text.length > 0 && !contentArea.text.endsWith("\n")) contentArea.text += "\n";
        contentArea.text += appCtx.generatePassword();
        contentArea.text += "\n";
        contentArea.cursorPosition = contentArea.length;
    }

    Shortcut {
        sequence: "Ctrl+G"
        onActivated: insertPassword()
    }

    Text {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.topMargin: appWindow.margin
        text: id == -1 ? qsTr("Create a new note") : qsTr("Edit note")
        color: Material.foreground
        Layout.fillWidth: true
        font.pixelSize: 32
        Layout.bottomMargin: 24
    }

    TextField {
        id: titleField
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        text: root.title
        maximumLength: 64
        Layout.fillWidth: true
        placeholderText: qsTr("Title")
    }

    ErrorMessage {
        id: titleMsg
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.topMargin: 4
        Layout.bottomMargin: 10
    }

    TextField {
        id: summaryField
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        text: root.summary
        maximumLength: 128
        Layout.fillWidth: true
        placeholderText: qsTr("Summary")
    }

    ErrorMessage {
        id: summaryMsg
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.topMargin: 4
        Layout.bottomMargin: 16
    }

    Flickable {
        id: flickable

        ScrollBar.vertical: ScrollBar {
           active: true
           topPadding: 18
           bottomPadding: 8
           rightPadding: 8
        }

        flickableDirection: Flickable.VerticalFlick

        boundsBehavior: Flickable.StopAtBounds
        clip: true
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.minimumHeight: 100

        TextArea.flickable: TextArea {
            id: contentArea
            padding: 27
            text: root.content
            placeholderText: qsTr("Note")
            KeyNavigation.priority: KeyNavigation.BeforeItem
            KeyNavigation.tab: updateCheck.enabled ? updateCheck : saveButton
            KeyNavigation.backtab: summaryField

            onHeightChanged: {
                flickable.contentY += font.pixelSize * 2;
            }
        }
    }

    CheckBox {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        id: updateCheck
        leftPadding: 0
        text: qsTr("Update time") + (id != -1 ? ` (${qsTr("last edited")}: ${root.time})` : "")
        checked: true
        enabled: id != -1
    }

    RowLayout {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.bottomMargin: appWindow.margin
        Layout.preferredWidth: parent.width

        RowLayout {
            Button {
                id: saveButton
                text: qsTr("Save")
                Layout.preferredHeight: 54
                icon.source: "icons/floppy.svg"
                icon.width: 18
                icon.height: 18
                Layout.preferredWidth: 120
                antialiasing: false
                smooth: false
                onClicked: {
                    let correctFields = 2;
                    let flags = 0;

                    let title = titleField.text.trim();
                    let sum = summaryField.text.trim();

                    if (title.length === 0) {
                        titleMsg.show(qsTr("Field cannot be empty"));
                        correctFields--;
                    }
                    else titleMsg.hide();

                    if (title !== root.title) {
                        flags |= 1;
                    }
                    if (sum.length === 0) {
                        summaryMsg.show(qsTr("Field cannot be empty"));
                        correctFields--;
                    }
                    else summaryMsg.hide();

                    if (correctFields < 2) return;

                    let content = contentArea.text.trim();

                    if (sum !== root.summary) {
                        flags |= 2;
                    }

                    if (content !== root.content) {
                        flags |= 4;
                    }

                    appCtx.onNoteUpdateRequested(id, title, sum, content, flags, updateCheck.checked);

                    appWindow.goBackRequest();
                }
            }

            Button {
                id: cancelButton
                Layout.preferredHeight: 54
                Layout.preferredWidth: 100
                text: qsTr("Cancel")
                icon.width: 18
                icon.height: 18
                onClicked: appWindow.goBackRequest();
            }
        }
        
        RowLayout {
            Layout.alignment: Qt.AlignRight

            Button {
                text: qsTr("Generate password")
                Layout.preferredHeight: 54
                Layout.preferredWidth: 200
                icon.source: "icons/lock.svg"
                icon.width: 18
                icon.height: 18
                onClicked: insertPassword()
            }

            Button {
                text: qsTr("Delete note")
                Layout.preferredHeight: 54
                Layout.preferredWidth: 150
                icon.source: "icons/bin.svg"
                icon.width: 18
                icon.height: 18
                enabled: id != -1
                onClicked: {
                    appWindow.popupRequest("editor", qsTr("Are you sure you want to remove selected note? This action CANNOT BE UNDONE"), true);
                }
            }
        }

        Connections {
            target: appWindow

            function onPopupConfirmation(label) {
                if (label !== "editor") return;

                appCtx.onNoteRemovalRequested(root.index)
                appWindow.goBackRequest();
            }
        }
    }
}
