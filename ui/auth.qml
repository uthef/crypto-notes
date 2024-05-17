import QtQuick

import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: authItem
    signal error(string msg)
    signal erasePass()

    property string name;

    Component.onCompleted: {
        passwordField.forceActiveFocus();
    }

    ColumnLayout {
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 8

        Label {
            Layout.alignment: Qt.AlignHCenter
            font.pixelSize: 16
            Layout.fillWidth: true
            Layout.maximumWidth: 400
            text: "Master password"
        }

        TextField {
            id: passwordField
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.maximumWidth: 400
            maximumLength: 128
            onTextChanged: errorMessage.hide()
            passwordCharacter: '●'
            echoMode: "Password"
            Keys.onReturnPressed: continueButton.clicked()
        }

        ErrorMessage {
            id: errorMessage
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            id: continueButton
            icon.source: "icons/check.svg"
            icon.width: 18
            icon.height: 18
            Layout.alignment: Qt.AlignHCenter
            text: "Continue"
            onClicked: appWindow.passwordReady(passwordField.text)
            font.pixelSize: 16
            Layout.preferredHeight: passwordField.height + 10
            Layout.maximumWidth: 400
            Layout.fillWidth: true
        }
    }

    onError: (msg) => errorMessage.show(msg);
    onErasePass: {
        passwordField.text = "";
        passwordField.forceActiveFocus();
    }
}
