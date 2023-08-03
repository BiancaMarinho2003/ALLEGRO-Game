#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

//CONSTANTES E VARIAVEIS GLOBAIS
const float FPS = 60;

const int PRATOS = 8;

const int SCREEN_W = 960;
const int SCREEN_H = 540;

const int CHAO_H = 60;

const int NAVE_W = 100;
const int NAVE_H = 50;

const int x_palito = 106;
const int y_palito = 100;
const int distancia_entre = 106;
const int tamanho_palito = 300;
const int espessura_palito = 10;

const int tamanho_prato = 60;
const int espessura_prato = 10;

int score=0;
int maior_score=0;

//FUNÇÕES

typedef struct Nave {
    float x;
    float vel;
    int dir, esq;
    int action;
    ALLEGRO_COLOR cor;
} Nave;

void initNave(Nave* nave) {
    nave->x = SCREEN_W / 2;
    nave->vel = 5;
    nave->dir = 0;
    nave->esq = 0;
    nave->cor = al_map_rgb(231, 80, 156);
    nave->action=0;
}

void desenhar_triangulo(Nave nave) {
    float y_base = SCREEN_H - CHAO_H / 2;
    al_draw_filled_triangle(nave.x, y_base - NAVE_H, nave.x - NAVE_W / 2, y_base, nave.x + NAVE_W / 2, y_base, nave.cor);
}

void update_nave(Nave* nave) {
    if (nave->dir && nave->x + nave->vel <= SCREEN_W - 50) {
        nave->x += nave->vel;
    }

    if (nave->esq && nave->x - nave->vel >= 50) {
        nave->x -= nave->vel;
    }
}

void desenhar_cenario() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(0, SCREEN_H - CHAO_H, SCREEN_W, SCREEN_H, al_map_rgb(0, 245, 0));
}


void desenhar_palitos() {
    int i;
    for (i = 0; i < PRATOS; i++) {
        al_draw_filled_rectangle(x_palito + distancia_entre * i, y_palito, x_palito + espessura_palito + distancia_entre * i, y_palito + tamanho_palito, al_map_rgb(100, 0, 100));
    }
}


typedef struct Prato{
    float power;
    float tempo;
    float x;
    float y;
    int apareceu;
}Prato;

void inicio_prato(Prato *prato,float tempo_pratos[]) {
    int i;
    for(i=0;i<PRATOS;i++){
        prato[i].x=x_palito + espessura_palito/2 + distancia_entre * i;
        prato[i].y=y_palito;
        prato[i].tempo=tempo_pratos[i];
        prato[i].power=0;
        prato[i].apareceu=0;
    }
}

void desenhar_prato(Prato *prato, ALLEGRO_TIMER *timer, int *playing,int *game_over){
    int i;
    for(i=0;i<PRATOS;i++){
        if(prato[i].apareceu==1 && prato[i].power<255 ){
            al_draw_filled_rectangle(prato[i].x-tamanho_prato/2,prato[i].y-espessura_palito,
                                     prato[i].x+tamanho_prato/2,prato[i].y,
                                     al_map_rgb(255, 255-prato[i].power, 255-prato[i].power));

        }

        else if(prato[i].power == 255){//ou seja, o azul e o verde chegaram a 0;

            while(prato[i].y<SCREEN_H-CHAO_H){

                desenhar_cenario();
                desenhar_palitos();

                prato[i].y+=8;

                al_draw_filled_rectangle(prato[i].x-tamanho_prato/2,prato[i].y-espessura_palito,
                                        prato[i].x+tamanho_prato/2,prato[i].y,
                                        al_map_rgb(255,0,0));
                al_flip_display();
                al_rest(0.1);

            }
        *playing=0;
        *game_over=1;
         break;

        }


    }

}


