#include"snake.h"
#include<ncurses.h>
#include<stdlib.h>
#include <unistd.h>
snakepart::snakepart(int col,int row)
{
    x=col;
    y=row;
}
snakepart::snakepart()
{
    x=0;
    y=0;
}
snakeclass::snakeclass()
{
    initscr();
    nodelay(stdscr,true); // program(GETCH) will not wait until user presses key . program will run even without any key being pressed
    keypad(stdscr,true); // for up down arrows function keys 
    noecho(); //no input text
    curs_set(0); //no curser
    getmaxyx(stdscr,maxheight,maxwidth);
    partchar=(char)219; // Character for The snake
    oldchar='.';  //charcter for boundary;
    foo='*';    //character for food
    food.x=0; //initialize food direction
    food.y=0; //initialize food direction y
    for(int i=0;i<5;i++)
    {
        snake.push_back(snakepart(40+i,10));  //initial snake location
    }
    points=0;
    del=110000;
    get=false;
    direction='l'; //direction the snake will start
    srand(time(0));
    putfood();
    for(int i=0;i<maxwidth-1;i++)  //Make two horizantal boundaries
    {
        move(0,i);
        addch(oldchar);
        move(maxheight-2,i);
        addch(oldchar);
    }
    for(int i=0;i<maxheight-1;i++)  //make two vertical boundaries till one less so that last line score can be printed
    {
        move(i,0);
        addch(oldchar);
        move(i,maxwidth-1);
        addch(oldchar);
    }
    move(maxheight,0);
    printw("Score:%d",points);
    for(int i=0;i<snake.size();i++)
    {
        move(snake[i].y,snake[i].x);        //print the snake
        addch(partchar);
    }
    move(food.y,food.x);
    addch(foo);
    refresh();
}
snakeclass::~snakeclass()
{
    nodelay(stdscr,false);  // so that game stops after snake hits edges or itself and not directly move out of program
    getch();    // wait after game over .after game ovr press any button to move out of program
    endwin();
}
void snakeclass::putfood()
{
    while(1){
        int tmpx=rand()%maxwidth+1;   //left vertical boundary limit
        int tmpy=rand()%maxheight+1;    //upper boundary limit
        for(int i=0;i<snake.size();i++)
        {
            if(snake[i].x==tmpx && snake[i].y==tmpy)
                continue; 
        }
        if(tmpx>=maxwidth-2 || tmpy>=maxheight-3) //right boundary limit and lower boundary limit
            continue;
        food.x=tmpx; //assign new food point
        food.y=tmpy;
        break;
    }
    move(food.y,food.x);    //move cursor to new point
    addch(foo);
    
}
bool snakeclass::collision()
{
    if(snake[0].x==0 || snake[0].y==0 || snake[0].x==maxwidth-1 || snake[0].y==maxheight-2) //check if head of snake is on the edges
        return true;
    for(int i=2;i<snake.size();i++)      //check if head of snake is colliding with itself
    {
        if(snake[0].x==snake[i].x && snake[0].y==snake[i].y)
            return true;
    }
    if(snake[0].x==food.x && snake[0].y==food.y)   //check if head is colliding with food point
    {
        get=true;
        points+=10;
        putfood();      //generate new food point
        move(maxheight-1,0);
        printw("Score:%d",points);
        if((points%100)==0)
            del-=10000;     //increase speed of snake once points reaches 100. increase difficulty. lower sleep below the loops
    }
    else
    {
        get=false;
    }
    return false;
}

void snakeclass::movesnake()
{
    int temp=getch();
    switch (temp)
    {
    case KEY_LEFT: if (direction!='r')      //KEY_LEFT=left direction . if snake is moving in right direction,it cannot turn towards the left side
        direction='l';
        break;
    case KEY_UP: if (direction!='d')    //KEY_LEFT=up direction . if snake is moving in down direction,it cannot turn towards the up direction
        direction='u';
        break;
    case KEY_RIGHT: if (direction!='l')
        direction='r';
        break;
    case KEY_DOWN: if (direction!='u')
        direction='d';
        break;
    
    case KEY_BACKSPACE: direction='q';  //quit game on pressing backspace
        break;
    }
    if(!get)            //if food point is not reached by the snake
    {
        move(snake[snake.size()-1].y,snake[snake.size()-1].x);  //move to the last element in the snake vector
        addch(' ');     //replace last point by blank space
        refresh();
        snake.pop_back();   //pop out the last element in snake vector
    }
    if(direction=='l')
        snake.insert(snake.begin(),snakepart(snake[0].x-1,snake[0].y)); //insert in the beginning a snakepart type vector element where the first element's x coordinate is decreased by one, as moving towards the left
    else if(direction=='r')
        snake.insert(snake.begin(),snakepart(snake[0].x+1,snake[0].y));  //insert in the beginning a snakepart type vector element where the first element's x coordinate is increased by one, as moving towards the right
    else if(direction=='u')
        snake.insert(snake.begin(),snakepart(snake[0].x,snake[0].y-1));
    else if(direction=='d')
        snake.insert(snake.begin(),snakepart(snake[0].x,snake[0].y+1));
    move(snake[0].y,snake[0].x);    //print the head which was popped out in the if statement above
    addch(partchar);
    refresh();
}
void snakeclass::start()
{
    while(1){
    if(collision()){
        move(maxheight/2-2,maxwidth/2);
        printw("Game Over");
        break;
    }
    movesnake();
    if(direction=='q')
        break;
    usleep(del);        //usleep is needed as this control speed of the snake, i.e at what speed each loop will be executed if this is not there snake will start at 40,10 but will run through the loops and go the boundary and game will be over as looping would be done almost instantly not visible to the eye
    }
}