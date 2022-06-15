import QtQuick
import QtQuick.Controls 2.5
import QtQuick.Layouts
import Qt.labs.platform 1.1

Window {
    id: main_window
    width:  1280
    height: 720
    visible: true
    title: qsTr("Directory scanner & cleaner")
    color: 'lightgrey'

    GridLayout{
        anchors.fill: parent
        anchors.margins: 39
        rows: 4
        columns: 2
        rowSpacing: 10
        columnSpacing: 25

        FolderDialog {
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
            font {
                bold: true
                pixelSize: 16
            }
        }

        Rectangle {
            id: current_directory_path
            property alias directory_path: current_directory_path_text_edit.text

            Layout.row: 1
            Layout.column: 0
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
                        text: {
                            if (column === 0)
                                file_name
                            else if (column === 1)
                                inner_files
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
}
