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
    //Material.theme: Material.Dark
    palette.highlight: "blue"
    palette.buttonText: "blue"

    Connections {
        target: SettingsController
        function onHistoryPathInvalid(){
            warning_dialog.open();
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
        function onSetupModelCanceled(){
            cancelation_dialog.close();
        }
    }

    MenuBar{
        id: main_window_menu_bar
            Menu{
                title:  "Options"
                font {
                    pixelSize: 10
                }
                MenuItem{
                    id: settings_menu_item
                    objectName: "settings_menu_item"
                    property variant win
                    property bool clicked: false
                    text: "Settings..."
                    font {
                        pixelSize: 10
                    }

                    signal openSettingsWindow()

                    onTriggered: {
                        settings_menu_item.openSettingsWindow();
                    }
                }
            }
        }

    GridLayout{
        anchors{
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: main_window_menu_bar.bottom
        }

        anchors.margins: 39
        rows: 4
        columns: 2
        rowSpacing: 10
        columnSpacing: 25

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
            //color: '#3fcccccc'
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

            ScrollView {
                id: frame
                clip: true
                anchors.fill: parent

                ScrollBar.vertical.policy: ScrollBar.AsNeeded
                ScrollBar.horizontal.policy: ScrollBar.AsNeeded

                Flickable {
                    id: flickable_frame
                    contentHeight: frame.height + 1
                    contentWidth: frame.width + 1
                    anchors.fill: parent

                    boundsBehavior: Flickable.StopAtBounds

                    Row {
                        id: headers

                        anchors {
                            left: parent.left
                            right: parent.right
                            top: parent.top
                            margins: 10
                        }
                        height: 24

                        Button {
                            id: file_name_header
                            implicitWidth: frame.width / 1.19
                            height: parent.height
                            Text{
                                anchors.fill: parent
                                anchors.margins: 10
                                text: "File name"
                                verticalAlignment: Text.AlignVCenter
                            }
                            enabled: false
                        }
                        Button {
                            id: items_header
                            implicitWidth: frame.width / 15
                            height: parent.height
                            Text{
                                anchors.fill: parent
                                anchors.margins: 10
                                text: "Items"
                                verticalAlignment: Text.AlignVCenter
                            }
                            enabled: false
                        }
                        Button {
                            id: size_header
                            implicitWidth: frame.width / 15
                            height: parent.height
                            Text {
                                anchors.fill: parent
                                anchors.margins: 10
                                text: "Size"
                                verticalAlignment: Text.AlignVCenter
                            }
                            enabled: false
                        }
                    }

                    TreeView {
                        id: tree_view
                        anchors {
                            left: parent.left
                            right: parent.right
                            top: headers.bottom
                            bottom: parent.bottom
                            margins: 10
                        }

                        model: FileSystemModel
                        selectionModel: model.itemSelectionModel
                        clip: true

                        delegate: TreeViewDelegate {
                            id: delegate_item
                            // z hack used to allow other column`s content be always over selection
                            // rectangle(instantiated on column 0 with Loader element)
                            z: column != 0 ? 1 : 0

                            implicitWidth: {
                                if (column === 0)
                                    file_name_header.implicitWidth
                                else if (column === 1)
                                    items_header.implicitWidth
                                else if (column === 2)
                                    size_header.implicitWidth
                            }

                            onImplicitContentWidthChanged: {
                                if (column === 0) {
                                    file_name_header.implicitWidth = Math.max(implicitWidth, content_item.implicitWidth + 21 * depth)
                                }

                                flickable_frame.contentWidth = (file_name_header.implicitWidth + items_header.implicitWidth + size_header.implicitWidth + 20) > frame.width + 1 ? (file_name_header.implicitWidth + items_header.implicitWidth + size_header.implicitWidth + 20) :  frame.width + 1
                                flickable_frame.contentHeight = (tree_view.rows * delegate_item.implicitHeight + tree_view.rowSpacing * tree_view.rows + 24 + 30) > frame.height + 1 ? (tree_view.rows * delegate_item.implicitHeight + tree_view.rowSpacing * tree_view.rows + 24 + 30) : frame.height + 1
                            }

                            property bool selected : FileSystemController.isSelectionStateChanged && tree_view.selectionModel.isSelected(tree_view.modelIndex(row, column))
                            contentItem: Text {
                                // Loader element loads its component after text is loaded so the current element is not visible
                                // That hack allows force that text item be always on selection rectangle
                                z: 1
                                id: content_item
                                anchors.leftMargin: leftMargin
                                anchors.rightMargin: rightMargin
                                text: {
                                    if (column === 0)
                                        file_name
                                    else if (column === 1)
                                        inner_files
                                    else
                                        file_size
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    propagateComposedEvents: true
                                }
                            }

                            Loader {
                                x: delegate_item.indicator.x + delegate_item.indicator.width
                                y: delegate_item.indicator.y
                                width: tree_view.contentWidth - delegate_item.indicator.width -
                                       (depth * delegate_item.indicator.width)
                                height: content_item.implicitHeight

                                active: column === 0
                                visible: column === 0

                                sourceComponent:
                                    Rectangle {
                                    id: selection_rectangle
                                    anchors.fill: parent

                                    color: selected ? "#f0f0f0" : "white"
                                    border.width: selected ? 1 : 0
                                    border.color: "black"
                                    radius: 3

                                    MouseArea {
                                        anchors.fill: parent

                                        onClicked: {
                                            FileSystemController.currentlySelectedIndex = tree_view.modelIndex(row, column)
                                            console.log(tree_view.selectionModel)
                                        }
                                    }
                                }
                            } // Loader ends
                        } // TreeViewDelegate ends
                    } // TreeView ends
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

        Button {
            id: delete_button

            Layout.row: 3
            Layout.column: 1
            width: 89

            text: "Delete"

            onClicked: {
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
            cancelation_dialog.open()
        }
    }

    Dialog {
        id: cancelation_dialog
        objectName: "cancelation_dialog"
        anchors.centerIn: parent
        closePolicy: Popup.NoAutoClose
        title: qsTr("Waiting for cancelation...")
        contentItem: ProgressBar {
                indeterminate: true
        }
        modal: true
    }
}
