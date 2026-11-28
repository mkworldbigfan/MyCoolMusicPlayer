#include "app_state.h"

bool has_extension(const char* fn, const char* ext) {
    size_t l1 = strlen(fn), l2 = strlen(ext);
    if (l1 < l2) return false;
    return (strcasecmp(fn + l1 - l2, ext) == 0);
}

static int utf8_next_char(const char* s, uint32_t* outCp) {
    unsigned char c0 = (unsigned char)s[0];
    if (c0 == 0) {
        if (outCp) *outCp = 0;
        return 0;
    }
    if (c0 < 0x80) {
        if (outCp) *outCp = c0;
        return 1;
    }
    if ((c0 & 0xE0) == 0xC0 && (s[1] & 0xC0) == 0x80) {
        if (outCp) *outCp = ((uint32_t)(c0 & 0x1F) << 6) | (uint32_t)(s[1] & 0x3F);
        return 2;
    }
    if ((c0 & 0xF0) == 0xE0 && (s[1] & 0xC0) == 0x80 && (s[2] & 0xC0) == 0x80) {
        if (outCp) *outCp = ((uint32_t)(c0 & 0x0F) << 12) | ((uint32_t)(s[1] & 0x3F) << 6) | (uint32_t)(s[2] & 0x3F);
        return 3;
    }
    if ((c0 & 0xF8) == 0xF0 && (s[1] & 0xC0) == 0x80 && (s[2] & 0xC0) == 0x80 && (s[3] & 0xC0) == 0x80) {
        if (outCp) *outCp = ((uint32_t)(c0 & 0x07) << 18) | ((uint32_t)(s[1] & 0x3F) << 12) | ((uint32_t)(s[2] & 0x3F) << 6) | (uint32_t)(s[3] & 0x3F);
        return 4;
    }
    if (outCp) *outCp = 0xFFFD;
    return 1;
}

static inline bool is_custom_font_cp(uint32_t cp) {
    return (cp >= 32 && cp <= 126);
}

void nuke_weird_utf8_chars(const char* src, char* dst, size_t dstSize) {
    if (!src || !dst || dstSize == 0) return;
    size_t s = 0, d = 0;

    
    if ((unsigned char)src[0] == 0xEF && (unsigned char)src[1] == 0xBB && (unsigned char)src[2] == 0xBF) {
        s = 3;
    }

    while (src[s] != '\0' && d + 1 < dstSize) {
        uint32_t cp = 0;
        int len = utf8_next_char(src + s, &cp);
        if (len <= 0) break;

        if (cp == '\t') {
            dst[d++] = ' ';
        } else if (cp >= 32 && cp != 0xFFFD) {
            if (d + (size_t)len < dstSize) {
                for (int k = 0; k < len; k++) dst[d++] = src[s + k];
            } else {
                break;
            }
        }
        s += (size_t)len;
    }
    dst[d] = '\0';
    while (d > 0 && (dst[d-1] == ' ' || dst[d-1] == '\t' || dst[d-1] == '\r' || dst[d-1] == '\n')) {
        dst[--d] = '\0';
    }
}

void strip_extension_copy(const char* src, char* dst, size_t dstSize) {
    if (!dst || dstSize == 0) return;
    if (!src) { dst[0] = '\0'; return; }
    snprintf(dst, dstSize, "%s", src);
    if (strcmp(dst, "..") == 0) return;
    char* dot = strrchr(dst, '.');
    if (dot && dot > dst) *dot = '\0';
}

void utf8_truncate_chars(const char* src, char* dst, size_t dstSize, int maxChars) {
    if (!src || !dst || dstSize == 0) return;
    size_t s = 0, d = 0;
    int chars = 0;
    while (src[s] != '\0' && chars < maxChars) {
        uint32_t cp = 0;
        int len = utf8_next_char(src + s, &cp);
        if (len <= 0 || d + (size_t)len >= dstSize) break;
        for (int k = 0; k < len; k++) dst[d++] = src[s + k];
        s += (size_t)len;
        chars++;
    }
    dst[d] = '\0';
}

