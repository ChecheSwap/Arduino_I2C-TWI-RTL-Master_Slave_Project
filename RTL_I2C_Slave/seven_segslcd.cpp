//@CHECHESWAP JESUS JOSE NAVARRETE BACA JUNIO 2019
#include "seven_segslcd.h"
#include <Arduino.h>

seven_segslcd::seven_segslcd(){
    this->initial_commit(150);
}

seven_segslcd::seven_segslcd(unsigned short int rb)
{
    this->initial_commit(rb);
}

void seven_segslcd::initial_commit(unsigned short int rb){
  this->SN=4;
  this->numbers = new unsigned short int[10];
  this->numbers[0] = 63;
  this->numbers[1] = 6;
  this->numbers[2] = 91;
  this->numbers[3] = 79;
  this->numbers[4] = 102;
  this->numbers[5] = 109;
  this->numbers[6] = 125;
  this->numbers[7] = 71;
  this->numbers[8] = 127;
  this->numbers[9] = 103;

  this->positions = new unsigned short int[4];
  this->positions[0] = 0b001110;
  this->positions[1] = 0b001101;
  this->positions[2] = 0b001011;
  this->positions[3] = 0b000111;

  this->char_count = 37;
  this->charset = new unsigned short int[char_count];
  this->charimage = new char[char_count];

  this->charset[0] = B00000000; this->charimage[0] = ' ';
  this->charset[1] = B01111100;  this->charimage[1] = 'B';
  this->charset[2] = B00111001;  this->charimage[2] = 'C';
  this->charset[3] = B01011110;  this->charimage[3] = 'D';
  this->charset[4] = B01111001;  this->charimage[4] = 'E';
  this->charset[5] = B01110001;  this->charimage[5] = 'F';
  this->charset[6] = B00111101;  this->charimage[6] = 'G';
  this->charset[7] = B01110110;  this->charimage[7] = 'H';
  this->charset[8] = B00000110;  this->charimage[8] = 'I';
  this->charset[9] = B00001110;  this->charimage[9] = 'J';
  this->charset[10] = B01110110; this->charimage[10] = 'K';
  this->charset[11] = B00111000; this->charimage[11] = 'L';
  this->charset[12] = B01010100; this->charimage[12] = 'N';
  this->charset[13] = B00111111; this->charimage[13] = 'O';
  this->charset[14] = B01110011; this->charimage[14] = 'P';
  this->charset[15] = B01100111; this->charimage[15] = 'Q';
  this->charset[16] = B01010000; this->charimage[16] = 'R';
  this->charset[17] = B01101101; this->charimage[17] = 'S';
  this->charset[18] = B01111000; this->charimage[18] = 'T';
  this->charset[19] = B00111110; this->charimage[19] = 'U';
  this->charset[20] = B00111110; this->charimage[20] = 'V';
  this->charset[21] = B01110110; this->charimage[21] = 'X';
  this->charset[22] = B01101110; this->charimage[22] = 'Y';
  this->charset[23] = B01011011; this->charimage[23] = 'Z';
  this->charset[24] = B01000000; this->charimage[24] = '-';
  this->charset[25] = B10000000; this->charimage[25] = '.';
  this->charset[26] = B01110111;  this->charimage[26] = 'A';


  this->charset[27] = this->numbers[0];  this->charimage[27] = '0';
  this->charset[28] = this->numbers[1];  this->charimage[28] = '1';
  this->charset[29] = this->numbers[2];  this->charimage[29] = '2';
  this->charset[30] = this->numbers[3]; this->charimage[30] = '3';
  this->charset[31] = this->numbers[4];  this->charimage[31] = '4';
  this->charset[32] = this->numbers[5];  this->charimage[32] = '5';
  this->charset[33] = this->numbers[6];  this->charimage[33] = '6';
  this->charset[34] = this->numbers[7];  this->charimage[34] = '7';
  this->charset[35] = this->numbers[8];  this->charimage[35] = '8';
  this->charset[36] = this->numbers[9];  this->charimage[36] = '9';

  DDRD = B11111111;
  DDRB = B001111;

  PORTB = B000000;
  PORTD = this->numbers[0];

  this->counter = 0;

  this->regexp = new int[this->SN]; //0=>3

  for(int x=0; x<SN; ++x){
    this->regexp[x] = this->charset[0]; //DEFAULT
  }

  this->flags = false;

  this->fix = new int[4];

  for (int x = 0; x < SN; ++x) {
    this->fix[x] = rb * (1 + x);
  }

  this->spit_mode = true; //NUMBERS METHOD

  this->A = 0;
  this->B = 0;
  this->C = 0;
  this->D = 0;

  this->t_records[0][1]=150;
  this->t_records[0][2]=200;
  this->t_records[0][3]=250;
  this->t_records[0][4]=300;

  this->curr_index = 0;
  this->tmark_scroll = 0;
  this->_synchronized_scroll1 = false;

  this->changue_lapse = 500;

  this->xscrollDir = LEFT;

  this->baseWordLength = -1;

  this->correct_index_stmt =false;
}

