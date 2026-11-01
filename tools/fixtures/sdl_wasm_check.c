/* Source-owned SDL host ABI check; no game or firmware content. */
#include <math.h>
#include <stdio.h>
#include <SDL3/SDL.h>

typedef struct Shared {
    SDL_Mutex *mutex;
    SDL_Semaphore *ready;
    int value;
} Shared;

static int worker(void *opaque)
{
    Shared *state = opaque;
    SDL_LockMutex(state->mutex);
    state->value = 42;
    SDL_UnlockMutex(state->mutex);
    SDL_SignalSemaphore(state->ready);
    return 7;
}

int main(void)
{
    if (SDL_GetVersion() != SDL_VERSION || !SDL_Init(0)) return 1;
    Shared state = { SDL_CreateMutex(), SDL_CreateSemaphore(0), 0 };
    if (!state.mutex || !state.ready) return 2;
    SDL_Thread *thread = SDL_CreateThread(worker, "port-check", &state);
    if (!thread || !SDL_WaitSemaphoreTimeout(state.ready, 5000)) return 3;
    SDL_LockMutex(state.mutex);
    int observed = state.value;
    SDL_UnlockMutex(state.mutex);
    int exit_code = -1;
    SDL_WaitThread(thread, &exit_code);
    if (observed != 42 || exit_code != 7) return 4;
    SDL_DestroySemaphore(state.ready);
    SDL_DestroyMutex(state.mutex);

    const Sint16 input[] = { -32768, -32768, 0, 0, 16384, 16384 };
    const float expected[] = { -1.0f, 0.0f, 0.5f };
    SDL_AudioSpec source = { SDL_AUDIO_S16, 2, 48000 };
    SDL_AudioSpec destination = { SDL_AUDIO_F32, 1, 48000 };
    Uint8 *converted = NULL;
    int length = 0;
    if (!SDL_ConvertAudioSamples(&source, (const Uint8 *)input, sizeof(input),
                                 &destination, &converted, &length)) return 5;
    if (length != sizeof(expected)) return 6;
    for (unsigned i = 0; i < SDL_arraysize(expected); ++i) {
        if (fabsf(((float *)converted)[i] - expected[i]) > 0.00001f) return 7;
    }
    SDL_free(converted);
    SDL_Quit();
    puts("Wasm SDL3: version, thread/mutex/semaphore and PCM conversion checks passed");
    return 0;
}