void update_prato(Prato *prato,ALLEGRO_TIMER *timer,Nave nave){
    int i=0;
    for(i=0;i<PRATOS;i++){
        if(al_get_timer_count(timer)/(int)FPS>=prato[i].tempo && !prato[i].apareceu){
                prato[i].apareceu=1;

    }

    if(prato[i].apareceu==1 && al_get_timer_count(timer)%(int)FPS==0){
            prato[i].power+=8.5; //255(cor)/30=8,5(quanto de power to subtraindo da cor); depois de 30 segundos, o prato chega ao vermelho
    }
    else if(prato[i].apareceu==1 && nave.x>=x_palito+distancia_entre*i && nave.x<=x_palito+espessura_palito+distancia_entre*i && nave.action==1 && nave.dir==0 && nave.esq==0){
        if(prato[i].power >= 8.5)
            prato[i].power-=8.5;

    }

}

}

update_palitos(Prato *prato,Nave nave){
    int i;
    for(i=0;i<PRATOS;i++){
        if(prato[i].apareceu==1 && nave.action==1 && prato[i].power==0){
              al_draw_filled_rectangle(x_palito + distancia_entre * i, y_palito,
                                        x_palito + espessura_palito + distancia_entre * i, y_palito + tamanho_palito,
                                        al_map_rgb(166, 245,100));
        }

    }

}

void reiniciarNave(Nave* nave);
void reiniciarPratos(Prato* prato, float tempo_pratos[]);
void reiniciarCenario(ALLEGRO_DISPLAY* display);

void reiniciarNave(Nave* nave) {
    nave->x = SCREEN_W /  2;
    nave->vel = 5;
    nave->dir = 0;
    nave->esq = 0;
    nave->cor = al_map_rgb(231, 80, 156);
    nave->action = 0;
}

void reiniciarPratos(Prato* prato, float tempo_pratos[]) {
    int i;
    for (i = 0; i < PRATOS; i++) {
        prato[i].x = x_palito + espessura_palito / 2 + distancia_entre * i;
        prato[i].y = y_palito;
        prato[i].tempo = tempo_pratos[i];
        prato[i].power = 0;
        prato[i].apareceu = 0;
    }
}

void reiniciarCenario(ALLEGRO_DISPLAY* display) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
}


void reiniciarJogo(Nave* nave, Prato* prato, float tempo_pratos[], ALLEGRO_DISPLAY* display) {
    reiniciarNave(nave);
    reiniciarPratos(prato, tempo_pratos);
    reiniciarCenario(display);
}


