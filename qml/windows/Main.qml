import QtQuick 2.15
import QtQuick.Controls 2.15

import FusionFFmpegStudio as FFS

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 800
    minimumWidth: 800
    height: 600
    minimumHeight: 600
    title: "Fusion FFmpeg Studio"

    property double resizeAnimationTime: 350

    Behavior on width {
        NumberAnimation {
            duration: resizeAnimationTime
            easing.type: Easing.InOutCubic
        }
    }

    Behavior on height {
        NumberAnimation {
            duration: resizeAnimationTime
            easing.type: Easing.InOutCubic
        }
    }

    Behavior on x {
        NumberAnimation {
            duration: resizeAnimationTime
            easing.type: Easing.InOutCubic
        }
    }

    Behavior on y {
        NumberAnimation {
            duration: resizeAnimationTime
            easing.type: Easing.InOutCubic
        }
    }

    MenuBar {
        id: menuBar
    }

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: FFS.Welcome{}

        property var menuRegistry: ({})

        Component {
            id: dynamicMenuComponent
            Menu {}
        }

        Component {
            id: dynamicMenuItemComponent
            MenuItem {}
        }

        function addMenu(title) {
            if (!title || title.trim() === "") {
                return null
            }

            if (menuRegistry[title]) {
                console.warn("Menu already exists:", title)
                return menuRegistry[title].menu
            }

            const menu = dynamicMenuComponent.createObject(menuBar, {
                "title": title
            })

            if (!menu) {
                return null
            }

            menuBar.addMenu(menu)

            menuRegistry[title] = {
                menu: menu,
                items: []
            }

            return menu
        }

        function addMenuItem(menuTitle, text, callback, options) {
            const menuEntry = menuRegistry[menuTitle]

            if (!menuEntry) {
                console.error("Menu does not exist:", menuTitle)
                return null
            }

            if (!text || text.trim() === "") {
                return null
            }

            const properties = Object.assign({}, options || {}, {
                text: text
            })

            const menuItem = dynamicMenuItemComponent.createObject(
                menuEntry.menu.contentItem,
                properties
            )

            if (!menuItem) {
                console.error("Failed to create menu item:", text)
                return null
            }

            if (callback && typeof callback === "function") {
                menuItem.triggered.connect(callback)
            }

            menuEntry.menu.addItem(menuItem)

            menuEntry.items.push({
                type: "item",
                text: text,
                item: menuItem
            })

            return menuItem
        }

        function removeMenuItem(menuTitle, text) {
            const menuEntry = menuRegistry[menuTitle]

            if (!menuEntry) {
                console.warn("Menu does not exist:", menuTitle)
                return false
            }

            for (let i = 0; i < menuEntry.items.length; ++i) {
                const entry = menuEntry.items[i]

                if (entry.type === "item" && entry.text === text) {
                    menuEntry.menu.removeItem(entry.item)
                    entry.item.destroy()

                    menuEntry.items.splice(i, 1)

                    return true
                }
            }

            console.warn("Menu item not found:", text)
            return false
        }

        function removeMenu(title) {
            const menuEntry = menuRegistry[title]

            if (!menuEntry) {
                console.warn("Menu does not exist:", title)
                return false
            }

            menuBar.removeMenu(menuEntry.menu)

            menuEntry.menu.destroy()

            delete menuRegistry[title]

            return true
        }

        function clearMenus() {
            const menuTitles = Object.keys(menuRegistry)

            for (const title of menuTitles) {
                removeMenu(title)
            }
        }

        function getMenu(title) {
            const menuEntry = menuRegistry[title]

            return menuEntry ? menuEntry.menu : null
        }

        function hasMenu(title) {
            return menuRegistry[title] !== undefined
        }

        function windowMaximize() {
            windowResize(
                mainWindow.screen.desktopAvailableWidth,
                mainWindow.screen.desktopAvailableHeight
            )
        }

        function windowFullscreen() {
            mainWindow.showFullScreen()
        }

        function windowResizePercent(widthPercent, heightPercent) {
            if (!widthPercent || widthPercent <= 0 || widthPercent > 100 ||
                !heightPercent || heightPercent <= 0 || heightPercent > 100) {
                console.warn("Could not resize main window: Invalid percent dimensions: " + widthPercent + "x" + heightPercent)
            }
            windowResize(
                mainWindow.screen.desktopAvailableWidth * (widthPercent/100),
                mainWindow.screen.desktopAvailableHeight * (heightPercent/100)
            )
        }

        function windowResize(width, height) {
            if (width <= 0 || height <= 0) {
                console.warn("Could not resize main window: Invalid dimensions: " + width + "x" + height)
                return
            }

            console.debug("Resizing main window to " + width + "x" + height)

            const screen = mainWindow.screen

            const availableWidth = screen.desktopAvailableWidth
            const availableHeight = screen.desktopAvailableHeight

            const targetWidth = Math.min(width, availableWidth)
            const targetHeight = Math.min(height, availableHeight)

            mainWindow.width = targetWidth
            mainWindow.height = targetHeight

            // Center window on the screen
            mainWindow.x = screen.virtualX + (screen.width - targetWidth) / 2
            mainWindow.y = screen.virtualY + (screen.height - targetHeight) / 2
        }

        function windowSetLimits(maxWidth, maxHeight, minWidth, minHeight) {
            if (maxHeight && maxHeight > 0 && maxWidth && maxWidth > 0) {
                mainWindow.maximumHeight = maxHeight
                mainWindow.maximumWidth = maxWidth
            }
            if (minHeight && minHeight > 0 && minWidth && minWidth > 0) {
                mainWindow.minimumHeight = minHeight
                mainWindow.minimumWidth = minWidth
            }
        }
    }

    Connections {
        target: mainController

        function onNavigateTo(pane, attributes) {
            let paneUrl = mainController.paneToUrl(pane)
            if (paneUrl === "") {
                console.error("Unknown pane: " + pane)
                return
            }
            stack.push(paneUrl, attributes)
        }

        function onAddWindow(window, modal) {
            let windowUrl = mainController.windowToUrl(window)
            if (windowUrl === "") {
                console.error("Unknown window: " + window)
                return
            }

            let component = Qt.createComponent(windowUrl)

            if (component.status !== Component.Ready) {
                console.error(
                    "Failed to create window:",
                    windowUrl,
                    "\n",
                    component.errorString()
                )
                return
            }

            let windowObject = component.createObject(null, {
                "parentWindow": mainWindow
            })

            if (!windowObject) {
                console.error("Failed to instantiate:", windowUrl)
                return
            }

            windowObject.transientParent = mainWindow
            windowObject.modality = modal
                ? Qt.WindowModal
                : Qt.NonModal

            windowObject.show()
        }

        function onBack() {
            stack.pop()
        }
    }
}
