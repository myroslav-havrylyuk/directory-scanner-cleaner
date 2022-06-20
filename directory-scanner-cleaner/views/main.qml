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
                property int file_name_column_max_width: 0
                property int inner_files_column_max_width: 0
                property int file_size_column_max_width: 0

                id: tree_view
                anchors{
                    fill: parent
                    margins: 10
                }

                model: FileSystemModel
                selectionModel: TestItemSelectionModel
                clip: true

                onCollapsed: {
                    file_name_column_max_width = 0
                    inner_files_column_max_width = 0
                    file_size_column_max_width = 0
                }

                /* Problems with the current implementation:
                   1) When top object`s name is the biggest in the tree which is then expanded we can see file_name area
                   becoming larger by the value of "levelHorizontalDifference". The problem is the invalid "depth" value for
                   that top element(on start that equals 0 which is correct while later it can receive depth value of 1 or bigger).
                   2) On some level we can get the situation when file_name value is not aligned with the other rows` respected
                   values.
                */

                delegate: TreeViewDelegate {
                    id: tdelegate
                    property bool isSelected: FileSystemController.selected && tree_view.selectionModel.isSelected(tree_view.modelIndex(row, column))
                    property int levelHorizontalDifference: 16

                    contentItem: Item {
                        id: item
                        implicitHeight: delegate_row_layout.getHeight()
                        implicitWidth: delegate_row_layout.getWidth()
                        width: implicitWidth
                        height: implicitHeight

                        RowLayout {
                            id: delegate_row_layout
                            anchors.fill: parent
                            spacing: 0
                            readonly property int elements_number: 2

                            function getWidth() {
                                return (row_selection_area.getWidth() + row_selected_check_box.width +
                                        delegate_row_layout.spacing * (delegate_row_layout.elements_number - 1))
                            }

                            function getHeight() {
                                return row_selection_area.getHeight()
                            }

                            CheckBox {
                                property bool isExternalChange: false
                                id: row_selected_check_box

                                checkState: {
                                    isSelected ? Qt.Checked : Qt.Unchecked
                                }

                                onCheckedChanged: {
                                    if (isExternalChange === false) {
                                        FileSystemController.currentlySelectedIndex = tree_view.modelIndex(row, column)
                                    }

                                    isExternalChange = false
                                }
                            }

                            Rectangle {
                                id: row_selection_area
                                Layout.fillHeight: true
                                Layout.preferredWidth: getWidth()
                                border.color: isSelected ? "black" : "white"
                                border.width: isSelected ? 1 : 0

                                function getWidth() {
                                    return Math.max(tree_view.file_name_column_max_width, file_name_column.implicitWidth) +
                                           Math.max(tree_view.inner_files_column_max_width, inner_files_column.implicitWidth) +
                                           Math.max(tree_view.file_size_column_max_width, file_size_column.implicitWidth) +
                                           data_row_layout.spacing * (data_row_layout.elements_number - 1) -
                                           levelHorizontalDifference * depth
                                }

                                function getHeight() {
                                    return Math.max(file_name_column.height, inner_files_column.height, file_size_column.height)
                                }

                                MouseArea {
                                    anchors.fill: parent

                                    onClicked: {
                                        row_selected_check_box.isExternalChange = true
                                        FileSystemController.currentlySelectedIndex = tree_view.modelIndex(row, column)
                                    }
                                }

                                RowLayout {
                                    readonly property int elements_number: 3

                                    id: data_row_layout
                                    anchors.fill: parent
                                    spacing: 10

                                    Text {
                                        id: file_name_column
                                        text: file_name
                                        Layout.minimumWidth: tree_view.file_name_column_max_width -
                                                             levelHorizontalDifference * depth

                                        onContentWidthChanged: {
                                            var horizontalIdentity = levelHorizontalDifference * depth

                                            if ((horizontalIdentity + implicitWidth) > tree_view.file_name_column_max_width) {
                                                tree_view.file_name_column_max_width = (horizontalIdentity + implicitWidth)
                                            }
                                        }
                                    }

                                    Text {
                                        id: inner_files_column
                                        text: inner_files
                                        Layout.minimumWidth: tree_view.inner_files_column_max_width

                                        onContentWidthChanged: {
                                            var horizontalIdentity = levelHorizontalDifference * depth

                                            if ((horizontalIdentity + implicitWidth) > tree_view.inner_files_column_max_width) {
                                                tree_view.inner_files_column_max_width = (horizontalIdentity + implicitWidth)
                                            }
                                        }
                                    }

                                    Text {
                                        id: file_size_column
                                        text: file_size
                                        Layout.minimumWidth: tree_view.file_size_column_max_width

                                        onContentWidthChanged: {
                                            var horizontalIdentity = levelHorizontalDifference * depth

                                            if ((horizontalIdentity + implicitWidth) > tree_view.file_size_column_max_width) {
                                                tree_view.file_size_column_max_width = (horizontalIdentity + implicitWidth)
                                            }
                                        }
                                    }
                                }
                            }
                       }
                    } // end of the "contentItem: Item"
                } // End of the "delegate: TreeViewDelegate"
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
