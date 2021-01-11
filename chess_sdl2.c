#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

struct bak {
  int board_bak[10][11];
  int turn_bak;
  struct bak* next;
};

int main(int argc, char** arcv)
{
  //定义棋盘并放置棋子
  int board[10][11] = {
    [5][1]=1, [5][10]=-1,
    [4][1]=2, [6][1]=2, [4][10]=-2, [6][10]=-2,
    [3][1]=3, [7][1]=3, [3][10]=-3, [7][10]=-3,
    [2][1]=4, [8][1]=4, [2][10]=-4, [8][10]=-4,
    [1][1]=5, [9][1]=5, [1][10]=-5, [9][10]=-5,
    [2][3]=6, [8][3]=6, [2][8]=-6, [8][8]=-6,
    [1][4]=7, [3][4]=7, [5][4]=7, [7][4]=7, [9][4]=7,
    [1][7]=-7, [3][7]=-7, [5][7]=-7, [7][7]=-7, [9][7]=-7,
  };

  struct bak* latest_bak = NULL;
  struct bak* new_bak = NULL;

  //初始化
  SDL_Init( SDL_INIT_EVERYTHING );
  TTF_Init();
  //定义基本变量
  SDL_Window* screen_window = SDL_CreateWindow( "Chinese Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 450, 600,  SDL_WINDOW_SHOWN);
  SDL_Surface* screen = SDL_GetWindowSurface( screen_window );
  TTF_Font* font = TTF_OpenFont( "default.ttf", 32);
  SDL_Color red = { 220, 0, 0 };
  SDL_Color black = { 0, 0, 0 };
  SDL_Color white = { 255, 255, 255};
  SDL_Rect offset_bg;
  SDL_Rect offset_chess;
  SDL_Rect offset_msg;
  SDL_Rect offset_highlight;
  SDL_Rect offset_load_back;
  SDL_Event event;
  //定义基本图片
  SDL_Surface* chessboard = IMG_Load( "chessboard.jpg" );
  SDL_Surface* chessbg = IMG_Load( "chessbg.png" );
  SDL_Surface* chessselect = IMG_Load( "chessselect.png" );
  //定义棋子
  SDL_Surface* red_shuai   = TTF_RenderUTF8_Solid( font, "帅", red);
  SDL_Surface* black_jiang = TTF_RenderUTF8_Solid( font, "将", black );
  SDL_Surface* red_shi     = TTF_RenderUTF8_Solid( font, "仕", red);
  SDL_Surface* black_shi   = TTF_RenderUTF8_Solid( font, "士", black);
  SDL_Surface* red_xiang   = TTF_RenderUTF8_Solid( font, "相", red);
  SDL_Surface* black_xiang = TTF_RenderUTF8_Solid( font, "象", black);
  SDL_Surface* red_ma      = TTF_RenderUTF8_Solid( font, "马", red);
  SDL_Surface* black_ma    = TTF_RenderUTF8_Solid( font, "马", black);
  SDL_Surface* red_che     = TTF_RenderUTF8_Solid( font, "车", red);
  SDL_Surface* black_che   = TTF_RenderUTF8_Solid( font, "车", black);
  SDL_Surface* red_pao     = TTF_RenderUTF8_Solid( font, "炮", red);
  SDL_Surface* black_pao   = TTF_RenderUTF8_Solid( font, "炮", black);
  SDL_Surface* red_bing    = TTF_RenderUTF8_Solid( font, "兵", red);
  SDL_Surface* black_zu    = TTF_RenderUTF8_Solid( font, "卒", black);

  //定义其它文字
  SDL_Surface* red_turn    = TTF_RenderUTF8_Solid( font, "红方走子", red);
  SDL_Surface* black_turn  = TTF_RenderUTF8_Solid( font, "黑方走子", black);
  SDL_Surface* red_win     = TTF_RenderUTF8_Solid( font, "红方胜利！", red);
  SDL_Surface* black_win   = TTF_RenderUTF8_Solid( font, "黑方胜利！", black);
  SDL_Surface* load_back   = TTF_RenderUTF8_Solid( font, "悔棋", black);

  offset_msg.x = 90;
  offset_msg.y = 530;
  offset_load_back.x = 350;
  offset_load_back.y = 530;
  
  int turn = 1;//记录是红方回合还是黑方回合
  
  //-------------------------------------------------------------------------------------------
  //下面一个循环执行一次是一个回合
  //-------------------------------------------------------------------------------------------
  int win = 0;//胜负
  
  for(;;)
    {
    Turn_start:
      win = 0;
      //-----------------------------------------------------------------
      //图形输出
      //-----------------------------------------------------------------
      SDL_BlitSurface( chessboard, NULL, screen, NULL);
      for(int i = 10; i >= 1; i--)
        {
          for(int j = 1; j <= 9; j++)
            {
              offset_bg.x = -45 + j*50;
              offset_bg.y = 505-50*i;
              offset_chess.x = -42 + j*50;
              offset_chess.y = 506 - 50*i;
                
              if(board[j][i] != 0)
                SDL_BlitSurface( chessbg, NULL, screen, &offset_bg);
              switch (board[j][i])
                {
                case  1: SDL_BlitSurface(   red_shuai, NULL, screen, &offset_chess);break;
                case -1: SDL_BlitSurface( black_jiang, NULL, screen, &offset_chess);break;
                case  2: SDL_BlitSurface(     red_shi, NULL, screen, &offset_chess);break;
                case -2: SDL_BlitSurface(   black_shi, NULL, screen, &offset_chess);break;
                case  3: SDL_BlitSurface(   red_xiang, NULL, screen, &offset_chess);break;
                case -3: SDL_BlitSurface( black_xiang, NULL, screen, &offset_chess);break;
                case  4: SDL_BlitSurface(      red_ma, NULL, screen, &offset_chess);break;
                case -4: SDL_BlitSurface(    black_ma, NULL, screen, &offset_chess);break;
                case  5: SDL_BlitSurface(     red_che, NULL, screen, &offset_chess);break;
                case -5: SDL_BlitSurface(   black_che, NULL, screen, &offset_chess);break;
                case  6: SDL_BlitSurface(     red_pao, NULL, screen, &offset_chess);break;
                case -6: SDL_BlitSurface(   black_pao, NULL, screen, &offset_chess);break;
                case  7: SDL_BlitSurface(    red_bing, NULL, screen, &offset_chess);break;
                case -7: SDL_BlitSurface(    black_zu, NULL, screen, &offset_chess);break;
                }
            }
        }
      SDL_UpdateWindowSurface( screen_window );

      //---------------------------------------------------------------------------------------
      //胜负
      //---------------------------------------------------------------------------------------

      for(int i = 1; i < 11; i++)
        {
          for(int j = 1; j < 10; j++)
            {
              if(board[j][i] == 1)
                win = win + 1;
              if(board[j][i] == -1)
                win = win - 1;
            }
        }
      
      if(win == 1)
        {
          SDL_BlitSurface( red_win, NULL, screen, &offset_msg);
          SDL_UpdateWindowSurface( screen_window );
          for(;;)
            {
              while( SDL_PollEvent( &event ))
                {
                  if( event.type == SDL_QUIT )
                    {
                      SDL_DestroyWindow( screen_window );
                      SDL_Quit();
                      return 0;
                    }
                }
            }
        }
      if(win == -1)
        {
          SDL_BlitSurface( black_win, NULL, screen, &offset_msg);
          SDL_UpdateWindowSurface( screen_window );
          for(;;)
            {
              while( SDL_PollEvent( &event ))
                {
                  if( event.type == SDL_QUIT )
                    {
                      SDL_DestroyWindow( screen_window );
                      SDL_Quit();
                      return 0;
                    }
                }
            }
        }
      
      //--------------------------------------------------------------------------------
      //下面开始输入程序
      //--------------------------------------------------------------------------------

      //起步位置与目地位置
      int j_from = 0;
      int i_from = 0;
      int j_to = 0;
      int i_to = 0;
      int input_get = 0;
      int distance = 0;
      if(turn > 0)
        SDL_BlitSurface( red_turn, NULL, screen, &offset_msg);
      else
        SDL_BlitSurface( black_turn, NULL, screen, &offset_msg);
      SDL_BlitSurface( load_back, NULL, screen, &offset_load_back );
      SDL_UpdateWindowSurface( screen_window );
      
    In_put_from:
      input_get = 0;
      
      while(!input_get)
        {
          while(SDL_PollEvent( &event ))
            {
              if( event.type == SDL_MOUSEBUTTONDOWN )
                {
                  j_from = (int)( event.button.x/50 + 1 );
                  i_from = (int)( 10 - event.button.y/50 );
                  //printf("\n%d\n%d\n%d\n%d", j_from, i_from, event.button.x, event.button.y);
                  input_get = 1;
                }
              if( event.type == SDL_QUIT )
                {
                  SDL_Quit();
                  return 0;
                }
            }
        }
      //启用悔棋
      if( j_from >= 7 && i_from < 1 )
        {
          new_bak = latest_bak;
          latest_bak = (*latest_bak).next;
          memcpy( board, (*new_bak).board_bak, sizeof( board ));
          turn = (*new_bak).turn_bak;
          free(new_bak);
          new_bak = NULL;
          goto Turn_start;
        }
         
      //判断输入是否合法
      if(j_from < 1 || i_from < 1 || j_from > 9 || i_from > 10)
        goto In_put_from;
      else if(board[j_from][i_from]*turn <= 0)
        goto In_put_from;

      //高亮选中的棋子
      offset_highlight.x = -50 + 50*j_from;
      offset_highlight.y = 500 - 50*i_from;
      SDL_BlitSurface( chessselect, NULL, screen, &offset_highlight);
      SDL_UpdateWindowSurface( screen_window );
      
    In_put_to:

      input_get = 0;
      while(!input_get)
        {
          while(SDL_PollEvent( &event ))
            {
              if( event.type == SDL_MOUSEBUTTONDOWN )
                {
                  j_to = (int)( event.button.x/50 + 1 );
                  i_to = (int)( 10 - event.button.y/50 );
                  //printf("\n%d\n%d\n%d\n%d", j_to, i_to, event.button.x, event.button.y);
                  input_get = 1;
                }
              if( event.type == SDL_QUIT )
                {
                  SDL_Quit();
                  return 0;
                }
            }
        }
      //启用悔棋
      if( j_to >= 7 && i_to < 1 )
        {
          new_bak = latest_bak;
          latest_bak = (*latest_bak).next;
          memcpy( board, (*new_bak).board_bak, sizeof( board ));
          turn = (*new_bak).turn_bak;
          free(new_bak);
          new_bak = NULL;
          goto Turn_start;
        }
      
      //取消选中
      if(j_to == j_from && i_to == i_from)
        goto Turn_start;
      //判断移动是否合法
      if(j_to < 1 || i_to < 1 || j_to > 9 || i_to > 10)//是否在棋盘内
        goto In_put_to;
      else if(board[j_to][i_to]*turn > 0)//是否有己方的子
        goto In_put_to;
      switch (abs(board[j_from][i_from]))
        {
        case  1://帅的走棋------------------------------------------------------
          {
            if( j_to == j_from && board[j_to][i_to] == -1*turn )
              {
                //printf("\nkiller 1\n");

                for(int flag = i_from + turn ; turn*flag < turn*(i_to - turn) ; flag = flag + turn)
                  {
                    //printf("\n%d\n%d\n", j_from, flag);
                    if(board[j_from][flag] != 0 )
                      {
                        //printf("\nYou Dead!!!!!!!!!!!!\n");
                        goto In_put_to;
                      }
                  }
                break;
              }
            else
              {
                //printf("\nkiller 2\n");
                if( (i_to != i_from && j_to != j_from) || abs(j_to-j_from) > 1 || abs(i_to-i_from) > 1 )
                  goto In_put_to;
                if( j_to < 4 || j_to > 6 || turn*i_to > turn*(int)(5.5-turn*2.5) )
                  goto In_put_to;
                break;
              }
          }
        case  2://仕的走棋------------------------------------------------------
          {
            if( !(((j_to == 4 || j_to == 6) && (i_to == (int)(5.5-turn*4.5) || i_to == (int)(5.5-turn*2.5))) || ( j_to == 5 && i_to == (int)(5.5-turn*3.5))))
              goto In_put_to;
            if( abs(j_to-j_from) > 1 || abs(i_to-i_from) > 1)
              goto In_put_to;
            break;
          }
        case  3://相的走棋-------------------------------------------------------
          {
            if( turn*i_to > turn*(int)(5.5-turn*0.5) )
              goto In_put_to;
            if( abs(j_to-j_from) != 2 || abs(i_to-i_from) != 2 )
              goto In_put_to;
            break;
          }
        case  4://马的走棋-------------------------------------------------------
          {
            if( abs(j_to-j_from) == 2 && abs(i_to-i_from) == 1)
              {
                if(board[(j_to+j_from)/2][i_from] != 0)
                  goto In_put_to;
              }
            if( abs(j_to-j_from) == 1 && abs(i_to-i_from) == 2)
              {
                if(board[j_from][(i_to+i_from)/2] != 0)
                  goto In_put_to;
              }
            if(!((abs(j_to-j_from) == 2 && abs(i_to-i_from) == 1) || (abs(j_to-j_from) == 1 && abs(i_to-i_from) == 2 )))
              goto In_put_to;
            break;
          }
        case  5://车的走棋--------------------------------------------------------
          {
            if( j_to != j_from && i_to != i_from )
              goto In_put_to;
            distance = 0;
            if( j_to == j_from )
              {
                distance = (i_to-i_from)/abs(i_to-i_from);
                for(int flag = i_from ; distance*flag < distance*(i_to - distance) ; flag = flag + distance)
                  {
                    if(board[j_from][flag] != 0 && flag != i_from)
                      goto In_put_to;
                  }
              }
            if( i_to == i_from )
              {
                distance = (j_to-j_from)/abs(j_to-j_from);
                for(int flag = j_from ; distance*flag < distance*(j_to - distance) ; flag = flag + distance)
                  {
                    if(board[flag][i_from] != 0 && flag != j_from)
                      goto In_put_to;
                  }
              }
            break;
          }
        case  6://炮的走棋-------------------------------------------------------
          {
            if( j_to != j_from && i_to != i_from )
              goto In_put_to;
            int distance = 0;
            int chesses = 0;
            if( j_to == j_from )
              {
                distance = (i_to-i_from)/abs(i_to-i_from);
                for(int flag = i_from ; distance*flag <= distance*(i_to - distance) ; flag = flag + distance)
                  {
                    if(board[j_from][flag] != 0 && flag != i_from)
                      chesses = chesses + 1;
                  }
              }
            if( i_to == i_from )
              {
                distance = (j_to-j_from)/abs(j_to-j_from);
                for(int flag = j_from ; distance*flag <= distance*(j_to - distance) ; flag = flag + distance)
                  {
                    if(board[flag][i_from] != 0 && flag != j_from)
                      chesses = chesses + 1;
                  }
              }
            //printf("%d\n",chesses);
            if( chesses > 1 )
              goto In_put_to;
            if( chesses == 1 && board[j_to][i_to] == 0 )
              goto In_put_to;
            if( chesses < 1 && turn*board[j_to][i_to] < 0 )
              goto In_put_to;
            break;
          }
        case  7://兵的走棋-----------------------------------------------------------
          {
            if( turn*i_from <= turn*(int)(5.5-0.5*turn))
              {
                if( i_to - i_from != turn || j_to != j_from )
                  goto In_put_to;
              }
            if( turn*i_from >= turn*(int)(5.5+0.5*turn))
              {
                if( (i_to != i_from && j_to != j_from) || abs(j_to-j_from) > 1 || abs(i_to-i_from) > 1 || turn*(i_to-i_from) < 0 )
                  {
                    //printf("\n%d\n%d\n%d\n%d\n", j_to, i_to, j_from, i_from);
                    goto In_put_to;
                  }
              }
            break;
          }
        }
      
      //备份现状以供悔棋
      new_bak = malloc( sizeof( struct bak ) );
      (*new_bak).next = latest_bak;
      memcpy( (*new_bak).board_bak, board, sizeof(board) );
      (*new_bak).turn_bak = turn;
      latest_bak = new_bak;
      new_bak = NULL;

      //移动棋子
      board[j_to][i_to] = board[j_from][i_from];
      board[j_from][i_from] = 0;

      //回合刷新
      turn = -1*turn;
          
    }

}









