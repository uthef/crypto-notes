import QtQuick

import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Dialogs


ColumnLayout {
    property string name;

    Text {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.topMargin: appWindow.margin
        text: "Configuration"
        color: Material.foreground
        Layout.fillWidth: true
        font.pixelSize: 32
        Layout.bottomMargin: 24
    }

    RowLayout {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        spacing: 8

        TextField {
            id: pathField
            text: appCtx.dbFolder
            enabled: false
            Layout.fillWidth: true
            placeholderText: "Database path"
        }

        Button {
            text: "Browse…"
            Layout.preferredHeight: pathField.height + 10
            onClicked: folderDialog.open()
        }
    }

    RowLayout {
        Layout.topMargin: 36
        spacing: 8
        Layout.fillWidth: true
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin

        TextField {
            id: oldPassField
            placeholderText: "Old password"
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            maximumLength: 128
            onTextChanged: errorMessage.hide()
            passwordCharacter: '●'
            echoMode: "Password"
        }

        TextField {
            id: newPassField
            placeholderText: "New password"
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            maximumLength: 128
            onTextChanged: errorMessage.hide()
            passwordCharacter: '●'
            echoMode: "Password"
        }
    }

    ErrorMessage {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin

        id: errorMessage
    }

    Button {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin

        text: "Update password"
        Layout.preferredHeight: pathField.height + 10
        onClicked: {
            if (newPassField.length === 0 || oldPassField.length === 0) {
                errorMessage.show("Both old and new passwords must be provided");
                return;
            }

            appCtx.onPasswordUpdateRequested(oldPassField.text, newPassField.text);
        }
    }

    Item  {
        Layout.fillHeight: true
    }

    Button {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.bottomMargin: appWindow.margin

        text: "Go back"
        Layout.preferredHeight: pathField.height + 10
        onClicked: appWindow.goBackRequest()
    }

    FolderDialog {
        id: folderDialog
        modality: "WindowModal"
        title: "Choose new database directory"

        onAccepted: {
            appCtx.onNewDbPathSelected(folderDialog.currentFolder);
        }
    }

    Connections {
        target: appCtx

        function onPasswordUpdateResult(msg, success) {
            if (success) {
                oldPassField.text = "";
                newPassField.text = "";
                return;
            }

            errorMessage.show(msg);
        }
    }
}
