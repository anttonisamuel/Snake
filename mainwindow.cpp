/*
####################################################################
# TIE-02201 Ohjelmointi 2: Perusteet, K2019                        #
# TIE-02207 Programming 2: Basics, S2019                           #
#                                                                  #
# Project4: Snake: The Revengeance                                 #
# Program description: "While revenge is served cold and vengeance #
#                       is served hot revengeance is often seen    #
#                       served with Cajun with a side of onion     #
#                       rings and often dumplings in the Orient."  #
#                       - urbandictionary                          #
#                                                                  #
# File: main_window.cpp                                            #
# Description: Defines a class implementing a UI for the game.     #
#                                                                  #
# Author: Anttoni Tukia, 267137, anttoni.tukia@tuni.fi             #
####################################################################
*/

#include "main_window.hh"
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QDebug>


MainWindow::MainWindow(QWidget* parent):

    // Constructor
    QMainWindow(parent) {
    ui_.setupUi(this);
    ui_.graphicsView->setScene(&scene_);

    ui_.sizeSpinBox->setMinimum(1);
    ui_.sizeSpinBox->setMaximum(3);

    ui_.lcdNumberMin->setPalette(Qt::black);
    ui_.lcdNumberSec->setPalette(Qt::black);
    ui_.lcdNumberHighScore->setPalette(Qt::black);

    // Connect all widgets to functions
    connect(&timer_, &QTimer::timeout, this, &MainWindow::moveSnake);
    connect(&game_time_, &QTimer::timeout, this, &MainWindow::add_gametime);
    connect(ui_.playButton, &QPushButton::clicked, this, &MainWindow::play);
    connect(ui_.quitButton, &QPushButton::clicked, this, &MainWindow::quit);
    connect(ui_.pauseResumeButton, &QPushButton::clicked, this, &MainWindow::pauseResume);
    connect(ui_.checkBoxEasyMode, &QCheckBox::stateChanged,this, &MainWindow::toggle_easymode);
    connect(ui_.checkBoxWalls, &QCheckBox::stateChanged, this, &MainWindow::walls);
}

// Funtion dictates events for each key used in game.
void MainWindow::keyPressEvent(QKeyEvent* event) {

    switch (event->key()) {

    // When A key is pressed
    case Qt::Key_A:

        // Snake can't go backwards.
        // The key works the opposite way if snake has eaten an opposite item.
        if (direction_x_ != 1 and opposite_keys_ == false)
        {
            direction_x_ = -1;
            direction_y_ = 0;
        }
        else if (direction_x_ != -1 and opposite_keys_ == true)
        {
            direction_x_ = 1;
            direction_y_ = 0;
        }
        break;

    case Qt::Key_D:

        if (direction_x_ != -1 and opposite_keys_ == false)
        {
            direction_x_ = 1;
            direction_y_ = 0;
        }
        else if (direction_x_ != 1 and opposite_keys_ == true)
        {
            direction_x_ = -1;
            direction_y_ = 0;
        }
        break;

    case Qt::Key_W:

        if (direction_y_ != 1 and opposite_keys_ == false)
        {
            direction_x_ = 0;
            direction_y_ = -1;
        }
        else if (direction_y_ != -1 and opposite_keys_ == true)
        {
            direction_x_ = 0;
            direction_y_ = 1;
        }
        break;

    case Qt::Key_S:

        if (direction_y_ != -1 and opposite_keys_ == false)
        {
            direction_x_ = 0;
            direction_y_ = 1;
        }
        else if (direction_y_ != 1 and opposite_keys_ == true)
        {
            direction_x_ = 0;
            direction_y_ = -1;
        }
        break;

    case Qt::Key_Space:

        //Game can't be paused while not playing
        if (game_on_ == true)
        {
            pauseResume();
        }
        break;

    default:
        break;
    }
}

// Fuction is called when Play-button is pressed.
// Starts and re-starts the game
void MainWindow::play() {

    // Get the valuen for map size ja reset needed elements in case it's a re-start.
    size_ = ui_.sizeSpinBox->value();
    game_on_ = true;
    opposite_keys_ = false;
    played_mins_ = 0;
    played_secs_ = 0;
    ui_.lcdNumberMin->display(played_mins_);
    ui_.lcdNumberSec->display(played_secs_);
    ui_.textBrowser->clear();

    // Disable buttons so game settings can't be tempered with during the game.
    ui_.playButton->setEnabled(false);
    ui_.sizeSpinBox->setEnabled(false);
    ui_.checkBoxEasyMode->setEnabled(false);
    ui_.checkBoxWalls->setEnabled(false);

    // Create snakes head and place it on the map.
    const QRectF head_rect(0, 0, 1, 1);
    const QPen pen(Qt::black, 0);
    const QBrush brush(Qt::black);
    head_ = scene_.addRect(head_rect, pen, brush);
    head_->setPos(9, 5);
    head_->setZValue(1);

    // Insert food on the map.
    insertFood();

    // Insert opposite item if player hasn't selected easy mode.
    if (easy_mode == false)
    {
        insertopposite();
    }

    // Adjust screen and start timers
    adjustSceneArea();
    timer_.start(time_);
    game_time_.start(second_);

}

