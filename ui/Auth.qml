import QtQuick

import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: authItem
    signal error(string msg)
    signal erasePass()

    property string name

    Component.onCompleted: {
        passwordField.forceActiveFocus();
    }

    ColumnLayout {
        width: parent.width
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 8

        Item {
            Layout.fillHeight: true
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            font.pixelSize: 16
            Layout.fillWidth: true
            Layout.maximumWidth: 400
            text: qsTr("Master password")
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
            text: ` ${qsTr("Continue")}` 
            font.pixelSize: 16
            onClicked: appWindow.passwordReady(passwordField.text)
            Layout.preferredHeight: passwordField.height + 10
            Layout.maximumWidth: 400
            Layout.fillWidth: true
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: appWindow.margin

            Image {
                source: "icons/language.svg"
                sourceSize.width: 24
                sourceSize.height: 24
                Layout.alignment: Qt.AlignVCenter
            }

            Label {
                Layout.rightMargin: 8
                text: qsTr("Interface language")
            }

            ComboBox {
                Layout.preferredWidth: 150
                
                model: ListModel {
                    ListElement {
                        value: "en"
                        text: "English"
                    }

                    ListElement {
                        value: "ru"
                        text: "Русский"
                    }
                }

                textRole: "text"
                valueRole: "value"

                Component.onCompleted: {
                    let langCode = appCtx.language();
                    if (indexOfValue(langCode) >= 0) currentValue = langCode;
                }

                onActivated: {
                    appCtx.onLanguageChange(currentValue, true);
                    appEngine.uiLanguageChanged();
                }
            }
        }
    }

    onError: (msg) => errorMessage.show(msg);
    onErasePass: {
        passwordField.text = "";
        passwordField.forceActiveFocus();
    }
}
