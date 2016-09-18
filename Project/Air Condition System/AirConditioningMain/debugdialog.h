#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class debugDialog;
}

class debugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit debugDialog(QWidget *parent = 0);
    ~debugDialog();  
    QLabel * labels[6];
private:
    Ui::debugDialog *ui;

private slots:
    void updateFunction();
};

#endif // DEBUGDIALOG_H
