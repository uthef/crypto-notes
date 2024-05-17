import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


ColumnLayout {
    id: root

    property int id;
    property int index;
    property string title;
    property string summary;
    property string content;

    Text {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.topMargin: appWindow.margin
        text: id == -1 ? "Create a new note" : "Edit note"
        color: Material.foreground
        Layout.fillWidth: true
        font.pixelSize: 32
        Layout.bottomMargin: 16
    }

    TextField {
        id: titleField
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        text: root.title
        maximumLength: 64
        Layout.fillWidth: true
        placeholderText: "Title"
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
        placeholderText: "Summary"
    }

    ErrorMessage {
        id: summaryMsg
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.topMargin: 4
        Layout.bottomMargin: 16
    }


    Flickable {
        ScrollBar.vertical: ScrollBar {
           active: true
        }

        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.minimumHeight: 100

        TextArea.flickable: TextArea {
            id: contentArea
            text: root.content
            placeholderText: "Note"
            KeyNavigation.priority: KeyNavigation.BeforeItem
            KeyNavigation.tab: updateCheck.enabled ? updateCheck : saveButton
        }
    }

    CheckBox {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        id: updateCheck
        leftPadding: 0
        text: "Update time"
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
                text: "Save"
                Layout.preferredHeight: 54
                icon.source: "icons/floppy.svg"
                icon.width: 15
                icon.height: 15
                antialiasing: false
                smooth: false
                onClicked: {
                    let correctFields = 2;
                    let flags = 0;

                    let title = titleField.text.trim();
                    let sum = summaryField.text.trim();

                    if (title.length === 0) {
                        titleMsg.show("Field cannot be empty");
                        correctFields--;
                    }
                    else titleMsg.hide();

                    if (title !== root.title) {
                        flags |= 1;
                    }
                    if (sum.length === 0) {
                        summaryMsg.show("Field cannot be empty");
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
                text: "Cancel"
                icon.width: 15
                icon.height: 15
                onClicked: appWindow.goBackRequest();
            }
        }

        Button {
            text: "Delete note"
            Layout.preferredHeight: 54
            Layout.alignment: Qt.AlignRight
            icon.source: "icons/bin.svg"
            icon.width: 15
            icon.height: 15
            enabled: id != -1
            onClicked: {
                appWindow.popupRequest("editor", "Are you sure you want to remove selected note? This action CANNOT BE UNDONE", true);
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