void init_custom_font(void) { //reminds me to maro...
    fontSheet1 = C2D_SpriteSheetLoad("romfs:/gfx/font1.t3x");
    fontSheet2 = C2D_SpriteSheetLoad("romfs:/gfx/font2.t3x");

    if (fontSheet1) {
        C2D_Image img1 = C2D_SpriteSheetGetImage(fontSheet1, 0);
        fontTex1 = img1.tex;
    }
    if (fontSheet2) {
        C2D_Image img2 = C2D_SpriteSheetGetImage(fontSheet2, 0);
        fontTex2 = img2.tex;
    }
    if (!fontTex1 && fontTex2) fontTex1 = fontTex2;
    if (!fontTex2 && fontTex1) fontTex2 = fontTex1;

    
    sysTextBuf = C2D_TextBufNew(1024);
    sysMeasureBuf = C2D_TextBufNew(256);

    
    
    for (int i = 0; i < 96; i++) {
        int col = i % 16;
        int row = i / 16;
        fontGlyphs[i].width = 32;
        fontGlyphs[i].height = 64;
        fontGlyphs[i].left = (float)(col * 32) / 512.0f;
        fontGlyphs[i].right = (float)((col + 1) * 32) / 512.0f;
        fontGlyphs[i].top = 1.0f - (float)(row * 64) / 512.0f;
        fontGlyphs[i].bottom = 1.0f - (float)((row + 1) * 64) / 512.0f;
    }
}

static float measure_sys_run(const char* seg, float scale) {
    if (!sysMeasureBuf || !seg || seg[0] == '\0') return 0.0f;
    C2D_TextBufClear(sysMeasureBuf);
    C2D_Text t;
    if (!C2D_TextParse(&t, sysMeasureBuf, seg)) return 0.0f;
    return t.width * (scale * 1.45f);
}

static float draw_sys_run(const char* seg, float x, float y, float scale, u32 color) {
    if (!sysTextBuf || !seg || seg[0] == '\0') return 0.0f;
    C2D_Text t;
    if (!C2D_TextParse(&t, sysTextBuf, seg)) return 0.0f;
    C2D_TextOptimize(&t);
    float sysScale = scale * 1.45f;
    float sysY = y + (10.0f * scale);
    C2D_DrawText(&t, C2D_WithColor, x, sysY, CFG_DEPTH_TEXTS, sysScale, sysScale, color);
    return t.width * sysScale;
}

float measure_custom_font(float scale, const char* str) {
    if (!str) return 0.0f;
    float w = 0.0f;
    float step = 20.0f * scale;
    char runBuf[256];
    size_t runLen = 0;

    for (size_t i = 0; str[i] != '\0'; ) {
        uint32_t cp = 0;
        int len = utf8_next_char(str + i, &cp);
        if (len <= 0) break;

        if (is_custom_font_cp(cp)) {
            if (runLen > 0) {
                runBuf[runLen] = '\0';
                w += measure_sys_run(runBuf, scale);
                runLen = 0;
            }
            if (cp == ' ') w += 14.0f * scale;
            else w += step;
        } else {
            if (runLen + (size_t)len < sizeof(runBuf)) {
                for (int k = 0; k < len; k++) runBuf[runLen++] = str[i + k];
            } else {
                runBuf[runLen] = '\0';
                w += measure_sys_run(runBuf, scale);
                runLen = 0;
                for (int k = 0; k < len; k++) runBuf[runLen++] = str[i + k];
            }
        }
        i += (size_t)len;
    }
    if (runLen > 0) {
        runBuf[runLen] = '\0';
        w += measure_sys_run(runBuf, scale);
    }
    return w;
}

