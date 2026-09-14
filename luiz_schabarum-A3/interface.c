#include "interface.h"

void
desenha_hud (void)
{
  al_draw_filled_rectangle (0, 0, LARGURA, 40, al_map_rgba (0, 0, 0, 140));
  for (int i = 0; i < MAX_HP; i++)
    {
      float hx = 12 + i * 28.0f, hy = 8;
      ALLEGRO_COLOR cor = (i < hp) ? al_map_rgb (220, 40, 40)
                                   : al_map_rgba (80, 20, 20, 120);
      al_draw_filled_circle (hx + 5, hy + 5, 5, cor);
      al_draw_filled_circle (hx + 14, hy + 5, 5, cor);
      ALLEGRO_VERTEX v[3] = { { hx, hy + 8, 0, 0, 0, cor },
                              { hx + 19, hy + 8, 0, 0, 0, cor },
                              { hx + 9, hy + 20, 0, 0, 0, cor } };
      al_draw_prim (v, NULL, NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_LIST);
    }
  char buf[32];
  int seg = (int)tempo;
  snprintf (buf, sizeof (buf), "%02d:%02d", seg / 60, seg % 60);
  al_draw_text (fonte_pequena, al_map_rgb (240, 240, 240), LARGURA - 10, 11,
                ALLEGRO_ALIGN_RIGHT, buf);
}

void
desenha_menu (void)
{
  /* ── fundo azul escuro ── */
  al_clear_to_color (al_map_rgb (8, 10, 40));

  /* ── estrelas ── */
  for (int i = 0; i < 80; i++)
    {
      float sx = (float)((i * 173 + 50) % (LARGURA - 10)) + 5;
      float sy = (float)((i * 97 + 30) % (ALTURA - 10)) + 5;
      float br = (i % 3 == 0) ? 255 : 160;
      al_draw_filled_circle (sx, sy, (i % 5 == 0) ? 2 : 1,
                             al_map_rgb (br, br, br));
    }

  /* ── meteoro── */
  /* cauda */
  for (int i = 8; i >= 1; i--)
    {
      float ox = i * 18.0f;
      float oy = i * 14.0f;
      float rad = 28.0f - i * 2.5f;
      int alpha = 200 - i * 22;
      al_draw_filled_circle (LARGURA / 2 + ox, ALTURA / 2 - 60 + oy, rad,
                             al_map_rgba (200, 80, 20, alpha));
    }
  /* núcleo */
  al_draw_filled_circle (LARGURA / 2, ALTURA / 2 - 60, 44,
                         al_map_rgb (220, 40, 10));
  /* brilho interno */
  al_draw_filled_circle (LARGURA / 2 - 10, ALTURA / 2 - 75, 20,
                         al_map_rgb (255, 160, 60));
  al_draw_filled_circle (LARGURA / 2 - 16, ALTURA / 2 - 82, 10,
                         al_map_rgb (255, 230, 180));

  /* ── título ── */
  al_draw_text (fonte_grande, al_map_rgba (0, 0, 0, 180), LARGURA / 2 + 3, 33,
                ALLEGRO_ALIGN_CENTRE, "Dino-P Run");
  al_draw_text (fonte_grande, al_map_rgb (255, 210, 50), LARGURA / 2, 30,
                ALLEGRO_ALIGN_CENTRE, "Dino-P Run");

  /* ── dois botões lado a lado ── */
  float bw = 180, bh = 64;
  float b1x = LARGURA / 2 - bw - 14; /* botão esquerdo */
  float b2x = LARGURA / 2 + 14;      /* botão direito  */
  float by = ALTURA / 2 + 60;

  /* botão ENTER */
  al_draw_filled_rounded_rectangle (b1x, by, b1x + bw, by + bh, 10, 10,
                                    al_map_rgb (30, 140, 50));
  al_draw_rounded_rectangle (b1x, by, b1x + bw, by + bh, 10, 10,
                             al_map_rgb (80, 220, 100), 2.5f);
  al_draw_text (fonte_pequena, al_map_rgb (255, 255, 255), b1x + bw / 2,
                by + 10, ALLEGRO_ALIGN_CENTRE, "[ ENTER ]");
  al_draw_text (fonte_pequena, al_map_rgb (180, 255, 180), b1x + bw / 2,
                by + 36, ALLEGRO_ALIGN_CENTRE, "Iniciar");

  /* botão ESC */
  al_draw_filled_rounded_rectangle (b2x, by, b2x + bw, by + bh, 10, 10,
                                    al_map_rgb (140, 30, 30));
  al_draw_rounded_rectangle (b2x, by, b2x + bw, by + bh, 10, 10,
                             al_map_rgb (220, 80, 80), 2.5f);
  al_draw_text (fonte_pequena, al_map_rgb (255, 255, 255), b2x + bw / 2,
                by + 10, ALLEGRO_ALIGN_CENTRE, "[ ESC ]");
  al_draw_text (fonte_pequena, al_map_rgb (255, 180, 180), b2x + bw / 2,
                by + 36, ALLEGRO_ALIGN_CENTRE, "Sair");

  /* ── controles ── */
  float cy = by + bh + 24;
  al_draw_filled_rounded_rectangle (LARGURA / 2 - 220, cy, LARGURA / 2 + 220,
                                    cy + 108, 10, 10,
                                    al_map_rgba (0, 0, 0, 120));
  al_draw_text (fonte_pequena, al_map_rgb (255, 255, 255), LARGURA / 2, cy + 8,
                ALLEGRO_ALIGN_CENTRE, "CONTROLES:");
  al_draw_text (fonte_pequena, al_map_rgb (180, 220, 255), LARGURA / 2,
                cy + 32, ALLEGRO_ALIGN_CENTRE, "A D  ou  <- ->   =   Mover");
  al_draw_text (fonte_pequena, al_map_rgb (180, 220, 255), LARGURA / 2,
                cy + 56, ALLEGRO_ALIGN_CENTRE,
                "ESPACO / W / Seta cima   =   Pular");
  al_draw_text (fonte_pequena, al_map_rgb (180, 220, 255), LARGURA / 2,
                cy + 72, ALLEGRO_ALIGN_CENTRE,
                "ESC durante o jogo   =   Volta ao menu");
}

