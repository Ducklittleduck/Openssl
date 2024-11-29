#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define SIZE 15
#define CHARSIZE 3
#define MAXLINE 1000
void initRecordBorard(void);
void innerLayoutToDisplayArray(void);
void displayBoard(void);
int scan_inputxy(int *input1, int *input2);
int check_out(int Intinp1, int Intinp2);
int check_repd(int Intinp1, int Intinp2);
int player1wins(int Intinp1, int Intinp2);
void display_newone(int Intinp1, int Intinp2);
int player2wins(int Intinp1, int Intinp2);
int sq_ai(int color, int *input1, int *input2);
int check_forbidden(int color, int Intinp1, int Intinp2);
void DetType(int color, int x, int y);
void CalcuPoint(int color, int x, int y);
void CalcuInfo(int color, int x, int y);
void ResetScore(void);
void ResetType(void);
int SameScoreCheckPosition(int p1x, int p1y, int p2x, int p2y);

struct SinDirInfo{//单方向得分信息
    int linkNum; //连珠数量
    int oppNum; //两端对手棋子的数量   
};
    
struct PointScore{//计算单点得分
    struct SinDirInfo info[4];//记录四方向信息
    int score;//给当前点评分
};

struct PointScore myBoardScore[15][15];//记录每点得分

int live[2][6];//记录活x数量
int half[2][6];//记录半活x数量
int dead[2][6];//记录死x数量


//棋盘使用的是UTF8编码，每一个中文字符占用3个字节。

//空棋盘模板 
char arrayForEmptyBoard[SIZE][SIZE*CHARSIZE+1] = 
{
		"┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"
};
//此数组存储用于显示的棋盘 
char arrayForDisplayBoard[SIZE][SIZE*CHARSIZE+1];
 
char play1Pic[]="●";//黑棋子;
char play1CurrentPic[]="▲"; 

char play2Pic[]="◎";//白棋子;
char play2CurrentPic[]="△";

//此数组用于记录当前的棋盘的格局 
int arrayForInnerBoardLayout[SIZE][SIZE];

