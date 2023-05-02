#ifndef EDITORPANE_H
#define EDITORPANE_H

#include <QWidget>

namespace Ui {
class EditorPane;
}

class EditorPane : public QWidget
{
    Q_OBJECT

public:
    explicit EditorPane(QWidget *parent = nullptr);
    ~EditorPane();

private:
    Ui::EditorPane *ui;
};

#endif // EDITORPANE_H