void
desenha_gameover (void)
{
  /* ── fundo escuro ── */
  for (int y = 0; y < ALTURA; y++)
    {
      float t = (float)y / ALTURA;
      al_draw_line (
          0, y, LARGURA, y,
          al_map_rgb ((int)(15 + t * 10), (int)(5 + t * 5), (int)(5 + t * 5)),
          1.0f);
    }

  /* ── estrelas ── */
  for (int i = 0; i < 60; i++)
    {
      float sx = (float)((i * 173 + 50) % (LARGURA - 10)) + 5;
      float sy = (float)((i * 97 + 30) % (ALTURA - 10)) + 5;
      al_draw_filled_circle (sx, sy, 1, al_map_rgba (180, 180, 180, 120));
    }

  /* ── cratera no chão ── */
  float cx = LARGURA / 2.0f;
  float cy = ALTURA - 80.0f;

  al_draw_filled_ellipse (cx, cy + 20, 110, 22, al_map_rgb (20, 10, 10));
  al_draw_filled_ellipse (cx, cy + 20, 90, 16, al_map_rgb (35, 18, 12));
  /* bordas da cratera */
  for (int i = 0; i < 10; i++)
    {
      float ang = i * (ALLEGRO_PI * 2.0f / 10.0f);
      float dist = 100.0f + (i * 37 % 20);
      float ex = cx + cosf (ang) * dist;
      float ey = cy + 20 + sinf (ang) * 20;
      al_draw_filled_circle (ex, ey, 6 + i % 4, al_map_rgb (50, 25, 15));
    }

  /* ── meteoro caído ── */
  /* cauda */
  for (int i = 6; i >= 1; i--)
    {
      float ox = i * 14.0f;
      float oy = -i * 10.0f;
      float rad = 20.0f - i * 2.0f;
      al_draw_filled_circle (cx + ox, cy + oy, rad,
                             al_map_rgba (80, 40, 20, 80));
    }
  /* núcleo */
  al_draw_filled_circle (cx, cy, 40, al_map_rgb (90, 35, 15));
  al_draw_filled_circle (cx, cy, 30, al_map_rgb (60, 20, 10));
  /* rachadura */
  al_draw_line (cx - 10, cy - 20, cx + 16, cy + 18, al_map_rgb (180, 80, 20),
                3.0f);
  al_draw_line (cx, cy - 10, cx - 14, cy + 20, al_map_rgb (150, 60, 10), 2.0f);

  /* ── fumaça ── */
  for (int i = 0; i < 5; i++)
    {
      float fx = cx - 20 + i * 12.0f;
      float fy = cy - 40 - i * 18.0f;
      float rad = 10.0f + i * 5.0f;
      al_draw_filled_circle (fx, fy, rad,
                             al_map_rgba (60, 60, 60, 120 - i * 20));
    }

  /* ── chão ── */
  al_draw_filled_rectangle (0, ALTURA - 60, LARGURA, ALTURA,
                            al_map_rgb (20, 12, 8));
  al_draw_filled_rectangle (0, ALTURA - 60, LARGURA, ALTURA - 50,
                            al_map_rgb (30, 18, 10));

  /* ── personagem ── */
  float px = cx + 60;
  float py = ALTURA - 60 - 40;
  float sw = al_get_bitmap_width (spr_parado);
  float sh = al_get_bitmap_height (spr_parado);
  /* rotaciona 90 graus */
  ALLEGRO_TRANSFORM t, orig;
  al_copy_transform (&orig, al_get_current_transform ());
  al_identity_transform (&t);
  al_rotate_transform (&t, ALLEGRO_PI / 2.0f);
  al_translate_transform (&t, px + 54, py);
  al_use_transform (&t);
  al_draw_scaled_bitmap (spr_parado, 0, 0, sw, sh, 0, 0, 54, 54, 0);
  al_use_transform (&orig);

  /* ── caixa do texto ── */
  al_draw_filled_rounded_rectangle (LARGURA / 2 - 240, 100, LARGURA / 2 + 240,
                                    260, 14, 14, al_map_rgba (0, 0, 0, 180));
  al_draw_rounded_rectangle (LARGURA / 2 - 240, 100, LARGURA / 2 + 240, 260,
                             14, 14, al_map_rgb (180, 40, 40), 2.5f);

  /* ── textos ── */
  al_draw_text (fonte_grande, al_map_rgba (0, 0, 0, 200), LARGURA / 2 + 2, 122,
                ALLEGRO_ALIGN_CENTRE, "GAME OVER");
  al_draw_text (fonte_grande, al_map_rgb (220, 40, 40), LARGURA / 2, 120,
                ALLEGRO_ALIGN_CENTRE, "GAME OVER");

  al_draw_text (fonte_pequena, al_map_rgb (200, 200, 200), LARGURA / 2, 178,
                ALLEGRO_ALIGN_CENTRE, "O meteoro te pegou...");

  /* ── tempo ── */
  char buf[32];
  int seg = (int)tempo;
  snprintf (buf, sizeof (buf), "Tempo: %02d:%02d", seg / 60, seg % 60);
  al_draw_text (fonte_pequena, al_map_rgb (160, 160, 160), LARGURA / 2, 218,
                ALLEGRO_ALIGN_CENTRE, buf);

  /* ── instrução ── */
  al_draw_text (fonte_pequena, al_map_rgb (120, 120, 120), LARGURA / 2,
                ALTURA - 24, ALLEGRO_ALIGN_CENTRE,
                "[ ENTER ]  Tentar novamente     [ ESC ]  Sair");
}

