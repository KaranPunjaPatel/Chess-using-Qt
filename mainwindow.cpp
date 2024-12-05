#include <QGridLayout>
#include "mainwindow.h"
// #include "squarewidget.h"

#define NUMBER_OF_SQUARES 64


constexpr int di[8] = { 1, -1, 0, 0, 1, -1, 1, -1 };
constexpr int dj[8] = { 0, 0, 1, -1, 1, -1, -1, 1 };
constexpr int diK[8] = { -2, -2, -1, 1, 2, 2, 1, -1 };
constexpr int djK[8] = { -1, 1, 2, 2, 1, -1, -2, -2 };


// ************************************* //
//      Slot method which is called
//      when the new game button is
//      clicked and it tries to
//      connect to the server
// ************************************* //
void MainWindow::onNewGameClicked(){

    if (_controller.isConnected()) {
        _controller.disconnect();
    }
    auto ip = QString("127.0.0.1");
    int port = 12345;
    _controller.connectToDevice(ip, port);

}

// ************************************* //
//      Helper method to connect all
//      networking signals and slots
// ************************************* //
void MainWindow::setDeviceContoller()
{
    connect(&_controller, &DeviceController::connected, this, &MainWindow::device_connected);
    connect(&_controller, &DeviceController::disconnected, this, &MainWindow::device_disconnected);
    connect(&_controller, &DeviceController::stateChanged, this, &MainWindow::device_stateChanged);
    connect(&_controller, &DeviceController::errorOccurred, this, &MainWindow::device_errorOccurred);
    connect(&_controller, &DeviceController::dataReady, this, &MainWindow::device_dataReady);
}

// ************************************* //
//      Slot method which is called
//      when the user has connected
//      to the server
// ************************************* //
void MainWindow::device_connected()
{
    waitingForGame = true;
    // qInfo() << "Play";
    stackedWidget->setCurrentIndex(1);

}

// ************************************* //
//      Slot method which is called
//      when the user has disconnected
//      from the server
// ************************************* //
void MainWindow::device_disconnected()
{
    // TODO: Show screen saying player disconnected
    qInfo() << "Disconnected from Device";
}

// ************************************* //
//      Slot method which is called
//      when the there a change in
//      the network
// ************************************* //
void MainWindow::device_stateChanged(QAbstractSocket::SocketState state)
{
    qInfo() << "State change" << state;
}

// ************************************* //
//      Slot method which is called
//      when the there is an error
//      in the network
// ************************************* //
void MainWindow::device_errorOccurred(QAbstractSocket::SocketError error)
{
    qInfo() << "Something went wrong handle it" << error;
}

// ************************************* //
//      Slot method which is called
//      when the user has gotten a
//      packet from the server
// ************************************* //
void MainWindow::device_dataReady(QByteArray data)
{
    // TODO: The first packet we get from server will specify the color we will be playing B/W.
    // Here it means we have received the opponents move so we need to make the opponents move
    // Now we need to make a move
    // Convert data to QString and trim any leading/trailing whitespace
    QString receivedData = QString::fromUtf8(data).trimmed();

    qInfo() << receivedData;
    if(waitingForGame){
        // qInfo() << receivedData;



        // Set pieces according  to the color setup
        if(receivedData == "Black"){
            isWhitePlayer = false;
            myTurn = false;

            pRook      = new SquareWidget(promotionPage, 0, 0, false, ROOK);
            pBishop    = new SquareWidget(promotionPage, 0, 0, false, BISHOP);
            pKnight    = new SquareWidget(promotionPage, 0, 0, false, KNIGHT);
            pQueen     = new SquareWidget(promotionPage, 0, 0, false, QUEEN);

            defaultSetUpPiece(pQueen,   QString(":/res/res/piece/queen-b.svg"));
            defaultSetUpPiece(pBishop, QString(":/res/res/piece/bishop-b.svg"));
            defaultSetUpPiece(pKnight, QString(":/res/res/piece/knight-b.svg"));
            defaultSetUpPiece(pRook,   QString(":/res/res/piece/rook-b.svg"));



        }else if(receivedData == "White"){
            isWhitePlayer = true;
            myTurn = true;

            pRook      = new SquareWidget(promotionPage, 0, 0, true, ROOK);
            pBishop    = new SquareWidget(promotionPage, 0, 0, true, BISHOP);
            pKnight    = new SquareWidget(promotionPage, 0, 0, true, KNIGHT);
            pQueen     = new SquareWidget(promotionPage, 0, 0, true, QUEEN);

            defaultSetUpPiece(pQueen,   QString(":/res/res/piece/queen-w.svg"));
            defaultSetUpPiece(pBishop, QString(":/res/res/piece/bishop-w.svg"));
            defaultSetUpPiece(pKnight, QString(":/res/res/piece/knight-w.svg"));
            defaultSetUpPiece(pRook,   QString(":/res/res/piece/rook-w.svg"));

        }else{
            return;
        }
        // Call piece setup function
        waitingForGame = false;


        promotionLayout->addWidget(pBishop, 0, Qt::AlignCenter);
        promotionLayout->addWidget(pQueen, 0, Qt::AlignCenter);
        promotionLayout->addWidget(pRook, 0, Qt::AlignCenter);
        promotionLayout->addWidget(pKnight, 0, Qt::AlignCenter);

        promotionPieces[0] = pQueen;
        promotionPieces[1] = pRook;
        promotionPieces[2] = pBishop;
        promotionPieces[3] = pKnight;


        for (SquareWidget *p : promotionPieces) {
            connect(p, &SquareWidget::pieceClicked, this, &MainWindow::onPromotionPieceClicked);
        }

        allocateAllResource();

        allPieces.resize(32);

        allPieces[0]  = bRook1;
        allPieces[1]  = bKnight1;
        allPieces[2]  = bBishop1;
        allPieces[3]  = bQueen;
        allPieces[4]  = bKing;
        allPieces[5]  = bBishop2;
        allPieces[6]  = bKnight2;
        allPieces[7]  = bRook2;
        allPieces[8]  = bPawn1;
        allPieces[9]  = bPawn2;
        allPieces[10] = bPawn3;
        allPieces[11] = bPawn4;
        allPieces[12] = bPawn5;
        allPieces[13] = bPawn6;
        allPieces[14] = bPawn7;
        allPieces[15] = bPawn8;
        allPieces[16] = wPawn1;
        allPieces[17] = wPawn2;
        allPieces[18] = wPawn3;
        allPieces[19] = wPawn4;
        allPieces[20] = wPawn5;
        allPieces[21] = wPawn6;
        allPieces[22] = wPawn7;
        allPieces[23] = wPawn8;
        allPieces[24] = wRook1;
        allPieces[25] = wKnight1;
        allPieces[26] = wBishop1;
        allPieces[27] = wQueen;
        allPieces[28] = wKing;
        allPieces[29] = wBishop2;
        allPieces[30] = wKnight2;
        allPieces[31] = wRook2;


        // Connect the pieceClicked signal for all pieces
        for (SquareWidget *p : allPieces) {
            connect(p, &SquareWidget::pieceClicked, this, &MainWindow::onPieceClicked);
        }
        setStartingPositionPieces();
        stackedWidget->setCurrentIndex(2);
        // stackedWidget->setCurrentIndex(3);
    }else{
        // Here it means we have got a move packet or a deciding packet whether he quit
        // qInfo() << "Ignore " << myTurn;
        if(myTurn) return;

        QStringList pieces = receivedData.split( "," );
        // qInfo() <<"Length: "<<pieces.length();
        if(pieces.length()==2){
            if(pieces.at(1) == "Remove"){
                int index = pieces.at(0).toInt();
                removePiece(allPieces[index]->x, allPieces[index]->y);
            }
            return;
        }

        if(pieces.length() < 3) return;

        int index = pieces.at(0).toInt();
        int xcord = pieces.at(1).toInt();
        int ycord = pieces.at(2).toInt();
        xcord = 7-xcord;
        ycord = 7-ycord;

        if(pieces.length()==4 && pieces.at(3) == "Castle"){


            selectedPiece = allPieces[index];
            board->removeWidget(selectedPiece);
            movePiece(xcord,ycord);
            selectedPiece = nullptr;
            myTurn = true;
            promotionPieceTypeSelected = nullptr;
            return;
        }

        if(pieces.length()==5 && pieces.at(3) == "Replace"){

            removePiece(xcord, ycord);

            SquareWidget* temp = new SquareWidget(centralWidget, xcord, ycord, !isWhitePlayer);
            QString path = ":/res/res/piece/";
            switch(pieces.at(4).toInt()){
            case 1:
                temp->type = QUEEN;
                path+="queen";
                break;
            case 2:
                temp->type = ROOK;
                path+="rook";
                break;
            case 3:
                temp->type = BISHOP;
                path+="bishop";
                break;
            case 4:
                temp->type = KNIGHT;
                path+="knight";
                break;
            default:
                break;
            }

            if(temp->isWhitePiece){
                path+= "-w.svg";
            }else{
                path+= "-b.svg";
            }

            defaultSetUpPiece(temp, path);
            placePiece(temp);

            connect(temp, &SquareWidget::pieceClicked, this, &MainWindow::onPieceClicked);
            selectedPiece = temp;
            allPieces.push_back(temp);

            checkIfCheck();
            selectedPiece = nullptr;
            myTurn = true;
            promotionPieceTypeSelected = nullptr;
            return;
        }


        selectedPiece = allPieces[index];
        if(checkIfPiece(xcord,ycord)){
            if(checkIfSameColorPiece(xcord,ycord)){
                qInfo() << "Error move";
                return;
            }
            removePiece(xcord,ycord);
        }

        for(auto& p: specialBoardSquares){
            board->removeWidget(p);
            delete p;
        }
        specialBoardSquares.clear();

        movePiece(xcord,ycord);

        if(checkIfStaleMate()){
            endLabel->setText(QString("You Draw"));
            stackedWidget->setCurrentIndex(43);
        }else if(checkIfCheckMate()){
            endLabel->setText(QString("You Lose"));
            stackedWidget->setCurrentIndex(4);
        }

        selectedPiece = nullptr;
        myTurn = true;
        promotionPieceTypeSelected = nullptr;
    }

}