int main(){

    initRecordBorard();    //初始化一个空棋盘  
    innerLayoutToDisplayArray();
    displayBoard();
    int CurrentPlayer = 1;
    int Intinp1, Intinp2;
    int *input1;
    int *input2;
    int win = 0;
    input1 = &Intinp1;
    input2 = &Intinp2;
    int mode;
    printf("The author is Gao Siqi\n");
    printf("Please choose one mode to play:\n1 for pvp\n2 for pve\n");//选择模式
	scanf("%d", &mode);

    if(mode == 1){//人人对战
        while(1){
            if(CurrentPlayer == 1){
                printf("Player1 please enter your chess(enter 'quit' can quit this game):\n");
                if(scan_inputxy(input1, input2) == -1){
                    break;
                }else if(check_out(Intinp1, Intinp2) == 1){
                    printf("Your input has exceeded the maximum range of the board, please re-enter\n");
                }else if(check_repd(Intinp1, Intinp2) == 1){
                    printf("Your entries are duplicated, please re-enter\n");
                }else if(check_forbidden(1, Intinp1, Intinp2) == 1){
                    printf("You chose a forbidden position! You lose!\n");
                    return 0;
                }else{
                    //printf("score = %d\n", myBoardScore[Intinp1][Intinp2].score);
                    //sleep(1);
                    ResetScore();
                    ResetType();
                    CalcuInfo(1, Intinp1, Intinp2);
                    DetType(1, Intinp1, Intinp2);
                    CalcuPoint(1, Intinp1, Intinp2);
                    CalcuInfo(2, Intinp1, Intinp2);
                    DetType(2, Intinp1, Intinp2);
                    CalcuPoint(2, Intinp1, Intinp2);
                    //printf("score = %d\n", myBoardScore[Intinp1][Intinp2].score);
                    //sleep(1);

                    arrayForInnerBoardLayout[Intinp1][Intinp2]=1;
                
                    innerLayoutToDisplayArray();  //将心中的棋盘转成用于显示的棋盘
                    display_newone(Intinp1, Intinp2);
                    displayBoard();          //显示棋盘
                    win = player1wins(Intinp1, Intinp2);
                    if (win == 1){
				        printf("Player1 wins!\n");
				        return 0;
			        }
                    
                    CurrentPlayer = 2;
                }
            }else{
                printf("Player2 please enter your chess:(enter 'quit' can quit this game)\n");
                if(scan_inputxy(input1, input2) == -1){
                    break;
                }else if(check_out(Intinp1, Intinp2) == 1){
                    printf("Your input has exceeded the maximum range of the board, please re-enter\n");
                }else if(check_repd(Intinp1, Intinp2) == 1){
                    printf("Your entries are duplicated, please re-enter\n");
                }else{

                    //printf("score = %d\n", myBoardScore[Intinp1][Intinp2].score);
                    //sleep(1);
                    ResetScore();
                    ResetType();
                    CalcuInfo(2, Intinp1, Intinp2);
                    DetType(2, Intinp1, Intinp2);
                    CalcuPoint(2, Intinp1, Intinp2);
                    CalcuInfo(1, Intinp1, Intinp2);
                    DetType(1, Intinp1, Intinp2);
                    CalcuPoint(1, Intinp1, Intinp2);
                    //printf("score = %d\n", myBoardScore[Intinp1][Intinp2].score);
                    //sleep(1);
                    arrayForInnerBoardLayout[Intinp1][Intinp2]=2;
                
                    innerLayoutToDisplayArray();  //将心中的棋盘转成用于显示的棋盘 
                    display_newone(Intinp1, Intinp2);
                    displayBoard();          //显示棋盘 
                    win = player2wins(Intinp1, Intinp2);
                    if (win == 1){
				        printf("Player2 wins!\n");
				        return 0;
			        }
                
                    CurrentPlayer = 1;
                }   
            }
        }
    }else if(mode == 2){//人机对战
        int color;
        printf("Please choose your color:\n1 for black\n2 for white\n");
        scanf("%d", &color);
        if(color == 1){
            while(1){
                if(CurrentPlayer == 1){
                    printf("Please enter your chess(enter 'quit' can quit this game):\n");
                    if(scan_inputxy(input1, input2) == -1){
                        break;
                    }else if(check_out(Intinp1, Intinp2) == 1){
                        printf("Your input has exceeded the maximum range of the board, please re-enter\n");
                    }else if(check_repd(Intinp1, Intinp2) == 1){
                        printf("Your entries are duplicated, please re-enter\n");
                    }else if(check_forbidden(1, Intinp1, Intinp2) == 1){
                        printf("You chose a forbidden position! You lose!\n");
                        return 0;
                    }else{
                        arrayForInnerBoardLayout[Intinp1][Intinp2]=1;

                        innerLayoutToDisplayArray();  //将心中的棋盘转成用于显示的棋盘
                        display_newone(Intinp1, Intinp2);
                        displayBoard();          //显示棋盘
                        win = player1wins(Intinp1, Intinp2);
                        if (win == 1){
				            printf("You win!\n");
				            return 0;
			            }
                
                        CurrentPlayer = 2;
                    }
                }else{
                    sq_ai(2, input1, input2);
                    arrayForInnerBoardLayout[Intinp1][Intinp2]=2;
                
                    innerLayoutToDisplayArray();  //将心中的棋盘转成用于显示的棋盘 
                    display_newone(Intinp1, Intinp2);
                    displayBoard();          //显示棋盘 
                    win = player2wins(Intinp1, Intinp2);
                    if (win == 1){
				        printf("You lose!\n");
				        return 0;
			        }
                    //printf("Intinp1 = %d, Intinp2 = %d\n", Intinp1, Intinp2);
                    printf("The Position is:%d, %c\n", (15 - Intinp1), ('A' + Intinp2));
                
                    CurrentPlayer = 1;
                }   
            }
        }else if(color == 2){
            while(1){
                if(CurrentPlayer == 1){
                    sq_ai(1, input1, input2);
                    //printf("Intinp1 = %d, Intinp2 = %d\n", Intinp1, Intinp2);
                    arrayForInnerBoardLayout[Intinp1][Intinp2]=1;
                
                    innerLayoutToDisplayArray();  //将心中的棋盘转成用于显示的棋盘 
                    display_newone(Intinp1, Intinp2);
                    displayBoard();          //显示棋盘 
                    win = player1wins(Intinp1, Intinp2);
                    if (win == 1){
				        printf("You lose!\n");
				        return 0;
			        }
                    //printf("Intinp1 = %d, Intinp2 = %d\n", Intinp1, Intinp2);
                    printf("The Position is:%d, %c\n", (15 - Intinp1), ('A' + Intinp2));
                    CurrentPlayer = 2;
                }else{
                    printf("Please enter your chess(enter 'quit' can quit this game):\n");
                    if(scan_inputxy(input1, input2) == -1){
                        break;
                    }else if(check_out(Intinp1, Intinp2) == 1){
                        printf("Your input has exceeded the maximum range of the board, please re-enter\n");
                    }else if(check_repd(Intinp1, Intinp2) == 1){
                        printf("Your entries are duplicated, please re-enter\n");
                    }else{
                        arrayForInnerBoardLayout[Intinp1][Intinp2]=2;

                        innerLayoutToDisplayArray();  //将心中的棋盘转成用于显示的棋盘
                        display_newone(Intinp1, Intinp2);
                        displayBoard();          //显示棋盘
                        win = player2wins(Intinp1, Intinp2);
                        if (win == 1){
				            printf("You win!\n");
				            return 0;
			            }
                
                        CurrentPlayer = 1;
                    }
                }   
            }
        }else{//选择报错
            printf("error: please enter '1' or '2' to choose\n");
        }
    
        return 0;
    }
}