void seven_segslcd::set_scroll_direction(scroll_direction xdir){
    this->xscrollDir = xdir;

    if((!(xdir != LEFT))&&(this->baseWordLength > -1)){
        this->curr_index = this->baseWordLength-1;
        this->correct_index_stmt =true;
    }
}

void seven_segslcd::check() {
  refresh_2();
}

void seven_segslcd::set_string_static(char*rr){
    this->spit_mode = false;
    this->drop_string_and_fill(rr);
}

void seven_segslcd::run_scroll(){
    this->refresh_2();
    this->_s0();
}

void seven_segslcd::set_scroll_lapse(short int val){
    if(val > 0){
        this->changue_lapse = val;
    }
}

void  seven_segslcd::_s0(){
    if(!this->_synchronized_scroll1){
        this->_synchronized_scroll1 = !this->_synchronized_scroll1;
        this->tmark_scroll = millis();

        if(this->xscrollDir == RIGHT){
            this->word_manipulation_right();
        }
        else{
            this->word_manipulation_left();
        }
    }
    else if(this->tick(this->tmark_scroll, millis(), this->changue_lapse)){
        this->tmark_scroll = millis();
        if(this->xscrollDir == RIGHT){
            this->word_manipulation_right();
        }
        else{
            this->word_manipulation_left();
        }
    }
}

int seven_segslcd::getIdMapped(char key){
    int val=-1;

    for(int x=this->char_count-1; x>=0; --x){
        if(key == (char)this->charimage[x]){
            val=x;
            break;
        }
    }

    if(!(val != -1)){
        val = 0;
    }

    return val;
}

void seven_segslcd::drop_string_and_fill(char*arr){
    int n = this->getLength(arr);
    int i = 0;
    if(0 < n){
        for(int x=SN-1; x>=0; --x){
            if(x <= n){
                this->regexp[x] = this->getIdMapped(arr[i]);
            }
            else{
                break;
            }
            ++i;
        }
    }
}
void seven_segslcd::word_manipulation_left(){
    if(!this->correct_index_stmt){
        this->curr_index = this->baseWordLength-1;
        this->correct_index_stmt = !this->correct_index_stmt;
    }

    if(this->curr_index < 0){
        this->curr_index = this->baseWordLength-1;
    }

    short int index=this->curr_index;

    for(int x=SN-1; x>-1; --x){

        this->regexp[x] = this->getIdMapped(this->reversedWord[index]);

        if(index == 0){
            index = this->baseWordLength;
        }

        --index;
    }

    --this->curr_index;
}

void seven_segslcd::word_manipulation_right(){

    if(this->curr_index > this->baseWordLength-1){
        this->curr_index = 0;
    }

    short int index = this->curr_index;

    for(int x=0; x<this->SN; ++x){

        this->regexp[x] = this->getIdMapped(this->reversedWord[index]); //WRAPPER ;)

        if(index == this->baseWordLength-1){
            index = -1;
        }

        ++index;
    }

    ++this->curr_index;
}

