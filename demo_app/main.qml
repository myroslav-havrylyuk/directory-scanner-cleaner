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
                FileManagerController.activePath = folder
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
                text: FileManagerController.activePath // could be a different folder for Linux

                Keys.onReturnPressed: {
                    FileManagerController.activePath = text
                }
            }
        }

        Rectangle {

            height: include_text.implicitHeight
            Layout.row: 2
            Layout.column: 0
            Layout.fillWidth: true
            clip: true
            color: 'lightgrey'

            Row{
                anchors.fill: parent
                spacing: 15
                leftPadding: 5
                rightPadding: 5
                clip: true

                Text{
                    id: include_text
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    verticalAlignment: Text.AlignVCenter
                    text: "Include:"
                    font {
                        bold: true
                        pixelSize: 16
                    }
                }
                CheckBox{

                    width: 124
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    text: "Subdirectories"
                }
                CheckBox{

                    width: 124
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }

                    text: "Equal files"
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

                model: FileManagerModel
                clip: true

                delegate: TreeViewDelegate {
                    contentItem: Text {
                        anchors.leftMargin: leftMargin
                        anchors.rightMargin: rightMargin
                        text: file_name
                        text: {
                            if (column === 0)
                                file_name
                            else if (column === 1)
                                inner_files
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

        Rectangle {

            width: 89
            Layout.row: 3
            Layout.column: 1
            Layout.fillHeight: true
            clip: true

            color: 'lightgrey'


            Column {
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    topMargin: 5
                    bottomMargin: 5
                }

                spacing: 5
                Text {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    text: qsTr("Filters")
                    font {
                        pixelSize: 18
                        bold: true
                    }
                }

                Text {
                    text: qsTr("Older then:")
                }
                Rectangle {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    height: 24
                    border.color: "black"
                    border.width: 1
                    clip: true
                    TextEdit{
                        anchors{
                            fill: parent
                            leftMargin: 3
                            topMargin: 3
                        }
                    }
                }
                Text {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    text: qsTr("Larger then:")
                }
                Rectangle {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    height: 24
                    border.color: "black"
                    border.width: 1
                    clip: true
                    TextEdit{
                        anchors{
                            fill: parent
                            leftMargin: 3
                            topMargin: 3
                        }
                    }
                }
                Text {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    text: qsTr("By pattern:")
                }
                Rectangle {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    height: 24
                    border.color: "black"
                    border.width: 1
                    clip: true
                    TextEdit{
                        anchors{
                            fill: parent
                            leftMargin: 3
                            topMargin: 3
                        }
                    }
                }
                Button{
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    height: 24

                    text: qsTr("Filter")
                }


            }
            Button{

                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                height: 24

                text: qsTr("Delete")
            }
        }
    }
}
