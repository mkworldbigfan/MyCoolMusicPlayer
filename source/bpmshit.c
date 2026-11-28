#include "app_state.h"

void reset_beat_tracking(bool resetTag) {
    beatQueueHead = 0;
    beatQueueTail = 0;
    beatLpBass = 0.0f;
    beatLpMid = 0.0f;
    beatPrevBass = 0.0f;
    beatPrevMid = 0.0f;
    beatAvgEnergy = 0.015f;
    lastDetectedBeatTime = -1.0f;
    nextVisualBeatTime = 0.0f;
    visualBeatLocked = false;
    beatSideToggle = false;
    osfWriteIdx = 0;
    osfCount = 0;
    osfFramesSinceCalc = 0;
    bpmLastCand = 120.0f;
    bpmCandVotes = 0;
    if (resetTag) hasTrackBpmTag = false;
}

static float sample_osf_acf(const float* env, int n, float lag, float normDenom) {
    int iLag = (int)lag;
    float frac = lag - (float)iLag;
    if (iLag < 1 || iLag + 1 >= n) return 0.0f;
    int count = n - iLag - 1;
    if (count < 32) return 0.0f;
    float s0 = 0.0f, s1 = 0.0f;
    for (int i = 0; i < count; i++) {
        float x = env[i];
        s0 += x * env[i + iLag];
        s1 += x * env[i + iLag + 1];
    }
    return (s0 * (1.0f - frac) + s1 * frac) / normDenom;
}

static void guess_bpm_magic(float envRate) {
    int n = (osfCount < OSF_LEN) ? osfCount : OSF_LEN;
    if (n < 256 || envRate <= 1.0f) return;

    float env[OSF_LEN];
    int start = (osfWriteIdx - n + OSF_LEN) % OSF_LEN;
    float mean = 0.0f;
    for (int i = 0; i < n; i++) {
        float v = osfBuf[(start + i) % OSF_LEN];
        env[i] = v;
        mean += v;
    }
    mean /= (float)n;

    float energySum = 0.0f;
    for (int i = 0; i < n; i++) {
        env[i] -= mean;
        energySum += env[i] * env[i];
    }
    if (energySum < 1e-5f) return;

    int bestBpm = 120;
    float bestScore = -1.0f;

    for (int bpm = 70; bpm <= 175; bpm++) {
        float lag = envRate * 60.0f / (float)bpm;
        float r1 = sample_osf_acf(env, n, lag, energySum);
        float r2 = sample_osf_acf(env, n, lag * 2.0f, energySum);
        float rHalf = (lag * 0.5f >= 12.0f) ? sample_osf_acf(env, n, lag * 0.5f, energySum) : 0.0f;

        float score = r1 + 0.45f * r2 + 0.15f * rHalf;
        if (score <= 0.0f) continue;

        float logDist = log2f((float)bpm / 120.0f) / 0.55f;
        float prior = expf(-0.5f * logDist * logDist);
        score *= (0.65f + 0.35f * prior);

        if (fabsf((float)bpm - detectedBPM) < 3.0f) score *= 1.06f;

        if (score > bestScore) {
            bestScore = score;
            bestBpm = bpm;
        }
    }

    if (bestScore > 0.06f) {
        if (fabsf((float)bestBpm - bpmLastCand) < 4.0f) {
            bpmCandVotes++;
        } else {
            bpmLastCand = (float)bestBpm;
            bpmCandVotes = 1;
        }

        if (bpmCandVotes >= 2 || fabsf((float)bestBpm - detectedBPM) <= 10.0f) {
            float alpha = (fabsf((float)bestBpm - detectedBPM) > 12.0f) ? 0.45f : 0.20f;
            detectedBPM = detectedBPM * (1.0f - alpha) + (float)bestBpm * alpha;
        }
    }
}

void do_the_bpm_thingy(const int16_t* pcm, int frames) {
    if (!pcm || frames <= 0 || currentSampleRate <= 0) return;

    const int subWin = 512;
    int offset = 0;
    float minGap = (hasTrackBpmTag && detectedBPM > 40.0f) ? (60.0f / detectedBPM) * 0.55f : 0.24f;
    if (minGap < 0.22f) minGap = 0.22f;
    if (minGap > 0.48f) minGap = 0.48f;

    while (offset < frames) {
        int count = frames - offset;
        if (count > subWin) count = subWin;

        float sumBass = 0.0f;
        float sumMid = 0.0f;
        for (int i = 0; i < count; i++) {
            int idx = (offset + i) * 2;
            float mono = ((float)pcm[idx] + (float)pcm[idx + 1]) * (0.5f / 32768.0f);
            beatLpBass += 0.025f * (mono - beatLpBass);
            beatLpMid  += 0.120f * (mono - beatLpMid);
            sumBass += fabsf(beatLpBass);
            sumMid  += fabsf(beatLpMid - beatLpBass);
        }

        float eBass = sumBass / (float)count;
        float eMid  = sumMid / (float)count;
        float fBass = eBass - beatPrevBass; if (fBass < 0.0f) fBass = 0.0f;
        float fMid  = eMid - beatPrevMid;   if (fMid < 0.0f)  fMid = 0.0f;
        beatPrevBass = eBass;
        beatPrevMid  = eMid;

        float flux = fBass * 1.6f + fMid * 0.7f;
        osfBuf[osfWriteIdx] = flux;
        osfWriteIdx = (osfWriteIdx + 1) % OSF_LEN;
        if (osfCount < OSF_LEN) osfCount++;
        osfFramesSinceCalc++;

        float frameTime = (float)((double)(samplesFed + (uint64_t)offset) / (double)currentSampleRate);
        if (flux > beatAvgEnergy * 1.55f && (eBass + eMid) > 0.022f &&
            (lastDetectedBeatTime < 0.0f || (frameTime - lastDetectedBeatTime) >= minGap)) {
            lastDetectedBeatTime = frameTime;
            int nextTail = (beatQueueTail + 1) % MAX_PENDING_BEATS;
            if (nextTail != beatQueueHead) {
                pendingBeatTimes[beatQueueTail] = frameTime;
                beatQueueTail = nextTail;
            }
        }

        beatAvgEnergy = beatAvgEnergy * 0.96f + flux * 0.04f;
        if (beatAvgEnergy < 0.008f) beatAvgEnergy = 0.008f;
        offset += count;
    }

    if (!hasTrackBpmTag && osfFramesSinceCalc >= 43 && osfCount >= 256) {
        osfFramesSinceCalc = 0;
        guess_bpm_magic((float)currentSampleRate / (float)subWin);
    }

    samplesFed += (uint64_t)frames;
}

