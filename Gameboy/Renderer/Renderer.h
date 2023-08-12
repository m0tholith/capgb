#include <string>
#include <SDL/SDL.h>
#include <array>
#include <vector>

#ifndef RENDERER_H
#define RENDERER_H

class Renderer
{
private:
    constexpr static const int32_t g_kRenderDeviceFlags = -1;
    constexpr static const int32_t g_kErrorOccurred = -1;

    constexpr static const char* g_kWindowTitle = "GB.cap";

    uint64_t totalFramesRendered = 0;
    uint64_t lastTick = 0;

    SDL_Window* CreateCenteredWindow(uint32_t width, uint32_t height, std::string title);

    // Create SDL renderer and configure whether or not to use Hardware Acceleration
    SDL_Renderer* CreateRenderer(SDL_Window* pWindow, bool hardwareAccelerated);

    // Create an SDL Texture to use as a "back buffer"
    SDL_Texture* CreateBackBufferTexture(SDL_Renderer* pRenderer);

    // Initialize SDL Components 
    int32_t SetupSDL(SDL_Window** ppWindow, SDL_Renderer** ppRenderer, SDL_Texture** ppTexture);

    // Call this once during each render loop in order to determine when the user wishes to terminate the program
    bool ProcessInput();
public:
    constexpr static const int32_t g_kRenderWidth = 256;
    constexpr static const int32_t g_kRenderHeight = 240;
    constexpr static const int32_t g_kWindowWidth = g_kRenderWidth * 4;
    constexpr static const int32_t g_kWindowHeight = g_kRenderHeight * 4;

    SDL_Window* pWindow = nullptr;
    SDL_Renderer* pRenderer = nullptr;
    SDL_Texture* pTexture = nullptr;

    bool running = true;

    uint64_t totalTicks = 0;

    std::array<uint32_t, g_kRenderWidth * g_kRenderHeight> pixels;

    // Call this within every render loop
    // Fills screen with randomly generated colored pixels
    int32_t Render();
    int Start();
    void ChangeColor(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    // Free resources
    void Shutdown(SDL_Window** ppWindow, SDL_Renderer** ppRenderer, SDL_Texture** ppTexture);
};

#endif // !RENDERER_H