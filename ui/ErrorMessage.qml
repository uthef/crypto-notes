import QtQuick

import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts


RowLayout {
    id: errorMsg
    opacity: 0
    signal show(string msg)
    signal hide()

    spacing: 4


    onShow: (msg) => {
        errorText.text = msg;
        errorMsg.state = "visible";
    }

    onHide: () => {
        errorMsg.state = "invisible";
    }

    states: [
        State {
            name: "invisble"
            PropertyChanges {
                target: errorMsg
                opacity: 0
            }
        },

        State {
            name: "visible"
            PropertyChanges {
                target: errorMsg
                opacity: 1
            }
        }

    ]

    transitions: [
        Transition {
            from: "*"
            to: "*"

            PropertyAnimation {
                property: "opacity"
                duration: 200
            }
        }

    ]

    Image {
        id: icon
        mipmap: true
        source: "icons/error.svg"
        Layout.preferredHeight: 16
        Layout.preferredWidth: 16
    }

    Text {
        id: errorText
        text: "Error text"
        wrapMode: Text.WordWrap
        Layout.maximumWidth: 400 - (icon.width + parent.spacing)
        Layout.fillWidth: true
        color: "#d95148"
    }
}

