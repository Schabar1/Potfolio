#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define LARGURA 800
#define ALTURA 600
#define FPS 60.0
#define MUNDO_W 5000
#define CHAO_Y 500
#define VELOCIDADE 4.0f
#define FORCA_PULO -12.0f
#define GRAVIDADE 0.5f
#define MAX_QUEDA 14.0f
#define MAX_HP 3
#define I_FRAMES 45

/* ── Estados ── */
typedef enum
{
  ESTADO_MENU = 0,
  ESTADO_JOGANDO,
  ESTADO_GAMEOVER,
  ESTADO_VITORIA
} Estado;

/* ── Plataforma ── */
typedef struct
{
  float x, y, w, h;
} Plataforma;
#define MAX_PLAT 12
extern Plataforma plataformas[MAX_PLAT];

/* ── Perigos ── */
typedef enum
{
  PERIGO_LAVA,
  PERIGO_METEORO,
  PERIGO_TRONCO,
  PERIGO_BURACO,
  PERIGO_AREIA,
  PERIGO_CIPO
} TipoPerigo;
typedef struct
{
  TipoPerigo tipo;
  float x, y, w, h;
  float vel_x, origem_x, alcance, fase;
} Perigo;
#define MAX_PERIGOS 11
extern Perigo perigos[MAX_PERIGOS];

/* ── Variáveis globais do jogo ── */
extern float player_x, player_y, vel_x, vel_y;
extern int no_chao, hp, i_frames;
extern int no_cipo;
extern float cipo_x;
extern float camera_x, tempo;
extern Estado estado;
extern int teclas[ALLEGRO_KEY_MAX];
/* ── Sprites do personagem ── */
extern ALLEGRO_BITMAP *spr_parado;
extern ALLEGRO_BITMAP *spr_agacha;
extern ALLEGRO_BITMAP *spr_pula;
extern ALLEGRO_BITMAP *spr_andar[2];
extern ALLEGRO_BITMAP *spr_escala;
/* ── Fontes ── */
extern ALLEGRO_FONT *fonte_grande;
extern ALLEGRO_FONT *fonte_pequena;

/* ── Utilitário ── */
int colide_rect (float ax, float ay, float aw, float ah, float bx, float by,
                 float bw, float bh);

#endif