#include "syscalls.h"
#include "stdio.h"

char modname[] __attribute__((section(".modname"), aligned(4))) = "snek";

struct int_vec2 {
    int x;
    int y;
};

struct vec_2_node {
    struct int_vec2 pos;
    struct vec_2_node *next; 
};

static struct int_vec2 snake_pos = {1, 1};
static struct int_vec2 grid_top_left_cnr = {5,5};
static struct int_vec2 direction = {1,0}; // move to the right by default
static struct vec_2_node fruits= { {7,3}, 0};

static int width = 50;
static int height = 15;
long int time_elapsed = 0;
char current_dir = 0; // 0 = right, 1= down, 2 = left, 3 = up

void draw_snake(int prev_x, int prev_y){
    // draw the head of the snake first
    write_char_at(' ', prev_x + grid_top_left_cnr.x, prev_y + grid_top_left_cnr.y);
    write_char_at('O', snake_pos.x + grid_top_left_cnr.x, snake_pos.y + grid_top_left_cnr.y);
}

void draw_fruits(){
    struct vec_2_node* iter = &fruits;
    while(iter != 0){
        write_char_at('*', iter->pos.x + grid_top_left_cnr.x, iter->pos.y + grid_top_left_cnr.y);
        iter = iter->next;
    }
}

void handle_input(){
   char input = get();
   if(input == 'w'){
        direction.x = 0;
        direction.y = -1;
   }else if(input == 's'){
        direction.x =0;
        direction.y = 1;
   }else if(input == 'a'){
        direction.x = -1;
        direction.y = 0;
   }else if(input == 'd'){
        direction.x = 1;
        direction.y = 0;
   }
}

int main(){
    update_echo(0);
    reset_screen();
    draw_grid();
    draw_snake(snake_pos.x, snake_pos.y);
    while(1){
        time_elapsed++;
        if(time_elapsed > 100000000){
            handle_input();
            int prev_x = snake_pos.x;
            int prev_y = snake_pos.y;
            snake_pos.x += direction.x;
            snake_pos.y += direction.y;
            draw_snake(prev_x, prev_y);
            draw_fruits();
            time_elapsed = 0;
        }
    }
}


void wait(int iterations){
    for(int i=0; i<iterations; i++){}
}

void draw_grid(){
    for(int i=0; i<width; i++){
        for(int j = 0; j<height;j++){
            if(i == 0 || j == 0 || i== width-1 || j ==height-1){
                write_char_at('#', grid_top_left_cnr.x + i, grid_top_left_cnr.y + j);
            }else{
                write_char_at(' ', grid_top_left_cnr.x + i, grid_top_left_cnr.y + j);
            }
        }
    }
}