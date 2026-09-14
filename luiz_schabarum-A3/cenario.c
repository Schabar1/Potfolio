#include "cenario.h"

void
desenha_cenario (void)
{
  /* ── céu noturno ── */
  for (int y = 0; y < CHAO_Y; y++)
    {
      float t = (float)y / CHAO_Y;
      al_draw_line (0, y, LARGURA, y,
                    al_map_rgb ((int)(5 + t * 15), (int)(5 + t * 10),
                                (int)(20 + t * 30)),
                    1.0f);
    }

  /* ── estrelas ── */
  for (int i = 0; i < 120; i++)
    {
      float sx = (float)((i * 173 + 11) % (LARGURA - 4)) + 2;
      float sy = (float)((i * 97 + 7) % (CHAO_Y - 4)) + 2;
      float br = (i % 4 == 0) ? 255 : (i % 3 == 0) ? 180 : 120;
      float r = (i % 7 == 0) ? 2.0f : 1.0f;
      al_draw_filled_circle (sx, sy, r, al_map_rgb (br, br, br));
    }

  /* ── lua ── */
  float lua_x = 680 - camera_x * 0.01f;
  al_draw_filled_circle (lua_x, 55, 30, al_map_rgb (230, 225, 180));
  al_draw_filled_circle (lua_x + 10, 50, 24, al_map_rgb (10, 8, 25));

  /* ── meteoro no céu ── */
  float mx = 300 - camera_x * 0.03f;
  float my = 80;
  /* cauda */
  for (int i = 8; i >= 1; i--)
    {
      float ox = i * 16.0f;
      float oy = i * 12.0f;
      float rad = 22.0f - i * 2.0f;
      int alpha = 180 - i * 20;
      al_draw_filled_circle (mx + ox, my - oy, rad,
                             al_map_rgba (200, 80, 20, alpha));
    }
  /* núcleo */
  al_draw_filled_circle (mx, my, 36, al_map_rgb (200, 50, 10));
  al_draw_filled_circle (mx, my, 26, al_map_rgb (240, 100, 20));
  al_draw_filled_circle (mx - 8, my - 10, 12, al_map_rgb (255, 180, 60));
  al_draw_filled_circle (mx - 12, my - 15, 6, al_map_rgb (255, 240, 180));

  /* ── montanhas distantes ── */
  float om = camera_x * 0.1f;
  int mont_x[] = { 0, 120, 220, 380, 500, 620, 750, 880 };
  int mont_h[] = { 180, 240, 160, 280, 200, 260, 180, 220 };
  int mont_w[] = { 160, 180, 140, 200, 160, 180, 150, 170 };
  for (int i = 0; i < 8; i++)
    {
      float bx = fmodf (mont_x[i] - om + 1200, 1200) - 200;
      float by = CHAO_Y;
      float bh = (float)mont_h[i];
      float bw = (float)mont_w[i];
      /* montanha base */
      ALLEGRO_VERTEX v[3] = {
        { bx, by, 0, 0, 0, al_map_rgb (25, 20, 35) },
        { bx + bw / 2, by - bh, 0, 0, 0, al_map_rgb (40, 32, 55) },
        { bx + bw, by, 0, 0, 0, al_map_rgb (25, 20, 35) },
      };
      al_draw_prim (v, NULL, NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_LIST);
      /* neve/brilho no topo */
      ALLEGRO_VERTEX vt[3] = {
        { bx + bw / 2 - 18, by - bh + 40, 0, 0, 0, al_map_rgb (60, 50, 80) },
        { bx + bw / 2, by - bh, 0, 0, 0, al_map_rgb (90, 75, 120) },
        { bx + bw / 2 + 18, by - bh + 40, 0, 0, 0, al_map_rgb (60, 50, 80) },
      };
      al_draw_prim (vt, NULL, NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_LIST);
    }

  /* ── montanhas próximas ── */
  float om2 = camera_x * 0.2f;
  int mont2_x[] = { 50, 250, 450, 650, 850, 1050 };
  int mont2_h[] = { 130, 160, 110, 150, 140, 120 };
  int mont2_w[] = { 200, 220, 180, 210, 190, 200 };
  for (int i = 0; i < 6; i++)
    {
      float bx = fmodf (mont2_x[i] - om2 + 1400, 1400) - 200;
      float by = CHAO_Y;
      float bh = (float)mont2_h[i];
      float bw = (float)mont2_w[i];
      ALLEGRO_VERTEX v[3] = {
        { bx, by, 0, 0, 0, al_map_rgb (18, 14, 28) },
        { bx + bw / 2, by - bh, 0, 0, 0, al_map_rgb (30, 24, 45) },
        { bx + bw, by, 0, 0, 0, al_map_rgb (18, 14, 28) },
      };
      al_draw_prim (v, NULL, NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_LIST);
    }

  /* ── chão ── */
  al_draw_filled_rectangle (0, CHAO_Y, LARGURA, CHAO_Y + 12,
                            al_map_rgb (40, 25, 10));
  al_draw_filled_rectangle (0, CHAO_Y + 12, LARGURA, ALTURA,
                            al_map_rgb (25, 15, 8));
  /* rachaduras na terra */
  for (int i = 0; i < 5; i++)
    {
      float ly = CHAO_Y + 20 + i * 12.0f;
      float lx_off = fmodf (camera_x * 0.05f + i * 20.0f, 50.0f);
      for (float lx = -lx_off; lx < LARGURA; lx += 50.0f)
        al_draw_line (lx, ly, lx + 20, ly, al_map_rgba (60, 30, 10, 100),
                      1.0f);
    }
  /* brasas no chão */
  for (int i = 0; i < 12; i++)
    {
      float bx = fmodf ((float)(i * 137) - camera_x * 0.05f + 1000, LARGURA);
      al_draw_filled_circle (bx, CHAO_Y + 6, 2,
                             al_map_rgba (220, 80, 10, 160));
    }

  /* ── plataformas ── */
  for (int i = 0; i < MAX_PLAT; i++)
    {
      float px = plataformas[i].x - camera_x;
      float py = plataformas[i].y;
      float pw = plataformas[i].w;
      float ph = plataformas[i].h;
      if (px + pw < 0 || px > LARGURA)
        continue;
      if (py >= CHAO_Y)
        continue;
      al_draw_filled_rectangle (px, py, px + pw, py + ph,
                                al_map_rgb (45, 35, 25));
      al_draw_filled_rectangle (px, py, px + pw, py + 4,
                                al_map_rgb (80, 50, 20));
      /* textura de pedra */
      for (float lx = px + 10; lx < px + pw; lx += 18)
        al_draw_line (lx, py + 4, lx + 8, py + ph,
                      al_map_rgba (30, 20, 10, 80), 1.0f);
    }
}

