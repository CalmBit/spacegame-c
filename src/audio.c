#include "audio.h"

#include "memory.h"
#include "error.h"
#include "wav.h"

#include <alc.h>
#include <alext.h>

#include <stdio.h>
#include <float.h>

LPALGENEFFECTS alGenEffects = NULL;
LPALDELETEEFFECTS alDeleteEffects = NULL;
LPALISEFFECT alIsEffect = NULL;
LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots = NULL;
LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots = NULL;
LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot = NULL;
LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti = NULL;
LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv = NULL;
LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf = NULL;
LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv = NULL;
LPALEFFECTI alEffecti = NULL;
LPALEFFECTF alEffectf = NULL;

ALenum last_al_error;
ALCenum last_alc_error;

static void _audio_initialize_alext(void);

static void _audio_check_error(const char *fmt);

static void _audio_context_check_error(const char *fmt);

static void _audio_mandate_context(const char *reason);

static void _audio_mandate_efx(const char *reason);

static const char *_audio_alcerror(ALCenum code);

//===========================================================================//
//=================SYSTEM====================================================//
//===========================================================================//

void audio_init(void) {
    primary_audio = audio_create();
}

void audio_cleanup(void) {
    if (primary_audio != NULL) {
        audio_destroy(primary_audio);
    }
}

audio_system_t *audio_create(void) {
    audio_system_t *aud;
    ALuint i;
    ALint attribs[4] = {0};

    aud = (audio_system_t *) memory_alloc(SPC_MU_SOUND, sizeof(audio_system_t));
    aud->device = alcOpenDevice(NULL);
    if (!aud->device) {
        error("unable to open default audio device");
    }

    if (alcIsExtensionPresent(aud->device, "ALC_EXT_EFX") == AL_FALSE) {
        warning("unable to initialize EFX extension - continuing w/o EFX");
        aud->has_efx = false;
    } else {
        aud->has_efx = true;
        attribs[0] = ALC_MAX_AUXILIARY_SENDS;
        attribs[1] = SPC_SEND_SLOT_COUNT;
    }

    aud->context = alcCreateContext(aud->device,
                                    aud->has_efx ? attribs : NULL);

    if (!aud->context) {
        error("unable to create audio context: %s",
              _audio_alcerror(alcGetError(aud->device)));
    }

    if (!alcMakeContextCurrent(aud->context)) {
        error("unable to make OpenAL context current: %s",
              _audio_alcerror(alcGetError(aud->device)));
    }

    if (aud->has_efx) {
        alcGetIntegerv(aud->device, ALC_MAX_AUXILIARY_SENDS, 1,
                       &aud->send_count);
        if (aud->send_count < SPC_SEND_SLOT_COUNT) {
            error("reservation of %i send slots was unsuccessful, got %i",
                  SPC_SEND_SLOT_COUNT, aud->send_count);
        }
        _audio_initialize_alext();

        alGetError();
        for (i = 0; i < SPC_EFFECT_SLOT_COUNT; i++) {
            alGenAuxiliaryEffectSlots(1, &aud->effect_slots[i].id);
            aud->effect_slots[i].occupied = false;
            _audio_check_error("unable to create effect slots");
        }
    } else {
        aud->send_count = 0;
    }

    return aud;
}

void audio_destroy(audio_system_t *aud) {
    alcMakeContextCurrent(NULL);

    alcDestroyContext(aud->context);
    _audio_context_check_error("error while destroying audio context");

    alcCloseDevice(aud->device);
    _audio_context_check_error("error while closing audio device");

    memory_free(aud);
}

//===========================================================================//
//=================SOURCE====================================================//
//===========================================================================//

