/* Source-owned browser host check. This is not an Xbox/game renderer. */
#define SDL_MAIN_USE_CALLBACKS 1
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <GLES3/gl3.h>
#include <emscripten.h>

static SDL_Window *window;
static SDL_GLContext context;
static SDL_AudioStream *audio;
static SDL_AtomicInt audio_calls, audio_errors;
static int frames, keys;
static bool pixels_ok, stopping;

static SDL_AppResult failure(const char *message)
{
    EM_ASM({ document.getElementById('status').textContent = UTF8ToString($0);
             document.body.dataset.error = 'true'; }, message);
    return SDL_APP_FAILURE;
}

static void fill_audio(void *userdata, SDL_AudioStream *stream,
                        int additional_amount, int total_amount)
{
    (void)userdata;
    (void)total_amount;
    const float silence[1024] = { 0 };
    SDL_AddAtomicInt(&audio_calls, 1);
    while (additional_amount > 0) {
        int bytes = SDL_min(additional_amount, (int)sizeof(silence));
        if (!SDL_PutAudioStreamData(stream, silence, bytes)) {
            SDL_AddAtomicInt(&audio_errors, 1);
            break;
        }
        additional_amount -= bytes;
    }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    (void)appstate;
    (void)argc;
    (void)argv;
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
        return failure(SDL_GetError());
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    window = SDL_CreateWindow("Browser host check", 64, 64, SDL_WINDOW_OPENGL);
    if (!window || !(context = SDL_GL_CreateContext(window))) {
        return failure(SDL_GetError());
    }
    const GLubyte *version = glGetString(GL_VERSION);
    if (!version) return failure("No GL version");
    EM_ASM({ document.getElementById('graphics').textContent = UTF8ToString($0); }, version);
    SDL_AudioSpec spec = { SDL_AUDIO_F32, 1, 48000 };
    audio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec,
                                      fill_audio, NULL);
    if (!audio || !SDL_ResumeAudioStreamDevice(audio)) return failure(SDL_GetError());
    int gamepads = 0;
    SDL_JoystickID *ids = SDL_GetGamepads(&gamepads);
    SDL_free(ids);
    EM_ASM({ document.body.dataset.gamepads = $0; }, gamepads);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    (void)appstate;
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.scancode == SDL_SCANCODE_SPACE) {
        keys++;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    (void)appstate;
    if (stopping) return SDL_APP_SUCCESS;
    if (EM_ASM_INT({ return document.body.dataset.error === 'true'; })) {
        return failure("Browser runtime failed; see the local console");
    }
    glViewport(0, 0, 64, 64);
    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if (!frames) {
        unsigned char pixel[4] = { 0 };
        glReadPixels(32, 32, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
        pixels_ok = abs((int)pixel[0] - 64) <= 1 &&
                    abs((int)pixel[1] - 128) <= 1 &&
                    abs((int)pixel[2] - 191) <= 1 && pixel[3] == 255;
    }
    if (!pixels_ok || glGetError() != GL_NO_ERROR) return failure("GL pixel check failed");
    if (SDL_GetAtomicInt(&audio_errors)) return failure("Audio queue failed");
    SDL_GL_SwapWindow(window);
    frames++;
    EM_ASM({ document.body.dataset.frames = $0;
             document.body.dataset.audio = $1;
             document.body.dataset.keys = $2;
             document.body.dataset.pixels = 'passed';
             document.getElementById('status').textContent =
                 'Frames: ' + $0 + '; silent audio callbacks: ' + $1 + '; Space presses: ' + $2;
           }, frames, SDL_GetAtomicInt(&audio_calls), keys);
    return SDL_APP_CONTINUE;
}

EMSCRIPTEN_KEEPALIVE void sdl_port_stop(void)
{
    stopping = true;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    (void)appstate;
    if (audio) SDL_DestroyAudioStream(audio);
    if (context) SDL_GL_DestroyContext(context);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
    EM_ASM({ document.body.dataset.stopped = 'true'; });
    if (result == SDL_APP_SUCCESS) {
        EM_ASM({ document.getElementById('status').textContent = 'Stopped'; });
    }
}