//禁手
int check_forbidden(int color, int Intinp1, int Intinp2){
    ResetType();
    CalcuInfo(color, Intinp1, Intinp2);
    DetType(color, Intinp1, Intinp2);
    if(myBoardScore[Intinp1][Intinp2].info[0].linkNum > 5 || myBoardScore[Intinp1][Intinp2].info[1].linkNum > 5 
    || myBoardScore[Intinp1][Intinp2].info[2].linkNum > 5 || myBoardScore[Intinp1][Intinp2].info[3].linkNum > 5){//判断长连
        //printf("forbidden: have a long!\n");
        //sleep(1);
        return 1;
    }else if(live[color - 1][3] >= 2){//判断双三
        //printf("forbidden: have 2 live3!\n");
        //sleep(1);
        return 1;
    }else if(live[color - 1][4] + half[color - 1][4] >= 2){//判断双四
        //printf("forbidden: have 2 live4!\n");
        //sleep(1);
        return 1;
    }
    return 0;//没有违反禁手
}

//计算落子位置
int sq_ai(int color, int *input1, int *input2){
    /*int i = 0;  //这里是一个非常笨笨的大猩猩
    int j = 0;
    for(i = 0; i <= 14; i++){
        for(j = 0; j <= 14; j++){
            if(arrayForInnerBoardLayout[i][j] == 0){
                *input1 = i;
                *input2 = j;
                return 0;
            }
        }
    }*/
    int i, j;
    int x = 0;
    int y = 0;
    int score = 0;
    ResetScore();//清空分数
    ResetType();//清空类型记录
    
    for(i = 0; i <= 14; i++){
        for(j = 0; j <= 14; j++){
            
            if(arrayForInnerBoardLayout[i][j] == 0){
                CalcuInfo(color, i, j);
                DetType(color, i, j);
                CalcuPoint(color, i, j);
                CalcuInfo(3 - color, i, j);
                DetType(3 - color, i, j);
                CalcuPoint(3 - color, i, j);
                if(myBoardScore[i][j].score > score){
                    if(color == 1){
                        if(check_forbidden(color, i, j) == 1){
                            ;
                        }else{
                            score = myBoardScore[i][j].score;
                            x = i;
                            y = j;
                        }
                    }else{
                        score = myBoardScore[i][j].score;
                        x = i;
                        y = j;
                    }
                }else if(myBoardScore[i][j].score == score){
                    if(SameScoreCheckPosition(x, y, i, j) == 2){
                        if(check_forbidden(color, i, j) == 1){
                            ;
                        }else{
                            score = myBoardScore[i][j].score;
                            x = i;
                            y = j;
                        }
                    }
                }
                ResetScore();//清空分数
                ResetType();//清空类型记录
            }
        }
    }
    //printf("x = %d\ny = %d\n", x, y);
    //sleep(1);
    /*if(color == 1){
        if(check_forbidden(color, x, y) == 1){
        arrayForInnerBoardLayout[x][y] == 3;
        sq_ai(color, input1, input2);
        arrayForInnerBoardLayout[x][y] == 0;
        }
    }*/
    
    //printf("score = %d\n", score);
    //sleep(1);
    *input1 = x;
    *input2 = y;
    
    return 0;
}

