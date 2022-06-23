import QtQuick
import QtQuick.Controls 2.5
import QtQuick.Layouts
import Qt.labs.platform as Platform
import QtQuick.Dialogs

ApplicationWindow {
    id: main_window
    width:  1280
    height: 720
    visible: true
    title: qsTr("Directory scanner & cleaner")
    //color: 'lightgrey'
    Material.theme: Material.Dark


    Connections {
        target: FileSystemController
        function onActivePathInvalid(){
            warning_dialog.open();
        }
        function onSetupModelCanceled(){
            progress_dialog.close();
        }
    }

    Connections {
        target: FileSystemModel
        function onModelSetupStarted(){
            progress_dialog.open();
            console.log('opened progress dialog');
        }
        function onModelSetupFinished(){
            progress_dialog.close();
            console.log('closed progress dialog');
        }
    }

    GridLayout{
        anchors.fill: parent
        anchors.margins: 39
        rows: 4
        columns: 2
        rowSpacing: 10
        columnSpacing: 25

        MenuBar{
                Menu{
                    title:  "&Options"
                    MenuItem{
                        property variant win
                        property bool clicked: false
                        text: "&Settings"
                        onTriggered: {
                            if(!clicked)
                            {
                                var component = Qt.createComponent("settingswindow.qml")
                                win = component.createObject(main_window)
                                clicked = true
                            }
                            SettingsController.state = 1
                            win.show()
                        }
                    }
                }
            }

        Platform.FolderDialog {
            id: folder_dialog

            onAccepted: {
                FileSystemController.activePath = folder
            }
        }

        Text {
            id: browse_button
            Layout.row: 0
            Layout.column: 0
            text: "Please choose a directory to scan: "
            //color: 'white'
            font {
                bold: true
                pixelSize: 16
            }
        }

        Rectangle {
            id: current_directory_path
            property alias directory_path: current_directory_path_text_edit.text
            color: '#3fcccccc'
            Layout.row: 1
            Layout.column: 0
            Layout.fillWidth: true
            height: 24
            //border.color: "black"
            //border.width: 1
            clip: true
            TextEdit {
                id: current_directory_path_text_edit
                //color: 'white'
                anchors{
                    fill: parent
                    leftMargin: 3
                    topMargin: 3
                }
                text: FileSystemController.activePath // could be a different folder for Linux

                Keys.onReturnPressed: {
                    FileSystemController.activePath = text
                }
            }
        }

        Rectangle {
            Layout.row: 3
            Layout.column: 0
            Layout.fillHeight: true
            Layout.fillWidth: true
            //color: '#3fcccccc'
            TreeView {
                id: tree_view
                anchors{
                    fill: parent
                    margins: 10
                }

                model: FileSystemModel
                clip: true

                delegate: TreeViewDelegate {
                    contentItem: Text {
                        anchors.leftMargin: leftMargin
                        anchors.rightMargin: rightMargin
                        //color: 'white'
                        text: {
                            if (column === 0)
                                file_name
                            else if (column === 1)
                                inner_files
                            else
                                file_size
                        }
                    }
                }
            }
        }


        Button {
            id: browse_current_directory_path_button

            Layout.row: 1
            Layout.column: 1
            width: 89

            text: "Browse folder"

            onClicked: {
                folder_dialog.currentFolder = current_directory_path.directory_path
                folder_dialog.open()
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

    Dialog {
        id: progress_dialog
        objectName: "progress_dialog"
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape
        title: qsTr("Scanning files...")
        contentItem: ProgressBar {
                indeterminate: true
            }
        modal: true
        standardButtons: Dialog.Cancel

        signal cancelSetupModel()

        onRejected: {
            progress_dialog.cancelSetupModel();
            console.log("Cancel clicked");
        }
    }
}