void draw_custom_font(float x, float y, float scale, u32 color, const char* str) { //NO PLEASE, NO MORE DRAWING PLEASEEEE RNFRNFSDH
    if (!fontTex1 || !fontTex2 || !str) return;

    C3D_Tex* curTex = (global10FpsFrame == 0) ? fontTex1 : fontTex2;
    float curX = x;
    float step = 20.0f * scale;

    C2D_ImageTint tint;
    C2D_PlainImageTint(&tint, color, 1.0f);

    char runBuf[256];
    size_t runLen = 0;

    for (size_t i = 0; str[i] != '\0'; ) {
        uint32_t cp = 0;
        int len = utf8_next_char(str + i, &cp);
        if (len <= 0) break;

        if (is_custom_font_cp(cp)) {
            if (runLen > 0) {
                runBuf[runLen] = '\0';
                curX += draw_sys_run(runBuf, curX, y, scale, color);
                runLen = 0;
            }
            if (cp == ' ') {
                curX += 14.0f * scale;
            } else {
                int idx = (int)cp - 32;
                C2D_Image charImg = { curTex, &fontGlyphs[idx] };
                C2D_DrawImageAt(charImg, curX, y, CFG_DEPTH_TEXTS, &tint, scale, scale);
                curX += step;
            }
        } else {
            if (runLen + (size_t)len < sizeof(runBuf)) {
                for (int k = 0; k < len; k++) runBuf[runLen++] = str[i + k];
            } else {
                runBuf[runLen] = '\0';
                curX += draw_sys_run(runBuf, curX, y, scale, color);
                runLen = 0;
                for (int k = 0; k < len; k++) runBuf[runLen++] = str[i + k];
            }
        }
        i += (size_t)len;
    }
    if (runLen > 0) {
        runBuf[runLen] = '\0';
        draw_sys_run(runBuf, curX, y, scale, color);
    }
}

void draw_custom_font_centered(float cx, float y, float scale, u32 color, const char* str) {
    float w = measure_custom_font(scale, str);
    draw_custom_font(cx - (w * 0.5f), y, scale, color, str);
}

void draw_marquee_font(float x, float y, float maxW, float scale, u32 color, const char* str, float time) {
    if (!fontTex1 || !fontTex2 || !str) return;

    float textW = measure_custom_font(scale, str);
    C3D_Tex* curTex = (global10FpsFrame == 0) ? fontTex1 : fontTex2;
    float step = 20.0f * scale;

    if (textW <= maxW) {
        draw_custom_font(x, y, scale, color, str);
        return;
    }

    float excess = textW - maxW;
    float scrollSpeed = 28.0f; 
    float travelTime = excess / scrollSpeed;
    if (travelTime < 0.6f) travelTime = 0.6f;
    float pauseTime = 1.4f;
    float period = 2.0f * (pauseTime + travelTime);
    float t = fmodf(time, period);
    if (t < 0.0f) t += period;

    float offset = 0.0f;
    if (t < pauseTime) {
        offset = 0.0f;
    } else if (t < pauseTime + travelTime) {
        float p = (t - pauseTime) / travelTime;
        float smooth = p * p * (3.0f - 2.0f * p);
        offset = smooth * excess;
    } else if (t < 2.0f * pauseTime + travelTime) {
        offset = excess;
    } else {
        float p = (t - (2.0f * pauseTime + travelTime)) / travelTime;
        float smooth = p * p * (3.0f - 2.0f * p);
        offset = (1.0f - smooth) * excess;
    }

    float startX = x - offset;
    float fadeEdge = 16.0f;
    float remRight = excess - offset;
    float leftStrength  = (offset < fadeEdge)   ? (offset / fadeEdge)   : 1.0f;
    float rightStrength = (remRight < fadeEdge) ? (remRight / fadeEdge) : 1.0f;

    for (size_t i = 0; str[i] != '\0'; ) {
        uint32_t cp = 0;
        int len = utf8_next_char(str + i, &cp);
        if (len <= 0) break;

        float charX = startX;
        if (cp == ' ') {
            startX += 14.0f * scale;
            i += (size_t)len;
            continue;
        }

        char gBuf[8];
        float gW = step;
        bool isCustom = is_custom_font_cp(cp);
        if (!isCustom) {
            for (int k = 0; k < len && k < 7; k++) gBuf[k] = str[i + k];
            gBuf[len < 7 ? len : 7] = '\0';
            gW = measure_sys_run(gBuf, scale);
            if (gW <= 0.5f) gW = 14.0f * scale;
        }

        startX += gW;
        i += (size_t)len;

        float charCenter = charX + gW * 0.5f;
        if (charX + 2.0f < x || charX + gW > x + maxW + 2.0f) continue;

        float rawL = (charCenter - x) / fadeEdge;
        if (rawL < 0.0f) rawL = 0.0f; else if (rawL > 1.0f) rawL = 1.0f;
        float alphaL = 1.0f - leftStrength * (1.0f - rawL);

        float rawR = (x + maxW - charCenter) / fadeEdge;
        if (rawR < 0.0f) rawR = 0.0f; else if (rawR > 1.0f) rawR = 1.0f;
        float alphaR = 1.0f - rightStrength * (1.0f - rawR);

        float alpha = alphaL * alphaR;
        if (alpha <= 0.02f) continue;

        u32 a = (u32)(((color >> 24) & 0xFF) * alpha);
        u32 charColor = (color & 0x00FFFFFF) | (a << 24);

        if (isCustom) {
            int idx = (int)cp - 32;
            C2D_ImageTint tint;
            C2D_PlainImageTint(&tint, charColor, 1.0f);
            C2D_Image charImg = { curTex, &fontGlyphs[idx] };
            C2D_DrawImageAt(charImg, charX, y, CFG_DEPTH_TEXTS, &tint, scale, scale);
        } else {
            draw_sys_run(gBuf, charX, y, scale, charColor);
        }
    }
}

