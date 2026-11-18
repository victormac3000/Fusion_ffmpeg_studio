// MyTextField.qml
import QtQuick
import QtQuick.Controls

TextField {
    id: root

    // ── Colors ────────────────────────────────────────────────
    property color idleColor:            "#F6FBF9"
    property color hoverColor:           "#00A1E1"
    property color editingColor:         hoverColor
    property color selectedTextColorOverride: "lightblue"

    // ── Behaviour ────────────────────────────────────────────
    property bool   editable:        true
    property bool   showTooltip:     true
    property string tooltipText:     ""
    property int    minimumLength:   0

    // ── State ────────────────────────────────────────────────
    property bool editing: false
    readonly property bool overflowing:
        contentWidth > (width - leftPadding - rightPadding)

    // ── Signals ──────────────────────────────────────────────
    signal committed(string text)
    signal cancelled()
    // ── Auto-scaling ─────────────────────────────────────────
    property bool autoScale:                    false
    property real autoScaleReferencePointSize:  100
    property int  autoScaleMinimumPointSize:    13
    property int  autoScaleMaximumPointSize:    600

    // The design/natural size that the field should *never* shrink below.
    // When autoScale is off, this is simply font.pointSize. When autoScale
    // is on, we latch font.pointSize at the moment autoScale flips on, so
    // turning autoScale on doesn't overwrite itself in a loop.
    property real autoScaleFloorPointSize: 0

    onAutoScaleChanged: {
        if (autoScale && autoScaleFloorPointSize <= 0)
            autoScaleFloorPointSize = font.pointSize > 0 ? font.pointSize
                                                         : 13   // sensible default
    }

    TextMetrics {
        id: autoScaleMetrics
        font.family:            root.font.family
        font.weight:            root.font.weight
        font.styleName:         root.font.styleName
        font.capitalization:    root.font.capitalization
        font.letterSpacing:     root.font.letterSpacing
        font.wordSpacing:       root.font.wordSpacing
        font.hintingPreference: root.font.hintingPreference
        font.preferShaping:     root.font.preferShaping
        font.pointSize:         root.autoScaleReferencePointSize
        text: root.text.length > 0 ? root.text : "X"
    }

    readonly property real _autoScaleAvailableWidth:
        Math.max(1, root.width  - root.leftPadding - root.rightPadding)
    readonly property real _autoScaleAvailableHeight:
        Math.max(1, root.height - root.topPadding  - root.bottomPadding)
    readonly property real _autoScaleRefWidth:
        Math.max(1, autoScaleMetrics.advanceWidth)
    readonly property real _autoScaleRefHeight:
        Math.max(1, autoScaleMetrics.height)
    readonly property real _autoScaleFactor:
        Math.min(_autoScaleAvailableWidth  / _autoScaleRefWidth,
                 _autoScaleAvailableHeight / _autoScaleRefHeight)

    readonly property real _autoScaleRawPointSize:
        root.autoScaleReferencePointSize * _autoScaleFactor

    // The size we actually apply: grow above the design size when there is
    // room, but never fall below it. This is what makes the field scroll
    // (like with autoScale off) when the text doesn't fit instead of
    // shrinking it to fit.
    readonly property real _autoScaleFittedPointSize:
        Math.max(
            Math.max(root.autoScaleMinimumPointSize,
                     root.autoScaleFloorPointSize),
            Math.min(root.autoScaleMaximumPointSize,
                     root._autoScaleRawPointSize))

    Binding on font.pointSize {
        when: root.autoScale
        value: root._autoScaleFittedPointSize
        restoreMode: Binding.RestoreBindingOrValue
    }

    // Prevent a leftover pixelSize (e.g. -1 from a style or from a previous
    // explicit set) from shadowing the pointSize we just computed.
    Binding on font.pixelSize {
        when: root.autoScale
        value: -1
        restoreMode: Binding.RestoreBindingOrValue
    }

    // ── Appearance ───────────────────────────────────────────
    readOnly: !editing
    cursorVisible: editing
    selectByMouse: true
    verticalAlignment: TextInput.AlignVCenter
    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0
    background: null
    selectedTextColor: selectedTextColorOverride

    color: editing               ? editingColor
         : hoverHandler.hovered  ? hoverColor
                                 : idleColor

    // ── Minimum-length enforcement ───────────────────────────
    // Any change that would leave fewer than `minimumLength` chars is
    // rejected and the previous text is restored on the next tick.
    // We remember the last "valid" text so we can restore it.
    property string _lastValidText: ""
    property bool   _restoring:     false

    Component.onCompleted: _lastValidText = text

    onTextChanged: {
        if (_restoring) return
        if (text.length < minimumLength) {
            _restoring = true
            // restore on the next tick so we don't fight the control's
            // own update of cursorPosition / selection during this change
            Qt.callLater(() => {
                text        = _lastValidText
                cursorPosition = Math.min(cursorPosition, text.length)
                _restoring  = false
            })
        } else {
            _lastValidText = text
        }
    }

    // Keep _lastValidText in sync if the caller assigns a valid text
    // directly (e.g. model reload) while minimumLength is non-zero.
    onMinimumLengthChanged: {
        if (text.length >= minimumLength)
            _lastValidText = text
    }

    // ── Read-only: keep the visible window at the start ─────
    property int _cursorBeforeEdit: 0

    function _pinToStart() {
        if (!editing && cursorPosition !== 0)
            cursorPosition = 0
    }

    onContentWidthChanged: if (!editing) Qt.callLater(_pinToStart)
    onWidthChanged:        if (!editing) Qt.callLater(_pinToStart)

    // ── Edit lifecycle ───────────────────────────────────────
    property string _textOnEditStart: ""
    property bool   _wasCancelled:    false

    onEditingChanged: {
        if (editing) {
            _textOnEditStart  = text
            _wasCancelled     = false
            cursorPosition    = _cursorBeforeEdit
        } else {
            _cursorBeforeEdit = cursorPosition
            cursorPosition    = 0
            Qt.callLater(_pinToStart)
            if (_wasCancelled) {
                _wasCancelled = false
                cancelled()
            } else {
                committed(text)
            }
        }
    }

    onAccepted:          editing = false
    onActiveFocusChanged: if (!activeFocus) editing = false

    Keys.onEscapePressed: (event) => {
        if (!editing) { event.accepted = false; return }
        _wasCancelled = true
        text          = _textOnEditStart
        editing       = false
        event.accepted = true
    }

    // ── Hover ────────────────────────────────────────────────
    HoverHandler {
        id: hoverHandler
        cursorShape: root.editable ? Qt.IBeamCursor : Qt.ArrowCursor
    }

    // ── Click-to-edit ────────────────────────────────────────
    MouseArea {
        anchors.fill: parent
        enabled: !root.editing && root.editable
        cursorShape: Qt.IBeamCursor
        onClicked: (mouse) => {
            root.editing = true
            root.forceActiveFocus()
            const pos = root.positionAt(mouse.x, mouse.y)
            if (pos >= 0) root.cursorPosition = pos
        }
    }

    // ── Tooltip ──────────────────────────────────────────────
    ToolTip.visible: showTooltip && hoverHandler.hovered && overflowing && !editing
    ToolTip.text:    tooltipText !== "" ? tooltipText : text
    ToolTip.delay:   0
}