audio_src_t *audio_src_create(void) {
    audio_src_t *src;
    size_t i;

    _audio_mandate_context("create source");

    src = (audio_src_t *) memory_alloc(SPC_MU_SOUND, sizeof(audio_src_t));
    alGenSources(1, &src->id);
    _audio_check_error("unable to generate source");

    src->pitch = 1;
    alSourcef(src->id, AL_PITCH, src->pitch);
    _audio_check_error("unable to set source 'AL_PITCH'");

    src->gain = 1;
    alSourcef(src->id, AL_GAIN, src->gain);
    _audio_check_error("unable to set source 'AL_GAIN'");

    src->pos[0] = src->pos[1] = src->pos[2] = 0.0f;
    alSource3f(src->id, AL_POSITION, src->pos[0], src->pos[1], src->pos[2]);
    _audio_check_error("unable to set source 'AL_POSITION'");

    src->vel[0] = src->vel[1] = src->vel[2] = 0.0f;
    alSource3f(src->id, AL_VELOCITY, src->vel[0], src->vel[1], src->vel[2]);
    _audio_check_error("unable to set source 'AL_VELOCITY'");

    src->looping = AL_FALSE;
    alSourcei(src->id, AL_LOOPING, src->looping);
    _audio_check_error("unable to set source 'AL_LOOPING'");

    if (primary_audio->has_efx) {
        // by default, send it through every single
        // send we have.
        for (i = 0; i < SPC_EFFECT_SLOT_COUNT; i++) {
            alSource3i(src->id, AL_AUXILIARY_SEND_FILTER,
                       primary_audio->effect_slots[i].id,
                       i, 0);
        }
        _audio_check_error("unable to connect source to effect");
    }

    alGenBuffers(1, &src->buffer);
    _audio_check_error("unable to generate audio buffer");

    wav_t *wav = wav_load("schut_lq.wav");

    alBufferData(src->buffer, wav_get_format(wav),
                 wav->data.buffer,
                 wav->data.len,
                 wav->sample_rate);
    _audio_check_error("unable to bind data to audio buffer");


    alSourcei(src->id, AL_BUFFER, src->buffer);
    _audio_check_error("unable to set source buffer");


    alSourcePlay(src->id);

    wav_destroy(wav);

    return src;
}

void audio_src_destroy(audio_src_t *src) {
    _audio_mandate_context("destroy source");

    if (src == NULL) {
        error("cannot destroy NULL source");
    }
    alDeleteSources(1, &src->id);
    _audio_check_error("unable to delete source");
    alDeleteBuffers(1, &src->buffer);
    _audio_check_error("unable to delete buffer");
    memory_free((void *) src);
}

static void _audio_src_setf(audio_src_t *src, ALfloat *dst, ALfloat value,
                            ALfloat min, ALfloat max, const char *name,
                            ALenum type) {
    _audio_mandate_context("set source value");

    if (src == NULL) {
        error("cannot set value '%s' on NULL src", name);
    }

    if (value < min || value > max) {
        warning("invalid source '%s' value %f - ignoring", name, value);
        return;
    }
    *dst = value;

    alSourcef(src->id, type, *dst);
    _audio_check_error("unable to set source value");
}

static void _audio_src_setb(audio_src_t *src, ALboolean *dst, bool value,
                            const char *name, ALenum type) {
    _audio_mandate_context("set source value");

    if (src == NULL) {
        error("cannot set value '%s' on NULL src", name);
    }

    *dst = value ? AL_TRUE : AL_FALSE;

    alSourcei(src->id, type, *dst);
    _audio_check_error("unable to set source value");
}

static void _audio_src_set3f(audio_src_t *src, ALfloat *dst,
                             ALfloat x, ALfloat y, ALfloat z,
                             const char *name, ALenum type) {
    _audio_mandate_context("set source value");

    if (src == NULL) {
        error("cannot set value '%s' on NULL src", name);
    }

    dst[0] = x;
    dst[1] = x;
    dst[2] = x;

    alSourcefv(src->id, type, dst);
    _audio_check_error("unable to set source value");
}

void audio_src_set_pitch(audio_src_t *src, ALfloat pitch) {
    _audio_src_setf(src, &src->pitch, pitch, 0.5f, 2.0f, "pitch", AL_PITCH);
}

void audio_src_set_gain(audio_src_t *src, ALfloat gain) {
    _audio_src_setf(src, &src->gain, gain, 0.0f, 2.0f, "gain", AL_GAIN);
}

void audio_src_set_pos(audio_src_t *src, ALfloat x, ALfloat y, ALfloat z) {
    _audio_src_set3f(src, src->pos, x, y, z, "position", AL_POSITION);
}

void audio_src_set_vel(audio_src_t *src, ALfloat x, ALfloat y, ALfloat z) {
    _audio_src_set3f(src, src->vel, x, y, z, "velocity", AL_VELOCITY);
}

void audio_src_set_looping(audio_src_t *src, bool looping) {
    _audio_src_setb(src, &src->looping, looping, "looping", AL_LOOPING);
}

//===========================================================================//
//=================REVERB====================================================//
//===========================================================================//

