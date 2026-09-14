#include "perigos.h"
#include "player.h"

void
atualiza_perigos (void)
{
  for (int i = 0; i < MAX_PERIGOS; i++)
    {
      perigos[i].fase += 1.0f / FPS;

      /* movimento do tronco */
      if (perigos[i].tipo == PERIGO_TRONCO)
        {
          perigos[i].x += perigos[i].vel_x;
          if (perigos[i].x > perigos[i].origem_x + perigos[i].alcance)
            {
              perigos[i].x = perigos[i].origem_x + perigos[i].alcance;
              perigos[i].vel_x = -fabsf (perigos[i].vel_x);
            }
          if (perigos[i].x < perigos[i].origem_x)
            {
              perigos[i].x = perigos[i].origem_x;
              perigos[i].vel_x = fabsf (perigos[i].vel_x);
            }
        }

      /* cipó: verifica se jogador está perto e aperta cima */
      if (perigos[i].tipo == PERIGO_CIPO)
        {
          float cx = perigos[i].x + 10; /* centro do cipó */
          float dist = player_x + 16 - cx;
          if (dist < 0)
            dist = -dist;
          int perto = (dist < 30) && (player_y + 54 > perigos[i].y)
                      && (player_y < perigos[i].y + perigos[i].h);
          if (perto && (teclas[ALLEGRO_KEY_UP] || teclas[ALLEGRO_KEY_W]))
            {
              no_cipo = 1;
              cipo_x = cx;
            }
          continue; /* cipó não causa dano */
        }

      /* buraco: verifica se caiu no vão */
      if (perigos[i].tipo == PERIGO_BURACO)
        {
          float bx = perigos[i].x;
          float bw = perigos[i].w;
          if (player_x + 28 > bx && player_x < bx + bw
              && player_y + 54 >= CHAO_Y)
            tomar_dano ();
          continue;
        }

      /* areia */
      if (perigos[i].tipo == PERIGO_AREIA)
        {
          if (colide_rect (player_x + 4, player_y, 24, 54, perigos[i].x,
                           perigos[i].y, perigos[i].w, perigos[i].h))
            {
              vel_x *= 0.3f;
              /* dano a cada 2 segundos */
              int tick = (int)(perigos[i].fase * FPS);
              if (tick > 0 && tick % (int)(2.0f * FPS) == 0)
                tomar_dano ();
            }
          continue;
        }

      /* outros: colisão direta */
      if (colide_rect (player_x + 4, player_y, 24, 54, perigos[i].x,
                       perigos[i].y, perigos[i].w, perigos[i].h))
        tomar_dano ();
    }
}

static void
desenha_lava (float sx, float sy, float w)
{
  /* base da lava */
  al_draw_filled_rectangle (sx, sy, sx + w, sy + 20, al_map_rgb (180, 40, 0));
  al_draw_filled_rectangle (sx, sy, sx + w, sy + 10, al_map_rgb (220, 80, 0));
  /* bolhas */
  int n = (int)(w / 18);
  for (int i = 0; i < n; i++)
    {
      float bx = sx + 8 + i * 18.0f;
      al_draw_filled_circle (bx, sy + 5, 5, al_map_rgb (255, 140, 0));
      al_draw_filled_circle (bx, sy + 5, 2, al_map_rgb (255, 220, 80));
    }
  /* borda escura */
  al_draw_rectangle (sx, sy, sx + w, sy + 20, al_map_rgb (120, 20, 0), 1.5f);
}

static void
desenha_meteoro_chao (float sx, float sy, float fase)
{
  /* cratera pequena */
  al_draw_filled_ellipse (sx + 14, sy + 34, 18, 6, al_map_rgb (30, 15, 5));
  /* cauda */
  for (int i = 3; i >= 0; i--)
    {
      float fy = sy - 10 - i * 12.0f;
      float fr = 6.0f + i * 3.0f;
      float foff = sinf (fase * 2.0f + i) * 4.0f;
      al_draw_filled_circle (sx + 14 + foff, fy, fr,
                             al_map_rgba (60, 60, 60, 100 - i * 20));
    }
  /* núcleo */
  al_draw_filled_circle (sx + 14, sy + 18, 16, al_map_rgb (90, 35, 10));
  al_draw_filled_circle (sx + 14, sy + 18, 11, al_map_rgb (140, 60, 15));
  /* brasa */
  float pulso = 0.5f + sinf (fase * 5.0f) * 0.5f;
  al_draw_filled_circle (sx + 18, sy + 14, 5,
                         al_map_rgba (255, 160, 0, (int)(200 * pulso)));
  al_draw_filled_circle (sx + 18, sy + 14, 2,
                         al_map_rgba (255, 240, 100, 220));
}

