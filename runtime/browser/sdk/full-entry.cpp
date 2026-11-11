// Full native SDK boot path inside a browser-owned Emscripten worker.
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>
#include <emscripten.h>
#include "app/app.h"
#include "core/input.h"
#include "core/log.h"
#include "kernel/kernel.h"
#include "kernel/disc.h"

const app::GameInfo app::kGame = {
    .title = L"Fuzion Frenzy", .short_title = L"Fuzion Frenzy", .name = L"FuzionFrenzy",
    .log = "fuzion", .settings = "settings.toml", .splash = L"",
    .missing = L"Select your compatible local Fuzion Frenzy disc image.",
    .images = {{"default.xbe", "FuzionFrenzy"}}, .options = {},
    .load = nullptr, .save = nullptr, .init = nullptr, .loaded = nullptr,
    .window = nullptr, .message = nullptr, .ui = nullptr, .pad = nullptr,
    .rumble = nullptr, .picture_top = 0, .picture_height = 480,
};

extern "C" EMSCRIPTEN_KEEPALIVE int ff_browser_boot() {
    static bool booted = false;
    if (booted) return 2;
    try {
        std::filesystem::create_directories("/hdd/logs");
        logx::Init("/hdd/logs", "fuzion");
        app::Load();
        input::Init();
        std::ifstream file("/game/default.xbe", std::ios::binary);
        if (!file) { std::fprintf(stderr, "Missing locally admitted /game/default.xbe\n"); return 3; }
        std::vector<uint8_t> image((std::istreambuf_iterator<char>(file)), {});
        if (image.size() != 2834432) { std::fprintf(stderr, "Incompatible local XBE size\n"); return 4; }
        auto disc = std::make_unique<kernel::Disc>();
        std::string discError;
        if (!disc->Open("/game/game.iso", discError)) {
            std::fprintf(stderr, "Cannot mount local game disc: %s\n", discError.c_str());
            return 8;
        }
        kernel::MountDisc(std::move(disc));
        if (!kernel::Boot(std::move(image), "default.xbe", "/game", "/hdd")) return 5;
        booted = true;
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "SDK boot failed: %s\n", error.what());
        return 6;
    } catch (...) {
        std::fprintf(stderr, "SDK boot failed with a guest/runtime exception\n");
        return 7;
    }
}