audio_reverb_t *audio_reverb_create(void) {
    audio_reverb_t *reverb;

    _audio_mandate_context("create reverb");
    _audio_mandate_efx("create reverb");

    reverb = memory_alloc(SPC_MU_SOUND, sizeof(audio_reverb_t));

    alGenEffects(1, &reverb->id);
    _audio_check_error("unable to generate effect");

    // default values according to OpenAL specification
    // we don't need to re-set these, as they're 
    // already the standard.

    reverb->density = 1.0f;
    reverb->diffusion = 1.0f;
    reverb->gain = 0.32f;
    reverb->decay = 1.49f;
    reverb->reflection_gain = 0.05f;
    reverb->reflection_delay = 0.007f;
    reverb->late_gain = 1.26f;
    reverb->late_delay = 0.011f;
    reverb->rolloff_factor = 0.0f;
    reverb->slot_id = -1;

    alEffecti(reverb->id, AL_EFFECT_TYPE, AL_EFFECT_REVERB);
    _audio_check_error("unable to set effect type");


    return reverb;
}

void audio_reverb_destroy(audio_reverb_t *reverb) {
    if (reverb == NULL) {
        error("cannot destroy NULL reverb");
    }

    _audio_mandate_efx("destroy reverb");

    if (reverb->slot_id != -1) {
        audio_unslot_reverb(reverb);
    }
    alDeleteEffects(1, &reverb->id);
    memory_free(reverb);
}

static void _audio_reverb_setf(audio_reverb_t *reverb, ALfloat *dst,
                               ALfloat value, ALfloat min, ALfloat max,
                               const char *name, ALenum type) {
    _audio_mandate_context("set reverb value");
    _audio_mandate_efx("set reverb value");

    if (reverb == NULL) {
        error("cannot set value '%s' on NULL reverb", name);
    }

    if (value < min || value > max) {
        warning("invalid reverb %s value %f - ignoring", name, value);
        return;
    }
    *dst = value;

    alEffectf(reverb->id, type, *dst);
    _audio_check_error("unable to set reverb value");
}

void audio_reverb_set_density(audio_reverb_t *reverb, ALfloat density) {
    _audio_reverb_setf(reverb, &reverb->density, density, 0.0f, 1.0f,
                       "density", AL_REVERB_DENSITY);
}

void audio_reverb_set_diffusion(audio_reverb_t *reverb, ALfloat diffusion) {
    _audio_reverb_setf(reverb, &reverb->diffusion, diffusion, 0.0f, 1.0f,
                       "diffusion", AL_REVERB_DIFFUSION);
}

void audio_reverb_set_gain(audio_reverb_t *reverb, ALfloat gain) {
    _audio_reverb_setf(reverb, &reverb->gain, gain, 0.0f, 1.0f,
                       "gain",
                       AL_REVERB_GAIN);
}

void audio_reverb_set_decay(audio_reverb_t *reverb, ALfloat decay) {
    _audio_reverb_setf(reverb, &reverb->decay, decay, 0.0f, 20.0f,
                       "decay",
                       AL_REVERB_DECAY_TIME);
}

void audio_reverb_set_rgain(audio_reverb_t *reverb, ALfloat rgain) {
    _audio_reverb_setf(reverb, &reverb->reflection_gain, rgain, 0.0f, 3.16f,
                       "rgain", AL_REVERB_REFLECTIONS_GAIN);
}

void audio_reverb_set_rdelay(audio_reverb_t *reverb, ALfloat rdelay) {
    _audio_reverb_setf(reverb, &reverb->reflection_delay, rdelay, 0.0f, 0.3f,
                       "rdelay", AL_REVERB_REFLECTIONS_DELAY);
}

void audio_reverb_set_lgain(audio_reverb_t *reverb, ALfloat lgain) {
    _audio_reverb_setf(reverb, &reverb->late_gain, lgain, 0.0f, 10.0f,
                       "lgain", AL_REVERB_LATE_REVERB_GAIN);
}

void audio_reverb_set_ldelay(audio_reverb_t *reverb, ALfloat ldelay) {
    _audio_reverb_setf(reverb, &reverb->late_delay, ldelay, 0.0f, 0.1f,
                       "ldelay", AL_REVERB_LATE_REVERB_DELAY);
}

void audio_reverb_set_rolloff_factor(audio_reverb_t *reverb,
                                     ALfloat rolloff_factor) {
    _audio_reverb_setf(reverb, &reverb->rolloff_factor,
                       rolloff_factor, 0.0f, 10.0f, "rolloff_factor",
                       AL_REVERB_ROOM_ROLLOFF_FACTOR);
}

