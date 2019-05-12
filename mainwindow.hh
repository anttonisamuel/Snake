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
# File: main_window.hh                                             #
# Description: Declares a class implementing a UI for the game.    #
#                                                                  #
# Author: Anttoni Tukia, 267137, anttoni.tukia@tuni.fi             #
####################################################################
*/

#ifndef PRG2_SNAKE2_MAINWINDOW_HH
#define PRG2_SNAKE2_MAINWINDOW_HH

#include "ui_main_window.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QTimer>
#include <random>



/* \class MainWindow
 * \brief Implements the main window through which the game is played.
 */
class MainWindow: public QMainWindow {
    Q_OBJECT

public:

    /* \brief Construct a MainWindow.
     *
     * \param[in] parent The parent widget of this MainWindow.
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /* \brief Destruct a MainWindow.
     */
    ~MainWindow() override = default;

    /* \brief Change the Snake's bearing when certain keys get pressed.
     *
     * \param[in] event Contains data on pressed and released keys.
     */
    void keyPressEvent(QKeyEvent* event) override;

    void moveitem(QGraphicsRectItem* item);

    void grow_snake(const QPointF new_tail);

    void add_gametime();

    void insertopposite();

    void checkcollisions(QPointF &new_head_pos);
    void tailcolor();
    void checkfeeding(const QPointF &new_head_pos, const QPointF &old_head_pos);
private slots:

    /* \brief Start the game.
     */
    void play();

    /* \brief Move the Snake by a square and check for collisions.
     *
     * The game ends if a wall or the Snake itself gets in the way.
     * When a food gets eaten a point is gained and the Snake grows.
     */
    void moveSnake();

    void quit();
    void insertFood();

    void pauseResume();

    void game_over();


    void toggle_easymode();

    void walls();

private:

    /* \brief Make the play field visible and fit it into the view.
     *
     * Should be called every time the field's size changes.
     */
    void adjustSceneArea();

    int size_;
    int high_score_ = 0;

    int time_ = 800;
    int second_ = 1000;

    int played_mins_ = 0;
    int played_secs_ = 0;

    bool game_on_ = false;
    bool pause_ = false;
    bool easy_mode = false;
    bool opposite_keys_ = false;
    bool walls_ = false;

    int direction_x_ = -1;
    int direction_y_ = 0;

    QList<QGraphicsRectItem*> tail_;

    Ui::MainWindow ui_;                 /**< Accesses the UI widgets. */

    QGraphicsRectItem* head_ = nullptr; /**< The food item in the scene. */
    QGraphicsRectItem* food_ = nullptr;
    QGraphicsRectItem* opposite_ = nullptr;

    QGraphicsScene scene_;              /**< Manages drawable objects. */
    QTimer timer_;                      /**< Triggers the Snake to move. */
    QTimer game_time_;
    std::default_random_engine rng_;    /**< Randomizes food locations. */


};  // class MainWindow


#endif  // PRG2_SNAKE2_MAINWINDOW_HH
