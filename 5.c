#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

#define OBS_UP 0
#define OBS_DOWN 1
#define OBS_LEFT 2
#define OBS_RIGHT 3

#define LENGTH 60
#define WIDTH 20

char key;
char board[WIDTH][LENGTH];
int obs_rate = 10;

typedef struct coordinate
{
    int x;
    int y;
    
}  Coordinate;

typedef struct object
{
    Coordinate position;
    char Object;
    
} Object;

typedef struct obstacle
{
    Coordinate position;
    char Object;
    int direction;
    
} Obstacle;

void gotoXY(int X, int Y);
void Board(Object *B);
void update_object_pos(Object *U, int colour_val, char Object);
int Move(Object *A);
void colour_object(WORD colour);
int Produce_Obstacles();
int Score();
int CheckCollision(Object *player, Obstacle *obstacle);
const char* auto_control();
const char* manual_control();
void record();

Object player;

int main()
{
    
    system("cls");
    
    
    char control_type;
    printf("WELCOME TO MY FIRST C PROGRAM!\n\n");
    printf("It is a simple DODGING game!\n\n");
    printf("What type of game controls would you like?\n");
    printf("Press 'A' for automatic, 'M' for manual, or any other key to exit: ");
    scanf("%c",&control_type);
    
    
        if (control_type == 'A')
        {
            system("cls");
            auto_control();
        }
        else if (control_type == 'M')
        {
            system("cls");        
            manual_control();
        }
        else 
        {
            exit(0);
        }
        
}

void gotoXY(int X, int Y)
{
    HANDLE handle;
    COORD coord;
    fflush(stdout);
    coord.X = X;
    coord.Y = Y;
    handle  = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle, coord);
}

void Board(Object *B)
{
    int y = 0;
    
    while (y < WIDTH)
    {
        int x = 0;
        
        while(x < LENGTH)
        {
            if (x == 0 && y == 0)
            {
                board[y][x] = 218;
            }
            else if (x == 0 && y == 19)
            {
                board[y][x] = 192;
            }
            else if (x == 59 && y == 0)
            {
                board[y][x] = 191;
            }      
            else if (x == 59 && y == 19)
            {
                board[y][x] = 217;
            }              
            else if (y == 0 || y == 19)
            {
                board[y][x] = 196;
            }
            else if (x == 0 || x == 59)
            {
                board[y][x] = 179;
            }
            else if (x == B->position.x && y == B->position.y)
            {
                colour_object(12);
                board[y][x] = B->Object;
            }
            else 
            {
                colour_object(7);
                board[y][x] = ' ';
            }
            printf("%c", board[y][x]);             
            x++;
        }
        printf("\n");
        y++;
    }
    
    
}    

void update_object_pos(Object *U, int colour_val, char Object)
{
    gotoXY(U->position.x, U->position.y);
    colour_object(colour_val);
    printf("%c",Object);
}

int Move(Object *A)
{
    if(A->position.x > 0 && A->position.x < 59 && A->position.y > 0 && A->position.y < 19)
    {
        
        if (key==RIGHT)
        {
            update_object_pos(A, 7, ' ');
            A->position.x++;
            update_object_pos(A, 12, A->Object);    
            return 0;
        }
        else if (key==LEFT)
        {
            update_object_pos(A, 7, ' ');       
            A->position.x--;
            update_object_pos(A, 12, A->Object); 
            return 0;            
        }
        else if (key==UP)
        {
            update_object_pos(A, 7, ' ');      
            A->position.y--;
            update_object_pos(A, 12, A->Object);  
            return 0;       
        }
        else if (key==DOWN)
        {
            update_object_pos(A, 7, ' ');        
            A->position.y++;
            update_object_pos(A, 12, A->Object); 
            return 0;            
        }
    }
    
    else {
        
        return 1;
        
    }
}

void colour_object(WORD colour)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, colour);
}