void draw_wrapped_lyrics(float cx, float cy, float maxW, float scale, u32 color, const char* text) {
    if (!text || text[0] == '\0') return;

    float textW = measure_custom_font(scale, text);
    if (textW <= maxW) {
        draw_custom_font_centered(cx, cy, scale, color, text);
        return;
    }

    int len = (int)strlen(text);
    int mid = len / 2;
    int splitIdx = -1;
    int bestDist = 9999;

    for (int i = 0; i < len; i++) {
        if (text[i] == ' ') {
            int dist = abs(i - mid);
            if (dist < bestDist) {
                bestDist = dist;
                splitIdx = i;
            }
        }
    }
    
    bool splitOnSpace = (splitIdx > 0);
    if (!splitOnSpace) {
        bestDist = 9999;
        for (int i = 1; i < len; i++) {
            if (((unsigned char)text[i] & 0xC0) != 0x80) {
                int dist = abs(i - mid);
                if (dist < bestDist) {
                    bestDist = dist;
                    splitIdx = i;
                }
            }
        }
    }

    if (splitIdx > 0) {
        char line1[256];
        char line2[256];
        snprintf(line1, sizeof(line1), "%.*s", splitIdx, text);
        const char* p2 = text + (splitOnSpace ? splitIdx + 1 : splitIdx);
        while (*p2 == ' ') p2++;
        snprintf(line2, sizeof(line2), "%s", p2);

        float lineSpacing = 16.0f * scale * 2.4f;
        draw_custom_font_centered(cx, cy - lineSpacing * 0.45f, scale, color, line1);
        draw_custom_font_centered(cx, cy + lineSpacing * 0.55f, scale, color, line2);
    } else {
        float fitScale = scale * (maxW / textW);
        if (fitScale < 0.28f) fitScale = 0.28f;
        draw_custom_font_centered(cx, cy, fitScale, color, text);
    }
}

void draw_bouncing_loading_text(float x, float y, float scale, float animTime, u8 alpha) {
    if (!fontTex1 || !fontTex2 || alpha == 0) return;

    const char* msg = "Loading...";
    C3D_Tex* curTex = (global10FpsFrame == 0) ? fontTex1 : fontTex2;
    float step = 20.0f * scale;
    float curX = x;

    C2D_ImageTint tint;
    C2D_PlainImageTint(&tint, C2D_Color32(255, 255, 255, alpha), 1.0f);

    for (int i = 0; msg[i] != '\0'; i++) {
        float phase = animTime * 7.5f - (float)i * 0.55f;
        float s = sinf(phase);
        float bounceY = (s > 0.0f) ? (-s * 6.5f) : 0.0f;

        int idx = (int)(unsigned char)msg[i] - 32;
        if (idx >= 0 && idx < 96) {
            C2D_Image charImg = { curTex, &fontGlyphs[idx] };
            C2D_DrawImageAt(charImg, curX, y + bounceY, 0.99f, &tint, scale, scale);
        }
        curX += step;
    }
}

