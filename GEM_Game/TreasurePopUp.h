#ifndef TREASUREPOPUP_H
#define TREASUREPOPUP_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QMovie>
#include <QTimer>

class TreasurePopup : public QDialog
{
    Q_OBJECT

public:
    explicit TreasurePopup(QWidget *parent = nullptr);

private:

    QLabel *gifLabel;

    QLabel *winImage;

    QMovie *movie;
};

#endif // TREASUREPOPUP_H