int main(int argc, char** argv) {

    //ROTINAS DE INICIALIZAÇÃO

    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_FONT* font = NULL;
    srand(time(NULL));


    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");//inicializa o Allegro
        return -1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives!\n");//inicializa o módulo de primitivas do Allegro
        return -1;
    }

    if (!al_init_image_addon()) {
        fprintf(stderr, "failed to initialize image module!\n"); //inicializa o modulo que permite carregar imagens no jogo
        return -1;
    }

    al_init_font_addon();//inicializa o modulo allegro que carrega as fontes


    if (!al_init_ttf_addon()) {
        fprintf(stderr, "failed to load tff font module!\n");//inicializa o modulo allegro que entende arquivos tff de fontes
        return -1;
    }


    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "failed to create timer!\n");//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
        return -1;
    }


    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "failed to create display!\n"); //cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
        al_destroy_timer(timer);
        return -1;
    }


    font = al_load_font("arial.ttf", 32, 1);
    if (font == NULL) {
        fprintf(stderr, "font file does not exist or cannot be accessed!\n");//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    }


    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n"); //cria a fila de eventos
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    //instala o teclado
    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to install keyboard!\n"); //instala o teclado
        return -1;
    }

    //instala o mouse
    if (!al_install_mouse()) {
        fprintf(stderr, "failed to initialize mouse!\n");//instala o mouse
        return -1;
    }

    //registra na fila os eventos de tela (ex: clicar no X na janela)
    al_register_event_source(event_queue, al_get_display_event_source(display));
    //registra na fila os eventos de tempo: quando o tempo altera de t para t+1
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    //registra na fila os eventos de teclado (ex: pressionar uma tecla)
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    //registra na fila os eventos de mouse (ex: clicar em um botao do mouse)
    al_register_event_source(event_queue, al_get_mouse_event_source());

    int principal=1; //VARIAVEL QUE CONTROLA O INICIO DO LOOP PRINCIPAL

 while(principal==1){

      ALLEGRO_EVENT ev;
      if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {//O LOOP PRINCIPAL SÓ ACABA QUANDO O JOGADOR FECHAR A ABA
            principal = 0;
        }

    Nave nave;//CRIA A NAVE
    initNave(&nave);//INICIALIZA A NAVE

    int i = 1;
    float tempo_pratos[PRATOS]; //LOOP QUE CRIA O TEMPO RANDÔMICO DE CADA PRATO A CADA PARTIDA
    for (i = 0; i < PRATOS; i++) {
        int tempinho[] = { 18, 15, 12, 5 ,6, 9, 12, 21 };
        tempo_pratos[i] = tempinho[i] + rand() %4;
    }

    Prato prato[PRATOS];
    inicio_prato(prato,tempo_pratos);

    long long tempo_segundos=0;
    int menu=1;
    int playing = 0;
    int game_over=0;
    score = 0;

    while(menu==1){ //LOOP QUE CRIA A PAGINA INICIAL
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_filled_rectangle(0, SCREEN_H, SCREEN_W, SCREEN_H, al_map_rgb(0, 0, 0));

        char texto[50];
        sprintf(texto, "Pressione espaço para jogar");
        al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTER, texto);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            menu = 0;
            playing=0;
            game_over=0;
            principal=0;

        }

         if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
               if(ev.keyboard.keycode==ALLEGRO_KEY_SPACE){
                 menu=0;
                 playing=1;
               }
    }
            al_flip_display();
    }


    while (playing==1) {//LOOP QUE INICIA UMA PARTIDA

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev); //espera por um evento e o armazena na variavel de evento ev

        al_start_timer(timer);//INICIO DA CONTAGEM DO TEMPO DA PARTIDA;

        //se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
        if (ev.type == ALLEGRO_EVENT_TIMER) {


            desenhar_cenario();//CHAMA A FUNÇÃO DE DESENHAR O CENARIO

            tempo_segundos=al_get_timer_count(timer)/FPS;
            char texto[50];
            sprintf(texto,"%lld",tempo_segundos);
            al_draw_text(font,al_map_rgb(255,255,255),10,10,0,texto);

            score = (int)(al_get_timer_count(timer) / FPS); // DETERMINA A PONTUAÇÃO COMO O TEMPO DA PARTIDA
            if (score > maior_score)
            maior_score = score;//ATUALIZA O RECORD;

            desenhar_palitos();

            update_nave(&nave);

            desenhar_triangulo(nave);

            update_prato(prato,timer,nave);

            desenhar_prato(prato,timer,&playing,&game_over);

            update_palitos(prato,nave);

            al_flip_display();

            if (al_get_timer_count(timer) % (int)FPS == 0)
                printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer) / FPS));
        }

        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            playing = 0;
            principal=0;
            game_over=0;
            menu=0;
        }

        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                nave.esq = 1;
                break;

            case ALLEGRO_KEY_RIGHT:
                nave.dir = 1;
                break;

            case ALLEGRO_KEY_SPACE:
                nave.action=1;
                break;


            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                nave.esq = 0;
                break;

            case ALLEGRO_KEY_RIGHT:
                nave.dir = 0;
                break;

            case ALLEGRO_KEY_SPACE:
                nave.action=0;
                break;
            }

        }
    }

     while(game_over==1){
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_filled_rectangle(0, SCREEN_H, SCREEN_W, SCREEN_H, al_map_rgb(0, 0, 0));

      char texto[50];
        sprintf(texto, "Sua pontuação: %d", score);
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 - 50, ALLEGRO_ALIGN_CENTER, texto);

    char texto2[50];
        sprintf(texto2, "Maior pontuação: %d", maior_score);
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, texto2);

    char texto3[50];
        sprintf(texto3, "Pressione espaço para uma nova partida");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 50, ALLEGRO_ALIGN_CENTER, texto3);

        al_flip_display();

         if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {

        reiniciarJogo(&nave, prato, tempo_pratos,display);
        al_set_timer_count(timer, 0);
        al_start_timer(timer);
        score = 0;
        game_over = 0;
        menu=1;
            }

            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            game_over = 0;
            menu=0;
            playing=0;
            principal=0;
        }
    }
}
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