void
desenha_vitoria (void)
{
  /* ── céu ── */
  for (int y = 0; y < ALTURA; y++)
    {
      float t = (float)y / ALTURA;
      al_draw_line (0, y, LARGURA, y,
                    al_map_rgb ((int)(100 + t * 60), (int)(180 + t * 40),
                                (int)(255 - t * 40)),
                    1.0f);
    }

  /* ── sol ── */
  al_draw_filled_circle (LARGURA - 80, 80, 55, al_map_rgb (255, 245, 100));
  al_draw_filled_circle (LARGURA - 80, 80, 42, al_map_rgb (255, 230, 50));
  /* raios do sol */
  for (int i = 0; i < 12; i++)
    {
      float ang = i * (ALLEGRO_PI * 2.0f / 12.0f);
      float r1 = 58.0f, r2 = 76.0f;
      al_draw_line (LARGURA - 80 + cosf (ang) * r1, 80 + sinf (ang) * r1,
                    LARGURA - 80 + cosf (ang) * r2, 80 + sinf (ang) * r2,
                    al_map_rgb (255, 220, 50), 3.0f);
    }

  /* ── nuvens ── */
  ALLEGRO_COLOR branco = al_map_rgb (255, 255, 255);
  al_draw_filled_ellipse (160, 100, 70, 28, branco);
  al_draw_filled_ellipse (110, 108, 45, 20, branco);
  al_draw_filled_ellipse (210, 108, 45, 20, branco);

  al_draw_filled_ellipse (520, 140, 60, 22, branco);
  al_draw_filled_ellipse (476, 148, 38, 16, branco);
  al_draw_filled_ellipse (564, 148, 38, 16, branco);

  /* ── grama ── */
  al_draw_filled_rectangle (0, ALTURA - 100, LARGURA, ALTURA - 88,
                            al_map_rgb (60, 160, 50));
  al_draw_filled_rectangle (0, ALTURA - 88, LARGURA, ALTURA,
                            al_map_rgb (110, 70, 30));
  /* tufos de grama */
  for (int i = 0; i < 30; i++)
    {
      float gx = (float)(i * 137 % (LARGURA - 20)) + 10;
      float gy = ALTURA - 100;
      al_draw_filled_triangle (gx, gy, gx + 6, gy - 12, gx + 12, gy,
                               al_map_rgb (40, 180, 40));
      al_draw_filled_triangle (gx + 5, gy, gx + 11, gy - 14, gx + 17, gy,
                               al_map_rgb (50, 200, 50));
    }

  /* ── caixa do texto ── */
  al_draw_filled_rounded_rectangle (LARGURA / 2 - 260, 180, LARGURA / 2 + 260,
                                    310, 14, 14, al_map_rgba (0, 60, 0, 170));
  al_draw_rounded_rectangle (LARGURA / 2 - 260, 180, LARGURA / 2 + 260, 310,
                             14, 14, al_map_rgb (80, 220, 80), 3.0f);

  /* ── texto ── */
  al_draw_text (fonte_grande, al_map_rgba (0, 0, 0, 180), LARGURA / 2 + 2, 202,
                ALLEGRO_ALIGN_CENTRE, "VITORIA!");
  al_draw_text (fonte_grande, al_map_rgb (80, 220, 80), LARGURA / 2, 200,
                ALLEGRO_ALIGN_CENTRE, "VITORIA!");
  al_draw_text (fonte_pequena, al_map_rgb (220, 255, 220), LARGURA / 2, 248,
                ALLEGRO_ALIGN_CENTRE, "Voce sobreviveu!");

  /* ── tempo ── */
  char buf[32];
  int seg = (int)tempo;
  snprintf (buf, sizeof (buf), "Tempo: %02d:%02d", seg / 60, seg % 60);
  al_draw_text (fonte_pequena, al_map_rgb (240, 240, 240), LARGURA / 2, 282,
                ALLEGRO_ALIGN_CENTRE, buf);

  /* ── instrução ── */
  al_draw_text (fonte_pequena, al_map_rgb (60, 60, 60), LARGURA / 2,
                ALTURA - 40, ALLEGRO_ALIGN_CENTRE,
                "[ ENTER ]  Jogar novamente     [ ESC ]  Sair");
}