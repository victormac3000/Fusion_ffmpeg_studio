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