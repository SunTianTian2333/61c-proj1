#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t* state, int x, int y);
static void set_board_at(game_state_t* state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t* state, int snum);
static char next_square(game_state_t* state, int snum);
static void update_tail(game_state_t* state, int snum);
static void update_head(game_state_t* state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t* state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t* state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.
  game_state_t* state=(game_state_t *)malloc(sizeof(game_state_t));
  assert(state);
  
  state->x_size=14;
  state->y_size=10;
  
  state->num_snakes=1;
  state->snakes=(snake_t *)malloc(sizeof(snake_t)*state->num_snakes);
  assert(state->snakes);
  state->snakes->tail_x=4;
  state->snakes->tail_y=4;
  state->snakes->head_x=5;
  state->snakes->head_y=4;
  state->snakes->live=true;
  
  state->board=(char **)malloc(sizeof(char*)*state->y_size);
  assert(state->board);
  for(int i=0;i<state->y_size;i++){
     state->board[i]=(char*)malloc(sizeof(char)*state->x_size);
     assert(state->board[i]);
     for(int j=0;j<state->x_size;j++){
        if(i==0||i==state->y_size-1)
          state->board[i][j]='#';
          else if(j==0||j==state->x_size-1)
               state->board[i][j]='#';
               else state->board[i][j]=' ';
    } 
    }
    state->board[4][4]='d';
    state->board[4][5]='>';
    state->board[2][9]='*';
  return state;

}

/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
  for(int i=0;i<state->y_size;i++){
     free(state->board[i]);
  }
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
  for(int i=0;i<state->y_size;i++){
     for(int j=0;j<state->x_size;j++){
        fprintf(fp,"%c",state->board[i][j]);
     }
        fprintf(fp,"\n");
     }
     
  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
  // TODO: Implement this function.
  if(c=='w'||c=='a'||c=='s'||c=='d')
     return true;
     else return false;
}

static bool is_snake(char c) {
  // TODO: Implement this function.
  if(is_tail(c))
     return true;
  else if(c=='^'||c=='<'||c=='>'||c=='v'||c=='x')
       return true;
  else return false;
}

static char body_to_tail(char c) {
  // TODO: Implement this function.
  if(c=='^')
    return 'w';
  if(c=='<')
    return 'a';
  if(c=='>')
    return 'd';
  if(c=='v')
    return 's';
  else
       return '?';
}

static int incr_x(char c) {
  // TODO: Implement this function.
  if(c=='>'||c=='d')
    return 1;
  if(c=='<'||c=='a')
    return -1;
  return 0;
}

static int incr_y(char c) {
  // TODO: Implement this function.
  if(c=='v'||c=='s')
    return 1;
  if(c=='w'||c=='^')
    return -1;
  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int num=-1;
  char t='?';
  for(int y=0;y<state->y_size;y++)
     for(int x=0;x<state->x_size;x++){
         if(is_tail(get_board_at(state,x,y)))
            num++;
         if(num==snum){
         for(int i=0;i<state->num_snakes;i++){
           t=get_board_at(state,x,y);
           if(state->snakes[i].tail_x==x&&y==state->snakes[i].tail_y){
                t=get_board_at(state,state->snakes[i].head_x,state->snakes[i].head_y);
                if(incr_x(t)==1){
                   t=state->board[state->snakes[i].head_y][state->snakes[i].head_x+1];
                   }
                else if(incr_x(t)==-1){
                   t=state->board[state->snakes[i].head_y][state->snakes[i].head_x-1];
                   }
                if(incr_y(t)==1){
                   t=state->board[state->snakes[i].head_y+1][state->snakes[i].head_x];
                   }
                else if(incr_y(t)==-1){
                   t=state->board[state->snakes[i].head_y-1][state->snakes[i].head_x];
                   }
                
                  
                return t;
               }
            
         }              
     }     
}
return t;
}
/* Task 4.3 */
static void update_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int num=-1;
  char t='?';
  for(int y=0;y<state->y_size;y++)
     for(int x=0;x<state->x_size;x++){
         if(is_tail(get_board_at(state,x,y)))
            num++;
         if(num==snum){
           for(int i=0;i<state->num_snakes;i++)     //pipei
           {
              if(state->snakes[i].tail_y==y&&state->snakes[i].tail_x==x){
                   t=get_board_at(state,state->snakes[i].head_x,state->snakes[i].head_y);
      
                   if(incr_x(t)==1){
                       if(is_snake(next_square(state,num))||next_square(state,num)=='#'){
                          state->board[state->snakes[i].head_y][state->snakes[i].head_x]='x';
                          state->snakes[i].live=false;
                       }else{
                       state->board[state->snakes[i].head_y][state->snakes[i].head_x+1]=t;
                       state->snakes[i].head_x+=1;
                       }
                   }
                   else if(incr_x(t)==-1){
                       if(is_snake(next_square(state,num))||next_square(state,num)=='#'){
                          state->board[state->snakes[i].head_y][state->snakes[i].head_x]='x';
                          state->snakes[i].live=false;
                          }
                       else{
                       state->board[state->snakes[i].head_y][state->snakes[i].head_x-1]=t;
                       state->snakes[i].head_x-=1;
                       }
                   }
                   if(incr_y(t)==1){
                       if(is_snake(next_square(state,num))||next_square(state,num)=='#'){
                          state->board[state->snakes[i].head_y][state->snakes[i].head_x]='x';
                          state->snakes[i].live=false;
                          }
                       else{   
                       state->board[state->snakes[i].head_y+1][state->snakes[i].head_x]=t;
                       state->snakes[i].head_y+=1;
                       }
                   }
                   else if(incr_y(t)==-1){
                       if(is_snake(next_square(state,num))||next_square(state,num)=='#'){
                          state->board[state->snakes[i].head_y][state->snakes[i].head_x]='x';
                          state->snakes[i].live=false;
                          }
                       else {   
                       state->board[state->snakes[i].head_y-1][state->snakes[i].head_x]=t;
                       state->snakes[i].head_y-=1;
                       }
                   }          
                   
             }  
          }
         } 
    }    
  return;
}

