#include "app_state.h"
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include "stb_image.h"

static void draw_sprite_centered_tinted(int spriteIdx, float cx, float cy, float scaleX, float scaleY, const C2D_ImageTint* tint) {
    C2D_Image img = C2D_SpriteSheetGetImage(spriteSheet, spriteIdx);
    float w = (float)img.subtex->width * scaleX;
    float h = (float)img.subtex->height * scaleY;
    C2D_DrawImageAt(img, cx - (w * 0.5f), cy - (h * 0.5f), 0.70f, tint, scaleX, scaleY);
}

static void draw_sprite_centered(int spriteIdx, float cx, float cy, float scaleX, float scaleY) {
    draw_sprite_centered_tinted(spriteIdx, cx, cy, scaleX, scaleY, NULL);
}

static void draw_perspective_cover_sub(C3D_Tex* tex, const Tex3DS_SubTexture* subtex, float cx, float cy, float scale, float shearY, float rotAngle, float depth, u32 tintColor) {
    if (!tex || !subtex) return;
    C2D_Image cImg = { tex, subtex };
    C2D_ImageTint tint;
    if (tintColor != 0xFFFFFFFF) {
        u8 alpha = (u8)((tintColor >> 24) & 0xFF);
        
        C2D_PlainImageTint(&tint, C2D_Color32(0, 0, 0, alpha), 0.42f);
    }
    float sizeMul = 128.0f / (float)subtex->width;
    float halfW = (float)subtex->width * 0.5f;
    float halfH = (float)subtex->height * 0.5f;
    C3D_Mtx savedView;
    C2D_ViewSave(&savedView);

    C2D_ViewTranslate(cx, cy);
    if (rotAngle != 0.0f) C2D_ViewRotate(rotAngle);
    if (shearY != 0.0f)   C2D_ViewShear(0.0f, shearY);
    C2D_ViewScale(scale * sizeMul, scale * sizeMul);

    C2D_DrawImageAt(cImg, -halfW, -halfH, depth, (tintColor == 0xFFFFFFFF) ? NULL : &tint, 1.0f, 1.0f);

    C2D_ViewRestore(&savedView);
}

static void draw_perspective_cover(C3D_Tex* tex, float cx, float cy, float scale, float shearY, float rotAngle, float depth, u32 tintColor) {
    draw_perspective_cover_sub(tex, &coverSubtex, cx, cy, scale, shearY, rotAngle, depth, tintColor);
}

static void draw_perspective_quad_cover( //i mean... looks ok... yk...
    C3D_Tex* tex,
    float x0, float y0,  
    float x1, float y1,  
    float x2, float y2,  
    float x3, float y3,  
    float depth, u32 tintColor)
{
    if (!tex) return;

    float h_left = y3 - y0;
    float h_right = y2 - y1;
    if (h_left <= 0.0f || h_right <= 0.0f) return;

    if (depth >= CFG_DEPTH_COVER_MAIN - 0.05f) {
        u32 backColor = C2D_Color32(10, 11, 14, 255);
        C2D_DrawTriangle(x0, y0, backColor, x1, y1, backColor, x2, y2, backColor, depth - 0.001f);
        C2D_DrawTriangle(x0, y0, backColor, x2, y2, backColor, x3, y3, backColor, depth - 0.001f);
    }

    float q = h_left / h_right;
    const int NUM_STRIPS = CFG_PERSPECTIVE_SLICES;

    C2D_ImageTint tint;
    bool hasTint = (tintColor != 0xFFFFFFFF);
    if (hasTint) {
        
        if (depth >= CFG_DEPTH_COVER_SHADOW - 0.01f && depth <= CFG_DEPTH_COVER_SHADOW + 0.01f) {
            C2D_PlainImageTint(&tint, tintColor, 1.0f);
        } else {
            float alpha = (float)((tintColor >> 24) & 0xFF) / 255.0f;
            C2D_AlphaImageTint(&tint, alpha);
        }
    }

    Tex3DS_SubTexture sliceSubtex;
    sliceSubtex.top = 1.0f;
    sliceSubtex.bottom = 0.0f;
    sliceSubtex.width = 128;
    sliceSubtex.height = 128;

    for (int i = 0; i < NUM_STRIPS; i++) {
        float s0 = (float)i / (float)NUM_STRIPS;
        float s1 = (float)(i + 1) / (float)NUM_STRIPS;

        float u0 = s0 / (s0 + (1.0f - s0) * q);
        float u1 = s1 / (s1 + (1.0f - s1) * q);

        float xt0 = x0 + s0 * (x1 - x0);
        float yt0 = y0 + s0 * (y1 - y0);
        float xt1 = x0 + s1 * (x1 - x0);
        float yt1 = y0 + s1 * (y1 - y0);

        float xb0 = x3 + s0 * (x2 - x3);
        float yb0 = y3 + s0 * (y2 - y3);
        float xb1 = x3 + s1 * (x2 - x3);
        float yb1 = y3 + s1 * (y2 - y3);

        float sx0 = (xt0 + xb0) * 0.5f;
        float sx1 = (xt1 + xb1) * 0.5f;
        float sy_top = (yt0 < yt1) ? yt0 : yt1;
        float sy_bot = (yb0 > yb1) ? yb0 : yb1;

        float sw = (sx1 - sx0) + CFG_SUBPIXEL_OVERLAP;
        float sh = (sy_bot - sy_top) + 0.6f;

        sliceSubtex.left = u0;
        sliceSubtex.right = u1;

        C2D_Image sliceImg = { tex, &sliceSubtex };
        C2D_DrawParams params = {
            .pos = { .x = sx0, .y = sy_top, .w = sw, .h = sh },
            .center = { 0.0f, 0.0f },
            .depth = depth,
            .angle = 0.0f
        };
        C2D_DrawImage(sliceImg, &params, hasTint ? &tint : NULL);
    }
}
static C3D_Tex haloGlowTex;
static Tex3DS_SubTexture haloGlowSubtex;
static bool hasHaloGlowTex = false;

static C3D_Tex irisMaskTex;
static Tex3DS_SubTexture irisMaskSubtex;
static bool hasIrisMaskTex = false;
static uint8_t mortonLUT[64];
static uint8_t irisTileDirty[15][25];
static int irisPrevBaseAlpha = -1;

static void ensure_fx_textures(void) {
    if (!hasHaloGlowTex) {
        uint8_t glow[64 * 64 * 4];
        for (int y = 0; y < 64; y++) {
            for (int x = 0; x < 64; x++) {
                float dx = ((float)x + 0.5f - 32.0f) / 32.0f;
                float dy = ((float)y + 0.5f - 32.0f) / 32.0f;
                float r2 = dx * dx + dy * dy;
                float a = (r2 < 1.0f) ? expf(-r2 * 1.9f) * (1.0f - r2) : 0.0f;
                int idx = (y * 64 + x) * 4;
                glow[idx]     = 255;
                glow[idx + 1] = 255;
                glow[idx + 2] = 255;
                glow[idx + 3] = (uint8_t)(a * 255.0f);
            }
        }
        upload_rgba8_to_tex(&haloGlowTex, glow, 64, 64, 64, 64);
        haloGlowSubtex.width = 64;
        haloGlowSubtex.height = 64;
        haloGlowSubtex.left = 0.0f;
        haloGlowSubtex.top = 1.0f;
        haloGlowSubtex.right = 1.0f;
        haloGlowSubtex.bottom = 0.0f;
        hasHaloGlowTex = true;
    }
    if (!hasIrisMaskTex) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                uint32_t m = 0;
                for (int bit = 0; bit < 3; bit++) {
                    m |= ((x >> bit) & 1) << (bit * 2);
                    m |= ((y >> bit) & 1) << (bit * 2 + 1);
                }
                mortonLUT[y * 8 + x] = (uint8_t)m;
            }
        }
        if (C3D_TexInit(&irisMaskTex, 256, 128, GPU_RGBA8)) {
            C3D_TexSetFilter(&irisMaskTex, GPU_LINEAR, GPU_LINEAR);
            C3D_TexSetWrap(&irisMaskTex, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
            memset(irisMaskTex.data, 0, 256 * 128 * 4);
            C3D_TexFlush(&irisMaskTex);
        }
        memset(irisTileDirty, 0, sizeof(irisTileDirty));
        irisPrevBaseAlpha = 0;
        irisMaskSubtex.width = 200;
        irisMaskSubtex.height = 120;
        irisMaskSubtex.left = 0.0f;
        irisMaskSubtex.top = 1.0f;
        irisMaskSubtex.right = 200.0f / 256.0f;
        irisMaskSubtex.bottom = 1.0f - (120.0f / 128.0f);
        hasIrisMaskTex = true;
    }
}