// Creates food item and calls funtion "moveitem()" to place it on map.
void MainWindow::insertFood(){
    const QRectF food_rect(0.25, 0.25, 0.25, 0.25);
    const QPen pen(Qt::red, 0);
    const QBrush brush(Qt::red);
    food_ = scene_.addRect(food_rect, pen, brush);
    moveitem(food_);
}

// Moves wanted item to a random place on the map.
void MainWindow::moveitem(QGraphicsRectItem* item){

    while (true)
    {
        // Draw a random point from the map.
        std::uniform_int_distribution<int> distribution_x(0, (size_*10)-1);
        std::uniform_int_distribution<int> distribution_y(0, (size_*10)-1);
        bool flag = false;
        item->setPos(distribution_x(rng_), distribution_y(rng_));

        // If the recently drawn point has the same coorditanes than a tail element,
        // raise flag and draw a new one.
        for (int i = 0; i < tail_.size();i++)
        {
            if ((tail_[i]->scenePos() == item->scenePos()) or (head_->scenePos() == item->scenePos()))
            {
                flag = true;
            }
        }

        // Break out if the point doesn't share coordinates with the tail.
        if (flag == false)
        {
            break;
        }
    }
}

//Creates an opposite item and calls funtion "moveitem()" to place it on the map.
void MainWindow::insertopposite(){
    const QRectF opposite_rect(0.25, 0.25, 0.25, 0.25);
    const QPen pen(Qt::blue, 0);
    const QBrush brush(Qt::blue);
    opposite_ = scene_.addRect(opposite_rect, pen, brush);
    moveitem(opposite_);
}

// Controls the snakes movement on the map.
// Is called everytime "timer_" times out.
void MainWindow::moveSnake() {

    // Get heads currect position and add on deduct it's current direction to create movement.
    const QPointF old_head_pos = head_->scenePos();
    QPointF new_head_pos = old_head_pos + QPoint(direction_x_, direction_y_);
    head_->setPos(new_head_pos);

    // Check if the snake has collided with anything or has ate anything.
    checkcollisions(new_head_pos);
    checkfeeding(new_head_pos, old_head_pos);

    // Tail follows if there is any of it.
    if (tail_.size() > 0)
    {
        // Go through each element of the tail an move it forward.
        for (int i = tail_.size() -1; i >= 0; i--)
        {
            if (i == 0)
            {
                tail_[i]->setPos(old_head_pos.x(), old_head_pos.y());
            }
            else
            {
                tail_[i]->setPos(tail_[i - 1]->x(), tail_[i - 1]->y());
            }
        }
    }
}

// Checks if the snaske has collided with the wall or eaten its own tail.
// Is called every time the snake moves.
void MainWindow::checkcollisions(QPointF & new_head_pos){

    // If heads coordinates match with walls coordinates.
    if ((new_head_pos.x() >= size_*10) or (new_head_pos.x() < 0) or (new_head_pos.y() < 0) or (new_head_pos.y() >= size_*10))
    {
        // Go throght the wall and "teleport" to the other side if player has chosen not to have walls.
        if (walls_ == false)
        {
            if (new_head_pos.x() == -1)
            {
                new_head_pos = QPoint(size_*10 -1, new_head_pos.y());
            }
            else if (new_head_pos.x() == size_*10)
            {
                new_head_pos = QPoint(0, new_head_pos.y());
            }
            else if (new_head_pos.y() == -1)
            {
                new_head_pos = QPoint(new_head_pos.x(), size_*10-1);
            }
            else if (new_head_pos.y() == size_*10)
            {
                new_head_pos = QPoint(new_head_pos.x(), 0);
            }
            head_->setPos(new_head_pos);
        }

        // End game if player has chosen to have walls.
        else if (easy_mode == false)
        {
            game_over();
        }

    }

    // Check if snakes has eaten its own tail and end game if it had.
    for (int i = 0; i < tail_.size(); i++)
    {
        if((tail_[i]->x() == head_->x()) and (tail_[i]->y() == head_->y()))
        {
            game_over();
        }
    }
}