// ************************************* //
//      Main contructor for initial setup
// ************************************* //
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // ************************************* //
    //      Main Window
    // ************************************* //
    setWindowTitle("Chess");
    // resize(800, 800);
    setFixedSize(900,800);
    stackedWidget = new QStackedWidget();

    // ************************************* //
    //      Start Page
    // ************************************* //
    startPage = new QWidget;
    startMenuLayout = new QVBoxLayout(startPage);
    btnNewGame = new QPushButton("Play A Game");
    btnNewGame->setStyleSheet("background-color: white;border: 3px solid black;border-radius: 15px;color:black;padding: 20px;font-size: 28px ");
    startMenuLayout->addWidget(btnNewGame, 0, Qt::AlignCenter);

    // Connect the button's signal to switch to the label page
    connect(btnNewGame, &QPushButton::clicked, this, &MainWindow::onNewGameClicked);

    // ************************************* //
    //      Finding Opponent Page
    // ************************************* //
    labelPage = new QWidget;
    labelLayout = new QVBoxLayout(labelPage);
    label = new QLabel("Finding Opponent");
    label->setFont(QFont("Times",28));
    label->setAlignment(Qt::AlignCenter);
    labelLayout->addWidget(label, 0, Qt::AlignCenter);

    // ************************************* //
    //      Finding Opponent Page
    // ************************************* //
    centralWidget = new QWidget(this);

    // Setting up all the networking parts for the game with signals and slots
    setDeviceContoller();

    // Create a grid layout and set it on the central widget
    board = new QGridLayout(centralWidget);

    // Set margins and spacing for the layout
    board->setSpacing(0);
    board->setAlignment(Qt::AlignCenter);
    board->setContentsMargins(50, 0, 50, 0);

    // Add white and black board squares to the grid layout dynamically
    for(int i = 0, j = -1; i < NUMBER_OF_SQUARES; i++) {
        if(i % 8 == 0) j++;

        // Create new instances of SquareWidget for each square
        QSvgWidget *square = new QSvgWidget(centralWidget);
        if ((i + j) % 2 == 0) {
            square->load(QString(":/res/res/board/whitesquare.svg"));
        } else {
            square->load(QString(":/res/res/board/blacksquare.svg"));
        }

        // Set size policy for each square
        square->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // Add the square to the grid layout
        board->addWidget(square, j, i % 8);
    }

    // Set stretch factors for the rows and columns to ensure equal size
    for (int i = 0; i < 8; ++i) {
        board->setRowStretch(i, 1);
        board->setColumnStretch(i, 1);
    }

    // ************************************* //
    //      Promotion Piece Selection Page
    // ************************************* //
    promotionPage = new QWidget;
    promotionLayout = new QHBoxLayout(promotionPage);

    // ************************************* //
    //       Ending Declaration Page
    // ************************************* //
    endPage = new QWidget;
    endLayout = new QVBoxLayout(endPage);
    endLabel = new QLabel("ENDING");
    endLabel->setAlignment(Qt::AlignCenter);
    endLabel->setFont(QFont("Times",28));
    endLayout->addWidget(endLabel, 0, Qt::AlignCenter);


    // ************************************* //
    //       All Pages in window
    // ************************************* //
    setCentralWidget(stackedWidget);
    // Add pages to the stacked widget
    stackedWidget->addWidget(startPage);        // Index 0
    stackedWidget->addWidget(labelPage);        // Index 1
    stackedWidget->addWidget(centralWidget);    // Index 2
    stackedWidget->addWidget(promotionPage);    // Index 3
    stackedWidget->addWidget(endPage);          // Index 4

}

// ************************************* //
//      Checking whether the index
//      are not out of chess grid
// ************************************* //
bool MainWindow::checkIfWithinBoundary(int x, int y){
    if(x<0 || y<0 || x>7 || y>7) return false;
    return true;
    // return (!(x<0 || y<0 || x>7 || y>7));
}

// ************************************* //
//      Checking whether the index
//      has a piece of opposite color
//      to the selected piece
// ************************************* //
bool MainWindow::checkIfOppositeColorPiece(int x, int y){
    if(selectedPiece->isWhitePiece){
        for(size_t i = 0; i< allPieces.size(); i++){
            if(allPieces[i] != nullptr && !allPieces[i]->isWhitePiece && allPieces[i]->x == x && allPieces[i]->y == y) return true;
        }
    }else{
        for(size_t i = 0; i< allPieces.size(); i++){
            if(allPieces[i] != nullptr && allPieces[i]->isWhitePiece && allPieces[i]->x == x && allPieces[i]->y == y) return true;
        }
    }
    return false;
}

// ************************************* //
//      Checking whether the index
//      has a piece of same color
//      to the selected piece
// ************************************* //
bool MainWindow::checkIfSameColorPiece(int x, int y){
    if(selectedPiece->isWhitePiece){
        for(size_t i = 0; i< allPieces.size(); i++){
            if(allPieces[i] != nullptr && allPieces[i]->isWhitePiece && allPieces[i]->x == x && allPieces[i]->y == y) return true;
        }
    }else{
        for(size_t i = 0; i< allPieces.size(); i++){
            if(allPieces[i] != nullptr && !allPieces[i]->isWhitePiece && allPieces[i]->x == x && allPieces[i]->y == y) return true;
        }
    }
    return false;
}

// ************************************* //
//      Helper method to create
//      possible positions for pieces
//      and store them in an array
// ************************************* //
void MainWindow::putComponentOnBoard(QString name, int x, int y){
    SquareWidget *temp = new SquareWidget(centralWidget,x,y);
    defaultSetUpPiece(temp,name);
    placePiece(temp);
    connect(temp, &SquareWidget::pieceClicked, this, &MainWindow::onComponentClicked);
    otherComponents.push_back(temp);
}

// ************************************* //
//      Checking whether the index
//      has king of opposite color
//      to the selected piece
// ************************************* //
bool MainWindow::checkIfOppositeColorKing(int x, int y){
    if(selectedPiece->isWhitePiece){
        return (allPieces[4]->x == x && allPieces[4]->y == y);
    }else{
        return (allPieces[28]->x == x && allPieces[28]->y == y);
    }
}

// ************************************* //
//      Helper method to check
//      whether the index has a piece
//      same as the piece type in
//      array "types"
// ************************************* //
bool MainWindow::checkType(std::vector<Piece> types, int x, int y){
    if(selectedPiece->isWhitePiece){
        for(size_t i = 0; i < allPieces.size(); i++){
            if(allPieces[i] != nullptr && !allPieces[i]->isWhitePiece && allPieces[i]->type != PAWN && allPieces[i]->x == x && allPieces[i]->y == y){
                for(Piece p : types){
                    if(p == allPieces[i]->type){
                        return true;
                    }
                }
                return false;
            }
        }
    }else{
        for(size_t i = 0; i < allPieces.size(); i++){
            if(allPieces[i] != nullptr && allPieces[i]->isWhitePiece && allPieces[i]->type != PAWN && allPieces[i]->x == x && allPieces[i]->y == y){
                for(Piece p : types){
                    if(p == allPieces[i]->type){
                        return true;
                    }
                }
                return false;
            }
        }
    }
    return false;
}

