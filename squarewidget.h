#ifndef SQUAREWIDGET_H
#define SQUAREWIDGET_H

#include <QtSvgWidgets/QSvgWidget>
#include <QMouseEvent>
#include <QDrag>
#include <QSvgWidget>
#include <QMimeData>
#include <QDebug>

enum Piece{
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,
    PAWN
};

enum Move {
    NOT_MOVED,
    FIRST_MOVE_PAWN_CAN_BE_KILLED,
    FIRST_MOVE,
    MOVED
};

class SquareWidget : public QSvgWidget
{
    Q_OBJECT

public:

    int x,y;
    bool isWhitePiece;
    Move move;
    Piece type;

    explicit SquareWidget(QWidget *parent = nullptr) : QSvgWidget(parent){}
    explicit SquareWidget(QWidget *parent = nullptr, int _x = 0, int _y = 0, bool _isWhitePiece = false, Piece _type = PAWN) : QSvgWidget(parent), x(_x), y(_y), isWhitePiece(_isWhitePiece), move(NOT_MOVED), type(_type) {}

    // Override to return the height based on the given width
    int heightForWidth(int width) const override {
        return width;  // Maintain a 1:1 aspect ratio
    }

    // Enable the height-for-width behavior
    bool hasHeightForWidth() const override {
        return true;
    }




signals:
    void pieceClicked(SquareWidget* piece);
protected:
    // Mouse press event to initiate the drag
    void mousePressEvent(QMouseEvent *event) override {
        // qInfo()<< "HEre";
        if (event->button() == Qt::LeftButton) {
            emit pieceClicked(this);
        }
    }
};

class BoardWidget : public QSvgWidget
{
    Q_OBJECT

public:

    int x,y;

    explicit BoardWidget(QWidget *parent = nullptr) : QSvgWidget(parent){}
    explicit BoardWidget(QWidget *parent = nullptr, int _x = 0, int _y = 0) : QSvgWidget(parent), x(_x), y(_y) {}


// signals:
//     void pieceClicked(SquareWidget* piece);
// protected:
//     // Mouse press event to initiate the drag
//     void mousePressEvent(QMouseEvent *event) override {
//         // qInfo()<< "HEre";
//         if (event->button() == Qt::LeftButton) {
//             emit pieceClicked(this);
//         }
//     }
};
#endif // SQUAREWIDGET_H