//同分数判断更优位置
int SameScoreCheckPosition(int p1x, int p1y, int p2x, int p2y){
    int abu_1x = 0;
    int abu_1y = 0;
    int abu_2x = 0;
    int abu_2y = 0;
    int abu_1 = 0;
    int abu_2 = 0;

    abu_1x = p1x - 7;
    abu_1y = p1y - 7;
    abu_2x = p2x - 7;
    abu_2y = p2y - 7;

    if(abu_1x < 0){
        abu_1x *= -1;
    }
    if(abu_1y < 0){
        abu_1y *= -1;
    }
    if(abu_2x < 0){
        abu_2x *= -1;
    }
    if(abu_2y < 0){
        abu_2y *= -1;
    }

    abu_1 = abu_1x + abu_1y;
    abu_2 = abu_2x + abu_2y;

    if((abu_1) > (abu_2)){
        return 2;
    }else{
        return 1;
    }

}

//清空类型记录的函数
void ResetType(void){
    int i, j;
    for(i = 0; i < 2; i++){
        for(j = 0; j < 6; j++){
            live[i][j] = 0;//活x数量
            half[i][j] = 0;//半活x数量
            dead[i][j] = 0;//死x数量

        }
    }
}

//清空分数的函数
void ResetScore(void){
    int i, j;
    for(i = 0; i < 15; i++){
        for(j = 0; j <= 15; j++){
            myBoardScore[i][j].score = 0;
        }
    }
}

//判断连子类型的函数
void DetType(int color, int x, int y){
    int i = 0;
    int j = 0;
    i = myBoardScore[x][y].info[0].linkNum;
    j = myBoardScore[x][y].info[0].oppNum;

    if(j == 0){
        if(i > 5){
            live[color - 1][5]++;
        }else{
            live[color - 1][i]++;
        }
    }else if(j == 1){
        if(i > 5){
            half[color - 1][5]++;
        }else{
            half[color - 1][i]++;
        }
    }else if(j == 2){
        if(i > 5){
            dead[color - 1][5]++;
        }else{
            dead[color - 1][i]++;
        }
    }

    i = myBoardScore[x][y].info[1].linkNum;
    j = myBoardScore[x][y].info[1].oppNum;

    if(j == 0){
        if(i > 5){
            live[color - 1][5]++;
        }else{
            live[color - 1][i]++;
        }
    }else if(j == 1){
        if(i > 5){
            half[color - 1][5]++;
        }else{
            half[color - 1][i]++;
        }
    }else if(j == 2){
        if(i > 5){
            dead[color - 1][5]++;
        }else{
            dead[color - 1][i]++;
        }
    }

    i = myBoardScore[x][y].info[2].linkNum;
    j = myBoardScore[x][y].info[2].oppNum;

    if(j == 0){
        if(i > 5){
            live[color - 1][5]++;
        }else{
            live[color - 1][i]++;
        }
    }else if(j == 1){
        if(i > 5){
            half[color - 1][5]++;
        }else{
            half[color - 1][i]++;
        }
    }else if(j == 2){
        if(i > 5){
            dead[color - 1][5]++;
        }else{
            dead[color - 1][i]++;
        }
    }

    i = myBoardScore[x][y].info[3].linkNum;
    j = myBoardScore[x][y].info[3].oppNum;

    if(j == 0){
        if(i > 5){
            live[color - 1][5]++;
        }else{
            live[color - 1][i]++;
        }
    }else if(j == 1){
        if(i > 5){
            half[color - 1][5]++;
        }else{
            half[color - 1][i]++;
        }
    }else if(j == 2){
        if(i > 5){
            dead[color - 1][5]++;
        }else{
            dead[color - 1][i]++;
        }
    }
}

