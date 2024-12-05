#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "squarewidget.h"
#include <array>
#include <vector>
#include "devicecontroller.h"
#include <QThread>
#include <QEventLoop>

#define CHESS_BOARD_SIZE 8

struct PiecePosition
{
    SquareWidget *piece;
    int x;
    int y;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setStartingPositionPieces();
    void defaultSetUpPiece(SquareWidget *piece, QString name);

    void allocateAllResource();
    void placePiece(SquareWidget *p);
    bool movePiece(int x, int y);
    void showAllowedMoves();

    bool checkIfWithinBoundary(int x, int y);
    bool checkIfOppositeColorPiece(int x, int y);
    bool checkIfSameColorPiece(int x, int y);
    void putComponentOnBoard(QString name, int x, int y);
    void removePiece(int x, int y);
    bool checkPosition(int x, int y);

    void defaultSetUpBoardPiece(BoardWidget *piece, QString name);
    void placeBoardPiece(BoardWidget *p);

    bool checkIfOppositeColorKing(int x, int y);

    bool checkIfEnPassant(int x, int y);

    void bringPieceToFront();

    void addSpecialPiece(BoardWidget *temp, QString name);

    // For Discover Check
    bool allowedMove(int x, int y);
    bool checkType(std::vector<Piece> types, int x, int y);
    bool discoverCheck(SquareWidget *piece);

    bool checkIfCheck();
    bool checkIfPiece(int x, int y);
    bool checkIfCheckAtPosition(int x, int y);



    void checkIfAnyMoveForPiece(SquareWidget* piece, int* count);
    bool checkAnyMovePossible();
    bool checkIfCheckMate();

    void movePossible(int* count);
    bool checkPositionMove(int x, int y, int* count);

    bool checkIfStaleMate();

    ~MainWindow();
signals:
    void promotionPieceSelected();  // Signal to indicate the action is completed

private slots:
    void onNewGameClicked();

    void onPieceClicked(SquareWidget* piece);
    void onComponentClicked(SquareWidget* piece);

    void onPromotionPieceClicked(SquareWidget* piece);

    // Networking
    void device_connected();
    void device_disconnected();
    void device_stateChanged(QAbstractSocket::SocketState);
    void device_errorOccurred(QAbstractSocket::SocketError);
    void device_dataReady(QByteArray data);

private:

    QStackedWidget *stackedWidget;

    DeviceController _controller;

    void setDeviceContoller();

    QWidget *startPage;
    QVBoxLayout *startMenuLayout;
    QPushButton *btnNewGame;

    QWidget *labelPage;
    QVBoxLayout *labelLayout;
    QLabel *label;

    QWidget* promotionPage;
    QHBoxLayout* promotionLayout;
    SquareWidget *pRook;
    SquareWidget *pKnight;
    SquareWidget *pBishop;
    SquareWidget *pQueen;
    SquareWidget *promotionPieceTypeSelected = nullptr;
    std::array<SquareWidget*,4> promotionPieces;


    QWidget* endPage;
    QVBoxLayout *endLayout;
    QLabel *endLabel;

    QWidget *centralWidget;

    // Create a grid layout and set it on the central widget
    QGridLayout *board;


    bool waitingForGame;
    bool isWhitePlayer;
    bool myTurn;

    SquareWidget *selectedPiece; // Track the currently selected piece

    std::vector<SquareWidget*> allPieces;
    std::vector<BoardWidget*> specialBoardSquares;

    std::vector<SquareWidget*> otherComponents;

    SquareWidget *bRook1;
    SquareWidget *bKnight1;
    SquareWidget *bBishop1;
    SquareWidget *bQueen;
    SquareWidget *bKing;
    SquareWidget *bBishop2;
    SquareWidget *bKnight2;
    SquareWidget *bRook2;
    SquareWidget *bPawn1;
    SquareWidget *bPawn2;
    SquareWidget *bPawn3;
    SquareWidget *bPawn4;
    SquareWidget *bPawn5;
    SquareWidget *bPawn6;
    SquareWidget *bPawn7;
    SquareWidget *bPawn8;
    SquareWidget *wPawn1;
    SquareWidget *wPawn2;
    SquareWidget *wPawn3;
    SquareWidget *wPawn4;
    SquareWidget *wPawn5;
    SquareWidget *wPawn6;
    SquareWidget *wPawn7;
    SquareWidget *wPawn8;
    SquareWidget *wRook1;
    SquareWidget *wKnight1;
    SquareWidget *wBishop1;
    SquareWidget *wQueen;
    SquareWidget *wKing;
    SquareWidget *wBishop2;
    SquareWidget *wKnight2;
    SquareWidget *wRook2;

};
#endif // MAINWINDOW_H