// Function checks if the snake has eaten food or an opposite item.
// Called every the the snake moves.
void MainWindow::checkfeeding(const QPointF & new_head_pos, const QPointF & old_head_pos){

    // Check if heads coordinates match to opposite items coordinates and
    // the player has chosen to play on normal mode.
    if (easy_mode == false and (new_head_pos.x() == opposite_->x() and new_head_pos.y() == opposite_->y()))
    {

        //Switch keys to opposite if they are on normal mode or
        //switch them back to normal mode if they are on opposite mode.
        if (opposite_keys_ == false)
        {
            opposite_keys_ = true;
            moveitem(opposite_);
        }
        else
        {
            opposite_keys_ = false;
            moveitem(opposite_);
        }
    }

    // If head coordinates match to food coordinates, grow snake.
    if ((new_head_pos.x() == food_->x()) and (new_head_pos.y() == food_->y()))
    {
        grow_snake(old_head_pos);

        // Increase snakes movement speed if easy mode is off.
        if (easy_mode == false and time_ > 200)
        {
            time_ = time_*0.90;
            timer_.setInterval(time_);
        }
    }
}

// Funtion ends the game and does preparations for a new game.
// Called when snake eats its own tail or if walls are on and the snake collides with them.
void::MainWindow::game_over(){

    // Stop timers the contors snakes movement.
    timer_.stop();
    game_time_.stop();

    // Inform the player.
    ui_.textBrowser->setText("Snek is ded :(");
    ui_.textBrowser->append("Press Play for new game");

    // Unlock widgets.
    ui_.playButton->setEnabled(true);
    ui_.sizeSpinBox->setEnabled(true);
    ui_.checkBoxEasyMode->setEnabled(true);
    ui_.checkBoxWalls->setEnabled(true);

    // Set new high score if the current score is the best.
    if (high_score_ < tail_.size())
    {
        high_score_ = tail_.size();
        ui_.lcdNumberHighScore->display(high_score_);
    }

    // Reset the scene and variables for a new game.
    tail_.clear();
    scene_.clear();
    game_on_ = false;
    direction_x_ = -1;
    direction_y_ = 0;
    time_ = 800;

}

// Adds a piece to the tail.
// Called whenever snake eats food.
void::MainWindow::grow_snake(const QPointF new_tail){

    // Move food elsewhere.
    moveitem(food_);

    // Create a new graphicsitem and add it to tail.
    const QRectF tail_rect(0, 0, 1, 1);
    const QPen pen(Qt::black, 0);
    const QBrush brush(Qt::green);
    QGraphicsRectItem *tail = scene_.addRect(tail_rect, pen, brush);
    tail->setPos(new_tail.x(), new_tail.y());
    tail_.append(tail);
    tailcolor();
}

// Function makes snakes tail color vary,
// the closer that part is to the tail the more darker the color gets.
void::MainWindow::tailcolor(){

    for (int i = 1; i < tail_.size(); i++)
    {
        tail_[i]->setBrush(QColor(0, 255/i ,0));
    }
}

// Adjust the scene area to the chosen size.
void MainWindow::adjustSceneArea() {

    int field_size = size_ * 10;
    const QRectF area(0, 0, field_size, field_size);
    scene_.setSceneRect(area);
    ui_.graphicsView->fitInView(area);
}

// Funtion controls the two lcdnumber widgets showing current game time.
void MainWindow::add_gametime(){

    // Add one to minute counter if 60 second has passed.
    if (played_secs_ == 60)
    {
        played_secs_ = 0;
        ++played_mins_;
        ui_.lcdNumberMin->display(played_mins_);
    }

    // Add one to second counter
    ++played_secs_;
    ui_.lcdNumberSec->display(played_secs_);
}

// Closes the window
void MainWindow::quit()
{
    close();
}

// Funtion is used to pause and resume the game.
// Called the pause button is pressed.
void MainWindow::pauseResume()
{
    if (game_on_ == true)
    {
        // Pause game if the game on, resume if it is already paused.
        if (ui_.pauseResumeButton->text() == "Pause")
        {
            timer_.stop();
            game_time_.stop();
            ui_.pauseResumeButton->setText("Resume");
        }
        else
        {
            timer_.start(time_);
            game_time_.start(second_);
            ui_.pauseResumeButton->setText("Pause");
        }
    }
}

// Toggles easy mode on and off.
void MainWindow::toggle_easymode()
{
    if (ui_.checkBoxEasyMode->isChecked())
    {
        easy_mode = true;
    }
    else
    {
        easy_mode = false;
    }
}

// Toggles walls on and off.
void MainWindow::walls()
{
    if (ui_.checkBoxWalls->isChecked())
    {
        walls_ = true;
    }
    else
    {
        walls_ = false;
    }
}