void
desenha_saida (void)
{
  float sx = 4650 - camera_x;
  if (sx > LARGURA + 80 || sx < -80)
    return;

  /* ── rocha ao redor da caverna ── */
  al_draw_filled_rectangle (sx - 10, CHAO_Y - 120, sx + 130, CHAO_Y,
                            al_map_rgb (50, 40, 30));

  /* ── abertura escura da caverna ── */
  al_draw_filled_ellipse (sx + 55, CHAO_Y, 45, 60, al_map_rgb (5, 3, 8));
  /* recorte reto na base */
  al_draw_filled_rectangle (sx + 10, CHAO_Y - 30, sx + 100, CHAO_Y + 10,
                            al_map_rgb (5, 3, 8));

  /* ── detalhes da rocha ── */
  al_draw_filled_rectangle (sx - 10, CHAO_Y - 120, sx + 14, CHAO_Y,
                            al_map_rgb (60, 48, 35));
  al_draw_filled_rectangle (sx + 100, CHAO_Y - 100, sx + 130, CHAO_Y,
                            al_map_rgb (55, 44, 32));
  /* pedras no topo */
  ALLEGRO_VERTEX topo[3] = {
    { sx + 20, CHAO_Y - 120, 0, 0, 0, al_map_rgb (65, 52, 38) },
    { sx + 55, CHAO_Y - 145, 0, 0, 0, al_map_rgb (75, 60, 44) },
    { sx + 100, CHAO_Y - 118, 0, 0, 0, al_map_rgb (65, 52, 38) },
  };
  al_draw_prim (topo, NULL, NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_LIST);

  /* ── stalactites no teto da caverna ── */
  for (int i = 0; i < 4; i++)
    {
      float stx = sx + 28 + i * 16.0f;
      float sth = 10.0f + (i % 2) * 8.0f;
      ALLEGRO_VERTEX st[3] = {
        { stx, CHAO_Y - 56, 0, 0, 0, al_map_rgb (35, 28, 20) },
        { stx + 6, CHAO_Y - 56, 0, 0, 0, al_map_rgb (35, 28, 20) },
        { stx + 3, CHAO_Y - 56 + sth, 0, 0, 0, al_map_rgb (50, 40, 28) },
      };
      al_draw_prim (st, NULL, NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_LIST);
    }
}