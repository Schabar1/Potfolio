#include "cenario.h"
#include "game.h"
#include "interface.h"
#include "perigos.h"
#include "player.h"

/* 12 plataformas para chão e plataforma suspensa*/
Plataforma plataformas[MAX_PLAT] = {
  { 0, CHAO_Y, 500, 80 },    { 500, CHAO_Y, 200, 80 },
  { 800, CHAO_Y, 400, 80 },  { 1200, CHAO_Y, 500, 80 },
  { 1700, CHAO_Y, 100, 80 }, { 2150, CHAO_Y, 300, 80 },
  { 2450, CHAO_Y, 500, 80 }, { 2500, CHAO_Y - 120, 100, 16 },
  { 2950, CHAO_Y, 600, 80 }, { 3220, CHAO_Y - 130, 120, 16 },
  { 3550, CHAO_Y, 500, 80 }, { 4050, CHAO_Y, 650, 80 },
};

Perigo perigos[MAX_PERIGOS];

/* personagem */
float player_x, player_y, vel_x, vel_y;
int no_chao, hp, i_frames;
int no_cipo;
float cipo_x;

/* câmera e tempo de jogo */
float camera_x, tempo;
Estado estado;
int teclas[ALLEGRO_KEY_MAX];

/* fontes e sprites*/
ALLEGRO_FONT *fonte_grande;
ALLEGRO_FONT *fonte_pequena;
ALLEGRO_BITMAP *spr_parado;
ALLEGRO_BITMAP *spr_agacha;
ALLEGRO_BITMAP *spr_pula;
ALLEGRO_BITMAP *spr_andar[2];
ALLEGRO_BITMAP *spr_escala;

/* detecta colisão */
int
colide_rect (float ax, float ay, float aw, float ah, float bx, float by,
             float bw, float bh)
{
  return (ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by);
}

int
main (void)
{
  al_init ();
  al_init_primitives_addon ();
  al_init_image_addon ();
  al_install_keyboard ();
  al_init_font_addon ();
  al_init_ttf_addon ();

  /* cria janela */
  ALLEGRO_DISPLAY *janela = al_create_display (LARGURA, ALTURA);
  ALLEGRO_TIMER *timer = al_create_timer (1.0 / FPS);
  ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue ();

  al_set_window_title (janela, "Pitfall Adventure");

  /*registra eventos */
  al_register_event_source (fila, al_get_display_event_source (janela));
  al_register_event_source (fila, al_get_timer_event_source (timer));
  al_register_event_source (fila, al_get_keyboard_event_source ());
  al_start_timer (timer);

  fonte_grande = al_create_builtin_font ();
  fonte_pequena = al_create_builtin_font ();

  /* sprites */
  spr_parado = al_load_bitmap ("assets/player_parado.png");
  spr_agacha = al_load_bitmap ("assets/player_abaixa.png");
  spr_pula = al_load_bitmap ("assets/player_pula.png");
  spr_andar[0] = al_load_bitmap ("assets/player_anda1.png");
  spr_andar[1] = al_load_bitmap ("assets/player_anda2.png");
  spr_escala = al_load_bitmap ("assets/player_escala.png");

  /* inicia no menu */
  estado = ESTADO_MENU;
  memset (teclas, 0, sizeof (teclas));
  reiniciar ();

  int rodando = 1, redesenha = 0;

  while (rodando)
    {
      ALLEGRO_EVENT ev;
      /*limpa fila de eventos*/
      while (al_get_next_event (fila, &ev))
        {
          /* fehca no botão x */
          if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            rodando = 0;

          if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
              int k = ev.keyboard.keycode;
              teclas[k] = 1;
              /* esc fecha ou volta ao menu */
              if (k == ALLEGRO_KEY_ESCAPE)
                {
                  if (estado == ESTADO_JOGANDO)
                    estado = ESTADO_MENU;
                  else
                    rodando = 0;
                }
              /* enter ou espaço para iniciar*/
              if (k == ALLEGRO_KEY_ENTER || k == ALLEGRO_KEY_SPACE)
                {
                  if (estado == ESTADO_MENU)
                    {
                      reiniciar ();
                      estado = ESTADO_JOGANDO;
                    }
                  else if (estado == ESTADO_GAMEOVER
                           || estado == ESTADO_VITORIA)
                    {
                      estado = ESTADO_MENU;
                    }
                }
              /* pulo */
              if (estado == ESTADO_JOGANDO)
                if (k == ALLEGRO_KEY_SPACE || k == ALLEGRO_KEY_UP
                    || k == ALLEGRO_KEY_W)
                  if (no_chao)
                    {
                      vel_y = FORCA_PULO;
                      no_chao = 0;
                    }
              /* soltar cipo com espaço */
              if (k == ALLEGRO_KEY_SPACE || k == ALLEGRO_KEY_UP
                  || k == ALLEGRO_KEY_W)
                {
                  if (no_cipo)
                    {
                      /* solta o cipó e pula */
                      no_cipo = 0;
                      vel_y = FORCA_PULO;
                    }
                  else if (no_chao)
                    {
                      vel_y = FORCA_PULO;
                      no_chao = 0;
                    }
                }
            }
          if (ev.type == ALLEGRO_EVENT_KEY_UP)
            teclas[ev.keyboard.keycode] = 0;
          /* redesenha */
          if (ev.type == ALLEGRO_EVENT_TIMER)
            redesenha = 1;
        }
      /* redesenha 60x por segundo */
      if (redesenha)
        {
          redesenha = 0;
          /* atualiza durante jogo */
          if (estado == ESTADO_JOGANDO)
            {
              tempo += 1.0f / FPS;
              atualiza_personagem ();
              atualiza_perigos ();
              /* vitoria ou derrota */
              if (hp <= 0)
                estado = ESTADO_GAMEOVER;
              if (player_x >= 4600)
                estado = ESTADO_VITORIA;
            }

          /* estados */
          switch (estado)
            {
            case ESTADO_MENU:
              desenha_menu ();
              break;
            case ESTADO_JOGANDO:
              desenha_cenario ();
              desenha_saida ();
              desenha_perigos ();
              desenha_personagem ();
              desenha_hud ();
              break;
            case ESTADO_GAMEOVER:
              desenha_gameover ();
              break;
            case ESTADO_VITORIA:
              desenha_vitoria ();
              break;
            }
          /* mostra na tela */
          al_flip_display ();
        }
      /* aliviar cpu */
      al_rest (0.001);
    }
  /* limpa memoria*/
  al_destroy_font (fonte_grande);
  al_destroy_font (fonte_pequena);
  al_destroy_bitmap (spr_parado);
  al_destroy_bitmap (spr_agacha);
  al_destroy_bitmap (spr_pula);
  for (int i = 0; i < 2; i++)
    al_destroy_bitmap (spr_andar[i]);
  al_destroy_bitmap (spr_escala);
  al_destroy_display (janela);
  al_destroy_timer (timer);
  al_destroy_event_queue (fila);
  return 0;
}