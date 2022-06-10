import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform 1.1

Window {
    id: main_window
    width:  640
    height: 480
    visible: true
    title: qsTr("Demo")

    Rectangle {
        id: current_directory_path_item
        width: main_window.width * 0.8
        height: 100
        anchors.horizontalCenter: parent.horizontalCenter

        FolderDialog {
            id: folder_dialog

            onAccepted: {
                current_directory_path.directory_path = folder
            }
        }

        ColumnLayout {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            Text {
                id: browse_button
                anchors.fill: parent
                text: "Please choose a directory to scan: "
            }

            RowLayout {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right

                Rectangle {
                    property alias directory_path: current_directory_path_text_edit.text

                    id: current_directory_path
                    Layout.minimumWidth: parent.width - browse_current_directory_path_button.width
                    Layout.minimumHeight: parent.height
                    border.color: "black"
                    border.width: 1
                    clip: true

                    TextEdit {
                        id: current_directory_path_text_edit
                        anchors.fill: parent
                        anchors.leftMargin: 3
                        anchors.topMargin: 3
                        width: parent.width
                        text: "C:/" // could be a different folder for Linux
                    }
                }

                Button {
                    id: browse_current_directory_path_button
                    anchors.right: parent.right
                    text: "Browse folder"

                    onClicked: {
                        // need to validate folder - need to add controller
                        folder_dialog.currentFolder = "file:///" + current_directory_path.directory_path
                        folder_dialog.open()
                    }
                }
            }
        }
    }

    Rectangle {
        width: main_window.width * 0.8
        height: (parent.height - current_directory_path_item.height) * 0.95
        anchors.top: current_directory_path_item.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: "yellow"

        TreeView {
            anchors.fill: parent
            model: FileManagerModel
        }
    }
}