/* Task 4.4 */
static void update_tail(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int num=-1;
  char t='?';
  for(int y=0;y<state->y_size;y++)
     for(int x=0;x<state->x_size;x++){
         if(is_tail(get_board_at(state,x,y)))
            num++;
         if(num==snum){
            for(int i=0;i<state->num_snakes;i++)
               {
                  if(state->snakes[i].tail_y==y&&state->snakes[i].tail_x==x){
                     t=get_board_at(state,x,y);
                     if(incr_x(t)==1){
                       state->board[y][x]=' ';
                       if(state->board[y][x+1]=='>')
                          state->board[y][x+1]='d';
                       if(state->board[y][x+1]=='^')
                          state->board[y][x+1]='w';
                       if(state->board[y][x+1]=='<')
                          state->board[y][x+1]='a';
                       if(state->board[y][x+1]=='v')
                          state->board[y][x+1]='s';
                       state->snakes[i].tail_x++;
                      }
                     if(incr_x(t)==-1){
                       state->board[y][x]=' ';
                       if(state->board[y][x-1]=='>')
                          state->board[y][x-1]='d';
                       if(state->board[y][x-1]=='^')
                          state->board[y][x-1]='w';
                       if(state->board[y][x-1]=='<')
                          state->board[y][x-1]='a';
                       if(state->board[y][x-1]=='v')
                          state->board[y][x-1]='s';
                       state->snakes[i].tail_x--;
                      }
                     if(incr_y(t)==1){
                       state->board[y][x]=' ';
                       if(state->board[y+1][x]=='>')
                          state->board[y+1][x]='d';
                       if(state->board[y+1][x]=='^')
                          state->board[y+1][x]='w';
                       if(state->board[y+1][x]=='<')
                          state->board[y+1][x]='a';
                       if(state->board[y+1][x]=='v')
                          state->board[y+1][x]='s';
                       state->snakes[i].tail_y++;
                      }
                     if(incr_y(t)==-1){
                       state->board[y][x]=' ';
                       if(state->board[y-1][x]=='>')
                          state->board[y-1][x]='d';
                       if(state->board[y-1][x]=='^')
                          state->board[y-1][x]='w';
                       if(state->board[y-1][x]=='<')
                          state->board[y-1][x]='a';
                       if(state->board[y-1][x]=='v')
                          state->board[y-1][x]='s';
                       state->snakes[i].tail_y--;
                      }
                   }
              }
          }
         } 
        
  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  int num=4;
  for(int i=0;i<num;i++){
      char t =next_square(state,i);
      if(t=='*'){
        update_head(state,i);
        add_food(state);
      }
      else if(is_snake(t)||t=='#')
             update_head(state,i);
      else {
      update_head(state,i);
      update_tail(state,i);
      }
              
  }            
  return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implement this function.
  FILE *fp=fopen(filename,"r");
  int num1=0;
  int num2=0;
  int num3=0;
  char temp='?';
  while(fscanf(fp,"%c",&temp)!=EOF){
       if(temp!='\n')
         num1++;
       else{
       num3=num1;
       num1=0;
       num2++;
       }
  }
  fclose(fp);
  fp=fopen(filename,"r");
  
  game_state_t *state=(game_state_t *)malloc(sizeof(game_state_t)); 
  state->x_size=num3;
  state->y_size=num2;
  state->board=(char **)malloc(sizeof(char*)*state->y_size);
  char buf;
  for(int i=0;i<state->y_size;i++){
      state->board[i]=(char *)malloc(sizeof(char)*state->x_size);
      for(int k=0;k<state->x_size;k++){
          buf=fgetc(fp);
          if(buf=='\n')
            buf=fgetc(fp);
          state->board[i][k]=buf;
          }
  }
  fclose(fp);
  return state;
}

/* Task 6.1 */
static void find_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  for(int y=0;y<state->y_size;y++)
     for(int x=0;x<state->x_size;x++)
         if(is_tail(get_board_at(state,x,y)))
            for(int i=0;i<state->num_snakes;i++)
                if(state->snakes[i].tail_x==x&&state->snakes[i].tail_y==y){
                  char t=get_board_at(state,x,y);
                      while(is_snake(t)){
                           if(incr_x(t)==1){
                            t=get_board_at(state,++x,y);
                            if(!is_snake(t)){
                               --x;
                               break;
                               }
                            }
                           else if(incr_x(t)==-1){
                            t=get_board_at(state,--x,y);
                            if(!is_snake(t)){
                               ++x;
                               break;
                               }
                            }
                           else if(incr_y(t)==1){
                            t=get_board_at(state,x,++y);
                            if(!is_snake(t)){
                               --y;
                               break;
                               }
                            }
                           else if(incr_y(t)==-1){
                            t=get_board_at(state,x,--y);  
                            if(!is_snake(t)){
                               ++y;
                               break;
                               }
                            }
                        }
                  state->snakes[i].head_x=x;
                  state->snakes[i].head_y=y;
                 }
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  state->snakes=(snake_t*)malloc(sizeof(snake_t)*4);
  
  int num=-1;
  for(int y=0;y<state->y_size;y++)
     for(int x=0;x<state->x_size;x++){
         if(is_tail(get_board_at(state,x,y))){
            num++;
            state->snakes[num].tail_x=x;
            state->snakes[num].tail_y=y;
          }
     }
   state->num_snakes=num+1;
  return state;
}
















