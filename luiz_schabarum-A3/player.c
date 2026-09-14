#include "player.h"

static int walk_timer = 0;
static int walk_frame = 0;

static Perigo perigos_base[MAX_PERIGOS] = {
  /* buracos */
  { PERIGO_BURACO, 1800, CHAO_Y, 300, 80, 0, 0, 0, 0 },

  /* lava */
  { PERIGO_LAVA, 1250, CHAO_Y - 20, 80, 20, 0, 0, 0, 0 },
  { PERIGO_LAVA, 1420, CHAO_Y - 20, 80, 20, 0, 0, 0, 0 },

  /* cipó sobre o vão */
  { PERIGO_CIPO, 1950, CHAO_Y - 260, 20, 260, 0, 0, 0, 0 },

  /* meteorito */
  { PERIGO_METEORO, 2500, CHAO_Y - 36, 28, 36, 0, 0, 0, 0 },
  { PERIGO_METEORO, 2650, CHAO_Y - 36, 28, 36, 0, 0, 0, 0 },
  { PERIGO_METEORO, 2800, CHAO_Y - 36, 28, 36, 0, 0, 0, 0 },

  /* troncos */
  { PERIGO_TRONCO, 3000, CHAO_Y - 28, 28, 28, 2.5f, 3000, 160, 0 },
  { PERIGO_TRONCO, 3200, CHAO_Y - 28, 28, 28, -2.5f, 3200, 160, 0 },
  { PERIGO_TRONCO, 3400, CHAO_Y - 28, 28, 28, 3.0f, 3400, 120, 0 },

  /* areia movediça */
  { PERIGO_AREIA, 3600, CHAO_Y - 10, 220, 14, 0, 0, 0, 0 },
};

void
reiniciar (void)
{
  player_x = 100;
  player_y = CHAO_Y - 54;
  vel_x = 0;
  vel_y = 0;
  no_chao = 1;
  hp = MAX_HP;
  i_frames = 0;
  camera_x = 0;
  tempo = 0;
  no_cipo = 0;
  cipo_x = 0;
  for (int i = 0; i < MAX_PERIGOS; i++)
    perigos[i] = perigos_base[i];
}

void
tomar_dano (void)
{
  if (i_frames > 0)
    return;
  hp--;
  i_frames = I_FRAMES;
  vel_y = -6.0f;
}

void
atualiza_personagem (void)
{
  if (i_frames > 0)
    i_frames--;

  /* ── no cipó ── */
  if (no_cipo)
    {
      /* sobe e desce no cipó */
      if (teclas[ALLEGRO_KEY_UP] || teclas[ALLEGRO_KEY_W])
        player_y -= 2.5f;
      if (teclas[ALLEGRO_KEY_DOWN] || teclas[ALLEGRO_KEY_S])
        player_y += 2.5f;

      /* trava x no cipó */
      player_x = cipo_x - 16;
      vel_x = 0;
      vel_y = 0;

      /* câmera continua seguindo */
      float alvo = player_x - LARGURA / 3.0f;
      if (alvo < 0)
        alvo = 0;
      if (alvo > MUNDO_W - LARGURA)
        alvo = MUNDO_W - LARGURA;
      camera_x += (alvo - camera_x) * 0.15f;
      return;
    }

  /* ── movimento ── */
  vel_x = 0;
  if (teclas[ALLEGRO_KEY_LEFT] || teclas[ALLEGRO_KEY_A])
    vel_x = -VELOCIDADE;
  if (teclas[ALLEGRO_KEY_RIGHT] || teclas[ALLEGRO_KEY_D])
    vel_x = VELOCIDADE;

  vel_y += GRAVIDADE;
  if (vel_y > MAX_QUEDA)
    vel_y = MAX_QUEDA;

  float pes_antes = player_y + 54;
  player_x += vel_x;
  player_y += vel_y;
  float pes_depois = player_y + 54;

  /* verifica se há buraco sob o jogador */
  int em_buraco = 0;
  for (int i = 0; i < MAX_PERIGOS; i++)
    {
      if (perigos[i].tipo != PERIGO_BURACO)
        continue;
      if (player_x + 28 > perigos[i].x
          && player_x < perigos[i].x + perigos[i].w)
        em_buraco = 1;
    }

  if (!em_buraco && pes_depois >= CHAO_Y)
    {
      player_y = CHAO_Y - 54;
      vel_y = 0;
      no_chao = 1;
    }
  else if (!em_buraco)
    {
      no_chao = 0;
    }

  /* se caiu no buraco */
  if (player_y > ALTURA + 50)
    {
      tomar_dano ();
      player_x = 100;
      player_y = CHAO_Y - 54;
      vel_y = 0;
    }

  for (int i = 0; i < MAX_PLAT; i++)
    {
      float plx = plataformas[i].x, ply = plataformas[i].y;
      float plw = plataformas[i].w;
      if ((player_x + 28 > plx) && (player_x + 4 < plx + plw)
          && (pes_antes <= ply) && (pes_depois >= ply) && vel_y > 0)
        {
          player_y = ply - 54;
          vel_y = 0;
          no_chao = 1;
        }
    }

  if (player_x < 0)
    player_x = 0;

  float alvo = player_x - LARGURA / 3.0f;
  if (alvo < 0)
    alvo = 0;
  if (alvo > MUNDO_W - LARGURA)
    alvo = MUNDO_W - LARGURA;
  camera_x += (alvo - camera_x) * 0.15f;
}

void
desenha_personagem (void)
{
  if (i_frames > 0 && (i_frames / 5) % 2 == 0)
    return;

  /* ── escolhe o sprite ── */
  ALLEGRO_BITMAP *spr = spr_parado;

  if (no_cipo && spr_escala)
    spr = spr_escala;
  else if (!no_chao && spr_pula)
    spr = spr_pula;
  else if ((teclas[ALLEGRO_KEY_DOWN] || teclas[ALLEGRO_KEY_S]) && spr_agacha)
    spr = spr_agacha;
  else if (vel_x != 0 && spr_andar[walk_frame])
    {
      walk_timer++;
      if (walk_timer >= 8)
        {
          walk_timer = 0;
          walk_frame = (walk_frame + 1) % 4;
        }
      spr = spr_andar[walk_frame];
    }
  else
    {
      walk_timer = 0;
      walk_frame = 0;
    }

  /* se sprite ainda for NULL, não desenha */
  if (!spr)
    return;

  /* ── espelha se for para a esquerda ── */
  int flags = (vel_x < 0) ? ALLEGRO_FLIP_HORIZONTAL : 0;

  /* ── escala proporcional ── */
  float sw = al_get_bitmap_width (spr);
  float sh = al_get_bitmap_height (spr);
  float dest_h = 54.0f;
  float dest_w = sw * (dest_h / sh);
  float sx = player_x - camera_x;

  al_draw_scaled_bitmap (spr, 0, 0, sw, sh, sx, player_y, dest_w, dest_h,
                         flags);
}