static void update_iris_mask_tex(float lx1, float ly1, float lx2, float ly2, float r, uint8_t baseAlpha) {
    uint32_t* tiled = (uint32_t*)irisMaskTex.data;
    if (!tiled) return;

    float rIn = r - 1.2f;
    float rOut = r + 1.2f;
    float rIn2 = rIn * rIn;
    float rOut2 = rOut * rOut;
    float invSpan = 1.0f / (rOut2 - rIn2);
    bool alphaChanged = ((int)baseAlpha != irisPrevBaseAlpha);
    uint32_t baseCol = (uint32_t)baseAlpha;

    for (int ty = 0; ty < 15; ty++) {
        float wy0 = (float)(ty * 16) + 1.0f;
        float wy1 = wy0 + 14.0f;

        float cy1 = (ly1 < wy0) ? wy0 : ((ly1 > wy1) ? wy1 : ly1);
        float cy2 = (ly2 < wy0) ? wy0 : ((ly2 > wy1) ? wy1 : ly2);
        float minDy1 = ly1 - cy1;
        float minDy2 = ly2 - cy2;
        float fy1 = (fabsf(wy0 - ly1) > fabsf(wy1 - ly1)) ? (wy0 - ly1) : (wy1 - ly1);
        float fy2 = (fabsf(wy0 - ly2) > fabsf(wy1 - ly2)) ? (wy0 - ly2) : (wy1 - ly2);

        for (int tx = 0; tx < 25; tx++) {
            float wx0 = (float)(tx * 16) + 1.0f;
            float wx1 = wx0 + 14.0f;

            float cx1 = (lx1 < wx0) ? wx0 : ((lx1 > wx1) ? wx1 : lx1);
            float cx2 = (lx2 < wx0) ? wx0 : ((lx2 > wx1) ? wx1 : lx2);
            float minDx1 = lx1 - cx1;
            float minDx2 = lx2 - cx2;

            float minD2_1 = minDx1 * minDx1 + minDy1 * minDy1;
            float minD2_2 = minDx2 * minDx2 + minDy2 * minDy2;

            if (minD2_1 >= rOut2 && minD2_2 >= rOut2) {
                if (!alphaChanged && !irisTileDirty[ty][tx]) continue;
                uint32_t* tp = tiled + (ty * 32 + tx) * 64;
                for (int i = 0; i < 64; i++) tp[i] = baseCol;
                irisTileDirty[ty][tx] = 0;
                continue;
            }

            float fx1 = (fabsf(wx0 - lx1) > fabsf(wx1 - lx1)) ? (wx0 - lx1) : (wx1 - lx1);
            float fx2 = (fabsf(wx0 - lx2) > fabsf(wx1 - lx2)) ? (wx0 - lx2) : (wx1 - lx2);
            float maxD2_1 = fx1 * fx1 + fy1 * fy1;
            float maxD2_2 = fx2 * fx2 + fy2 * fy2;

            if (maxD2_1 <= rIn2 || maxD2_2 <= rIn2) {
                uint32_t* tp = tiled + (ty * 32 + tx) * 64;
                for (int i = 0; i < 64; i++) tp[i] = 0;
                irisTileDirty[ty][tx] = 1;
                continue;
            }

            uint32_t* tp = tiled + (ty * 32 + tx) * 64;
            for (int ly = 0; ly < 8; ly++) {
                float wy = wy0 + (float)ly * 2.0f;
                float dy1 = wy - ly1;
                float dy2 = wy - ly2;
                float dy1_2 = dy1 * dy1;
                float dy2_2 = dy2 * dy2;
                for (int lx = 0; lx < 8; lx++) {
                    float wx = wx0 + (float)lx * 2.0f;
                    float dx1 = wx - lx1;
                    float dx2 = wx - lx2;
                    float d2_1 = dx1 * dx1 + dy1_2;
                    float d2_2 = dx2 * dx2 + dy2_2;
                    float d2 = (d2_1 < d2_2) ? d2_1 : d2_2;

                    uint32_t a = baseCol;
                    if (d2 <= rIn2) {
                        a = 0;
                    } else if (d2 < rOut2) {
                        float t = (d2 - rIn2) * invSpan;
                        a = (uint32_t)((float)baseAlpha * t);
                    }
                    tp[mortonLUT[ly * 8 + lx]] = a;
                }
            }
            irisTileDirty[ty][tx] = 1;
        }
    }
    irisPrevBaseAlpha = (int)baseAlpha;
    C3D_TexFlush(&irisMaskTex);
}