void audio_slot_reverb(audio_reverb_t *reverb) {
    size_t i;

    _audio_mandate_context("slot reverb");
    _audio_mandate_efx("slot reverb");

    if (reverb->slot_id != -1) {
        error("tried to double slot reverb");
    }

    for (i = 0; i < SPC_EFFECT_SLOT_COUNT; i++) {
        if (!primary_audio->effect_slots[i].occupied) {
            alAuxiliaryEffectSloti(primary_audio->effect_slots[i].id,
                                   AL_EFFECTSLOT_EFFECT,
                                   reverb->id);
            _audio_check_error("unable to slot reverb");
            primary_audio->effect_slots[i].occupied = true;
            reverb->slot_id = i;
            return;
        }
    }

    error("unable to find open effect slot!!!");
}

void audio_unslot_reverb(audio_reverb_t *reverb) {
    _audio_mandate_context("unslot reverb");
    _audio_mandate_efx("unslot reverb");

    if (reverb->slot_id == -1) {
        error("tried to unslot free reverb");
    }

    alAuxiliaryEffectSloti(primary_audio->effect_slots[reverb->slot_id].id,
                           AL_EFFECTSLOT_EFFECT,
                           AL_EFFECT_NULL);
    _audio_check_error("unable to unslot reverb");
    reverb->slot_id = -1;
}

//===========================================================================//
//=================PROCEDURE HANDLING========================================//
// This is where we obtain most of the extension pointers for the OpenAL     //
// EFX stuff.                                                                //
//===========================================================================//


static void _audio_grab_procedure(const char *name, void **ptr) {
    *ptr = alGetProcAddress(name);
    if (!*ptr) {
        error("unable to get proc of '%s'", name);
    }
}

static void _audio_initialize_alext(void) {
    _audio_grab_procedure("alGenEffects",
                          (void **) &alGenEffects);
    _audio_grab_procedure("alDeleteEffects",
                          (void **) &alDeleteEffects);
    _audio_grab_procedure("alIsEffect",
                          (void **) &alIsEffect);
    _audio_grab_procedure("alGenAuxiliaryEffectSlots",
                          (void **) &alGenAuxiliaryEffectSlots);
    _audio_grab_procedure("alDeleteAuxiliaryEffectSlots",
                          (void **) &alDeleteAuxiliaryEffectSlots);
    _audio_grab_procedure("alIsAuxiliaryEffectSlot",
                          (void **) &alIsAuxiliaryEffectSlot);
    _audio_grab_procedure("alAuxiliaryEffectSloti",
                          (void **) &alAuxiliaryEffectSloti);
    _audio_grab_procedure("alAuxiliaryEffectSlotiv",
                          (void **) &alAuxiliaryEffectSlotiv);
    _audio_grab_procedure("alAuxiliaryEffectSlotf",
                          (void **) &alAuxiliaryEffectSlotf);
    _audio_grab_procedure("alAuxiliaryEffectSlotfv",
                          (void **) &alAuxiliaryEffectSlotfv);
    _audio_grab_procedure("alEffecti",
                          (void **) &alEffecti);
    _audio_grab_procedure("alEffectf",
                          (void **) &alEffectf);
}

//============================================================================//
//=================INTERNAL FUNCTIONS========================================//
//============================================================================//


static void _audio_mandate_context(const char *reason) {
    if (!primary_audio) {
        error("cannot %s with NULL audio context");
    }
}

static void _audio_mandate_efx(const char *reason) {
    if (!primary_audio->has_efx) {
        error("cannot %s while EFX is disabled");
    }
}

static const char *_audio_alerror(ALenum code) {
    if (code > 0)
        return al_error_codes[code - 0xA000];
    return al_error_codes[code];
}

static const char *_audio_alcerror(ALCenum code) {
    if (code > 0)
        return alc_error_codes[code - 0xA000];
    return alc_error_codes[code];
}

static void _audio_check_error(const char *fmt) {
    last_al_error = alGetError();
    if (last_al_error != AL_NO_ERROR) {
        error("%s: %s", fmt, _audio_alerror(last_al_error));
    }
}

static void _audio_context_check_error(const char *fmt) {
    last_alc_error = alcGetError(primary_audio->device);
    if (last_alc_error != ALC_NO_ERROR) {
        error("%s: %s", fmt, _audio_alcerror(last_alc_error));
    }
}
