import QtQuick

import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Dialogs


ColumnLayout {
    id: settings

    property string name
    property bool backupSuccess: false

    function setUiEnabled(value) {
        appWindow.stackViewTransitionAllowed = value;

        pathListView.buttonsEnabled = value;
        backButton.enabled = value;
        confirmPasswordButton.enabled = value;
        browseButton.enabled = value;
        startBackupButton.enabled = value;
        addPathButton.enabled = value;
        showInExplorerButton.enabled = value;
        restoreButton.enabled = value;
    }

    Text {
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.topMargin: appWindow.margin
        Layout.bottomMargin: appWindow.margin
        text: "Configuration"
        color: Material.foreground
        Layout.fillWidth: true
        font.pixelSize: 32
    }

    Rectangle {
        height: 1
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.fillWidth: true
        Layout.bottomMargin: appWindow.margin
        color: Material.dividerColor
    }

    ScrollView {
        id: settingsScrollView
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        clip: true
        rightPadding: appWindow.margin * 2
        contentWidth: availableWidth
        ScrollBar.vertical.policy: Qt.ScrollBarAlwaysOn
        
        Binding {
            target: settingsScrollView.contentItem
            property: "boundsBehavior"
            value: Flickable.StopAtBounds
        }

        ColumnLayout {
            width: parent.width
            height: settingsScrollView.availableHeight

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                TextField {
                    id: pathField
                    text: appCtx.dbPath
                    readOnly: true
                    Layout.fillWidth: true
                    placeholderText: "Database path"
                }

                Button {
                    id: browseButton
                    text: "Browse…"
                    Layout.preferredHeight: pathField.height + 10
                    onClicked: folderDialog.openForChangingDbPath()
                }
                
                Button {
                    id: showInExplorerButton
                    icon.source: "icons/folder.svg"
                    text: "Show"
                    Layout.preferredHeight: pathField.height + 10
                    onClicked: Qt.openUrlExternally(appCtx.dbDir())
                }
            }

            Text {
                Layout.topMargin: 36
                color: Material.foreground
                Layout.fillWidth: true
                font.pixelSize: 18
                text: "Update database password"
            }

            RowLayout {
                Layout.topMargin: appWindow.margin
                spacing: 8
                Layout.fillWidth: true

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
                Layout.topMargin: 4
                id: errorMessage
            }

            Button {
                id: confirmPasswordButton
                Layout.topMargin: appWindow.margin / 2

                text: "Confirm new password"
                Layout.preferredHeight: pathField.height + 10
                onClicked: {
                    if (newPassField.length === 0 || oldPassField.length === 0) {
                        errorMessage.show("Both old and new passwords must be provided");
                        return;
                    }

                    appCtx.onPasswordUpdateRequested(oldPassField.text, newPassField.text);
                }
            }

            Text {
                Layout.topMargin: 36
                color: Material.foreground
                Layout.fillWidth: true
                font.pixelSize: 18
                text: "Replace the existing database file"
            }

            RowLayout {
                Layout.bottomMargin: appWindow.margin

                Button {
                    id: restoreButton
                    text: "Select file"
                    Layout.preferredHeight: pathField.height + 10
                    enabled: pathListView.count > 0

                    onClicked: {
                        fileDialog.open();
                    }
                }

                BusyIndicator {
                    Layout.alignment: Qt.AlignVCenter
                    id: restorationPreloader
                    Layout.preferredHeight: startBackupButton.height / 2
                    Layout.preferredWidth: startBackupButton.height / 2
                    Layout.leftMargin: appWindow.margin / 2
                    padding: 2
                    running: false
                }
            }

            Text {
                Layout.topMargin: appWindow.margin
                color: Material.foreground
                Layout.fillWidth: true
                font.pixelSize: 18
                text: "Database backup directories"
            }

            Frame {
                id: pathListFrame
                Layout.topMargin: appWindow.margin
                padding: 1
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 270

                ListView {
                    id: pathListView
                    property bool buttonsEnabled: true
                    boundsBehavior: Flickable.StopAtBounds

                    anchors.fill: parent
                    model: appCtx.backupPathListModel
                    delegate: pathListDelegate
                    headerPositioning: ListView.OverlayHeader
                    clip: true
                    activeFocusOnTab: true

                    ScrollBar.vertical: ScrollBar {
                        id: vertScrollbar
                        active: true
                    }

                    onActiveFocusChanged: () => {
                        pathListFrame.background.border.color = activeFocus ? Material.accentColor : Material.frameColor
                    }
                }
            }
            
            RowLayout {
                Layout.bottomMargin: appWindow.margin

                Button {
                    id: startBackupButton
                    text: "Start backup"
                    Layout.preferredHeight: pathField.height + 10
                    enabled: pathListView.count > 0

                    onClicked: {

                        for (let i = 0, j = 0; i < pathListView.count; i++) {
                            let listItem = pathListView.itemAtIndex(i).data[0];
                            listItem.state = "backup_unknown";
                        }

                        appCtx.initiateBackup()
                        settings.setUiEnabled(false);                        
                        backupPreloader.running = true;
                    }
                }

                BusyIndicator {
                    Layout.alignment: Qt.AlignVCenter
                    id: backupPreloader
                    Layout.preferredHeight: startBackupButton.height / 2
                    Layout.preferredWidth: startBackupButton.height / 2
                    Layout.leftMargin: appWindow.margin / 2
                    padding: 2
                    running: false
                }

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    id: addPathButton
                    text: "Add a directory"
                    icon.source: "icons/plus.svg" 
                    Layout.alignment: Qt.AlignRight
                    Layout.preferredHeight: pathField.height + 10
                    onClicked: folderDialog.openForAddingBackupPath()
                }
            }
        }
    }

    Rectangle {
        height: 1
        Layout.fillWidth: true
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        color: Material.dividerColor
    }

    Button {
        id: backButton
        Layout.topMargin: appWindow.margin / 2
        Layout.leftMargin: appWindow.margin
        Layout.rightMargin: appWindow.margin
        Layout.bottomMargin: appWindow.margin

        text: "Go back"
        Layout.preferredHeight: pathField.height + 10
        onClicked: appWindow.goBackRequest()
    }

    FileDialog {
        id: fileDialog
        nameFilters: ["*.edb"]

        onAccepted: {
            appWindow.popupRequest("settings", 
                "THIS ACTION IS IRREVERSIBLE.\nPLEASE, DO A BACKUP FIRST IF YOU WANT TO PREVENT DATA LOSS.\n\n"+
                "Are you sure you want to replace your current database file with the selected one?",
                true);
        }
    }

    FolderDialog {
        id: folderDialog
        property string postAction
        property string oldPath: ""
        modality: "ApplicationModal"

        onAccepted: {
            if (postAction === "changeDbPath") {
                appCtx.onNewDbPathSelected(folderDialog.selectedFolder);
            }
            else if (postAction === "changeBackupPath") {
                appCtx.onBackupPathChangeRequested(oldPath, folderDialog.selectedFolder);
                oldPath = "";
            }
            else if (postAction === "addBackupPath") {
                if (appCtx.onBackupPathAdditionRequested(folderDialog.selectedFolder)) {
                    Qt.callLater(() => pathListView.positionViewAtEnd());
                }
            }

            postAction = ""
        }

        function openForChangingDbPath() {
            if (visible) return;
            postAction = "changeDbPath";
            title = "Choose a new database directory";
            currentFolder = appCtx.dbDir();
            open();
        }

        function openForChangingBackupPath(p) {
            if (visible) return;
            postAction = "changeBackupPath";
            title = "Choose another backup directory";
            oldPath = p;
            currentFolder = "file:///" + p;
            open();
        }

        function openForAddingBackupPath() {
            if (visible) return;
            postAction = "addBackupPath";
            title = "Adding a new backup directory";
            currentFolder = "";
            open();
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

        function onBackupCompleted(failedPaths) {
            settings.setUiEnabled(true);    
            backupPreloader.running = false;

            for (let i = 0; i < pathListView.count; i++) {
                let listItem = pathListView.itemAtIndex(i).data[0];

                if (failedPaths.indexOf(listItem.path) != -1) {
                    listItem.state = "backup_error";
                    continue;
                }

                listItem.state = "backup_success";
            }
        }

        function onRestorationCompleted(success) {
            if (!success) appWindow.popupRequest("error", "An error occurred. The program could not rewrite your database file", false);

            settings.setUiEnabled(true);
        }
    }

    Component {
        id: pathListDelegate

        Item {
            height: item.height
            width: pathListFrame.width

            BackupPathListRow {
                id: item
                clickable: true
                path: model.path
            }
        }
    }

    Connections {
        target: appWindow

        function onPopupConfirmation(label) {
            if (label !== "settings") return;
            settings.setUiEnabled(false);
            appCtx.onDatabaseFileRestorationRequested(fileDialog.currentFile);
        }
    }
}