// ************************************* //
//      Helper method to check
//      the piece which is asserted
//      to be a KING whether a check
//      has been made
// ************************************* //
bool MainWindow::discoverCheck(SquareWidget *piece){
    assert(piece->type == KING);

    bool res = false;
    // Upwards
    for(int i = piece->x-1; i>=0 ; i--){
        // if(i == x && piece->y == y)continue;
        if(!checkIfWithinBoundary(i, piece->y) || checkIfSameColorPiece(i, piece->y)) break;
        // qInfo()<< "hehe";
        if(checkIfOppositeColorPiece(i, piece->y)){
            if(checkType({ROOK,QUEEN},i, piece->y)) res = true;
            break;
        }
    }

    // Downwards
    for(int i = piece->x+1; i<8 ; i++){
        if(!checkIfWithinBoundary(i, piece->y) || checkIfSameColorPiece(i, piece->y)) break;
        // qInfo()<< "hehe2";
        if(checkIfOppositeColorPiece(i, piece->y)){
            // qInfo()<< "hehe3";
            if(checkType({ROOK,QUEEN},i, piece->y)) res = true;
            break;
        }
    }

    // Left side
    for(int i = piece->y-1; i>=0 ; i--){
        if(!checkIfWithinBoundary(piece->x, i) || checkIfSameColorPiece(piece->x, i)) break;
        if(checkIfOppositeColorPiece(piece->x, i)){
            if(checkType({ROOK,QUEEN},piece->x, i)) res = true;
            break;
        }
    }

    // Right side
    for(int i = piece->y+1; i<8 ; i++){
        if(!checkIfWithinBoundary(piece->x, i) || checkIfSameColorPiece(piece->x, i)) break;
        if(checkIfOppositeColorPiece(piece->x, i)){
            if(checkType({ROOK,QUEEN},piece->x, i)) res = true;
            break;
        }
    }

    // Top left side
    for(int i = piece->x-1, j = piece->y-1; i>=0 && j>=0 ; i--, j--){
        if(!checkIfWithinBoundary(i,j) || checkIfSameColorPiece(i,j)) break;
        if(checkIfOppositeColorPiece(i,j)){
            if(checkType({BISHOP,QUEEN},i,j)) res = true;
            break;
        }
    }

    // Top right side
    for(int i = piece->x-1, j = piece->y+1; i>=0 && j<8 ; i--, j++){
        if(!checkIfWithinBoundary(i,j) || checkIfSameColorPiece(i,j)) break;
        if(checkIfOppositeColorPiece(i,j)){
            if(checkType({BISHOP,QUEEN},i,j)) res = true;

            break;
        }
    }

    // Bottom left side
    for(int i = piece->x+1, j = piece->y-1; i<8 && j>=0 ; i++, j--){
        if(!checkIfWithinBoundary(i,j) || checkIfSameColorPiece(i,j)) break;
        if(checkIfOppositeColorPiece(i,j)){
            if(checkType({BISHOP,QUEEN},i,j)) res = true;
            break;
        }
    }

    // Bottom right side
    for(int i = piece->x+1, j = piece->y+1; i<8 && j<8 ; i++, j++){
        if(!checkIfWithinBoundary(i,j) || checkIfSameColorPiece(i,j)) break;
        if(checkIfOppositeColorPiece(i,j)){
            if(checkType({BISHOP,QUEEN},i,j)) res = true;
            break;
        }
    }

    if(isWhitePlayer){
        if(selectedPiece->isWhitePiece){
            for(int i = 8; i < 16; i++){
                if(allPieces[i]!=nullptr){
                    if(allPieces[i]->x+1 == piece->x && allPieces[i]->y+1 == piece->y){
                        res = true;
                    }else if(allPieces[i]->x+1 == piece->x && allPieces[i]->y-1 == piece->y){
                        res = true;
                    }
                }
            }
        }else{
            for(int i = 16; i < 24; i++){
                if(allPieces[i]!=nullptr){
                    if(allPieces[i]->x-1 == piece->x && allPieces[i]->y+1 == piece->y){
                        res = true;
                    }else if(allPieces[i]->x-1 == piece->x && allPieces[i]->y-1 == piece->y){
                        res = true;
                    }
                }
            }
        }
    }else{
        if(selectedPiece->isWhitePiece){
            for(int i = 8; i < 16; i++){
                if(allPieces[i]!=nullptr){
                    if(allPieces[i]->x-1 == piece->x && allPieces[i]->y+1 == piece->y){
                        res = true;
                    }else if(allPieces[i]->x-1 == piece->x && allPieces[i]->y-1 == piece->y){
                        res = true;
                    }
                }
            }
        }else{
            for(int i = 16; i < 24; i++){
                if(allPieces[i]!=nullptr){
                    if(allPieces[i]->x+1 == piece->x && allPieces[i]->y+1 == piece->y){
                        res = true;
                    }else if(allPieces[i]->x+1 == piece->x && allPieces[i]->y-1 == piece->y){
                        res = true;
                    }
                }
            }
        }
    }

    return res;
}

// ************************************* //
//      Helper method to check whether
//      if the selected piece is moved
//      to the index specified then a
//      discover check is not made
//      meaning the movement
//      of piece is allowed
// ************************************* //
bool MainWindow::allowedMove(int x, int y){
    selectedPiece->x = x;
    selectedPiece->y = y;
    if(selectedPiece->isWhitePiece){
        return !discoverCheck(allPieces[28]);
    }else{
        return !discoverCheck(allPieces[4]);
    }
}

// ************************************* //
//      Checking whether the index
//      has a pawn piece moved once
//      so that En Passant can be
//      performed on that piece
// ************************************* //
bool MainWindow::checkIfEnPassant(int x, int y){

    if(selectedPiece->isWhitePiece){
        for(int i = 8; i< 16; i++){
            if(allPieces[i] != nullptr && allPieces[i]->move == FIRST_MOVE_PAWN_CAN_BE_KILLED && allPieces[i]->x == x && allPieces[i]->y == y) return true;
        }
    }else{
        for(int i = 16; i< 24; i++){
            if(allPieces[i] != nullptr && allPieces[i]->move == FIRST_MOVE_PAWN_CAN_BE_KILLED && allPieces[i]->x == x && allPieces[i]->y == y) return true;
        }
    }
    return false;
}

// ************************************* //
//      Checking whether the index
//      is a position where the selected
//      piece can be moved to by performing
//      all the checks necessary
// ************************************* //
bool MainWindow::checkPosition(int x, int y){
    if(checkIfWithinBoundary(x,y)){
        if(checkIfOppositeColorPiece(x,y)){
            if(allowedMove(x,y)){
                putComponentOnBoard(QString(":/res/res/piecePresentSpot.svg"), x,y);
                return false;
            }
        }else if(!checkIfSameColorPiece(x,y)){
            if(allowedMove(x,y)){
                putComponentOnBoard(QString(":/res/res/spot.svg"), x,y);
                return true;
            }
        }
    }
    return false;
}