//计算分数的函数
void CalcuPoint(int color, int x, int y){
    int point = 0;
    if(live[color - 1][5] >= 1 || half[color - 1][5] >= 1 || dead[color - 1][5] >= 1){
        point = 100000;
    }else if(live[color - 1][4] >= 1){
        point = 10000 * live[color - 1][4];
    }else if(half[color - 1][4] >= 1){
        point = 1000 * half[color - 1][4];
    }else if(dead[color - 1][4] >= 1){
        point = 200 * dead[color - 1][4];
    }else if(live[color - 1][3] >= 1){
        point = 1000 * live[color - 1][3];
    }else if(half[color - 1][3] >= 1){
        point = 500 * half[color - 1][3];
    }else if(dead[color - 1][3] >= 1){
        point = 400 * dead[color - 1][3];
    }else if(live[color - 1][2] >= 1){
        point = 300 * live[color - 1][2];
    }else if(half[color - 1][2] >= 1){
        point = 200 * half[color - 1][2];
    }else if(dead[color - 1][2] >= 1){
        point = 100 * dead[color - 1][2];
    }else if(live[color - 1][1] >= 1){
        point = 50 * live[color - 1][1];
    }else if(half[color - 1][1] >= 1){
        point = 30 * half[color - 1][1];
    }else if(dead[color - 1][1] >= 1){
        point = 10 * dead[color - 1][1];
    }
    myBoardScore[x][y].score = myBoardScore[x][y].score + point;
}

