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

    constexpr static const char* g_kWindowTitle = "cap.gb";

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
    constexpr static const int32_t g_kRenderWidth = 160;
    constexpr static const int32_t g_kRenderHeight = 144;
    constexpr static const int32_t g_kWindowWidth = g_kRenderWidth * 6;
    constexpr static const int32_t g_kWindowHeight = g_kRenderHeight * 6;

    SDL_Window* pWindow = nullptr;
    SDL_Renderer* pRenderer = nullptr;
    SDL_Texture* pTexture = nullptr;

    bool running = true;

    uint64_t totalTicks = 0;

    uint32_t pixels[3][g_kRenderWidth * g_kRenderHeight]; 

    static uint32_t ARGB(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
    {
        return (r << 24) | (g << 16) | (b << 8) | a;
    }
    // Call this within every render loop
    // Fills screen with randomly generated colored pixels
    int32_t Render();
    int Start();
    void ChangeColor(uint8_t x, uint8_t y, uint32_t r, uint32_t g, uint32_t b, uint32_t a, int layer = 0);
    void ChangeColor(uint8_t x, uint8_t y, uint32_t color, int layer = 0);
    // Free resources
    void Shutdown(SDL_Window** ppWindow, SDL_Renderer** ppRenderer, SDL_Texture** ppTexture);
};

#endif // !RENDERER_H