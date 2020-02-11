#include<iostream>
#include<vector>
#ifndef SNAKE_H
#define SNAKE_H
#endif
struct snakepart{
    int x,y;
    snakepart();
    snakepart(int col,int row);
};
class snakeclass{ //for window
    int maxheight, maxwidth, points, del; //del=delay points=score
    char direction, partchar, oldchar, foo; //direction=input direction partchar=char for snake oldchar=char for boundary foo=char for food
    bool get; //get=true if snake gets food
    snakepart food; //location of food
    std::vector<snakepart> snake; //actual snake
    void putfood(); //food not inside snake and inside edge
    bool collision(); //if snake collides with edge or itself
    void movesnake(); //get one character and make it look like snake is moving
    public:
        snakeclass();
        ~snakeclass();
        void start(); 
};