//计算每点信息的函数
void CalcuInfo(int color, int x, int y){  
    int i, j;
    //计算左连
    int left = 0; 
    //计算左堵
    int left_opp = 0;
    arrayForInnerBoardLayout[x][y] = color;
    for(i = x, j = y; i <= 14 && i >= 0 && j <= 14 && j >= 0;){
        if(arrayForInnerBoardLayout[i][j] == color){
            left++;
        }else if(arrayForInnerBoardLayout[i][j] != 0){
            left_opp++;
            break;
        }else{
            break;
        }
        j--;
        if(j < 0){
            left_opp++;
        }
    }
    arrayForInnerBoardLayout[x][y] = 0;

    //计算右连
    int right = 0; 
    //计算右堵
    int right_opp = 0;
    arrayForInnerBoardLayout[x][y] = color;
    for(i = x, j = y; i <= 14 && i >= 0 && j <= 14 && j >= 0;){
        if(arrayForInnerBoardLayout[i][j] == color){
            right++;
        }else if(arrayForInnerBoardLayout[i][j] != 0){
            right_opp++;
            break;
        }else{
            break;
        }
        j++;
        if(j > 14){
            right_opp++;
        }
    }
    arrayForInnerBoardLayout[x][y] = 0;
    int hor = 0;
    int hor_opp = 0;
    hor = left + right - 1;
    hor_opp = left_opp + right_opp;
    myBoardScore[x][y].info[0].linkNum = hor;
    myBoardScore[x][y].info[0].oppNum = hor_opp;
    
    //计算上连
    int up = 0; 
    //计算上堵
    int up_opp = 0;
    arrayForInnerBoardLayout[x][y] = color;
    for(i = x, j = y; i <= 14 && i >= 0 && j <= 14 && j >= 0;){
        if(arrayForInnerBoardLayout[i][j] == color){
            up++;
        }else if(arrayForInnerBoardLayout[i][j] != 0){
            up_opp++;
            break;
        }else{
            break;
        }
        i--;
        if(i < 0){
            up_opp++;
        }
    }
    arrayForInnerBoardLayout[x][y] = 0;
    //计算下连
    int down = 0; 
    //计算下堵
    int down_opp = 0;
    arrayForInnerBoardLayout[x][y] = color;
    for(i = x, j = y; i <= 14 && i >= 0 && j <= 14 && j >= 0;){
        if(arrayForInnerBoardLayout[i][j] == color){
            down++;
        }else if(arrayForInnerBoardLayout[i][j] != 0){
            down_opp++;
            break;
        }else{
            break;
        }
        i++;
        if(i > 14){
            down_opp++;
        }
    }
    arrayForInnerBoardLayout[x][y] = 0;
    int ver = 0;
    int ver_opp = 0;
    ver = up + down - 1;
    ver_opp = up_opp + down_opp;
    myBoardScore[x][y].info[1].linkNum = ver;
    myBoardScore[x][y].info[1].oppNum = ver_opp;

    //计算右上连
    int rightup = 0; 
    //计算右上堵
    int rightup_opp = 0;
    arrayForInnerBoardLayout[x][y] = color;
    for(i = x, j = y; i <= 14 && i >= 0 && j <= 14 && j >= 0;){
        if(arrayForInnerBoardLayout[i][j] == color){
            rightup++;
        }else if(arrayForInnerBoardLayout[i][j] != 0){
            rightup_opp++;
            break;
        }else{
            break;
        }
        i--;
        j++;
        if(i < 0 || j > 14){
            rightup_opp++;
        }
    }
    arrayForInnerBoardLayout[x][y] = 0;
    //计算左下连
    int leftdown = 0; 
    //计算左下堵
    int leftdown_opp = 0;
    arrayForInnerBoardLayout[x][y] = color;
    for(i = x, j = y; i <= 14 && i >= 0 && j <= 14 && j >= 0;){
        if(arrayForInnerBoardLayout[i][j] == color){
            leftdown++;
        }else if(arrayForInnerBoardLayout[i][j] != 0){
            leftdown_opp++;
            break;
        }else{
            break;
        }
        i++;
        j--;
        if(i > 14 || j < 0){
            leftdown_opp++;
        }
    }
    arrayForInnerBoardLayout[x][y] = 0;
    int ru_ld = 0;
    int ru_ld_opp = 0;
    ru_ld = rightup + leftdown - 1;
    ru_ld_opp = rightup_opp + leftdown_opp;
    myBoardScore[x][y].info[2].linkNum = ru_ld;
    myBoardScore[x][y].info[2].oppNum = ru_ld_opp;

    //计算左上连
    int leftup = 0; 
    //计算左上堵
    int leftup_opp = 0;
    arrayForInnerBoardLayout[x][y] = color;
    for(i = x, j = y; i <= 14 && i >= 0 && j <= 14 && j >= 0;){
        if(arrayForInnerBoardLayout[i][j] == color){
            leftup++;
        }else if(arrayForInnerBoardLayout[i][j] != 0){
            leftup_opp++;
            break;
        }else{
            break;
        }
        i--;
        j--;
        if(i < 0 || j < 0){
            leftup_opp++;
        }
    }
    arrayForInnerBoardLayout[x][y] = 0;
    //计算右下连
    int rightdown = 0; 
    //计算右下堵
    int rightdown_opp = 0;
    arrayForInnerBoardLayout[x][y] = color;
    for(i = x, j = y; i <= 14 && i >= 0 && j <= 14 && j >= 0;){
        if(arrayForInnerBoardLayout[i][j] == color){
            rightdown++;
        }else if(arrayForInnerBoardLayout[i][j] != 0){
            rightdown_opp++;
            break;
        }else{
            break;
        }
        i++;
        j++;
        if(i > 14 || j > 14){
            rightdown_opp++;
        }
    }
    arrayForInnerBoardLayout[x][y] = 0;
    int lu_rd = 0;
    int lu_rd_opp = 0;
    lu_rd = leftup + rightdown - 1;
    lu_rd_opp = leftup_opp + rightdown_opp;
    myBoardScore[x][y].info[3].linkNum = lu_rd;
    myBoardScore[x][y].info[3].oppNum = lu_rd_opp;
}


