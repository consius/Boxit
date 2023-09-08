#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

// #define SIZEX 360
// #define SIZEY 360
#define DOT_SIZE 10
#define LINE_THICKNESS 8
#define DOT_SPACE 60
#define EMPTY 0
#define LINE 1
#define RED 1
#define BLUE 2

SDL_Window* win;
SDL_Renderer* rend;
int xDots;
int yDots;
typedef struct {
    int top;
    int right;
    int bottum;
    int left;
    int owner;
} box;
int currentPlayer;
int redBoxes;
int blueBoxes;

int Init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to Init SDL: %s", SDL_GetError());
        return -1;
    }

    win = SDL_CreateWindow("Box it!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (xDots+1)*DOT_SPACE, (yDots+1)*DOT_SPACE, 0);
    if(win == NULL)
    {
        printf("Failed to create SDL Window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if(rend == NULL)
    {
        printf("Failed to create SDL Renderer: %s", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return -1;
    }
    return 0;
}

int Render(box* boxes)
{
    SDL_SetRenderDrawColor(rend, 215, 215, 215, 255);
    SDL_RenderClear(rend);

    if(currentPlayer == BLUE)
    {
        SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
        SDL_Rect playerRect = {DOT_SPACE-DOT_SIZE, 10, (xDots-1)*DOT_SPACE+(2*DOT_SIZE), DOT_SPACE-30};
        SDL_RenderFillRect(rend, &playerRect);
    }
    if(currentPlayer == RED)
    {
        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
        SDL_Rect playerRect = {DOT_SPACE-DOT_SIZE, 10, (xDots-1)*DOT_SPACE+(2*DOT_SIZE), DOT_SPACE-30};
        SDL_RenderFillRect(rend, &playerRect);
    }
    for(int y = 1; y < yDots; y++)
    {
        for(int x = 1; x < xDots; x++)
        {
            if(boxes[(y*xDots)+x].owner == BLUE)
            {
                SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
                SDL_Rect fillBox = {x*DOT_SPACE, y*DOT_SPACE, DOT_SPACE, DOT_SPACE};
                SDL_RenderFillRect(rend, &fillBox);
            }
            if(boxes[(y*xDots)+x].owner == RED)
            {
                SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
                SDL_Rect fillBox = {x*DOT_SPACE, y*DOT_SPACE, DOT_SPACE, DOT_SPACE};
                SDL_RenderFillRect(rend, &fillBox);
            }

            if(boxes[(y*xDots)+x].top)
            {
                thickLineRGBA(rend, x*DOT_SPACE, y*DOT_SPACE, (x+1)*DOT_SPACE, y*DOT_SPACE, LINE_THICKNESS, 20, 20, 20, 255);
            }
            if(boxes[(y*xDots)+x].right)
            {
                thickLineRGBA(rend, (x+1)*DOT_SPACE, y*DOT_SPACE, (x+1)*DOT_SPACE, (y+1)*DOT_SPACE, LINE_THICKNESS, 20, 20, 20, 255);
            }
            if(boxes[(y*xDots)+x].bottum)
            {
                thickLineRGBA(rend, x*DOT_SPACE, (y+1)*DOT_SPACE, (x+1)*DOT_SPACE, (y+1)*DOT_SPACE, LINE_THICKNESS, 20, 20, 20, 255);
            }
            if(boxes[(y*xDots)+x].left)
            {
                thickLineRGBA(rend, x*DOT_SPACE, y*DOT_SPACE, x*DOT_SPACE, (y+1)*DOT_SPACE, LINE_THICKNESS, 20, 20, 20, 255);
            }
        }
    }

    int mousex, mousey;
    SDL_GetMouseState(&mousex, &mousey);
    if(mousex >= DOT_SPACE-LINE_THICKNESS && mousey >= DOT_SPACE-LINE_THICKNESS && mousex <= ((xDots)*DOT_SPACE)+LINE_THICKNESS && mousey <= ((yDots)*DOT_SPACE)+LINE_THICKNESS)
    {
        int cellposx = mousex/DOT_SPACE;
        int cellposy = mousey/DOT_SPACE;
        int currentcellx = cellposx*DOT_SPACE;
        int currentcelly = cellposy*DOT_SPACE;
        mousex -= currentcellx;
        mousey -= currentcelly;
        if(mousex >= DOT_SIZE && mousex <= DOT_SPACE-DOT_SIZE && mousey >= 0 && mousey <= LINE_THICKNESS)
        {
            if(boxes[(cellposy*xDots)+cellposx].top == EMPTY)
            {
                if(currentPlayer == BLUE)
                    thickLineRGBA(rend, currentcellx, currentcelly, currentcellx+DOT_SPACE, currentcelly, LINE_THICKNESS, 0, 0, 255, 180);
                else
                    thickLineRGBA(rend, currentcellx, currentcelly, currentcellx+DOT_SPACE, currentcelly, LINE_THICKNESS, 255, 0, 0, 180);
            }
        }
        if(mousex >= DOT_SPACE-LINE_THICKNESS && mousex <= DOT_SPACE && mousey >= DOT_SIZE && mousey <= DOT_SPACE-DOT_SIZE)
        {
            if(boxes[(cellposy*xDots)+cellposx].right == EMPTY)
            {
                if(currentPlayer == BLUE)
                    thickLineRGBA(rend, currentcellx+DOT_SPACE, currentcelly, currentcellx+DOT_SPACE, currentcelly+DOT_SPACE, LINE_THICKNESS, 0, 0, 255, 180);
                else
                    thickLineRGBA(rend, currentcellx+DOT_SPACE, currentcelly, currentcellx+DOT_SPACE, currentcelly+DOT_SPACE, LINE_THICKNESS, 255, 0, 0, 180);
            }
        }
        if(mousex >= DOT_SIZE && mousex <= DOT_SPACE-DOT_SIZE && mousey >= DOT_SPACE-LINE_THICKNESS  && mousey <= DOT_SPACE)
        {
            if(boxes[(cellposy*xDots)+cellposx].bottum == EMPTY)
            {
                if(currentPlayer == BLUE)
                    thickLineRGBA(rend, currentcellx, currentcelly+DOT_SPACE, currentcellx+DOT_SPACE, currentcelly+DOT_SPACE, LINE_THICKNESS, 0, 0, 255, 180);
                else
                    thickLineRGBA(rend, currentcellx, currentcelly+DOT_SPACE, currentcellx+DOT_SPACE, currentcelly+DOT_SPACE, LINE_THICKNESS, 255, 0, 0, 180);
            }
        }
        if(mousex >= 0 && mousex <= LINE_THICKNESS && mousey >= DOT_SIZE && mousey <= DOT_SPACE-DOT_SIZE)
        {
            if(boxes[(cellposy*xDots)+cellposx].left == EMPTY)
            {
                if(currentPlayer == BLUE)
                    thickLineRGBA(rend, currentcellx, currentcelly, currentcellx, currentcelly+DOT_SPACE, LINE_THICKNESS, 0, 0, 255, 180);
                else
                    thickLineRGBA(rend, currentcellx, currentcelly, currentcellx, currentcelly+DOT_SPACE, LINE_THICKNESS, 255, 0, 0, 180);
            }
        }
    }

    for(int y = DOT_SPACE; y <= yDots*DOT_SPACE; y+=DOT_SPACE)
    {
        for(int x = DOT_SPACE; x <= xDots*DOT_SPACE; x+=DOT_SPACE)
        {
            filledCircleRGBA(rend, x, y, DOT_SIZE, 20, 20, 20, 255);
        }
    }

    SDL_RenderPresent(rend);
    return 0;
}

int ChangePlayer()
{
    if(currentPlayer == RED)
        currentPlayer = BLUE;
    else
        currentPlayer = RED;
    return 0;
}

int CheckOwn(box* boxes)
{
    int owned = 0;
    for(int y = 1; y < yDots; y++)
    {
        for(int x = 1; x < xDots; x++)
        {
            if(boxes[(y*xDots)+x].owner == EMPTY)
            {
                if(boxes[(y*xDots)+x].top && boxes[(y*xDots)+x].right && boxes[(y*xDots)+x].bottum && boxes[(y*xDots)+x].left)
                {
                    boxes[(y*xDots)+x].owner = currentPlayer;
                    owned += currentPlayer;
                }
            }
        }
    }
    return owned;
}

int DrawLine(int x, int y, box* boxes)
{
    if(x <= DOT_SPACE-LINE_THICKNESS || y <= DOT_SPACE-LINE_THICKNESS || x >= ((xDots)*DOT_SPACE)+LINE_THICKNESS || y >= ((yDots)*DOT_SPACE)+LINE_THICKNESS)
    {
        return 0;
    }
    int cellposx = x/DOT_SPACE;
    int cellposy = y/DOT_SPACE;
    int currentcellx = cellposx*DOT_SPACE;
    int currentcelly = cellposy*DOT_SPACE;
    x -= currentcellx;
    y -= currentcelly;

    if(x >= DOT_SIZE && x <= DOT_SPACE-DOT_SIZE && y >= 0 && y <= LINE_THICKNESS)
    {
        if(boxes[(cellposy*xDots)+cellposx].top == EMPTY)
        {
            boxes[(cellposy*xDots)+cellposx].top = LINE;
            boxes[((cellposy-1)*xDots)+cellposx].bottum = LINE;
            if(!CheckOwn(boxes))
                ChangePlayer();
        }
    }
    if(x >= DOT_SPACE-LINE_THICKNESS && x <= DOT_SPACE && y >= DOT_SIZE && y <= DOT_SPACE-DOT_SIZE)
    {
        if(boxes[(cellposy*xDots)+cellposx].right == EMPTY)
        {
            boxes[(cellposy*xDots)+cellposx].right = LINE;
            boxes[(cellposy*xDots)+cellposx+1].left = LINE;
            if(!CheckOwn(boxes))
                ChangePlayer();
        }
    }
    if(x >= DOT_SIZE && x <= DOT_SPACE-DOT_SIZE && y >= DOT_SPACE-LINE_THICKNESS  && y <= DOT_SPACE)
    {
        if(boxes[(cellposy*xDots)+cellposx].bottum == EMPTY)
        {
            boxes[(cellposy*xDots)+cellposx].bottum = LINE;
            boxes[((cellposy+1)*xDots)+cellposx].top = LINE;
            if(!CheckOwn(boxes))
                ChangePlayer();
        }
    }
    if(x >= 0 && x <= LINE_THICKNESS && y >= DOT_SIZE && y <= DOT_SPACE-DOT_SIZE)
    {
        if(boxes[(cellposy*xDots)+cellposx].left == EMPTY)
        {
            boxes[(cellposy*xDots)+cellposx].left = LINE;
            boxes[(cellposy*xDots)+cellposx-1].right = LINE;
            if(!CheckOwn(boxes))
                ChangePlayer();
        }
    }
    return 0;
}

int checkWin(box* boxes)
{
    redBoxes = 0;
    blueBoxes = 0;
    for(int y = 1; y < yDots; y++)
    {
        for(int x = 1; x < xDots; x++)
        {
            if(boxes[(y*xDots)+x].owner == RED)
            {
                redBoxes += 1;
            }
            if(boxes[(y*xDots)+x].owner == BLUE)
            {
                blueBoxes += 1;
            }
        }
    }
    if(redBoxes+blueBoxes == (yDots-1)*(xDots-1))
    {
        return 1;
    }
    return 0;
}

int main(int argc, char** argv)
{
    xDots = 5;
    yDots = 5;
    if(Init() != 0)
    {
        return -1;
    }

    currentPlayer = RED;
    box boxes[(yDots+1)*(xDots+1)];
    for(int y = 0; y <= yDots; y++)
    {
        for(int x = 0; x <= xDots; x++)
        {
            boxes[(y*xDots)+x].top = EMPTY;
            boxes[(y*xDots)+x].right = EMPTY;
            boxes[(y*xDots)+x].bottum = EMPTY;
            boxes[(y*xDots)+x].left = EMPTY;
            boxes[(y*xDots)+x].owner = EMPTY;
        }
    }

    SDL_Event event;
    int quit = 0;
    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    quit = 1;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    DrawLine(event.button.x, event.button.y, boxes);
                    break;

                default: {}
            }
        }
        Render(boxes);
        if(checkWin(boxes))
        {
            printf("RED: %d       BLUE: %d\n", redBoxes, blueBoxes);
            if(redBoxes > blueBoxes)
            {
                printf("RED WON!!\n");
            }
            else if(blueBoxes > redBoxes)
            {
                printf("BLUE WON!!\n");
            }
            else
            {
                printf("ITS A DRAW.\n");
            }
            quit = 1;
        }
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