// ************************************* //
//      Helper method which calls
//      all checks necessary and places
//      viable spots on the board
//      where the selected piece
//      can be moved
// ************************************* //
void MainWindow::showAllowedMoves(){
    for(auto& p: otherComponents){
        board->removeWidget(p);
        delete p;
    }
    otherComponents.clear();

    const int tempX = selectedPiece->x;
    const int tempY = selectedPiece->y;

    switch(selectedPiece->type){
    case PAWN: {

        if(checkIfWithinBoundary(tempX-1,tempY+1)){
            if(checkIfOppositeColorPiece(tempX-1,tempY+1)){

                if(allowedMove(tempX-1,tempY+1)){
                    putComponentOnBoard(QString(":/res/res/piecePresentSpot.svg"), tempX-1,tempY+1);
                }
            }
        }

        if(checkIfWithinBoundary(tempX-1,tempY-1)){
            if(checkIfOppositeColorPiece(tempX-1,tempY-1)){

                if(allowedMove(tempX-1,tempY-1))
                    putComponentOnBoard(QString(":/res/res/piecePresentSpot.svg"), tempX-1,tempY-1);
            }
        }

        if(checkIfWithinBoundary(tempX-1,tempY)){
            if(!checkIfOppositeColorPiece(tempX-1,tempY) && !checkIfSameColorPiece(tempX-1,tempY)){
                if(allowedMove(tempX-1,tempY))
                    putComponentOnBoard(QString(":/res/res/spot.svg"), tempX-1,tempY);
            }
        }

        selectedPiece->x = tempX;
        selectedPiece->y = tempY;
        if(selectedPiece->move == NOT_MOVED){
            if(checkIfWithinBoundary(tempX-2,tempY)){
                if((!checkIfPiece(tempX-1,tempY)) && (!checkIfPiece(tempX-2,tempY))){
                // if(!checkIfPiece(tempX-2,tempY)){
                    if(allowedMove(tempX-2,tempY))
                        putComponentOnBoard(QString(":/res/res/spot.svg"), tempX-2,tempY);
                }
            }
        }

        if(checkIfWithinBoundary(tempX,tempY-1)){
            if(checkIfEnPassant(tempX,tempY-1)){
                if(allowedMove(tempX-1,tempY-1))
                    putComponentOnBoard(QString(":/res/res/piecePresentSpot.svg"), tempX-1,tempY-1);
            }
        }

        if(checkIfWithinBoundary(tempX,tempY+1)){
            if(checkIfEnPassant(tempX,tempY+1)){
                if(allowedMove(tempX-1,tempY+1))
                    putComponentOnBoard(QString(":/res/res/piecePresentSpot.svg"), tempX-1,tempY+1);
            }
        }
        break;
    }
    case ROOK:{
        // qInfo() << "Here";

        // Upwards
        for(int i = tempX-1; i>=0 ; i--){
            if(!checkPosition(i,tempY))break;
        }

        // Downwards
        for(int i = tempX+1; i<8 ; i++){
            if(!checkPosition(i,tempY))break;
        }

        // Left side
        for(int i = tempY-1; i>=0 ; i--){
            if(!checkPosition(tempX,i))break;
        }

        // Right side
        for(int i = tempY+1; i<8 ; i++){
            if(!checkPosition(tempX,i))break;
        }

        break;
    }
    case KNIGHT:{

        for(int i = 0; i <8; i++){
            checkPosition(tempX+diK[i], tempY+djK[i]);
        }

        break;
    }
    case BISHOP:{
        // Top left side
        for(int i = tempX-1, j = tempY-1; i>=0 && j>=0 ; i--, j--){
            if(!checkPosition(i,j))break;
        }

        // Top right side
        for(int i = tempX-1, j = tempY+1; i>=0 && j<8 ; i--, j++){
            if(!checkPosition(i,j))break;
        }

        // Bottom left side
        for(int i = tempX+1, j = tempY-1; i<8 && j>=0 ; i++, j--){
            if(!checkPosition(i,j))break;
        }

        // Bottom right side
        for(int i = tempX+1, j = tempY+1; i<8 && j<8 ; i++, j++){
            if(!checkPosition(i,j))break;
        }

        break;
    }
    case QUEEN:{
        // Upwards
        for(int i = tempX-1; i>=0 ; i--){
            if(!checkPosition(i,tempY))break;
        }

        // Downwards
        for(int i = tempX+1; i<8 ; i++){
            if(!checkPosition(i,tempY))break;
        }

        // Left side
        for(int i = tempY-1; i>=0 ; i--){
            if(!checkPosition(tempX,i))break;
        }

        // Right side
        for(int i = tempY+1; i<8 ; i++){
            if(!checkPosition(tempX,i))break;
        }

        // Top left side
        for(int i = tempX-1, j = tempY-1; i>=0 && j>=0 ; i--, j--){
            if(!checkPosition(i,j))break;
        }

        // Top right side
        for(int i = tempX-1, j = tempY+1; i>=0 && j<8 ; i--, j++){
            if(!checkPosition(i,j))break;
        }

        // Bottom left side
        for(int i = tempX+1, j = tempY-1; i<8 && j>=0 ; i++, j--){
            if(!checkPosition(i,j))break;
        }

        // Bottom right side
        for(int i = tempX+1, j = tempY+1; i<8 && j<8 ; i++, j++){
            if(!checkPosition(i,j))break;
        }

        break;
    }
    case KING:{

        for(int i = 0; i<8 ; i++){
            checkPosition(tempX+di[i],tempY+dj[i]);
        }

        if(selectedPiece->move == NOT_MOVED){
            if(isWhitePlayer){
                if(allPieces[24]->move == NOT_MOVED){
                    if(!checkIfPiece(7,1) && allowedMove(7,1) && !checkIfPiece(7,2) && allowedMove(7,2) && !checkIfPiece(7,3) && allowedMove(7,3)){
                        putComponentOnBoard(QString(":/res/res/spot.svg"), 7,2);
                    }
                }else if(allPieces[31]->move == NOT_MOVED){
                    if(!checkIfPiece(7,5) && allowedMove(7,5) && !checkIfPiece(7,6) && allowedMove(7,6)){
                        putComponentOnBoard(QString(":/res/res/spot.svg"), 7,6);
                    }
                }
            }else{
                if(allPieces[0]->move == NOT_MOVED){
                    if(!checkIfPiece(7,1) && allowedMove(7,1) && !checkIfPiece(7,2) && allowedMove(7,2)){
                        putComponentOnBoard(QString(":/res/res/spot.svg"), 7,1);
                    }
                }else if(allPieces[7]->move == NOT_MOVED){
                    if(!checkIfPiece(7,4) && allowedMove(7,4) && !checkIfPiece(7,5) && allowedMove(7,5) && !checkIfPiece(7,6) && allowedMove(7,6)){
                        putComponentOnBoard(QString(":/res/res/spot.svg"), 7,5);
                    }
            }
        }
        }

        break;
    }
    }

    selectedPiece->x = tempX;
    selectedPiece->y = tempY;
}

// ************************************* //
//      Slot method which is used to
//      select and deselect the
//      selected piece and show
//      allowed moves
// ************************************* //
void MainWindow::onPieceClicked(SquareWidget* piece) {

    // Can't select opposite player piece
    if(piece->isWhitePiece != isWhitePlayer || !myTurn) return;

    if (selectedPiece == piece) {
        // Deselect if the same piece is clicked again
        selectedPiece = nullptr;
        qInfo() << "Piece deselected" << piece->isWhitePiece;
        for(auto& p: otherComponents){
            board->removeWidget(p);
            delete p;
        }
        otherComponents.clear();

    } else {
        // Select the new piece
        selectedPiece = piece;
        qInfo() << "Piece selected" << piece->isWhitePiece;
        showAllowedMoves();
    }
}

void MainWindow::onPromotionPieceClicked(SquareWidget* piece){
    promotionPieceTypeSelected = piece;
    stackedWidget->setCurrentIndex(2);
    emit promotionPieceSelected();
}

// ************************************* //
//      Checking whether the
//      index has a piece
// ************************************* //
bool MainWindow::checkIfPiece(int x, int y){
    for(size_t i = 0; i< allPieces.size(); i++){
        if(allPieces[i] != nullptr && allPieces[i]->x == x && allPieces[i]->y == y) return true;
    }
    return false;
}

// ************************************* //
//      Checking whether the index
//      has the opposite color king
// ************************************* //
bool MainWindow::checkIfCheckAtPosition(int x, int y){
    if(checkIfOppositeColorKing(x,y)){
        return true;
    }
    return false;
}

// ************************************* //
//      Helper method for default
//      setup of the widget
// ************************************* //
void MainWindow::defaultSetUpBoardPiece(BoardWidget *piece, QString name){
    piece->load(name);
    piece->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    // piece->lower();
    // piece->stackUnder(selectedPiece);
}

// ************************************* //
//      Helper method to place
//      the given piece in the board
// ************************************* //
void MainWindow::placeBoardPiece(BoardWidget *p){
    board->addWidget(p, p->x, p->y);
}

// ************************************* //
//      Helper method to bring all
//      pieces on the board to the
//      front as the when the special
//      board pieces are placed they
//      default to the top so pieces
//      get hidden behind
// ************************************* //
void MainWindow::bringPieceToFront(){
    for(size_t i=0; i < allPieces.size(); i++){
        if(allPieces[i] != nullptr){
            allPieces[i]->raise();
        }
    }
}

// ************************************* //
//      Helper method to place
//      the special blue and gold
//      board piece in the board
// ************************************* //
void MainWindow::addSpecialPiece(BoardWidget *temp, QString name){
    defaultSetUpBoardPiece(temp, name);
    placeBoardPiece(temp);
    specialBoardSquares.push_back(temp);
}

