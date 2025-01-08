#include "syscalls.h"
#include "stdio.h"
#include "malloc.h"

char modname[] __attribute__((section(".modname"), aligned(4))) = "snek";

struct int_vec2 {
    int x;
    int y;
};

struct vec_2_node {
    struct int_vec2 pos;
    struct vec_2_node *next; 
};
static unsigned int score = 1;
static struct int_vec2 snake_pos = {1, 1};
static struct int_vec2 grid_top_left_cnr = {15,5};
static struct int_vec2 direction = {1,0}; // move to the right by default
static struct vec_2_node fruits= { {7,3}, 0};
static unsigned int high_score = 1;

static struct vec_2_node* body = 1;
static unsigned int state = 10;

static int width = 50;
static int height = 15;
long int time_elapsed = 0;
char current_dir = 0; // 0 = right, 1= down, 2 = left, 3 = up

char lbs1[] = "Score: ";
char lbs2[] = "  High Score: ";

void print_dec_uint(unsigned int num, int column, int row){
    if(num == 0){
        write_char_at('0', column, row);
        return;
    }
    char decimal_digits[] = "0123456789";
    unsigned int length = 0;
    unsigned int cpy = num;

    while(cpy != 0){
        cpy/=10;
        length++;
    }
    char *dec_string = malloc(length + 1);
    dec_string[length] = '\0';
    for (int i = 0; i < length; i++)
    {
        dec_string[length-i-1] = decimal_digits[num % 10];
        num/=10;
    }
    for(int i=0; i<length; i++){
        write_char_at(dec_string[i], column+i, row);
    }
}

void draw_snake(int prev_x, int prev_y){
    // draw the head of the snake first
    write_char_at(' ', prev_x + grid_top_left_cnr.x, prev_y + grid_top_left_cnr.y);
    write_char_at('O', snake_pos.x + grid_top_left_cnr.x, snake_pos.y + grid_top_left_cnr.y);
}

void draw_score_board(){
   for(int i=0; i<sizeof(lbs1); i++){
        write_char_at(lbs1[i], 1+i, 1);
   }
   print_dec_uint(score, sizeof(lbs1), 1);

   for(int i=0; i<sizeof(lbs2); i++){
        write_char_at(lbs2[i], 1+sizeof(lbs1)+i, 1);
   }
   print_dec_uint(high_score, sizeof(lbs1) + sizeof(lbs2), 1);
}

unsigned int random() {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void incr_score(){
   score++; 
   draw_score_board();
}

void draw_fruits(){
    struct vec_2_node* iter = &fruits;
    while(iter != 0){
        write_char_at('$', iter->pos.x + grid_top_left_cnr.x, iter->pos.y + grid_top_left_cnr.y);
        if((iter->pos.x + grid_top_left_cnr.x == snake_pos.x + grid_top_left_cnr.x) && (iter->pos.y + grid_top_left_cnr.y == snake_pos.y + grid_top_left_cnr.y)){
            incr_score();
            //just update the position of the current fruit
            fruits.pos.x = random()%(width-1);
            fruits.pos.y = random()%(height-1) + 1;
            add_segment(); 
        }
        iter = iter->next;
    }
}

void add_segment(){
    struct vec_2_node* new_segment = (struct vec_2_node *)malloc(sizeof( struct vec_2_node));
    if(body == 1){
        // no body segment exists currently
        body = new_segment;
        new_segment->next = 1;
    }
    else{
        // add the segment to the beginning of the list
        new_segment->next = body;
        body = new_segment;
    }
}

void update_segments(){
    struct vec_2_node* iter = body;
    if(iter == 1){return;}
    while(iter->next != 1){
        // first clear the previous one
        if(iter->pos.x != 0){
            write_char_at(' ', iter->pos.x + grid_top_left_cnr.x, iter->pos.y + grid_top_left_cnr.y);
        }
        iter->pos = iter->next->pos;
        iter = iter->next;
    }if(iter != 1){
        if(iter->pos.x != 0){
            write_char_at(' ', iter->pos.x + grid_top_left_cnr.x, iter->pos.y + grid_top_left_cnr.y);
        }
        iter->pos = snake_pos;
    }
}

void draw_segments(){
    struct vec_2_node* iter = body;
    while(iter != 1){
        if(iter->pos.x != 0){
            // we have a segment in probation
            write_char_at('*', iter->pos.x + grid_top_left_cnr.x, iter->pos.y + grid_top_left_cnr.y);
        }
        iter = iter->next;
    }
}

void handle_input(){
   char input = get();
   if(input == 'q'){
    
        exit();
   }
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

void int32_to_hex_string(int num, char* hexString){
    hexString[0] = '0';
    hexString[1] = 'x';
    const char hexDigits[] = "0123456789ABCDEF";
    const int bitMask = 0xF;
    for(int i = 0; i <= 7; i++){
         hexString[9-i] = hexDigits[(num >> i*4) & bitMask];
    }
}

void printHexInt(int num){
    char str[] = "           ";
    int32_to_hex_string(num, str);
    print(str);
}

void reset_game(){
    if(score > high_score){
        high_score = score;
    }
    score = 0;
    reset_screen();
    draw_score_board();
    draw_grid();
    snake_pos.x = 1;
    snake_pos.y = 1;
    direction.x = 1;
    direction.y = 0;
    state = 10;

    //free the body memory and then set body back to default
    body = 1;
}

void check_endgame(){
    if(snake_pos.x == 0 || snake_pos.y == 0 || snake_pos.x == width || snake_pos.y == height){
        reset_game();
    }
    struct vec_2_node* iter = body;
    while(iter != 1){
        if(iter->pos.x == snake_pos.x && iter->pos.y == snake_pos.y){
            reset_game();
            return;
        }
        iter = iter->next;
    }
}

int main(){
    update_echo(0);
    reset_screen();
    draw_grid();
    draw_score_board();
    draw_snake(snake_pos.x, snake_pos.y);
    while(1){
        time_elapsed++;
        if(time_elapsed > 100000000){
            handle_input();
            update_segments();
            // move the snakes head
            int prev_x = snake_pos.x;
            int prev_y = snake_pos.y;
            snake_pos.x += direction.x;
            snake_pos.y += direction.y;
            draw_fruits();
            draw_snake(prev_x, prev_y);
            draw_segments();
            check_endgame();
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