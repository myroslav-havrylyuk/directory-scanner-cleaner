import QtQuick
import QtQuick.Controls 2.5
import QtQuick.Layouts
import Qt.labs.platform as Platform
import QtQuick.Dialogs

Window {
    id: settings_window
    visible: true
    width: 700
    height: 200
    title: qsTr("Settings")
    color: 'lightgrey'
    modality: Qt.ApplicationModal
    palette.highlight: "blue"
    palette.buttonText: "blue"

    Connections {
        target: SettingsController
        function onHistoryPathInvalid(){
            console.log("history path invalid");
            warning_dialog.open();

        }
    }

    Platform.FolderDialog {
        id: folder_dialog

        onAccepted: {
            SettingsController.historyPath = folder
        }
    }

    GridLayout{
        anchors.fill: parent
        anchors.margins: 39
        rows: 4
        columns: 3
        rowSpacing: 10
        columnSpacing: 25

        Text {
            id: browse_button
            Layout.row: 0
            Layout.column: 1
            text: "The directory to save deletion history file:"
            font {
                bold: true
                pixelSize: 16
            }
        }

        Rectangle {
            id: current_directory_path
            property alias directory_path: current_directory_path_text_edit.text

            Layout.row: 1
            Layout.column: 1
            Layout.fillWidth: true
            height: 24
            border.color: "black"
            border.width: 1
            clip: true
            TextEdit {
                id: current_directory_path_text_edit
                anchors{
                    fill: parent
                    leftMargin: 3
                    topMargin: 3
                }
                text: SettingsController.historyPath

                Keys.onReturnPressed: {
                    SettingsController.historyPath = text
                }
            }
        }
        Button {
            id: browse_current_directory_path_button

            Layout.row: 1
            Layout.column: 2
            implicitWidth: 90

            text: "Browse folder"
            onClicked: {
                folder_dialog.currentFolder = current_directory_path.directory_path
                folder_dialog.open()
            }
        }

        Button{
            id: save_button
            objectName: "save_button"
            implicitWidth: 90
            Layout.row: 2
            Layout.column: 2
            Layout.alignment: Qt.AlignRight
            text: qsTr("Save")

            signal saveSettings()
            onClicked: {
                console.log('close button pressed');
                save_button.saveSettings();
                settings_window.close();
            }
        }
    }

    Dialog {
        id: warning_dialog
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape
        title: qsTr("No such directory")
        contentItem: Text {
            text: "The directory does not exist or entered wrong. Please check specified path one more time and try again!"
            }
        modal: true
        standardButtons: Dialog.Ok
        onAccepted: console.log("Ok clicked")
    }
}
