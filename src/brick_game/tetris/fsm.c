#ifndef FSM_C
#define FSM_C

#include "tetris.h"
#include "backend.h"
#include "fsm.h"
#include "bonus.h"
#include "ncurses.h"

// Функция для просчета одной итерации
void calculateTet(TetGame_t *game)
{
    if (game->counterIter >= COUNT_ITERATION * 15 / game->gameInfo->speed)
    {
        game->counterIter = 0;
        moveFigureDown(game->figure);

        if (collisionFigure(game))
        {
            moveFigureUp(game->figure);
            placeFigure(game);
            int count_rows = eraseLines(game);
            game->gameInfo->score = game->gameInfo->score + converterScore(count_rows);
            levelUp(game);
            updateFigure(game);
            if (collisionFigure(game))
            {
                game->gameStatus = GameOver;
            }
        }
    }
    else
    {
        switch (game->action)
        {
        case Right:
        {
            moveFigureRight(game->figure);
            if (collisionFigure(game))
                moveFigureLeft(game->figure);
            break;
        }
        case Left:
        {
            moveFigureLeft(game->figure);
            if (collisionFigure(game))
                moveFigureRight(game->figure);
            break;
        }
        case Up:
        {
            rotateFigure(game->figure);
            if (collisionFigure(game))
                for (int i = 0; i < 3; i++)
                {
                    rotateFigure(game->figure);
                }
            break;
        }
        case Down:
        {
            moveFigureDown(game->figure);
            if (collisionFigure(game))
            {
                moveFigureUp(game->figure);
                placeFigure(game);
                int count_rows = eraseLines(game);
                game->gameInfo->score = game->gameInfo->score + converterScore(count_rows);
                levelUp(game);
                updateFigure(game);
                if (collisionFigure(game))
                {
                    game->gameStatus = GameOver;
                }
            }
            break;
        }
        case Action:
        default:
            break;
        }
        game->counterIter++;
    }
}

// Обновляет текущий состояние игры
TetGame_t *updateCurrentState(TetGame_t *game)
{
    calculateTet(game);
    if (game->gameInfo->score > game->gameInfo->record)
    {
        game->gameInfo->record = game->gameInfo->score;
        writeDataBase(game->gameInfo->record);
    }
    return game;
}
#endif

// Инициализировать новую игру
void initGame(TetGame_t *game)
{
    game->figure = createRandomFigure();
    game->figureNext = createRandomFigure();
    game->field = createField();

    game->action = Start;

    game->gameInfo->level = 1;
    game->gameInfo->record = readDataBase();

    game->gameInfo->score = 0;
    game->gameInfo->speed = 5;

    game->counterIter = 1;
}