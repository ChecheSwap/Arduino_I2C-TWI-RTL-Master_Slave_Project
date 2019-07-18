//Powered by: @CHECHESWAP JESUS JOSE NAVARRETE BACA ~JUNIO 2019

#ifndef SEVEN_SEGSLCD_H
#define SEVEN_SEGSLCD_H
enum scroll_direction{
    LEFT ,
    RIGHT
};
class seven_segslcd
{
    public:
        seven_segslcd(); //CONSTRUCTOR BASE
        seven_segslcd(unsigned short int); //CONSTRUCTOR CON PARAMETRO DE REFRESCO

        void print_digit(short int, short int);//IMPRIME DIGITO 0-9 EN DETERMINADA POSICION
        void print_character(char,short int); //IMPRIME CARACTER EN DETERMINADA POSICION
        void set_number(int); //IMPRIME UN NUMERO DADO -> FUNCION DESENCADENADORA check()
        void set_string(char*); //INCORPORA EL STRING QUE SERA SCROLLEADO
        void set_string_static(char*); //IMPRIME STRING INDIVIDUAL (SELECCIONA 4 VALORES)
        void set_scroll_lapse(short int); //ASIGNA VALOR AL LAPSO DE MOVIMIENTO SCROLL

        void check(); //FUNCION DESENCADENADORA
        void run_scroll(); //FUNCION DESENCADENADORA
        void set_scroll_direction(scroll_direction);//ASIGNA DIRECCION $[xscrollDir]

    private:
        void refresh_1(); //METODO DE REFRESCO SECUENCIAL
        bool flags; //USADO EN REFRESCO 1
        unsigned long long int  t_records[2][4]; //MICROS PARA REFRESCO 1 JUNTO CON SUS MARGENES DE TIEMPO

        void refresh_2(); //METODO DE REFRESCO POR EJECUCION CICLICA
        int A, B, C, D; //CONTEOS DE EJECUCION CICLICA
        int * fix; //MARGENES DE COMPARACION DE EJECUCION CICLICA

        void spit_number(int); //ESCUPE VAL

        int * regexp; //REGISTRO DE PORTD (***VALS BASE***)

        int * split(int); //SEPARA POR METODO MATEMATICO SOLO INT
        int * vals; //REGISTRO P=4 CON INT SEPARADO
        int counter;//CONTADOR USADO EN SPLIT

        bool spit_mode; //BOLEANO DE DIRECCION PARA SPIT

        unsigned short int * numbers; //REGISTRO CON COMBINACIONES DE CODIGOS DE NUMERO
        unsigned short int * charset; //REGISTRO CON COMBINACIONES DE CODIGOS DE CARACTERES
        char * charimage;//REGISTRO CON CARACTERES PARA INDICE DE IMAGEN A CHARSET
        int char_count; //TOTAL DE CHARS PARA IMPRIMIR

        unsigned short int * positions;//REGISTRO CON COMBINACIONES DE CODIGO DE POSICION DEL SEGMENTO

        unsigned short int SN; //NUMERO DE SEGMENTOS = 4

        int getLength(char*); //CONTEO DE CHARS EN STRING || $[*] => CHAR DE FIN ||

        char * baseWord; //PALABRA BASE
        short int baseWordLength; //LENGTH DE PALABRA BASE
        char * get_reverse(char*,int); //INVERSION DE STRING
        char * reversedWord; //PALABRA INVERTIDA
        void word_manipulation_right();//MANEJO DEL STRING RIGHT
        void word_manipulation_left();//MANEJO DEL STRING LEFT
        bool correct_index_stmt; //BANDERA ESTABILIZADORA PARA RIGHT

        short int curr_index; //INDICE DE SCROLL ACTUAL
        void _s0(); //SCROLL
        unsigned long long int tmark_scroll; //TIME LAST
        bool _synchronized_scroll1; //BANDERA ESTABILIZADORA
        short int changue_lapse; //LAPSO DE CAMBIO
        scroll_direction xscrollDir;

        void drop_string_and_fill(char*); //RUTINA USADA EN SET_STRING_STATIC PARA RECORTAR STRING
        int getIdMapped(char); //OBTIENE EL ID EN LOS REGISTROS BINARIOS

    protected:
        bool tick(unsigned long long int, unsigned long long int, int); //FUNCION DE TIEMPO
        void initial_commit(unsigned short int); //STATEMENTS INICIALES

};

#endif