char* seven_segslcd::get_reverse(char*word,int n){
    char * newst = new char[n];
    int tmp = 0;
    for(int x=n-1; x>=0; --x){
        *(newst+tmp) = *(word+x);
        ++tmp;
    }
    return newst;
}

void seven_segslcd::set_string(char*word){
    this->baseWord = word;
    this->baseWordLength = this->getLength(this->baseWord);
    this->spit_mode = false;
    this->reversedWord = this->get_reverse(this->baseWord,this->baseWordLength);
}

int seven_segslcd::getLength(char*base){
    int tmp = 0;

    while(true){
        if(!(*(base+tmp)=='*')){
            ++tmp;
        }
        else{
            break;
        }
    }
    return tmp;
}

void seven_segslcd::refresh_2() {
  ++A; ++B; ++C; ++D;

  if (A >= fix[0]) {
    A = 0;
    spit_number(0);
  }

  if (B >= fix[1]) {
    B = 0;
    spit_number(1);
  }

  if (C >= fix[2]) {
    C = 0;
    spit_number(2);
  }

  if (D >= fix[3]) {
    D=0;
    spit_number(3);
  }
}

void seven_segslcd::refresh_1() {
  if (!flags) {
    unsigned long long int ra = micros();
    for (int x = 0; x < 4; ++x) {
      this->t_records[0][x] = ra;
    }

    flags = !flags;
  }

  if (tick(this->t_records[0][0] , micros(), this->t_records[1][0])) {
    this->t_records[0][0] = micros();
    spit_number(0);
  }

  if (tick(this->t_records[0][1] , micros(), this->t_records[1][1])) {
    this->t_records[0][1] = micros();
    spit_number(1);
  }


  if (tick(this->t_records[0][2] , micros(), this->t_records[1][2])) {
    this->t_records[0][2] = micros();
    spit_number(2);
  }

  if (tick(this->t_records[0][3] , micros(), this->t_records[1][3])) {
    this->t_records[0][3] = micros();
    spit_number(3);
  }
}

void seven_segslcd::spit_number(int xpos) {
  PORTB = this->positions[xpos];

  if(spit_mode){
    PORTD = this->numbers[this->regexp[xpos]];
  }
  else{
    PORTD = this->charset[this->regexp[xpos]];
  }
}

bool seven_segslcd::tick(unsigned long long int before, unsigned long long int now, int lapse) {
  if ((now - before) >= lapse) {
    return true;
  }
  return false;
}
void seven_segslcd::set_number(int number) {

  if (number <= 9999) {

    int * tmp = this->split(number);

    for (int x = 1; x <= tmp[0]; ++x) {
      this->regexp[x - 1] = tmp[x];
    }
  }
  else{
    for(int x=0; x<4; ++x){
        this->regexp[x] = 0;
    }
  }

  this->spit_mode = true;
}

void seven_segslcd::print_digit(short int number,short int pos) {

  if (!(number < 0 || number > 9) && (pos > -1) && (pos < 5)) {
    PORTB = this->positions[pos];
    PORTD = this->numbers[number];
  }
}

void seven_segslcd::print_character(char val, short int pos){

    if((-1<pos) && (pos<this->SN)){
        PORTB = this->positions[pos];
        PORTD = this->charset[this->getIdMapped(val)];
    }
}

int * seven_segslcd::split(int x) {

  if (!(x == 0)) {
    int tmp = x;

    while (x != 0) {
      x /= 10;
      ++this->counter;
    }
    this->vals = new int[this->counter + 1];
    counter = 0;

    while (tmp != 0) {
      ++counter;
      this->vals[counter] = tmp % 10;
      tmp /= 10;
    }
    this->vals[0] = counter;
  }
  else {
    this->vals = new int[2];
    this->vals[0] = 1;
    this->vals[1] = 0;
  }

  return this->vals;
}