// ************************************* //
//      Checking whether the a check
//      is made and so place the
//      gold and blue board pieces
//      along the path of check
// ************************************* //
bool MainWindow::checkIfCheck(){
    const int tempX = selectedPiece->x;
    const int tempY = selectedPiece->y;

    switch(selectedPiece->type){
    case PAWN: {
        if(myTurn){
            if(checkIfCheckAtPosition(tempX-1,tempY+1)){
                BoardWidget *temp = new BoardWidget(centralWidget, tempX-1,tempY+1);
                addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));

                BoardWidget *temp2 = new BoardWidget(centralWidget, tempX,tempY);
                addSpecialPiece(temp2, QString(":/res/res/board/bluesquare.svg"));

                bringPieceToFront();
                return true;
            }
                // putComponentOnBoard(QString(":/res/res/piecePresentSpot.svg"), tempX-1,tempY+1);


            if(checkIfCheckAtPosition(tempX-1,tempY-1)){
                BoardWidget *temp = new BoardWidget(centralWidget, tempX-1,tempY-1);
                addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));

                BoardWidget *temp2 = new BoardWidget(centralWidget, tempX,tempY);
                addSpecialPiece(temp2, QString(":/res/res/board/bluesquare.svg"));

                bringPieceToFront();
                return true;
            }
        }else{
            if(checkIfCheckAtPosition(tempX+1,tempY+1)){
                BoardWidget *temp = new BoardWidget(centralWidget, tempX+1,tempY+1);
                addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));

                BoardWidget *temp2 = new BoardWidget(centralWidget, tempX,tempY);
                addSpecialPiece(temp2, QString(":/res/res/board/bluesquare.svg"));

                bringPieceToFront();
                return true;
            }
            // putComponentOnBoard(QString(":/res/res/piecePresentSpot.svg"), tempX-1,tempY+1);


            if(checkIfCheckAtPosition(tempX+1,tempY-1)){
                BoardWidget *temp = new BoardWidget(centralWidget, tempX+1,tempY-1);
                addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));

                BoardWidget *temp2 = new BoardWidget(centralWidget, tempX,tempY);
                addSpecialPiece(temp2, QString(":/res/res/board/bluesquare.svg"));

                bringPieceToFront();
                return true;
            }
        }
        // qInfo() << "Check";
            // putComponentOnBoard(QString(":/res/res/piecePresentSpot.svg"), tempX-1,tempY-1);

        break;
    }
    case ROOK:{
        // Upwards
        for(int i = tempX-1; i>=0 ; i--){
            if(checkIfPiece(i,tempY)){
                if(checkIfCheckAtPosition(i,tempY)){
                    BoardWidget *temp = new BoardWidget(centralWidget, i,tempY);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i++;
                    while(i != tempX){
                        BoardWidget *temp = new BoardWidget(centralWidget, i,tempY);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i++;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }

        // Downwards
        for(int i = tempX+1; i<8 ; i++){
            if(checkIfPiece(i,tempY)){
                if(checkIfCheckAtPosition(i,tempY)){
                    BoardWidget *temp = new BoardWidget(centralWidget, i,tempY);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i--;
                    while(i != tempX){
                        BoardWidget *temp = new BoardWidget(centralWidget, i,tempY);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i--;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }

        // Left side
        for(int i = tempY-1; i>=0 ; i--){
            if(checkIfPiece(tempX,i)){
                if(checkIfCheckAtPosition(tempX,i)){
                    BoardWidget *temp = new BoardWidget(centralWidget, tempX,i);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i++;
                    while(i != tempY){
                        BoardWidget *temp = new BoardWidget(centralWidget, tempX,i);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i++;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }

        // Right side
        for(int i = tempY+1; i<8 ; i++){
            if(checkIfPiece(tempX,i)){
                if(checkIfCheckAtPosition(tempX,i)){
                    BoardWidget *temp = new BoardWidget(centralWidget, tempX,i);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i--;
                    while(i != tempY){
                        BoardWidget *temp = new BoardWidget(centralWidget, tempX,i);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i--;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }

        break;
    }
    case KNIGHT:{
        for(int i = 0; i <8; i++){
            if(checkIfCheckAtPosition(tempX+diK[i], tempY+djK[i])){
                BoardWidget *temp = new BoardWidget(centralWidget, tempX+diK[i], tempY+djK[i]);
                addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                BoardWidget *temp2 = new BoardWidget(centralWidget, tempX,tempY);
                addSpecialPiece(temp2, QString(":/res/res/board/bluesquare.svg"));
                bringPieceToFront();
                return true;
            }
        }

        break;
    }
    case BISHOP:{
        // Top left side
        for(int i = tempX-1, j = tempY-1; i>=0 && j>=0 ; i--, j--){
            if(checkIfPiece(i,j)){
                if(checkIfCheckAtPosition(i,j)){
                    BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i++;
                    j++;
                    while(i != tempX && j != tempY){
                        BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i++;
                        j++;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }

        // Top right side
        for(int i = tempX-1, j = tempY+1; i>=0 && j<8 ; i--, j++){
            if(checkIfPiece(i,j)){
                if(checkIfCheckAtPosition(i,j)){
                    BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i++;
                    j--;
                    while(i != tempX &&  j != tempY){
                        BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i++;
                        j--;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }

        // Bottom left side
        for(int i = tempX+1, j = tempY-1; i<8 && j>=0 ; i++, j--){
            if(checkIfPiece(i,j)){
                if(checkIfCheckAtPosition(i,j)){
                    BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i--;
                    j++;
                    while(i != tempX &&  j != tempY){
                        BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i--;
                        j++;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }

        // Bottom right side
        for(int i = tempX+1, j = tempY+1; i<8 && j<8 ; i++, j++){
            if(checkIfPiece(i,j)){
                if(checkIfCheckAtPosition(i,j)){
                    BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i--;
                    j--;
                    while(i != tempX &&  j != tempY){
                        BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i--;
                        j--;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }

        break;
    }
    case QUEEN:{
        // Upwards
        for(int i = tempX-1; i>=0 ; i--){
            if(checkIfPiece(i,tempY)){
                if(checkIfCheckAtPosition(i,tempY)){
                    BoardWidget *temp = new BoardWidget(centralWidget, i,tempY);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i++;
                    while(i != tempX){
                        BoardWidget *temp = new BoardWidget(centralWidget, i,tempY);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i++;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }
        // Downwards
        for(int i = tempX+1; i<8 ; i++){
            if(checkIfPiece(i,tempY)){
                if(checkIfCheckAtPosition(i,tempY)){
                    BoardWidget *temp = new BoardWidget(centralWidget, i,tempY);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i--;
                    while(i != tempX){
                        BoardWidget *temp = new BoardWidget(centralWidget, i,tempY);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i--;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }
        // Left side
        for(int i = tempY-1; i>=0 ; i--){
            if(checkIfPiece(tempX,i)){
                if(checkIfCheckAtPosition(tempX,i)){
                    BoardWidget *temp = new BoardWidget(centralWidget, tempX,i);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i++;
                    while(i != tempY){
                        BoardWidget *temp = new BoardWidget(centralWidget, tempX,i);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i++;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }
        // Right side
        for(int i = tempY+1; i<8 ; i++){
            if(checkIfPiece(tempX,i)){
                if(checkIfCheckAtPosition(tempX,i)){
                    BoardWidget *temp = new BoardWidget(centralWidget, tempX,i);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i--;
                    while(i != tempY){
                        BoardWidget *temp = new BoardWidget(centralWidget, tempX,i);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i--;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }
        // Top left side
        for(int i = tempX-1, j = tempY-1; i>=0 && j>=0 ; i--, j--){
            if(checkIfPiece(i,j)){
                if(checkIfCheckAtPosition(i,j)){
                    BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i++;
                    j++;
                    while(i != tempX &&  j != tempY){
                        BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i++;
                        j++;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }

        // Top right side
        for(int i = tempX-1, j = tempY+1; i>=0 && j<8 ; i--, j++){
            if(checkIfPiece(i,j)){
                if(checkIfCheckAtPosition(i,j)){
                    BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i++;
                    j--;
                    while(i != tempX &&  j != tempY){
                        BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i++;
                        j--;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }

        // Bottom left side
        for(int i = tempX+1, j = tempY-1; i<8 && j>=0 ; i++, j--){
            if(checkIfPiece(i,j)){
                if(checkIfCheckAtPosition(i,j)){
                    BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i--;
                    j++;
                    while(i != tempX &&  j != tempY){
                        BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i--;
                        j++;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }

        // Bottom right side
        for(int i = tempX+1, j = tempY+1; i<8 && j<8 ; i++, j++){
            if(checkIfPiece(i,j)){
                if(checkIfCheckAtPosition(i,j)){
                    BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                    addSpecialPiece(temp, QString(":/res/res/board/goldensquare.svg"));
                    i--;
                    j--;
                    while(i != tempX &&  j != tempY){
                        BoardWidget *temp = new BoardWidget(centralWidget, i,j);
                        addSpecialPiece(temp, QString(":/res/res/board/bluesquare.svg"));
                        i--;
                        j--;
                    }
                    bringPieceToFront();
                    return true;
                }
                break;
            }
        }

        break;
    }
    case KING:
    default:
        bringPieceToFront();
        break;
    }
    return false;
}

// ************************************* //
//      Helper method to move
//      the selected piece from its
//      position in the board to
//      the index provided and changing
//      the number of move enum
// ************************************* //
bool MainWindow::movePiece(int x, int y){
    selectedPiece->x = x;
    selectedPiece->y = y;
    placePiece(selectedPiece);

    for(int i=8; i<24; i++){
        if(allPieces[i]!= nullptr && allPieces[i]->move == FIRST_MOVE_PAWN_CAN_BE_KILLED) allPieces[i]->move = FIRST_MOVE;
    }

    if(selectedPiece->move == NOT_MOVED && selectedPiece->type == PAWN) selectedPiece->move = FIRST_MOVE_PAWN_CAN_BE_KILLED;
    else if(selectedPiece->move == NOT_MOVED) selectedPiece->move = FIRST_MOVE;
    else if(selectedPiece->move == FIRST_MOVE) selectedPiece->move = MOVED;

    if(selectedPiece->type == PAWN && x == 0){
        qInfo() << "Promotion";
        removePiece(x,y);
        stackedWidget->setCurrentIndex(3);

        QEventLoop loop;
        connect(this, &MainWindow::promotionPieceSelected, &loop, &QEventLoop::quit);
        loop.exec();  // Wait here until actionCompleted() is emitted

        SquareWidget* temp = new SquareWidget(centralWidget, x, y, promotionPieceTypeSelected->isWhitePiece, promotionPieceTypeSelected->type);

        QString path = ":/res/res/piece/";
        switch(promotionPieceTypeSelected->type){
        case QUEEN:
            path+="queen";
            break;
        case ROOK:
            path+="rook";
            break;
        case BISHOP:
            path+="bishop";
            break;
        case KNIGHT:
            path+="knight";
            break;
        default:
            break;
        }

        if(promotionPieceTypeSelected->isWhitePiece){
            path+= "-w.svg";
        }else{
            path+= "-b.svg";
        }

        defaultSetUpPiece(temp, path);
        placePiece(temp);

        connect(temp, &SquareWidget::pieceClicked, this, &MainWindow::onPieceClicked);
        selectedPiece = temp;
        allPieces.push_back(temp);
    }

    return checkIfCheck();
}



// ************************************* //
//      Helper method to remove
//      the piece at the index
//      from the board
// ************************************* //
void MainWindow::removePiece(int x , int y){
    for(size_t i = 0; i < allPieces.size(); i++){
        if(allPieces[i] != nullptr && allPieces[i]->x == x && allPieces[i]->y == y){
            allPieces[i]->x = -10;
            allPieces[i]->y = -10;
            board->removeWidget(allPieces[i]);
            delete allPieces[i];
            allPieces[i]= nullptr;
            return;
        }
    }
    return;
}

// ************************************* //
//      Slot method which is used to
//      move the selected piece to
//      the viable spot position
//      which is clicked
// ************************************* //
void MainWindow::onComponentClicked(SquareWidget *comp){
    // qInfo()<<"Moved";
    if(selectedPiece != nullptr && otherComponents.size() != 0) {
        // qInfo()<<"Can Move";
        for(auto& p: specialBoardSquares){
            board->removeWidget(p);
            delete p;
        }
        specialBoardSquares.clear();

        int tempX = comp->x;
        int tempY = comp->y;
        board->removeWidget(selectedPiece);

        if(selectedPiece->type == PAWN && selectedPiece->y != tempY){
            if(checkIfEnPassant(selectedPiece->x,selectedPiece->y-1)){
                QString enPassant;
                for(int i = 8; i < 24; i++){
                    if(allPieces[i]!=nullptr && allPieces[i]->x == selectedPiece->x && allPieces[i]->y == selectedPiece->y-1) enPassant += QString::number( i );
                }
                enPassant += ",Remove";
                removePiece(selectedPiece->x,selectedPiece->y-1);
                _controller.send(enPassant);
                qInfo() << enPassant;
            }
            else if(checkIfEnPassant(selectedPiece->x,selectedPiece->y+1)){
                QString enPassant;
                for(int i = 8; i < 24; i++){
                    if(allPieces[i]!=nullptr && allPieces[i]->x == selectedPiece->x && allPieces[i]->y == selectedPiece->y+1) enPassant += QString::number( i );
                }
                enPassant += ",Remove";
                removePiece(selectedPiece->x,selectedPiece->y+1);
                _controller.send(enPassant);
                qInfo() << enPassant;
            }
            _controller.flush();
        }

        int rookIndex = -1;
        int posX;
        int posY;

        if(selectedPiece->type == KING && selectedPiece->move == NOT_MOVED){
            if(isWhitePlayer){
                if(allPieces[24]->move == NOT_MOVED){ // 7 0
                    if(tempX == 7 && tempY == 2){
                        rookIndex = 24;
                        posX = 7;
                        posY = 3;
                    }
                }else if(allPieces[31]->move == NOT_MOVED){ // 7 7
                    if(tempX == 7 && tempY == 6){
                        rookIndex = 31;
                        posX = 7;
                        posY = 5;
                    }
                }
            }else{
                if(allPieces[0]->move == NOT_MOVED){ // 7 0
                    if(tempX == 7 && tempY == 1){
                        rookIndex = 0;
                        posX = 7;
                        posY = 2;
                    }
                }else if(allPieces[7]->move == NOT_MOVED){
                    if(tempX == 7 && tempY == 5){
                        rookIndex = 7;
                        posX = 7;
                        posY = 4;
                    }
                }
            }
        }


        if(checkIfOppositeColorPiece(tempX, tempY)){
            removePiece(tempX, tempY);
        }
        // int length = allPieces.size();
        size_t previousIndex;

        for(size_t i = 0; i < allPieces.size(); i++){
            if(allPieces[i] != nullptr && allPieces[i] == selectedPiece ) {
                previousIndex = i;
                break;
            }
        }
        bool val = movePiece(tempX, tempY);

        // val;

        for(auto& p: otherComponents){
            board->removeWidget(p);
            delete p;
        }
        otherComponents.clear();

        QString move;
        for(size_t i = 0; i < allPieces.size(); i++){
            if(allPieces[i] != nullptr && allPieces[i] == selectedPiece ) {
                move += QString::number( i );
                break;
            }
        }
        move += "," + QString::number( tempX ) + "," + QString::number( tempY );


        if(rookIndex != -1){

            QString castle;
            selectedPiece = allPieces[rookIndex];
            board->removeWidget(selectedPiece);
            movePiece(posX,posY);

            castle += QString::number( rookIndex ) + ",";
            castle += QString::number( posX ) + ",";
            castle += QString::number( posY ) + ",";
            castle += "Castle";
            _controller.send(castle);
            _controller.flush();
        }

        if(promotionPieceTypeSelected != nullptr){
            QString promotionCapture;
            promotionCapture += QString::number( previousIndex );
            promotionCapture += ",Remove";
            // removePiece(allPieces->x,temp->y);
            qInfo() << promotionCapture;
            _controller.send(promotionCapture);
            _controller.flush();



            move += ",Replace,";
            switch(promotionPieceTypeSelected->type){
            case QUEEN:
                move+="1";
                break;
            case ROOK:
                move+="2";
                break;
            case BISHOP:
                move+="3";
                break;
            case KNIGHT:
                move+="4";
                break;
            default:
                break;
            }
        }

        qInfo() << move ;
        _controller.send(move);

        if(checkIfStaleMate()){
            endLabel->setText(QString("You Draw"));
            stackedWidget->setCurrentIndex(4);
        }else if(checkIfCheckMate()){
            endLabel->setText(QString("You Win"));
            stackedWidget->setCurrentIndex(4);
        }

        selectedPiece = nullptr;
        myTurn = false;
        promotionPieceTypeSelected = nullptr;
    }
}

// ************************************* //
//      Helper method to allocate
//      and givedefault values to
//      pieces according to the
//      player color
// ************************************* //
void MainWindow::allocateAllResource(){
    if(isWhitePlayer){
        bRook1    = new SquareWidget(centralWidget, 0, 7, false, ROOK);
        bKnight1  = new SquareWidget(centralWidget, 0, 6, false, KNIGHT);
        bBishop1  = new SquareWidget(centralWidget, 0, 5, false, BISHOP);
        bQueen    = new SquareWidget(centralWidget, 0, 3, false, QUEEN);
        bKing     = new SquareWidget(centralWidget, 0, 4, false, KING);
        bBishop2  = new SquareWidget(centralWidget, 0, 2, false, BISHOP);
        bKnight2  = new SquareWidget(centralWidget, 0, 1, false, KNIGHT);
        bRook2    = new SquareWidget(centralWidget, 0, 0, false, ROOK);
        bPawn1    = new SquareWidget(centralWidget, 1, 7, false, PAWN);
        bPawn2    = new SquareWidget(centralWidget, 1, 6, false, PAWN);
        bPawn3    = new SquareWidget(centralWidget, 1, 5, false, PAWN);
        bPawn4    = new SquareWidget(centralWidget, 1, 4, false, PAWN);
        bPawn5    = new SquareWidget(centralWidget, 1, 3, false, PAWN);
        bPawn6    = new SquareWidget(centralWidget, 1, 2, false, PAWN);
        bPawn7    = new SquareWidget(centralWidget, 1, 1, false, PAWN);
        bPawn8    = new SquareWidget(centralWidget, 1, 0, false, PAWN);
        wPawn1    = new SquareWidget(centralWidget, 6, 0, true , PAWN);
        wPawn2    = new SquareWidget(centralWidget, 6, 1, true , PAWN);
        wPawn3    = new SquareWidget(centralWidget, 6, 2, true , PAWN);
        wPawn4    = new SquareWidget(centralWidget, 6, 3, true , PAWN);
        wPawn5    = new SquareWidget(centralWidget, 6, 4, true , PAWN);
        wPawn6    = new SquareWidget(centralWidget, 6, 5, true , PAWN);
        wPawn7    = new SquareWidget(centralWidget, 6, 6, true , PAWN);
        wPawn8    = new SquareWidget(centralWidget, 6, 7, true , PAWN);
        wRook1    = new SquareWidget(centralWidget, 7, 0, true , ROOK);
        wKnight1  = new SquareWidget(centralWidget, 7, 1, true , KNIGHT);
        wBishop1  = new SquareWidget(centralWidget, 7, 2, true , BISHOP);
        wQueen    = new SquareWidget(centralWidget, 7, 3, true , QUEEN);
        wKing     = new SquareWidget(centralWidget, 7, 4, true , KING);
        wBishop2  = new SquareWidget(centralWidget, 7, 5, true , BISHOP);
        wKnight2  = new SquareWidget(centralWidget, 7, 6, true , KNIGHT);
        wRook2    = new SquareWidget(centralWidget, 7, 7, true , ROOK);
    }else{
        bRook1    = new SquareWidget(centralWidget, 7, 0, false, ROOK);
        bKnight1  = new SquareWidget(centralWidget, 7, 1, false, KNIGHT);
        bBishop1  = new SquareWidget(centralWidget, 7, 2, false, BISHOP);
        bQueen    = new SquareWidget(centralWidget, 7, 4, false, QUEEN);
        bKing     = new SquareWidget(centralWidget, 7, 3, false, KING);
        bBishop2  = new SquareWidget(centralWidget, 7, 5, false, BISHOP);
        bKnight2  = new SquareWidget(centralWidget, 7, 6, false, KNIGHT);
        bRook2    = new SquareWidget(centralWidget, 7, 7, false, ROOK);
        bPawn1    = new SquareWidget(centralWidget, 6, 0, false, PAWN);
        bPawn2    = new SquareWidget(centralWidget, 6, 1, false, PAWN);
        bPawn3    = new SquareWidget(centralWidget, 6, 2, false, PAWN);
        bPawn4    = new SquareWidget(centralWidget, 6, 3, false, PAWN);
        bPawn5    = new SquareWidget(centralWidget, 6, 4, false, PAWN);
        bPawn6    = new SquareWidget(centralWidget, 6, 5, false, PAWN);
        bPawn7    = new SquareWidget(centralWidget, 6, 6, false, PAWN);
        bPawn8    = new SquareWidget(centralWidget, 6, 7, false, PAWN);
        wPawn1    = new SquareWidget(centralWidget, 1, 7, true , PAWN);
        wPawn2    = new SquareWidget(centralWidget, 1, 6, true , PAWN);
        wPawn3    = new SquareWidget(centralWidget, 1, 5, true , PAWN);
        wPawn4    = new SquareWidget(centralWidget, 1, 4, true , PAWN);
        wPawn5    = new SquareWidget(centralWidget, 1, 3, true , PAWN);
        wPawn6    = new SquareWidget(centralWidget, 1, 2, true , PAWN);
        wPawn7    = new SquareWidget(centralWidget, 1, 1, true , PAWN);
        wPawn8    = new SquareWidget(centralWidget, 1, 0, true , PAWN);
        wRook1    = new SquareWidget(centralWidget, 0, 7, true , ROOK);
        wKnight1  = new SquareWidget(centralWidget, 0, 6, true , KNIGHT);
        wBishop1  = new SquareWidget(centralWidget, 0, 5, true , BISHOP);
        wQueen    = new SquareWidget(centralWidget, 0, 4, true , QUEEN);
        wKing     = new SquareWidget(centralWidget, 0, 3, true , KING);
        wBishop2  = new SquareWidget(centralWidget, 0, 2, true , BISHOP);
        wKnight2  = new SquareWidget(centralWidget, 0, 1, true , KNIGHT);
        wRook2    = new SquareWidget(centralWidget, 0, 0, true , ROOK);
    }

}

// ************************************* //
//      Helper method for default
//      setup of the widget
// ************************************* //
void MainWindow::defaultSetUpPiece(SquareWidget *piece, QString name){
    piece->load(name);
    piece->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

// ************************************* //
//      Helper method to place
//      the given piece on the board
// ************************************* //
void MainWindow::placePiece(SquareWidget *p){
    board->addWidget(p, p->x, p->y);
}

// ************************************* //
//      Helper method to give the
//      widget name for the piece
// ************************************* //
void MainWindow::setStartingPositionPieces() {
    defaultSetUpPiece(bRook1,   QString(":/res/res/piece/rook-b.svg"));
    defaultSetUpPiece(bKnight1, QString(":/res/res/piece/knight-b.svg"));
    defaultSetUpPiece(bBishop1, QString(":/res/res/piece/bishop-b.svg"));
    defaultSetUpPiece(bKing,    QString(":/res/res/piece/king-b.svg"));
    defaultSetUpPiece(bQueen,   QString(":/res/res/piece/queen-b.svg"));
    defaultSetUpPiece(bBishop2, QString(":/res/res/piece/bishop-b.svg"));
    defaultSetUpPiece(bKnight2, QString(":/res/res/piece/knight-b.svg"));
    defaultSetUpPiece(bRook2,   QString(":/res/res/piece/rook-b.svg"));

    defaultSetUpPiece(bPawn1,   QString(":/res/res/piece/pawn-b.svg"));
    defaultSetUpPiece(bPawn2,   QString(":/res/res/piece/pawn-b.svg"));
    defaultSetUpPiece(bPawn3,   QString(":/res/res/piece/pawn-b.svg"));
    defaultSetUpPiece(bPawn4,   QString(":/res/res/piece/pawn-b.svg"));
    defaultSetUpPiece(bPawn5,   QString(":/res/res/piece/pawn-b.svg"));
    defaultSetUpPiece(bPawn6,   QString(":/res/res/piece/pawn-b.svg"));
    defaultSetUpPiece(bPawn7,   QString(":/res/res/piece/pawn-b.svg"));
    defaultSetUpPiece(bPawn8,   QString(":/res/res/piece/pawn-b.svg"));

    defaultSetUpPiece(wPawn1,   QString(":/res/res/piece/pawn-w.svg"));
    defaultSetUpPiece(wPawn2,   QString(":/res/res/piece/pawn-w.svg"));
    defaultSetUpPiece(wPawn3,   QString(":/res/res/piece/pawn-w.svg"));
    defaultSetUpPiece(wPawn4,   QString(":/res/res/piece/pawn-w.svg"));
    defaultSetUpPiece(wPawn5,   QString(":/res/res/piece/pawn-w.svg"));
    defaultSetUpPiece(wPawn6,   QString(":/res/res/piece/pawn-w.svg"));
    defaultSetUpPiece(wPawn7,   QString(":/res/res/piece/pawn-w.svg"));
    defaultSetUpPiece(wPawn8,   QString(":/res/res/piece/pawn-w.svg"));

    defaultSetUpPiece(wRook1,   QString(":/res/res/piece/rook-w.svg"));
    defaultSetUpPiece(wKnight1, QString(":/res/res/piece/knight-w.svg"));
    defaultSetUpPiece(wBishop1, QString(":/res/res/piece/bishop-w.svg"));
    defaultSetUpPiece(wKing,    QString(":/res/res/piece/king-w.svg"));
    defaultSetUpPiece(wQueen,   QString(":/res/res/piece/queen-w.svg"));
    defaultSetUpPiece(wBishop2, QString(":/res/res/piece/bishop-w.svg"));
    defaultSetUpPiece(wKnight2, QString(":/res/res/piece/knight-w.svg"));
    defaultSetUpPiece(wRook2,   QString(":/res/res/piece/rook-w.svg"));

    for(size_t i = 0; i< allPieces.size(); i++){
        placePiece(allPieces[i]);
    }
}

// ************************************* //
//      Checking whether the index
//      is a position where the selected
//      piece can be moved to by performing
//      all the checks necessary
//      and also incrementing count
// ************************************* //
bool MainWindow::checkPositionMove(int x, int y, int* count){
    if(checkIfWithinBoundary(x,y)){
        if(checkIfOppositeColorPiece(x,y)){
            if(allowedMove(x,y)){
                (*count)++;
                return false;
            }
        }else if(!checkIfSameColorPiece(x,y)){
            if(allowedMove(x,y)){
                (*count)++;
                return true;
            }
        }
    }
    return false;
}

// ************************************* //
//      Helper method which calls
//      all checks necessary and checks
//      if any move is possible and
//      increment count
// ************************************* //
void MainWindow::movePossible(int* count){

    const int tempX = selectedPiece->x;
    const int tempY = selectedPiece->y;

    switch(selectedPiece->type){
    case PAWN: {

        if(!myTurn){
            if(checkIfWithinBoundary(tempX-1,tempY+1)){
                if(checkIfOppositeColorPiece(tempX-1,tempY+1)){
                    if(allowedMove(tempX-1,tempY+1)){
                        (*count)++;
                    }
                }
            }

            if(checkIfWithinBoundary(tempX-1,tempY-1)){
                if(checkIfOppositeColorPiece(tempX-1,tempY-1)){
                    if(allowedMove(tempX-1,tempY-1))
                        (*count)++;
                }
            }
            if(checkIfWithinBoundary(tempX-1,tempY)){
                if(!checkIfPiece(tempX-1,tempY)){
                    if(allowedMove(tempX-1,tempY))
                        (*count)++;
                }
            }

            selectedPiece->x = tempX;
            selectedPiece->y = tempY;
            if(selectedPiece->move == NOT_MOVED){
                if(checkIfWithinBoundary(tempX-2,tempY)){
                    if((!checkIfPiece(tempX-1,tempY)) && (!checkIfPiece(tempX-2,tempY))){
                        if(allowedMove(tempX-2,tempY))
                            (*count)++;
                    }
                }
            }

            if(checkIfWithinBoundary(tempX,tempY-1)){
                if(checkIfEnPassant(tempX,tempY-1)){
                    if(allowedMove(tempX-1,tempY-1))
                        (*count)++;
                }
            }

            if(checkIfWithinBoundary(tempX,tempY+1)){
                if(checkIfEnPassant(tempX,tempY+1)){
                    if(allowedMove(tempX-1,tempY+1))
                        (*count)++;
                }
            }
        }else{
            if(checkIfWithinBoundary(tempX+1,tempY+1)){
                if(checkIfOppositeColorPiece(tempX+1,tempY+1)){
                    if(allowedMove(tempX+1,tempY+1)){
                        (*count)++;
                    }
                }
            }

            if(checkIfWithinBoundary(tempX+1,tempY-1)){
                if(checkIfOppositeColorPiece(tempX+1,tempY-1)){
                    if(allowedMove(tempX+1,tempY-1))
                        (*count)++;
                }
            }

            if(checkIfWithinBoundary(tempX+1,tempY)){
                if(!checkIfPiece(tempX+1,tempY)){
                    if(allowedMove(tempX+1,tempY))
                        (*count)++;
                }
            }

            selectedPiece->x = tempX;
            selectedPiece->y = tempY;
            if(selectedPiece->move == NOT_MOVED){
                if(checkIfWithinBoundary(tempX+2,tempY)){
                    if((!checkIfPiece(tempX+1,tempY)) && (!checkIfPiece(tempX+2,tempY))){
                        if(allowedMove(tempX+2,tempY))
                            (*count)++;
                    }
                }
            }

            if(checkIfWithinBoundary(tempX,tempY-1)){
                if(checkIfEnPassant(tempX,tempY-1)){
                    if(allowedMove(tempX+1,tempY-1))
                        (*count)++;
                }
            }

            if(checkIfWithinBoundary(tempX,tempY+1)){
                if(checkIfEnPassant(tempX,tempY+1)){
                    if(allowedMove(tempX+1,tempY+1))
                        (*count)++;
                }
            }
        }
        break;
    }
    case ROOK:{

        // Upwards
        for(int i = tempX-1; i>=0 ; i--){
            if(!checkPositionMove(i,tempY,count))break;
        }

        // Downwards
        for(int i = tempX+1; i<8 ; i++){
            if(!checkPositionMove(i,tempY,count))break;
        }

        // Left side
        for(int i = tempY-1; i>=0 ; i--){
            if(!checkPositionMove(tempX,i,count))break;
        }

        // Right side
        for(int i = tempY+1; i<8 ; i++){
            if(!checkPositionMove(tempX,i,count))break;
        }

        break;
    }
    case KNIGHT:{

        for(int i = 0; i <8; i++){
            checkPositionMove(tempX+diK[i], tempY+djK[i],count);
        }

        break;
    }
    case BISHOP:{
        // Top left side
        for(int i = tempX-1, j = tempY-1; i>=0 && j>=0 ; i--, j--){
            if(!checkPositionMove(i,j,count))break;
        }

        // Top right side
        for(int i = tempX-1, j = tempY+1; i>=0 && j<8 ; i--, j++){
            if(!checkPositionMove(i,j,count))break;
        }

        // Bottom left side
        for(int i = tempX+1, j = tempY-1; i<8 && j>=0 ; i++, j--){
            if(!checkPositionMove(i,j,count))break;
        }

        // Bottom right side
        for(int i = tempX+1, j = tempY+1; i<8 && j<8 ; i++, j++){
            if(!checkPositionMove(i,j,count))break;
        }

        break;
    }
    case QUEEN:{
        // Upwards
        for(int i = tempX-1; i>=0 ; i--){
            if(!checkPositionMove(i,tempY,count))break;
        }

        // Downwards
        for(int i = tempX+1; i<8 ; i++){
            if(!checkPositionMove(i,tempY,count))break;
        }

        // Left side
        for(int i = tempY-1; i>=0 ; i--){
            if(!checkPositionMove(tempX,i,count))break;
        }

        // Right side
        for(int i = tempY+1; i<8 ; i++){
            if(!checkPositionMove(tempX,i,count))break;
        }

        // Top left side
        for(int i = tempX-1, j = tempY-1; i>=0 && j>=0 ; i--, j--){
            if(!checkPositionMove(i,j,count))break;
        }

        // Top right side
        for(int i = tempX-1, j = tempY+1; i>=0 && j<8 ; i--, j++){
            if(!checkPositionMove(i,j,count))break;
        }

        // Bottom left side
        for(int i = tempX+1, j = tempY-1; i<8 && j>=0 ; i++, j--){
            if(!checkPositionMove(i,j,count))break;
        }

        // Bottom right side
        for(int i = tempX+1, j = tempY+1; i<8 && j<8 ; i++, j++){
            if(!checkPositionMove(i,j,count))break;
        }

        break;
    }
    case KING:{

        for(int i = 0; i<8 ; i++){
            checkPositionMove(tempX+di[i],tempY+dj[i],count);
        }

        if(selectedPiece->move == NOT_MOVED){
            if(isWhitePlayer){
                if(allPieces[24]->move == NOT_MOVED){
                    if(!checkIfPiece(7,1) && allowedMove(7,1) && !checkIfPiece(7,2) && allowedMove(7,2) && !checkIfPiece(7,3) && allowedMove(7,3)){
                        (*count)++;
                    }
                }else if(allPieces[31]->move == NOT_MOVED){
                    if(!checkIfPiece(7,5) && allowedMove(7,5) && !checkIfPiece(7,6) && allowedMove(7,6)){
                        (*count)++;
                    }
                }
            }else{
                if(allPieces[0]->move == NOT_MOVED){
                    if(!checkIfPiece(7,1) && allowedMove(7,1) && !checkIfPiece(7,2) && allowedMove(7,2)){
                        (*count)++;
                    }
                }else if(allPieces[7]->move == NOT_MOVED){
                    if(!checkIfPiece(7,4) && allowedMove(7,4) && !checkIfPiece(7,5) && allowedMove(7,5) && !checkIfPiece(7,6) && allowedMove(7,6)){
                        (*count)++;
                    }
                }
            }
        }

        break;
    }
    }

    selectedPiece->x = tempX;
    selectedPiece->y = tempY;
}

// ************************************* //
//      Checking whether the piece
//      piece can be moved by calling
//      the helper method and passing
//      the count
// ************************************* //
void MainWindow::checkIfAnyMoveForPiece(SquareWidget* piece, int* count){
    selectedPiece = piece;
    movePossible(count);
}

// ************************************* //
//      Checking whether the color
//      player has any move possible
// ************************************* //
bool MainWindow::checkAnyMovePossible(){
    int count = 0;
    if(selectedPiece->isWhitePiece){

        for(size_t i=0; i< allPieces.size(); i++){
            if(allPieces[i]!= nullptr && allPieces[i]->isWhitePiece) checkIfAnyMoveForPiece(allPieces[i], &count);
        }
    }else{
        for(size_t i=0; i< allPieces.size(); i++){
            if(allPieces[i]!= nullptr && !allPieces[i]->isWhitePiece) checkIfAnyMoveForPiece(allPieces[i], &count);
        }
    }

    // qInfo() << count;
    // countLabel->setText(QString::number(count));

    if(count > 0){
        return true;
    }
    return false;
}

// ************************************* //
//      Checking whether the checkmate
//      condition has been reached
// ************************************* //
bool MainWindow::checkIfCheckMate(){
    bool res = false;
    if(selectedPiece = allPieces[28], discoverCheck(allPieces[28])){
        res = !checkAnyMovePossible();
    }else if(selectedPiece = allPieces[4] , discoverCheck(allPieces[4])){
        res = !checkAnyMovePossible();
    }
    selectedPiece = nullptr;

    return res;
}

// ************************************* //
//      Checking whether the stalemate
//      condition has been reached
// ************************************* //
bool MainWindow::checkIfStaleMate(){
    if(myTurn){
        if(isWhitePlayer) selectedPiece = allPieces[0];
        else selectedPiece = allPieces[31];
    }else{
        if(isWhitePlayer) selectedPiece = allPieces[31];
        else selectedPiece = allPieces[0];
    }
    // if(selectedPiece->isWhitePiece)
    //     qInfo() << "White";
    // else qInfo() << "Black";

    bool res = !checkAnyMovePossible();

    // if(res) qInfo() << "Stalemate";
    // else qInfo() << "Not Stale";
    return res;
    // return !checkAnyMovePossible();
}

MainWindow::~MainWindow() {}