//胜负判断1
int player1wins(int Intinp1, int Intinp2){
    int left = 0;
    int right = 0;
    int up = 0;
    int down = 0;
    int rightup = 0;
    int rightdown = 0;
    int leftdown = 0;
    int leftup = 0;
    int i, j;
    //判断左
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; j--){
        if(arrayForInnerBoardLayout[i][j] == 1){
            left++;
        }else{
            break;
        }
    }
    //判断右
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; j++){
        if(arrayForInnerBoardLayout[i][j] == 1){
            right++;
        }else{
            break;
        }
    }
    //判断上
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; i--){
        if(arrayForInnerBoardLayout[i][j] == 1){
            up++;
        }else{
            break;
        }
    }
    //判断下
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; i++){
        if(arrayForInnerBoardLayout[i][j] == 1){
            down++;
        }else{
            break;
        }
    }
    //判断左下
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; i++, j--){
        if(arrayForInnerBoardLayout[i][j] == 1){
            leftdown++;
        }else{
            break;
        }
    }
    //判断右上
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; i--, j++){
        if(arrayForInnerBoardLayout[i][j] == 1){
            rightup++;
        }else{
            break;
        }
    }
    //判断左上
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; i--, j--){
        if(arrayForInnerBoardLayout[i][j] == 1){
            leftup++;
        }else{
            break;
        }
    }
    //判断右下
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; i++, j++){
        if(arrayForInnerBoardLayout[i][j] == 1){
            rightdown++;
        }else{
            break;
        }
    }
    if(left + right == 6){
		return 1;
	}
	if(up + down == 6){
		return 1;
	}
	if(leftup + rightdown == 6){
		return 1;
	}
	if(rightup + leftdown == 6){
		return 1;
	}
    else{
        return 0;
    }
}

//胜负判断2
int player2wins(int Intinp1, int Intinp2){
    int left = 0;
    int right = 0;
    int up = 0;
    int down = 0;
    int rightup = 0;
    int rightdown = 0;
    int leftdown = 0;
    int leftup = 0;
    int i, j;
    //判断左
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; j--){
        if(arrayForInnerBoardLayout[i][j] == 2){
            left++;
        }else{
            break;
        }
    }
    //判断右
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; j++){
        if(arrayForInnerBoardLayout[i][j] == 2){
            right++;
        }else{
            break;
        }
    }
    //判断上
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; i--){
        if(arrayForInnerBoardLayout[i][j] == 2){
            up++;
        }else{
            break;
        }
    }
    //判断下
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; i++){
        if(arrayForInnerBoardLayout[i][j] == 2){
            down++;
        }else{
            break;
        }
    }
    //判断左下
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; i++, j--){
        if(arrayForInnerBoardLayout[i][j] == 2){
            leftdown++;
        }else{
            break;
        }
    }
    //判断右上
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; i--, j++){
        if(arrayForInnerBoardLayout[i][j] == 2){
            rightup++;
        }else{
            break;
        }
    }
    //判断左上
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; i--, j--){
        if(arrayForInnerBoardLayout[i][j] == 2){
            leftup++;
        }else{
            break;
        }
    }
    //判断右下
    for(i = Intinp1, j = Intinp2; i <= 14 && i >= 0 && j <= 14 && j >= 0; i++, j++){
        if(arrayForInnerBoardLayout[i][j] == 2){
            rightdown++;
        }else{
            break;
        }
    }
    if(left + right == 6){
		return 1;
	}
	if(up + down == 6){
		return 1;
	}
	if(leftup + rightdown == 6){
		return 1;
	}
	if(rightup + leftdown == 6){
		return 1;
	}
    else{
        return 0;
    }
}

//显示新的落子
void display_newone(int Intinp1, int Intinp2){
    if(arrayForInnerBoardLayout[Intinp1][Intinp2] == 1){
        arrayForDisplayBoard[Intinp1][CHARSIZE*Intinp2] = play1CurrentPic[0];
        arrayForDisplayBoard[Intinp1][CHARSIZE*Intinp2+1] = play1CurrentPic[1];
        if(CHARSIZE == 3){
            arrayForDisplayBoard[Intinp1][CHARSIZE*Intinp2+2] = play1CurrentPic[2];
        }
    }else if(arrayForInnerBoardLayout[Intinp1][Intinp2] == 2){
        arrayForDisplayBoard[Intinp1][CHARSIZE*Intinp2] = play2CurrentPic[0];
        arrayForDisplayBoard[Intinp1][CHARSIZE*Intinp2+1] = play2CurrentPic[1];
        if(CHARSIZE == 3){
            arrayForDisplayBoard[Intinp1][CHARSIZE*Intinp2+2] = play2CurrentPic[2];
        }
    }
}