static void
desenha_tronco_queimado (float sx, float sy, float fase)
{
  float cx = sx + 14, cy = sy + 14;
  /* corpo */
  al_draw_filled_circle (cx, cy, 14, al_map_rgb (25, 20, 18));
  al_draw_filled_circle (cx, cy, 10, al_map_rgb (40, 32, 28));
  /* rachadura girando*/
  for (int i = 0; i < 4; i++)
    {
      float ang = fase * 3.0f + i * (ALLEGRO_PI / 2.0f);
      al_draw_line (cx, cy, cx + cosf (ang) * 9, cy + sinf (ang) * 9,
                    al_map_rgba (220, 80, 10, 200), 2.0f);
    }
  /* brasa */
  for (int i = 0; i < 5; i++)
    {
      float ang = fase * 2.0f + i * (ALLEGRO_PI * 2.0f / 5.0f);
      float r = 6.0f + (i % 2) * 3.0f;
      float pulso = 0.4f + sinf (fase * 4.0f + i) * 0.6f;
      al_draw_filled_circle (cx + cosf (ang) * r, cy + sinf (ang) * r, 2,
                             al_map_rgba (255, 120, 0, (int)(220 * pulso)));
    }
  al_draw_circle (cx, cy, 14, al_map_rgb (15, 12, 10), 1.5f);
}

static void
desenha_buraco (float sx, float w)
{
  /* buraco */
  al_draw_filled_rectangle (sx, CHAO_Y, sx + w, CHAO_Y + 80,
                            al_map_rgb (5, 5, 10));
  /* borda */
  al_draw_line (sx, CHAO_Y, sx + w, CHAO_Y, al_map_rgba (0, 0, 0, 200), 4.0f);
}

static void
desenha_areia (float sx, float sy, float w, float fase)
{
  /* base */
  al_draw_filled_rectangle (sx, sy, sx + w, sy + 14,
                            al_map_rgb (210, 180, 80));
  /* ondas */
  int ondas = (int)(w / 20);
  for (int i = 0; i < ondas; i++)
    {
      float wx = sx + fmodf (i * 20.0f + fase * 20.0f, w);
      float wy = sy + 5.0f + sinf (fase * 3.0f + i) * 2.0f;
      al_draw_arc (wx, wy, 6, 0, ALLEGRO_PI, al_map_rgba (180, 150, 50, 160),
                   1.5f);
    }
}

static void
desenha_cipo (float sx, float sy, float h, float fase)
{
  float swing = sinf (fase * 1.5f) * 10.0f;
  float cx = sx + 10;
  /* corda */
  al_draw_line (cx, sy, cx + swing, sy + h, al_map_rgb (60, 140, 30), 3.0f);
  /* folhas ao longo da corda */
  ALLEGRO_COLOR folha = al_map_rgb (40, 180, 50);
  for (int i = 0; i < 6; i++)
    {
      float t = (float)i / 5.0f;
      float lx = cx + swing * t;
      float ly = sy + h * t;
      float s = swing * t * 0.3f;
      al_draw_filled_ellipse (lx - 8 + s, ly, 8, 5, folha);
      al_draw_filled_ellipse (lx + 8 + s, ly, 8, 5, folha);
    }
  /* nó na ponta */
  al_draw_filled_circle (cx + swing, sy + h, 6, al_map_rgb (100, 70, 20));
}

void
desenha_perigos (void)
{
  for (int i = 0; i < MAX_PERIGOS; i++)
    {
      float sx = perigos[i].x - camera_x;
      if (sx + perigos[i].w < 0 || sx > LARGURA)
        continue;
      switch (perigos[i].tipo)
        {
        case PERIGO_LAVA:
          desenha_lava (sx, perigos[i].y, perigos[i].w);
          break;
        case PERIGO_METEORO:
          desenha_meteoro_chao (sx, perigos[i].y, perigos[i].fase);
          break;
        case PERIGO_TRONCO:
          desenha_tronco_queimado (sx, perigos[i].y, perigos[i].fase);
          break;
        case PERIGO_BURACO:
          desenha_buraco (sx, perigos[i].w);
          break;
        case PERIGO_AREIA:
          desenha_areia (sx, perigos[i].y, perigos[i].w, perigos[i].fase);
          break;
        case PERIGO_CIPO:
          desenha_cipo (sx, perigos[i].y, perigos[i].h, perigos[i].fase);
          break;
        }
    }
}