int Produce_Obstacles()
{
    static Obstacle obstacles[1000];
    static int obstacleCount = 0;
    int collision = 0;
    
    if(rand() % 500 < obs_rate)
    {
        Obstacle newObstacle;
        int spawn_direction = rand() % 4;
        
        //Top border
        if (spawn_direction == 0)
        {
            newObstacle.position.x = (rand() % 58) + 1;
            newObstacle.position.y = 1;
            newObstacle.direction  = OBS_DOWN;
        }
        //Bottom border
        else if (spawn_direction == 1)
        {
            newObstacle.position.x = (rand() % 58) + 1;
            newObstacle.position.y = 18;
            newObstacle.direction  = OBS_UP;
        }
       //Right border
       else if (spawn_direction == 2)
        {
            newObstacle.position.x = 58;
            newObstacle.position.y = (rand() % 18) + 1;
            newObstacle.direction  = OBS_LEFT;
        }
       //Left border        
       else if (spawn_direction == 3)
        {
            newObstacle.position.x = 1;
            newObstacle.position.y = (rand() % 18) + 1;
            newObstacle.direction  = OBS_RIGHT;
        }        
       
        obstacles[obstacleCount] = newObstacle;
        obstacleCount += 1;  
   }
   
   int i = 0;
   while (i < obstacleCount)
   {
       
       Obstacle *obstacle = &obstacles[i];
       gotoXY(obstacle->position.x, obstacle->position.y);
       colour_object(7);
       printf(" ");
       
       if(CheckCollision(&player,obstacle))
       {
           collision = 1;
           break;
       }
       
       //remove obstacles
       if(obstacle->direction == OBS_UP)
       {
           obstacle->position.y--;
           if(obstacle->position.y <= 1)
           {
               obstacles[i] = obstacles[obstacleCount - 1];
               obstacleCount--;
               Score();
               continue;
           }
           update_object_pos(obstacle, 7, 248);
           
       }
       
       else if(obstacle->direction == OBS_DOWN)
       {
           obstacle->position.y++;
           if(obstacle->position.y >= 18)
           {
               obstacles[i] = obstacles[obstacleCount - 1];
               obstacleCount--;
               Score();
               continue;
           }
           update_object_pos(obstacle, 7, 248);
     
       }      
       
       else if(obstacle->direction == OBS_LEFT)
       {
           obstacle->position.x--;
           if(obstacle->position.x <= 1)
           {
               obstacles[i] = obstacles[obstacleCount - 1];
               obstacleCount--;
               Score();
               continue;
           }
           update_object_pos(obstacle, 7, 248);
          
       } 
           
       else if(obstacle->direction == OBS_RIGHT)
       {
           obstacle->position.x++;
           if(obstacle->position.x >= 58)
           {
               obstacles[i] = obstacles[obstacleCount - 1];
               obstacleCount--;
               Score();
               continue;
           }
           update_object_pos(obstacle, 7, 248);
          
       } 
       
       update_object_pos(obstacle, 7, 248);
       i++; 
   }
   return collision;
}

int Score()
{
    static int count = 0;
    count++;
    gotoXY(64,3);
    printf("Score: %6d",count);
    return count;
}

int CheckCollision(Object *player, Obstacle *obstacle)
{
    if(player->position.x == obstacle->position.x && player->position.y == obstacle->position.y)
    {
        return 1;
    }
    return 0;
}

const char* auto_control() {
    
    srand(time(NULL));
    
    player.position.x = 30;
    player.position.y = 10;
    player.Object     = 254;
    Board(&player);
    gotoXY(64,3);
    printf("Score:      0");
    char* gametype = "Automatic";

    
    while (1)
    { 
        
        Sleep(100);
    
        int collision = Produce_Obstacles();
        
        
        if (kbhit())
        {
            key = getch();
        }
        
        if (Move(&player) == 1 || collision == 1)
        {
            gotoXY(25,10);
            colour_object(7);
            printf("GAME OVER");
            Sleep(3000);
            break;
        }
                    
        obs_rate += 1;
    }
    
    record(gametype);
    
}

const char* manual_control() {
    srand(time(NULL));
    
    player.position.x = 30;
    player.position.y = 10;
    player.Object     = 254;
    Board(&player);
    gotoXY(64,3);
    printf("Score:      0");
    char* gametype = "Manual";

    
    while (1)
    { 
        
        Sleep(100);
    
        int collision = Produce_Obstacles();
        
        
        if (kbhit())
        {
            key = getch();
            if (Move(&player) == 1)
            {
                gotoXY(25,10);
                colour_object(7);
                printf("GAME OVER");
                Sleep(3000);
                break;
            }
        }
        
        if (collision == 1)
        {
            gotoXY(25,10);
            colour_object(7);
            printf("GAME OVER");
            Sleep(3000);
            break;
        }
                    
        obs_rate += 1;
    }
    
    record(gametype);
}

void record(const char* gametype)
{
    char inputname[30], outputname[30], c;
    int final_score;
    FILE *RECORD;
    RECORD = fopen("game_record.txt","a+");
    system("cls");
    printf("What is your name?\n");
    scanf(" %[^\n]", inputname);
    
    int a = 0;
    while (inputname[a] != '\0')
    {
        outputname[0] = toupper(inputname[0]);
        if(inputname[a-1] == ' ')
        {
            outputname[a] = toupper(inputname[a]);
            outputname[a-1] = inputname[a-1];
        }
        else
        {
            outputname[a] = inputname[a];
        }
        a++;
    }
    outputname[a] = '\0';
    
    time_t current_time;
    current_time = time(NULL);
    
    fprintf(RECORD, "Name: | %s\n", outputname);
    fprintf(RECORD, "Played Date: | %s", ctime(&current_time));
    fprintf(RECORD, "Game Type: | %s\n", gametype);
    fprintf(RECORD, "Score: | %d\n\n\n", final_score = Score());
    
    fclose(RECORD);
    
    RECORD = fopen("Game_Record.txt", "r");
    c      = getc(RECORD);
    
    while (c != EOF)
    {
        putchar(c);
        c = getc(RECORD);
    }
    
    fclose(RECORD);
     
}
   
    