//检查坐标有没有重复的函数
int check_repd(int Intinp1, int Intinp2){
    int i, j;
    for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++){
            if(arrayForInnerBoardLayout[i][j] != 0){
                if(Intinp1 == i && Intinp2 == j){
                    return 1;
                }
            }
        }
    }
    return 0;
}


//检查坐标有没有越界的函数
int check_out(int Intinp1, int Intinp2){
    if(Intinp1 >= 15 || Intinp1 < 0 || Intinp2 > 15 || Intinp2 < 0){
        return 1;
    }else{
        return 0;
    }
}

//读入坐标数据的函数
int scan_inputxy(int *input1, int *input2){
    char inputxy[MAXLINE];
    int i = 0;
    int inp1 = 0;
    int inp2 = 0;
    scanf(" %s", inputxy);
    if(inputxy[0] == 'q' && inputxy[1] == 'u' && inputxy[2] == 'i' && inputxy[3] == 't'){
        return -1;//检查是不是quit
    }
    for(i = 0; inputxy[i] != '\0'; i++){//判断输入
        if((inputxy[i] <= '9') && (inputxy[i] >= '0')){
            inp1 = 10 * inp1 + (inputxy[i] - '0');
        }else if((inputxy[i] <= 'z') && (inputxy[i] >= 'a')){
            inp2 = inputxy[i] - 'a';
        }else if((inputxy[i] <= 'Z') && (inputxy[i] >= 'A')){
            inp2 = inputxy[i] - 'A';
        }else{
            ;
        }
    }
    inp1 = 15 - inp1;
    *input1 = inp1;
    *input2 = inp2;
    return 1;
}


//初始化一个空棋盘格局 
void initRecordBorard(void){
	//通过双重循环，将arrayForInnerBoardLayout清0
    int i, j;
    for(i = 0; i < SIZE; i++){
        for(j = 0; j <= SIZE*CHARSIZE+1; j++){
            arrayForInnerBoardLayout[i][j] = 0;
        }
    }
}

//将arrayForInnerBoardLayout中记录的棋子位置，转化到arrayForDisplayBoard中
void innerLayoutToDisplayArray(void){
	//第一步：将arrayForEmptyBoard中记录的空棋盘，复制到arrayForDisplayBoard中
        int i, j;
        for(i = 0; i < SIZE; i++){
            for(j = 0; j <= SIZE*CHARSIZE+1; j++)
                arrayForDisplayBoard[i][j] = arrayForEmptyBoard[i][j];
        }
	//第二步：扫描arrayForInnerBoardLayout，当遇到非0的元素，将●或者◎复制到arrayForDisplayBoard的相应位置上
	//注意：arrayForDisplayBoard所记录的字符是中文字符，每个字符占2个字节。●和◎也是中文字符，每个也占2个字节。
        for(i = 0; i < SIZE; i++){
            for(j = 0; j < SIZE; j++){
                if(arrayForInnerBoardLayout[i][j] == 1){
                    arrayForDisplayBoard[i][CHARSIZE*j] = play1Pic[0];
                    arrayForDisplayBoard[i][CHARSIZE*j+1] = play1Pic[1];
                    if(CHARSIZE == 3){
                        arrayForDisplayBoard[i][CHARSIZE*j+2] = play1Pic[2];
                    }
                }else if(arrayForInnerBoardLayout[i][j] == 2){
                    arrayForDisplayBoard[i][CHARSIZE*j] = play2Pic[0];
                    arrayForDisplayBoard[i][CHARSIZE*j+1] = play2Pic[1];
                    if(CHARSIZE == 3){
                        arrayForDisplayBoard[i][CHARSIZE*j+2] = play2Pic[2];
                    }
                }
            }
        }
}


//显示棋盘格局 
void displayBoard(void){
	int i;
	//第一步：清屏
	system("clear");   //清屏  
	//第二步：将arrayForDisplayBoard输出到屏幕上
        for(i = 0; i < SIZE; i++){
            printf("%3d %s\n", SIZE - i, arrayForDisplayBoard[i]);
        }
	

	//第三步：输出最下面的一行字母A B .... 
    printf("    ");
    for(i = 0; i < SIZE; i++){
        printf("%2c", 'A' + i);
    }
    printf("\n");

} 