void render_top_screen(void) {
    C2D_TargetClear(topTarget, C2D_Color32(10, 11, 14, 255));
    C2D_SceneBegin(topTarget);

    if (g_lastfm_login_active) {
        if (hasCoverCurrent) {
            C2D_Image cImg = { coverBlurTexCurrent.data ? &coverBlurTexCurrent : &coverTexCurrent, coverBlurTexCurrent.data ? &coverBlurSubtex : &coverSubtex };
            C2D_ImageTint bgTint;
            C2D_PlainImageTint(&bgTint, C2D_Color32(0, 0, 0, 255), 0.30f);
            C2D_DrawParams bgParams = {
                .pos    = { .x = -20.0f + coverBgPanX, .y = -20.0f + coverBgPanY,
                            .w = 440.0f, .h = 280.0f },
                .center = { 0.0f, 0.0f },
                .depth  = CFG_DEPTH_BG_BLUR,
                .angle  = 0.0f
            };
            C2D_DrawImage(cImg, &bgParams, &bgTint);
        }
        render_lastfm_login_screen();
        return;
    }

    if (hasCoverCurrent) {
        C2D_Image cImg = { coverBlurTexCurrent.data ? &coverBlurTexCurrent : &coverTexCurrent, coverBlurTexCurrent.data ? &coverBlurSubtex : &coverSubtex };
        C2D_ImageTint bgTint;
        
        C2D_PlainImageTint(&bgTint, C2D_Color32(0, 0, 0, 255), 0.30f);

        C2D_DrawParams bgParams = {
            .pos = {
                .x = -20.0f + coverBgPanX,
                .y = -20.0f + coverBgPanY,
                .w = 440.0f,
                .h = 280.0f
            },
            .center = { 0.0f, 0.0f },
            .depth = CFG_DEPTH_BG_BLUR,
            .angle = 0.0f
        };
        C2D_DrawImage(cImg, &bgParams, &bgTint);
        C2D_DrawRectSolid(0.0f, 0.0f, CFG_DEPTH_BG_DIMMER, 400.0f, 240.0f, C2D_Color32(8, 9, 12, 165));
    }

    float nlEase = noLyricsFocus * noLyricsFocus * (3.0f - 2.0f * noLyricsFocus);
    float lbEase = longBreakFocus * longBreakFocus * (3.0f - 2.0f * longBreakFocus);
    float idleEase = idleDimFocus * idleDimFocus * (3.0f - 2.0f * idleDimFocus);

    if (idleEase > 0.01f && lbEase < 0.99f) {
        C2D_DrawRectSolid(0.0f, 0.0f, 0.37f, 400.0f, 240.0f, C2D_Color32(0, 0, 0, (u32)(170.0f * idleEase * (1.0f - lbEase))));
    }

    float floatY = sinf(uiElapsedTime * 1.6f) * 3.5f * nlEase;
    float centerY = 86.0f + floatY;
    float halfSz = 61.0f + (beatPulseVal * 3.0f + fabsf(beatLpBass) * 6.0f) * lbEase;

    if (lbEase > 0.01f && hasCoverCurrent) {
        ensure_fx_textures();
        uint8_t hr = coverAvgR[slotCurrent];
        uint8_t hg = coverAvgG[slotCurrent];
        uint8_t hb = coverAvgB[slotCurrent];
        uint8_t hmax = hr > hg ? (hr > hb ? hr : hb) : (hg > hb ? hg : hb);
        if (hmax > 0 && hmax < 210) {
            float boost = 210.0f / (float)hmax;
            hr = (uint8_t)((float)hr * boost > 255.0f ? 255.0f : (float)hr * boost);
            hg = (uint8_t)((float)hg * boost > 255.0f ? 255.0f : (float)hg * boost);
            hb = (uint8_t)((float)hb * boost > 255.0f ? 255.0f : (float)hb * boost);
        }
        u8 haloAlpha = (u8)((185.0f + beatPulseVal * 70.0f) * lbEase);
        C2D_Image haloImg = { &haloGlowTex, &haloGlowSubtex };
        C2D_ImageTint haloTint;
        C2D_PlainImageTint(&haloTint, C2D_Color32(hr, hg, hb, haloAlpha), 1.0f);
        float haloSize = halfSz * (3.55f + beatPulseVal * 0.35f) * (0.75f + 0.25f * lbEase);
        C2D_DrawParams hp = {
            .pos = { 200.0f - haloSize * 0.5f, centerY - haloSize * 0.5f, haloSize, haloSize },
            .center = { 0.0f, 0.0f },
            .depth = 0.38f,
            .angle = 0.0f
        };
        C2D_DrawImage(haloImg, &hp, &haloTint);
    }

    const float Q_X0 = CFG_QUAD_X0 * (1.0f - nlEase) + (200.0f - halfSz) * nlEase;
    const float Q_Y0 = CFG_QUAD_Y0 * (1.0f - nlEase) + (centerY - halfSz) * nlEase;
    const float Q_X1 = CFG_QUAD_X1 * (1.0f - nlEase) + (200.0f + halfSz) * nlEase;
    const float Q_Y1 = CFG_QUAD_Y1 * (1.0f - nlEase) + (centerY - halfSz) * nlEase;
    const float Q_X2 = CFG_QUAD_X2 * (1.0f - nlEase) + (200.0f + halfSz) * nlEase;
    const float Q_Y2 = CFG_QUAD_Y2 * (1.0f - nlEase) + (centerY + halfSz) * nlEase;
    const float Q_X3 = CFG_QUAD_X3 * (1.0f - nlEase) + (200.0f - halfSz) * nlEase;
    const float Q_Y3 = CFG_QUAD_Y3 * (1.0f - nlEase) + (centerY + halfSz) * nlEase;
    float rEase = repeatFade * repeatFade * (3.0f - 2.0f * repeatFade);
    float peekFade = (1.0f - nlEase) * (1.0f - rEase);

    if (coverTransDir != TRANS_NONE && nlEase > 0.5f) {
        float t = coverTransProgress;
        float ease = t * t * (3.0f - 2.0f * t);
        float w = Q_X1 - Q_X0;
        float h = Q_Y3 - Q_Y0;

        if (hasCoverOld) {
            C2D_Image oImg = { &coverTexOld, &coverSubtex };
            C2D_ImageTint oTint;
            C2D_AlphaImageTint(&oTint, 1.0f - ease);
            C2D_DrawParams op = {
                .pos = { Q_X0, Q_Y0, w, h },
                .center = { 0.0f, 0.0f },
                .depth = CFG_DEPTH_TRANS_OLD,
                .angle = 0.0f
            };
            C2D_DrawImage(oImg, &op, &oTint);
        }
        if (hasCoverCurrent) {
            C2D_Image cImg = { &coverTexCurrent, &coverSubtex };
            C2D_ImageTint shTint;
            C2D_PlainImageTint(&shTint, C2D_Color32(0, 0, 0, CFG_COVER_SHADOW_ALPHA), 1.0f);
            C2D_DrawParams sp = {
                .pos = { Q_X0 + CFG_COVER_SHADOW_OFF_X, Q_Y0 + CFG_COVER_SHADOW_OFF_Y, w, h },
                .center = { 0.0f, 0.0f },
                .depth = CFG_DEPTH_COVER_SHADOW,
                .angle = 0.0f
            };
            C2D_DrawImage(cImg, &sp, &shTint);

            C2D_ImageTint cTint;
            C2D_AlphaImageTint(&cTint, ease);
            C2D_DrawParams cp = {
                .pos = { Q_X0, Q_Y0, w, h },
                .center = { 0.0f, 0.0f },
                .depth = CFG_DEPTH_COVER_MAIN,
                .angle = 0.0f
            };
            C2D_DrawImage(cImg, &cp, &cTint);
        }
    } else if (coverTransDir == TRANS_NEXT) {
        float t = coverTransProgress;
        bool isListFly = (coverScrollCount > 0 && coverScrollPos + 1 < coverScrollCount);
        bool fromFlat = (coverScrollCount > 0 && coverScrollPos > 0);
        float ease = isListFly ? t : (t * t * (3.0f - 2.0f * t));

        if (peekFade > 0.02f) {
            if (hasPeekNext2Tex && peekNext2SongIdx == deferredNextCoverIdx) {
                draw_perspective_cover_sub(&coverPeekNext2Tex, &cover64Subtex, CFG_PEEK_BOT_X, CFG_PEEK_BOT_Y, CFG_PEEK_COVER_SCALE * ease, 0.0f, 0.0f, CFG_DEPTH_PEEK_COVERS, C2D_Color32(130, 135, 145, (u32)(CFG_PEEK_COVER_ALPHA * ease * peekFade)));
            } else if (hasCoverNext && deferredNextCoverIdx < 0) {
                draw_perspective_cover(&coverTexNext, CFG_PEEK_BOT_X, CFG_PEEK_BOT_Y, CFG_PEEK_COVER_SCALE * ease, 0.0f, 0.0f, CFG_DEPTH_PEEK_COVERS, C2D_Color32(130, 135, 145, (u32)(CFG_PEEK_COVER_ALPHA * ease * peekFade)));
            }
        }

        if (hasCoverOld) {
            if (fromFlat) {
                float cy = 116.0f + (CFG_PEEK_TOP_Y - 116.0f) * ease;
                u32 oldAlpha = (u32)(CFG_PEEK_COVER_ALPHA * peekFade);
                draw_perspective_cover(&coverTexOld, CFG_PEEK_TOP_X, cy, CFG_PEEK_COVER_SCALE, 0.0f, 0.0f, CFG_DEPTH_TRANS_OLD, C2D_Color32(130, 135, 145, oldAlpha));
            } else {
                float ox0 = Q_X0 * (1.0f - ease) + (CFG_PEEK_TOP_X - CFG_COVER_HALF_SIZE) * ease;
                float oy0 = Q_Y0 * (1.0f - ease) + (CFG_PEEK_TOP_Y - CFG_COVER_HALF_SIZE) * ease;
                float ox1 = Q_X1 * (1.0f - ease) + (CFG_PEEK_TOP_X + CFG_COVER_HALF_SIZE) * ease;
                float oy1 = Q_Y1 * (1.0f - ease) + (CFG_PEEK_TOP_Y - CFG_COVER_HALF_SIZE) * ease;
                float ox2 = Q_X2 * (1.0f - ease) + (CFG_PEEK_TOP_X + CFG_COVER_HALF_SIZE) * ease;
                float oy2 = Q_Y2 * (1.0f - ease) + (CFG_PEEK_TOP_Y + CFG_COVER_HALF_SIZE) * ease;
                float ox3 = Q_X3 * (1.0f - ease) + (CFG_PEEK_TOP_X - CFG_COVER_HALF_SIZE) * ease;
                float oy3 = Q_Y3 * (1.0f - ease) + (CFG_PEEK_TOP_Y + CFG_COVER_HALF_SIZE) * ease;
                u32 oldAlpha = (u32)(255.0f * (1.0f - ease * 0.35f));

                draw_perspective_quad_cover(&coverTexOld,
                    ox0, oy0, ox1, oy1, ox2, oy2, ox3, oy3,
                    CFG_DEPTH_TRANS_OLD, C2D_Color32(255, 255, 255, oldAlpha));
            }
        }

        if (hasCoverCurrent) {
            if (isListFly) {
                float cy = CFG_PEEK_BOT_Y + (116.0f - CFG_PEEK_BOT_Y) * ease;
                draw_perspective_cover(&coverTexCurrent, CFG_PEEK_BOT_X, cy, CFG_PEEK_COVER_SCALE, 0.0f, 0.0f, CFG_DEPTH_COVER_MAIN, C2D_Color32(130, 135, 145, (u32)(CFG_PEEK_COVER_ALPHA * peekFade)));
            } else {
                float nx0 = (CFG_PEEK_BOT_X - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X0 * ease;
                float ny0 = (CFG_PEEK_BOT_Y - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y0 * ease;
                float nx1 = (CFG_PEEK_BOT_X + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X1 * ease;
                float ny1 = (CFG_PEEK_BOT_Y - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y1 * ease;
                float nx2 = (CFG_PEEK_BOT_X + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X2 * ease;
                float ny2 = (CFG_PEEK_BOT_Y + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y2 * ease;
                float nx3 = (CFG_PEEK_BOT_X - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X3 * ease;
                float ny3 = (CFG_PEEK_BOT_Y + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y3 * ease;

                draw_perspective_quad_cover(&coverTexCurrent,
                    nx0 + CFG_COVER_SHADOW_OFF_X, ny0 + CFG_COVER_SHADOW_OFF_Y,
                    nx1 + CFG_COVER_SHADOW_OFF_X, ny1 + CFG_COVER_SHADOW_OFF_Y,
                    nx2 + CFG_COVER_SHADOW_OFF_X, ny2 + CFG_COVER_SHADOW_OFF_Y,
                    nx3 + CFG_COVER_SHADOW_OFF_X, ny3 + CFG_COVER_SHADOW_OFF_Y,
                    CFG_DEPTH_COVER_SHADOW, C2D_Color32(0, 0, 0, (u32)(CFG_COVER_SHADOW_ALPHA * ease)));

                draw_perspective_quad_cover(&coverTexCurrent,
                    nx0, ny0, nx1, ny1, nx2, ny2, nx3, ny3,
                    CFG_DEPTH_COVER_MAIN, 0xFFFFFFFF);
            }
        }
    } else if (coverTransDir == TRANS_PREV) {
        float t = coverTransProgress;
        bool isListFly = (coverScrollCount > 0 && coverScrollPos + 1 < coverScrollCount);
        bool fromFlat = (coverScrollCount > 0 && coverScrollPos > 0);
        float ease = isListFly ? t : (t * t * (3.0f - 2.0f * t));

        if (peekFade > 0.02f) {
            if (hasPeekPrev2Tex && peekPrev2SongIdx == deferredPrevCoverIdx) {
                draw_perspective_cover_sub(&coverPeekPrev2Tex, &cover64Subtex, CFG_PEEK_TOP_X, CFG_PEEK_TOP_Y, CFG_PEEK_COVER_SCALE * ease, 0.0f, 0.0f, CFG_DEPTH_PEEK_COVERS, C2D_Color32(130, 135, 145, (u32)(CFG_PEEK_COVER_ALPHA * ease * peekFade)));
            } else if (hasCoverPrev && deferredPrevCoverIdx < 0) {
                draw_perspective_cover(&coverTexPrev, CFG_PEEK_TOP_X, CFG_PEEK_TOP_Y, CFG_PEEK_COVER_SCALE * ease, 0.0f, 0.0f, CFG_DEPTH_PEEK_COVERS, C2D_Color32(130, 135, 145, (u32)(CFG_PEEK_COVER_ALPHA * ease * peekFade)));
            }
        }
        if (hasCoverOld) {
            if (fromFlat) {
                float cy = 116.0f + (CFG_PEEK_BOT_Y - 116.0f) * ease;
                u32 oldAlpha = (u32)(CFG_PEEK_COVER_ALPHA * peekFade);
                draw_perspective_cover(&coverTexOld, CFG_PEEK_BOT_X, cy, CFG_PEEK_COVER_SCALE, 0.0f, 0.0f, CFG_DEPTH_TRANS_OLD, C2D_Color32(130, 135, 145, oldAlpha));
            } else {
                float ox0 = Q_X0 * (1.0f - ease) + (CFG_PEEK_BOT_X - CFG_COVER_HALF_SIZE) * ease;
                float oy0 = Q_Y0 * (1.0f - ease) + (CFG_PEEK_BOT_Y - CFG_COVER_HALF_SIZE) * ease;
                float ox1 = Q_X1 * (1.0f - ease) + (CFG_PEEK_BOT_X + CFG_COVER_HALF_SIZE) * ease;
                float oy1 = Q_Y1 * (1.0f - ease) + (CFG_PEEK_BOT_Y - CFG_COVER_HALF_SIZE) * ease;
                float ox2 = Q_X2 * (1.0f - ease) + (CFG_PEEK_BOT_X + CFG_COVER_HALF_SIZE) * ease;
                float oy2 = Q_Y2 * (1.0f - ease) + (CFG_PEEK_BOT_Y + CFG_COVER_HALF_SIZE) * ease;
                float ox3 = Q_X3 * (1.0f - ease) + (CFG_PEEK_BOT_X - CFG_COVER_HALF_SIZE) * ease;
                float oy3 = Q_Y3 * (1.0f - ease) + (CFG_PEEK_BOT_Y + CFG_COVER_HALF_SIZE) * ease;
                u32 oldAlpha = (u32)(255.0f * (1.0f - ease * 0.35f));

                draw_perspective_quad_cover(&coverTexOld,
                    ox0, oy0, ox1, oy1, ox2, oy2, ox3, oy3,
                    CFG_DEPTH_TRANS_OLD, C2D_Color32(255, 255, 255, oldAlpha));
            }
        }
        if (hasCoverCurrent) {
            if (isListFly) {
                float cy = CFG_PEEK_TOP_Y + (116.0f - CFG_PEEK_TOP_Y) * ease;
                draw_perspective_cover(&coverTexCurrent, CFG_PEEK_TOP_X, cy, CFG_PEEK_COVER_SCALE, 0.0f, 0.0f, CFG_DEPTH_COVER_MAIN, C2D_Color32(130, 135, 145, (u32)(CFG_PEEK_COVER_ALPHA * peekFade)));
            } else {
                float nx0 = (CFG_PEEK_TOP_X - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X0 * ease;
                float ny0 = (CFG_PEEK_TOP_Y - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y0 * ease;
                float nx1 = (CFG_PEEK_TOP_X + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X1 * ease;
                float ny1 = (CFG_PEEK_TOP_Y - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y1 * ease;
                float nx2 = (CFG_PEEK_TOP_X + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X2 * ease;
                float ny2 = (CFG_PEEK_TOP_Y + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y2 * ease;
                float nx3 = (CFG_PEEK_TOP_X - CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_X3 * ease;
                float ny3 = (CFG_PEEK_TOP_Y + CFG_COVER_HALF_SIZE) * (1.0f - ease) + Q_Y3 * ease;

                draw_perspective_quad_cover(&coverTexCurrent,
                    nx0 + CFG_COVER_SHADOW_OFF_X, ny0 + CFG_COVER_SHADOW_OFF_Y,
                    nx1 + CFG_COVER_SHADOW_OFF_X, ny1 + CFG_COVER_SHADOW_OFF_Y,
                    nx2 + CFG_COVER_SHADOW_OFF_X, ny2 + CFG_COVER_SHADOW_OFF_Y,
                    nx3 + CFG_COVER_SHADOW_OFF_X, ny3 + CFG_COVER_SHADOW_OFF_Y,
                    CFG_DEPTH_COVER_SHADOW, C2D_Color32(0, 0, 0, (u32)(CFG_COVER_SHADOW_ALPHA * ease)));

                draw_perspective_quad_cover(&coverTexCurrent,
                    nx0, ny0, nx1, ny1, nx2, ny2, nx3, ny3,
                    CFG_DEPTH_COVER_MAIN, 0xFFFFFFFF);
            }
        }

    } else {
        if (peekFade > 0.02f) {
            if (deferredPrevCoverIdx >= 0 && hasPeekPrev2Tex && peekPrev2SongIdx == deferredPrevCoverIdx) {
                draw_perspective_cover_sub(&coverPeekPrev2Tex, &cover64Subtex, CFG_PEEK_TOP_X, CFG_PEEK_TOP_Y, CFG_PEEK_COVER_SCALE, 0.0f, 0.0f, CFG_DEPTH_PEEK_COVERS, C2D_Color32(130, 135, 145, (u32)(CFG_PEEK_COVER_ALPHA * peekFade)));
            } else if (hasCoverPrev) {
                draw_perspective_cover(&coverTexPrev, CFG_PEEK_TOP_X, CFG_PEEK_TOP_Y, CFG_PEEK_COVER_SCALE, 0.0f, 0.0f, CFG_DEPTH_PEEK_COVERS, C2D_Color32(130, 135, 145, (u32)(CFG_PEEK_COVER_ALPHA * peekFade)));
            }
        }
        
        if (peekFade > 0.02f) {
            if (deferredNextCoverIdx >= 0 && hasPeekNext2Tex && peekNext2SongIdx == deferredNextCoverIdx) {
                draw_perspective_cover_sub(&coverPeekNext2Tex, &cover64Subtex, CFG_PEEK_BOT_X, CFG_PEEK_BOT_Y, CFG_PEEK_COVER_SCALE, 0.0f, 0.0f, CFG_DEPTH_PEEK_COVERS, C2D_Color32(130, 135, 145, (u32)(CFG_PEEK_COVER_ALPHA * peekFade)));
            } else if (hasCoverNext) {
                draw_perspective_cover(&coverTexNext, CFG_PEEK_BOT_X, CFG_PEEK_BOT_Y, CFG_PEEK_COVER_SCALE, 0.0f, 0.0f, CFG_DEPTH_PEEK_COVERS, C2D_Color32(130, 135, 145, (u32)(CFG_PEEK_COVER_ALPHA * peekFade)));
            }
        }
        
        if (hasCoverCurrent) {
            if (nlEase >= 0.999f) {
                C2D_Image cImg = { &coverTexCurrent, &coverSubtex };
                C2D_ImageTint shTint;
                C2D_PlainImageTint(&shTint, C2D_Color32(0, 0, 0, CFG_COVER_SHADOW_ALPHA), 1.0f);
                C2D_DrawParams sp = {
                    .pos = { Q_X0 + CFG_COVER_SHADOW_OFF_X, Q_Y0 + CFG_COVER_SHADOW_OFF_Y, Q_X1 - Q_X0, Q_Y3 - Q_Y0 },
                    .center = { 0.0f, 0.0f },
                    .depth = CFG_DEPTH_COVER_SHADOW,
                    .angle = 0.0f
                };
                C2D_DrawImage(cImg, &sp, &shTint);

                C2D_DrawParams cp = {
                    .pos = { Q_X0, Q_Y0, Q_X1 - Q_X0, Q_Y3 - Q_Y0 },
                    .center = { 0.0f, 0.0f },
                    .depth = CFG_DEPTH_COVER_MAIN,
                    .angle = 0.0f
                };
                C2D_DrawImage(cImg, &cp, NULL);
            } else {
                draw_perspective_quad_cover(&coverTexCurrent,
                    Q_X0 + CFG_COVER_SHADOW_OFF_X, Q_Y0 + CFG_COVER_SHADOW_OFF_Y,
                    Q_X1 + CFG_COVER_SHADOW_OFF_X, Q_Y1 + CFG_COVER_SHADOW_OFF_Y,
                    Q_X2 + CFG_COVER_SHADOW_OFF_X, Q_Y2 + CFG_COVER_SHADOW_OFF_Y,
                    Q_X3 + CFG_COVER_SHADOW_OFF_X, Q_Y3 + CFG_COVER_SHADOW_OFF_Y,
                    CFG_DEPTH_COVER_SHADOW, C2D_Color32(0, 0, 0, CFG_COVER_SHADOW_ALPHA));

                draw_perspective_quad_cover(&coverTexCurrent,
                    Q_X0, Q_Y0,
                    Q_X1, Q_Y1,
                    Q_X2, Q_Y2,
                    Q_X3, Q_Y3,
                    CFG_DEPTH_COVER_MAIN, 0xFFFFFFFF);
            }
        }
    }
    
    if (lbEase > 0.01f) {
        ensure_fx_textures();
        float ang = uiElapsedTime * 1.15f;
        float orbitX = 42.0f + 14.0f * lbEase;
        float orbitY = 34.0f + 12.0f * lbEase;
        float lx1 = 200.0f + cosf(ang) * orbitX;
        float ly1 = centerY + sinf(ang) * orbitY;
        float lx2 = 200.0f + cosf(ang + 3.14159265f) * orbitX;
        float ly2 = centerY + sinf(ang + 3.14159265f) * orbitY;
        float r = (56.0f + beatPulseVal * 22.0f + fabsf(beatLpBass) * 14.0f) * lbEase;
        if (r < 2.0f) r = 2.0f;
        float maskAlpha = 170.0f * ((idleEase > lbEase) ? idleEase : lbEase);

        update_iris_mask_tex(lx1, ly1, lx2, ly2, r, (uint8_t)maskAlpha);
        C2D_Image irisImg = { &irisMaskTex, &irisMaskSubtex };
        C2D_DrawParams ip = {
            .pos = { 0.0f, 0.0f, 400.0f, 240.0f },
            .center = { 0.0f, 0.0f },
            .depth = 0.47f,
            .angle = 0.0f
        };
        C2D_DrawImage(irisImg, &ip, NULL);
    }

    time_t rawtime;
    struct tm* ti;
    time(&rawtime);
    ti = localtime(&rawtime);
    char timeStr[32];
    int hour12 = ti->tm_hour % 12;
    if (hour12 == 0) hour12 = 12;
    snprintf(timeStr, sizeof(timeStr), "%d:%02d %s", hour12, ti->tm_min, (ti->tm_hour >= 12) ? "PM" : "AM");
    draw_custom_font(CFG_CLOCK_X, CFG_CLOCK_Y, CFG_CLOCK_SCALE, C2D_Color32(255, 255, 255, 255), timeStr);

    int wifiIcon = isWifiConnected
        ? ((global10FpsFrame == 0) ? sprites_wifi1_idx : sprites_wifi2_idx)
        : ((global10FpsFrame == 0) ? sprites_nowifi1_idx : sprites_nowifi2_idx);
    draw_sprite_centered(wifiIcon, CFG_WIFI_ICON_X, CFG_WIFI_ICON_Y, CFG_WIFI_ICON_SCALE, CFG_WIFI_ICON_SCALE);

    if (nlEase > 0.02f) {
        if (lyricsEnabled && lrcLineCount > 0 && currentLrcIndex >= 0 && lrcLines[currentLrcIndex].text[0] != '\0') {
            float scrollPos = (lyricScrollPos >= 0.0f) ? lyricScrollPos : (float)currentLrcIndex;
            float rel = (float)currentLrcIndex - scrollPos;
            if (rel > 1.0f) rel = 1.0f;
            if (rel < -1.0f) rel = -1.0f;

            if (currentLrcIndex > 0 && rel > 0.04f && lrcLines[currentLrcIndex - 1].text[0] != '\0') {
                float outAlpha = 255.0f * rel * nlEase;
                float outY = 160.0f - (1.0f - rel) * 10.0f;
                float outScale = 0.50f - (1.0f - rel) * 0.06f;
                draw_wrapped_lyrics(200.0f, outY, 320.0f, outScale, C2D_Color32(255, 255, 255, (u32)outAlpha), lrcLines[currentLrcIndex - 1].text);
            }

            float inAlpha = (1.0f - fabsf(rel) * 0.75f) * 255.0f * nlEase;
            float inY = 160.0f + rel * 10.0f;
            float inScale = 0.50f - fabsf(rel) * 0.05f + beatPulseVal * 0.015f;
            draw_wrapped_lyrics(200.0f, inY, 320.0f, inScale, C2D_Color32(255, 255, 255, (u32)inAlpha), lrcLines[currentLrcIndex].text);
        } else {
            u32 aCol = C2D_Color32(255, 255, 255, (u32)(255.0f * nlEase));
            int songIcon = (global10FpsFrame == 0) ? sprites_song1_idx : sprites_song2_idx;
            float titleScale = 0.56f;
            float titleW = measure_custom_font(titleScale, currentTrackTitle);
            float maxTextW = 240.0f;
            float shownW = (titleW < maxTextW) ? titleW : maxTextW;
            float totalW = 20.0f + shownW;
            float startX = 200.0f - totalW * 0.5f;

            draw_sprite_centered(songIcon, startX + 8.0f, 169.0f, 0.65f, 0.65f);
            draw_marquee_font(startX + 20.0f, 158.0f, maxTextW, titleScale, aCol, currentTrackTitle, uiElapsedTime);
        }
    }

    if (nlEase < 0.99f && lyricsEnabled && lrcLineCount > 0 && currentLrcIndex >= 0 && lrcLines[currentLrcIndex].text[0] != '\0') {
        float scrollPos = (lyricScrollPos >= 0.0f) ? lyricScrollPos : (float)currentLrcIndex;
        int centerIdx = (int)floorf(scrollPos + 0.5f);
        float lineStep = CFG_LYRICS_NEXT_Y - CFG_LYRICS_ACTIVE_Y;

        for (int i = centerIdx - 2; i <= centerIdx + 2; i++) {
            if (i < 0 || i >= lrcLineCount) continue;

            float rel = (float)i - scrollPos;
            if (rel <= -1.7f || rel >= 1.8f) continue;

            float y = CFG_LYRICS_ACTIVE_Y + rel * lineStep;
            float scale;
            float alpha;

            if (rel < -1.0f) {
                float t = (-rel - 1.0f) / 0.7f;
                if (t > 1.0f) t = 1.0f;
                scale = CFG_LYRICS_SUB_SCALE * (1.0f - 0.15f * t);
                alpha = 120.0f * (1.0f - t);
            } else if (rel < 0.0f) {
                float t = -rel;
                scale = CFG_LYRICS_ACTIVE_SCALE + (CFG_LYRICS_SUB_SCALE - CFG_LYRICS_ACTIVE_SCALE) * t;
                alpha = 255.0f + (120.0f - 255.0f) * t;
            } else if (rel <= 1.0f) {
                float t = rel;
                scale = CFG_LYRICS_ACTIVE_SCALE + (CFG_LYRICS_SUB_SCALE - CFG_LYRICS_ACTIVE_SCALE) * t;
                alpha = 255.0f + (140.0f - 255.0f) * t;
            } else {
                float t = (rel - 1.0f) / 0.8f;
                if (t > 1.0f) t = 1.0f;
                scale = CFG_LYRICS_SUB_SCALE * (1.0f - 0.15f * t);
                alpha = 140.0f * (1.0f - t);
            }

            if (alpha <= 1.0f) continue;
            u32 color = C2D_Color32(255, 255, 255, (u32)alpha);

            if (lrcLines[i].text[0] != '\0') {
                draw_wrapped_lyrics(CFG_LYRICS_CENTER_X, y, CFG_LYRICS_MAX_WIDTH, scale, color, lrcLines[i].text);
            }
        }
    } else if (nlEase < 0.99f && (!lyricsEnabled || lrcLineCount == 0 || currentLrcIndex < 0)) {
        
        int songIcon   = (global10FpsFrame == 0) ? sprites_song1_idx : sprites_song2_idx;
        int artistIcon = (global10FpsFrame == 0) ? sprites_artist1_idx : sprites_artist2_idx;
        int albumIcon  = (global10FpsFrame == 0) ? sprites_album1_idx : sprites_album2_idx;

        
        draw_sprite_centered(songIcon, CFG_META_ICONS_X, CFG_META_ICON_SONG_Y, CFG_META_ICONS_SCALE, CFG_META_ICONS_SCALE);
        draw_marquee_font(CFG_META_TEXT_X, CFG_META_TITLE_Y, CFG_META_TEXT_WIDTH, CFG_META_TITLE_SCALE, C2D_Color32(255, 255, 255, 255), currentTrackTitle, uiElapsedTime);

        
        draw_sprite_centered(artistIcon, CFG_META_ICONS_X, CFG_META_ICON_ARTIST_Y, CFG_META_ICONS_SCALE, CFG_META_ICONS_SCALE);
        draw_marquee_font(CFG_META_TEXT_X, CFG_META_ARTIST_Y, CFG_META_TEXT_WIDTH, CFG_META_ARTIST_SCALE, C2D_Color32(255, 255, 255, 255), currentTrackArtist, uiElapsedTime);

        
        draw_sprite_centered(albumIcon, CFG_META_ICONS_X, CFG_META_ICON_ALBUM_Y, CFG_META_ICONS_SCALE, CFG_META_ICONS_SCALE);
        draw_marquee_font(CFG_META_TEXT_X, CFG_META_ALBUM_Y, CFG_META_TEXT_WIDTH, CFG_META_ALBUM_SCALE, C2D_Color32(255, 255, 255, 255), currentTrackAlbum, uiElapsedTime);
    }

    float waveCenterY = CFG_WAVE_CENTER_Y;
    float progress = (songDuration > 0.0f) ? (songTime / songDuration) : 0.0f;
    if (progress > 1.0f) progress = 1.0f;
    int activeCols = (int)(progress * (float)NUM_WAVE_SAMPLES);

    for (int i = 0; i < NUM_WAVE_SAMPLES; i += CFG_WAVE_BAR_STEP) {
        float colX = CFG_WAVE_START_X + (float)i;
        float h = fullSongWaveform[i] * CFG_WAVE_MAX_HEIGHT;
        if (h < 1.5f) h = 1.5f;

        u32 col = (i <= activeCols) ? C2D_Color32(255, 255, 255, 255) : C2D_Color32(110, 115, 130, 140);
        C2D_DrawRectSolid(colX, waveCenterY - h, CFG_DEPTH_WAVEFORM, CFG_WAVE_BAR_WIDTH, h * 2.0f, col);
    }

    if (activeCols >= 0 && activeCols < NUM_WAVE_SAMPLES) {
        float playheadX = CFG_WAVE_START_X + (float)activeCols;
        float phH = fullSongWaveform[activeCols] * CFG_WAVE_MAX_HEIGHT + 3.0f;
        C2D_DrawRectSolid(playheadX - 0.5f, waveCenterY - phH, CFG_DEPTH_PLAYHEAD, CFG_PLAYHEAD_WIDTH, phH * 2.0f, C2D_Color32(255, 255, 255, 255));
    }

    int minE = (int)songTime / 60;
    int secE = (int)songTime % 60;
    int minT = (int)songDuration / 60;
    int secT = (int)songDuration % 60;
    char timeStrFull[32];
    snprintf(timeStrFull, sizeof(timeStrFull), "%d:%02d/%d:%02d", minE, secE, minT, secT);
    draw_custom_font(CFG_TIME_COUNTER_X, CFG_TIME_COUNTER_Y, CFG_TIME_COUNTER_SCALE, C2D_Color32(255, 255, 255, 255), timeStrFull);
}

void render_bottom_screen(void) {
    C2D_TargetClear(botTarget, C2D_Color32(185, 150, 115, 255));
    C2D_SceneBegin(botTarget);

    C2D_Image imgBg = C2D_SpriteSheetGetImage(spriteSheet, sprites_bg_idx);
    int startX = ((int)bgScrollX % 100) - 100;
    int startY = ((int)bgScrollY % 100) - 100;

    for (int x = startX; x < 320; x += 100) {
        for (int y = startY; y < 240; y += 100) {
            C2D_DrawImageAt(imgBg, (float)x, (float)y, 0.1f, NULL, 1.0f, 1.0f);
        }
    }

    int gustinaSpriteIdx = sprites_gustinaNothing1_idx;

    if (!isPlaying || isPaused) {
        gustinaSpriteIdx = (global10FpsFrame == 0) ? sprites_gustinaNothing1_idx : sprites_gustinaNothing2_idx;
    } else {
        if (gustinaState == GUSTINA_BOP_LEFT) {
            if (gustinaFrame == 0) gustinaSpriteIdx = sprites_gustinaBopLeft1_idx;
            else if (gustinaFrame == 1) gustinaSpriteIdx = sprites_gustinaBopLeft2_idx;
            else gustinaSpriteIdx = sprites_gustinaBopLeft3_idx;
        } else if (gustinaState == GUSTINA_BOP_RIGHT) {
            if (gustinaFrame == 0) gustinaSpriteIdx = sprites_gustinaBopRight1_idx;
            else if (gustinaFrame == 1) gustinaSpriteIdx = sprites_gustinaBopRight2_idx;
            else gustinaSpriteIdx = sprites_gustinaBopRight3_idx;
        } else {
            gustinaSpriteIdx = (global10FpsFrame == 0) ? sprites_gustinaIdle1_idx : sprites_gustinaIdle2_idx;
        }
    }

    int customIdx = -1;
    if (gustinaSpriteIdx == sprites_gustinaNothing1_idx) customIdx = 0;
    else if (gustinaSpriteIdx == sprites_gustinaNothing2_idx) customIdx = 1;
    else if (gustinaSpriteIdx == sprites_gustinaIdle1_idx) customIdx = 2;
    else if (gustinaSpriteIdx == sprites_gustinaIdle2_idx) customIdx = 3;
    else if (gustinaSpriteIdx == sprites_gustinaBopLeft1_idx) customIdx = 4;
    else if (gustinaSpriteIdx == sprites_gustinaBopLeft2_idx) customIdx = 5;
    else if (gustinaSpriteIdx == sprites_gustinaBopLeft3_idx) customIdx = 6;
    else if (gustinaSpriteIdx == sprites_gustinaBopRight1_idx) customIdx = 7;
    else if (gustinaSpriteIdx == sprites_gustinaBopRight2_idx) customIdx = 8;
    else if (gustinaSpriteIdx == sprites_gustinaBopRight3_idx) customIdx = 9;

    float gustinaX = (bottomScreenMode == 1) ? (CFG_GUSTINA_X + 18.0f) : CFG_GUSTINA_X;

    if (customIdx >= 0 && hasCustomGustina[customIdx]) {
        C2D_Image customImg = { &customGustinaTex[customIdx], &customGustinaSubtex[customIdx] };
        float sh = (customGustinaSubtex[customIdx].height > 0) ? (float)customGustinaSubtex[customIdx].height : 198.0f;
        float sw = (customGustinaSubtex[customIdx].width  > 0) ? (float)customGustinaSubtex[customIdx].width  : 160.0f;
        float drawScale = (198.0f * CFG_GUSTINA_SCALE_Y) / sh;
        float w = sw * drawScale;
        float h = sh * drawScale;
        C2D_DrawImageAt(customImg, gustinaX - (w * 0.5f), CFG_GUSTINA_Y - (h * 0.5f), 0.70f, NULL, drawScale, drawScale);
    } else {
        draw_sprite_centered(gustinaSpriteIdx, gustinaX, CFG_GUSTINA_Y, CFG_GUSTINA_SCALE_X, CFG_GUSTINA_SCALE_Y);
    }

    if (bottomScreenMode == 0) {
        draw_sprite_centered(wheelTouchSprite, CFG_WHEEL_CENTER_X, CFG_WHEEL_CENTER_Y, CFG_WHEEL_SCALE, CFG_WHEEL_SCALE);
        int repIcon = (global10FpsFrame == 0) ? sprites_repeat1_idx : sprites_repeat2_idx;
        C2D_ImageTint repTint;
        if (repeatEnabled) C2D_PlainImageTint(&repTint, C2D_Color32(120, 255, 170, 255), 0.35f);
        else               C2D_AlphaImageTint(&repTint, 0.35f);
        draw_sprite_centered_tinted(repIcon, CFG_REPEAT_ICON_X, CFG_REPEAT_ICON_Y, CFG_REPEAT_ICON_SCALE, CFG_REPEAT_ICON_SCALE, &repTint);

        int shufIcon = (global10FpsFrame == 0) ? sprites_suffle1_idx : sprites_suffle2_idx;
        C2D_ImageTint shufTint;
        if (shuffleEnabled) C2D_PlainImageTint(&shufTint, C2D_Color32(120, 255, 170, 255), 0.35f);
        else                C2D_AlphaImageTint(&shufTint, 0.35f);
        draw_sprite_centered_tinted(shufIcon, CFG_SHUFFLE_ICON_X, CFG_SHUFFLE_ICON_Y, CFG_SHUFFLE_ICON_SCALE, CFG_SHUFFLE_ICON_SCALE, &shufTint);

        if (bottomWaveThingyMode) {
            float barX = 14.0f;
            float barW = 292.0f;
            float barCenterY = 220.0f;
            float barMaxH = 14.0f;
            C2D_DrawRectSolid(8.0f, 202.0f, 0.72f, 304.0f, 34.0f, C2D_Color32(18, 16, 24, 155));

            float progress = (songDuration > 0.0f) ? (songTime / songDuration) : 0.0f;
            if (progress < 0.0f) progress = 0.0f;
            if (progress > 1.0f) progress = 1.0f;
            int totalCols = (int)(barW / 4.0f);
            int activeCol = (int)(progress * (float)totalCols);

            for (int c = 0; c < totalCols; c++) {
                int wIdx = (c * NUM_WAVE_SAMPLES) / totalCols;
                if (wIdx >= NUM_WAVE_SAMPLES) wIdx = NUM_WAVE_SAMPLES - 1;
                float h = fullSongWaveform[wIdx] * barMaxH;
                if (h < 1.5f) h = 1.5f;
                float cx = barX + (float)(c * 4);
                u32 col = (c <= activeCol) ? C2D_Color32(255, 255, 255, 255) : C2D_Color32(130, 135, 150, 145);
                C2D_DrawRectSolid(cx, barCenterY - h, 0.76f, 2.8f, h * 2.0f, col);
            }
            float phX = barX + progress * barW;
            C2D_DrawRectSolid(phX - 1.0f, barCenterY - (barMaxH + 2.0f), 0.78f, 2.5f, (barMaxH + 2.0f) * 2.0f, C2D_Color32(120, 235, 255, 255));
        }

        int volIcon = (global10FpsFrame == 0) ? sprites_volume1_idx : sprites_volume2_idx;
        draw_sprite_centered(volIcon, CFG_VOL_ICON_X, CFG_VOL_ICON_Y, CFG_VOL_ICON_SCALE, CFG_VOL_ICON_SCALE);

        char volTxt[32];
        int pct = (int)(volumeGain * 100.0f);
        snprintf(volTxt, sizeof(volTxt), "%d%%%s", pct, pct > 100 ? "" : "");
        u32 vCol = (pct > 100) ? C2D_Color32(240, 45, 45, 255) : C2D_Color32(255, 255, 255, 255);
        draw_custom_font(CFG_VOL_TEXT_X, CFG_VOL_TEXT_Y, CFG_VOL_TEXT_SCALE, vCol, volTxt);

        if (!bottomWaveThingyMode) {
            float rVis = 1.0f - (repeatFade * repeatFade * (3.0f - 2.0f * repeatFade));
            u8 nextAlpha = (u8)(255.0f * rVis);
            if (nextAlpha > 4) {
                int nextIcon = (global10FpsFrame == 0) ? sprites_nextSong1_idx : sprites_nextSong2_idx;
                C2D_ImageTint nextTint;
                C2D_AlphaImageTint(&nextTint, rVis);
                draw_sprite_centered_tinted(nextIcon, CFG_NEXT_ICON_X, CFG_NEXT_ICON_Y, CFG_NEXT_ICON_SCALE, CFG_NEXT_ICON_SCALE, &nextTint);

                int nextIdx = find_next_song_index(selectedFile, 1);
                u32 nextCol = C2D_Color32(255, 255, 255, nextAlpha);
                if (shuffleEnabled) {
                    if (upcomingShuffleIdx < 0 || upcomingShuffleIdx >= fileCount || fileList[upcomingShuffleIdx].is_dir ||
                        (fileCount > 1 && upcomingShuffleIdx == selectedFile)) {
                        upcomingShuffleIdx = find_random_song_index(selectedFile);
                    }
                    if (upcomingShuffleIdx >= 0) {
                        draw_marquee_font(CFG_NEXT_TEXT_X, CFG_NEXT_TEXT_Y, CFG_NEXT_TEXT_WIDTH, CFG_NEXT_TEXT_SCALE, nextCol, get_next_song_display(upcomingShuffleIdx, false), uiElapsedTime);
                    }
                } else if (nextIdx >= 0) {
                    draw_marquee_font(CFG_NEXT_TEXT_X, CFG_NEXT_TEXT_Y, CFG_NEXT_TEXT_WIDTH, CFG_NEXT_TEXT_SCALE, nextCol, get_next_song_display(nextIdx, false), uiElapsedTime);
                }
            }

            int lfmIcon = (global10FpsFrame == 0) ? sprites_lastfm1_idx : sprites_lastfm2_idx;
            if (lastfm_has_session) {
                C2D_ImageTint lfmTint;
                C2D_PlainImageTint(&lfmTint, C2D_Color32(90, 255, 130, 255), 0.70f);
                draw_sprite_centered_tinted(lfmIcon, CFG_LASTFM_BTN_X, CFG_LASTFM_BTN_Y,
                                            CFG_LASTFM_BTN_SCALE, CFG_LASTFM_BTN_SCALE, &lfmTint);
            } else {
                draw_sprite_centered(lfmIcon, CFG_LASTFM_BTN_X, CFG_LASTFM_BTN_Y,
                                     CFG_LASTFM_BTN_SCALE, CFG_LASTFM_BTN_SCALE);
            }
        }

        draw_custom_font(10.0f, 5.0f, 0.40f, C2D_Color32(255, 255, 255, 255), "Y: Files");
        u32 lrcToggleCol = lyricsEnabled ? C2D_Color32(255, 255, 255, 255) : C2D_Color32(180, 180, 180, 190);
        char lrcLabel[48];
        if (lyricsEnabled && lyricsDebugStatus[0] != '\0') {
            snprintf(lrcLabel, sizeof(lrcLabel), "X: LRC [ON] (%s)", lyricsDebugStatus);
        } else {
            snprintf(lrcLabel, sizeof(lrcLabel), lyricsEnabled ? "X: LRC [ON]" : "X: LRC [OFF]");
        }
        draw_custom_font(92.0f, 5.0f, 0.40f, lrcToggleCol, lrcLabel);

        u32 barBtnCol = bottomWaveThingyMode ? C2D_Color32(255, 255, 255, 255) : C2D_Color32(255, 255, 255, 255);
        draw_custom_font(10.0f, 18.0f, 0.40f, barBtnCol, bottomWaveThingyMode ? "R: Info" : "R: Bar");

    } else {
        int folderIcon = (global10FpsFrame == 0) ? sprites_folder1_idx : sprites_folder2_idx;
        draw_sprite_centered(folderIcon, CFG_LIST_FOLDER_ICON_X, CFG_LIST_FOLDER_ICON_Y, 0.55f, 0.55f);

        char dirShort[48];
        utf8_truncate_chars(currentDir, dirShort, sizeof(dirShort), 18);
        draw_custom_font(CFG_LIST_DIR_TEXT_X, CFG_LIST_DIR_TEXT_Y, 0.34f, C2D_Color32(255, 255, 255, 255), dirShort);

        u32 searchBoxCol = (searchFilter[0] != '\0') ? C2D_Color32(60, 175, 145, 210) : C2D_Color32(35, 35, 45, 175);
        C2D_DrawRectSolid(226.0f, 4.0f, 0.45f, 88.0f, 20.0f, searchBoxCol);
        draw_custom_font(232.0f, 220.0f, 0.40f, C2D_Color32(255, 255, 255, 255), "SEL: Search");

        if (searchFilter[0] != '\0') {
            char fPill[48];
            snprintf(fPill, sizeof(fPill), "Search filter: %s", searchFilter);
            char fTrunc[40];
            utf8_truncate_chars(fPill, fTrunc, sizeof(fTrunc), 24);
            draw_custom_font(30.0f, 28.0f, 0.42f, C2D_Color32(140, 255, 210, 255), fTrunc);
        }

        int cursorIcon = (global10FpsFrame == 0) ? sprites_cursor1_idx : sprites_cursor2_idx;

        if (fileCount == 0) {
            draw_custom_font(24.0f, 80.0f, 0.38f, C2D_Color32(255, 220, 160, 255), "nothingburger :(");
        }
        for (int i = 0; i < CFG_LIST_VISIBLE_ROWS; i++) {
            int idx = scrollOffset + i;
            float rowY = CFG_LIST_ROW_START_Y + (i * CFG_LIST_ROW_HEIGHT);
            if (idx < fileCount) {
                char rowPath[1024];
                if (strcmp(currentDir, "sdmc:/") == 0) snprintf(rowPath, sizeof(rowPath), "sdmc:/%s", fileList[idx].name);
                else snprintf(rowPath, sizeof(rowPath), "%s/%s", currentDir, fileList[idx].name);
                bool isActiveTrack = (isPlaying && !fileList[idx].is_dir && strcmp(currentTrackPath, rowPath) == 0);

                if (isActiveTrack && idx != selectedFile) {
                    C2D_DrawRectSolid(CFG_LIST_BOX_X, rowY - 8.0f, 0.35f, CFG_LIST_BOX_WIDTH, CFG_LIST_BOX_HEIGHT, C2D_Color32(60, 180, 140, 95));
                }
                if (idx == selectedFile) {
                    u32 selCol = isActiveTrack ? C2D_Color32(175, 240, 210, 200) : C2D_Color32(245, 230, 210, 185);
                    C2D_DrawRectSolid(CFG_LIST_BOX_X, rowY - 8.0f, 0.4f, CFG_LIST_BOX_WIDTH, CFG_LIST_BOX_HEIGHT, selCol);
                    draw_sprite_centered(cursorIcon, CFG_LIST_CURSOR_X, rowY + 6.0f, CFG_LIST_CURSOR_SCALE, CFG_LIST_CURSOR_SCALE);
                }

                u32 col = fileList[idx].is_dir ? C2D_Color32(255, 220, 100, 255) :
                          (isActiveTrack ? C2D_Color32(130, 255, 190, 255) : C2D_Color32(255, 255, 255, 255));
                char dispName[256];
                if (fileList[idx].is_dir) snprintf(dispName, sizeof(dispName), "%s", fileList[idx].name);
                else strip_extension_copy(fileList[idx].name, dispName, sizeof(dispName));

                if (idx == selectedFile) {
                    draw_marquee_font(30.0f, rowY - 1.0f, CFG_LIST_BOX_WIDTH - 28.0f, CFG_LIST_TEXT_SCALE, col, dispName, uiElapsedTime);
                } else {
                    char lineName[64];
                    utf8_truncate_chars(dispName, lineName, sizeof(lineName), 20);
                    draw_custom_font(30.0f, rowY, CFG_LIST_TEXT_SCALE, col, lineName);
                }
            }
        }
        if (fileCount > CFG_LIST_VISIBLE_ROWS) {
            float barTop = CFG_LIST_ROW_START_Y - 6.0f;
            float barH = (float)CFG_LIST_VISIBLE_ROWS * CFG_LIST_ROW_HEIGHT - 4.0f;
            C2D_DrawRectSolid(CFG_LIST_SCROLL_X, barTop, 0.45f, 4.0f, barH, C2D_Color32(0, 0, 0, 110));

            float thumbH = barH * ((float)CFG_LIST_VISIBLE_ROWS / (float)fileCount);
            if (thumbH < 16.0f) thumbH = 16.0f;
            int maxScroll = fileCount - CFG_LIST_VISIBLE_ROWS;
            float ratio = (maxScroll > 0) ? ((float)scrollOffset / (float)maxScroll) : 0.0f;
            float thumbY = barTop + ratio * (barH - thumbH);
            C2D_DrawRectSolid(CFG_LIST_SCROLL_X, thumbY, 0.5f, 4.0f, thumbH, C2D_Color32(255, 255, 255, 220));
        }

        draw_custom_font(10.0f, 220.0f, 0.45f, C2D_Color32(255, 255, 255, 255), "Y: Back");
        char countStr[32];
        snprintf(countStr, sizeof(countStr), "%d/%d L/R", (fileCount > 0 ? selectedFile + 1 : 0), fileCount);
        draw_custom_font(105.0f, 220.0f, 0.45f, C2D_Color32(220, 220, 220, 220), countStr);
    }

    float dimFocus = (longBreakFocus > idleDimFocus) ? longBreakFocus : idleDimFocus;
    if (dimFocus > 0.01f) {
        float dEase = dimFocus * dimFocus * (3.0f - 2.0f * dimFocus);
        C2D_DrawRectSolid(0.0f, 0.0f, 0.92f, 320.0f, 240.0f, C2D_Color32(0, 0, 0, (u32)(155.0f * dEase)));
    }
}
void open_3ds_keyboard_thingy(u64* lastTickPtr) {
    SwkbdState swkbd;
    char inputBuf[64];
    strncpy(inputBuf, searchFilter, sizeof(inputBuf) - 1);
    inputBuf[sizeof(inputBuf) - 1] = '\0';

    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, 60);
    swkbdSetInitialText(&swkbd, inputBuf);
    swkbdSetHintText(&swkbd, "Search...");
    swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Clear", false);
    swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "Search", true);
    swkbdSetFeatures(&swkbd, SWKBD_DARKEN_TOP_SCREEN | SWKBD_ALLOW_HOME);

    SwkbdButton btn = swkbdInputText(&swkbd, inputBuf, sizeof(inputBuf));
    if (btn == SWKBD_BUTTON_RIGHT) {
        strncpy(searchFilter, inputBuf, sizeof(searchFilter) - 1);
        searchFilter[sizeof(searchFilter) - 1] = '\0';
        apply_file_filter();
        refresh_the_3_covers();
    } else if (btn == SWKBD_BUTTON_LEFT) {
        searchFilter[0] = '\0';
        apply_file_filter();
        refresh_the_3_covers();
    }
    if (lastTickPtr) *lastTickPtr = osGetTime();
}

void load_gustina_mod_slop(void) {
    const char* filenames[10] = {
        "gustinaNothing1.png", "gustinaNothing2.png",
        "gustinaIdle1.png", "gustinaIdle2.png",
        "gustinaBopLeft1.png", "gustinaBopLeft2.png", "gustinaBopLeft3.png",
        "gustinaBopRight1.png", "gustinaBopRight2.png", "gustinaBopRight3.png"
    };

    for (int i = 0; i < 10; i++) {
        char path[256];
        snprintf(path, sizeof(path), "sdmc:/3ds/MCMP/mods/%s", filenames[i]);
        
        FILE* f = fopen(path, "rb");
        if (f) {
            fclose(f);
            int imgW = 0, imgH = 0, channels = 0;
            uint8_t* pixels = stbi_load(path, &imgW, &imgH, &channels, 4);
            if (pixels && imgW > 0 && imgH > 0) {
                uint8_t* resizedBuf = (uint8_t*)malloc(256 * 256 * 4);
                if (resizedBuf) {
                    memset(resizedBuf, 0, 256 * 256 * 4);
                    int copyW = (imgW < 256) ? imgW : 256;
                    int copyH = (imgH < 256) ? imgH : 256;
                    for (int y = 0; y < copyH; y++) {
                        memcpy(resizedBuf + (y * 256 * 4), pixels + (y * imgW * 4), copyW * 4);
                    }
                    upload_rgba8_to_tex(&customGustinaTex[i], resizedBuf, 256, 256, 256, 256);
                    customGustinaSubtex[i].width  = (u16)copyW;
                    customGustinaSubtex[i].height = (u16)copyH;
                    customGustinaSubtex[i].left   = 0.0f;
                    customGustinaSubtex[i].top    = 1.0f;
                    customGustinaSubtex[i].right  = (float)copyW / 256.0f;
                    customGustinaSubtex[i].bottom = 1.0f - ((float)copyH / 256.0f);
                    hasCustomGustina[i] = true;
                    free(resizedBuf);
                }
                stbi_image_free(pixels);
            }
        }
    }
}


