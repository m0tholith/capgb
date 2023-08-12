#include "Renderer.h"
#include "../HELPER.h"
#include <iostream>

int32_t CheckErrorSDL(int32_t result, std::string errorMessage)
{
    if (result)
        std::cout << errorMessage;

    return result;
}

inline uint32_t ARGB(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha)
{
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

//Marsaglia's xorshf generator (Fast Random Function)
static uint32_t s_randX = 123456789;
static uint32_t s_randY = 362436069;
static uint32_t s_randZ = 521288629;

inline uint32_t FastRand(void)
{
    uint32_t t;

    s_randX ^= s_randX << 16;
    s_randX ^= s_randX >> 5;
    s_randX ^= s_randX << 1;

    t = s_randX;
    s_randX = s_randY;
    s_randY = s_randZ;
    s_randZ = t ^ s_randX ^ s_randY;

    return s_randZ;
}

int32_t Renderer::Render()
{
    running = ProcessInput();

    uint64_t currentTick = SDL_GetPerformanceCounter();
    totalTicks += currentTick - lastTick;
    lastTick = currentTick;
    ++totalFramesRendered;

    // The Back Buffer texture may be stored with an extra bit of width (pitch) on the video card in order to properly
    // align it in VRAM should the width not lie on the correct memory boundary (usually four bytes).
    int32_t pitch = 0;

    // This will hold a pointer to the memory position in VRAM where our Back Buffer texture lies
    uint32_t* pPixelBuffer = nullptr;

    // Lock the memory in order to write our Back Buffer image to it
    if (!SDL_LockTexture(pTexture, NULL, (void**)&pPixelBuffer, &pitch))
    {
        pitch /= sizeof(uint32_t);
        for (int i = 0; i < g_kRenderWidth * g_kRenderHeight; i++)
            pPixelBuffer[i] = pixels[i];
        SDL_UnlockTexture(pTexture);

        SDL_RenderCopy(pRenderer, pTexture, NULL, NULL);
        SDL_RenderPresent(pRenderer);
        return 0;
    }
    else
        return g_kErrorOccurred;
}

int Renderer::Start()
{
    if (CheckErrorSDL(SetupSDL(&pWindow, &pRenderer, &pTexture), "Startup Failed. Aborting...\n"))
    {
        Shutdown(&pWindow, &pRenderer, &pTexture);
        return -1;
    }
    lastTick = SDL_GetPerformanceCounter();

    return 0;
}

void Renderer::ChangeColor(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    pixels[x + y * g_kRenderWidth] = ARGB(r, g, b, a);
}

bool Renderer::ProcessInput()
{
    // Return this value to tell the caller whether or not it should continue rendering
    // We will terminate the application if any key is pressed
    bool keepRenderLoopRunning = true;

    // Events are generated by SDL whenever something occurs system-wide
    // We are only interested in keyboard events and when the user closes the window
    // We will terminate the application if a key is pressed or if the window is manually closed
    SDL_Event event;

    // Process all events and return whether or not to quit

    /*const Uint8* state = SDL_GetKeyboardState(nullptr);

    if (state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL])
    {
        std::cout << "\n\nwritten to logfile\n\n";
        HELPER::WriteLogFile();
    }*/

    while (SDL_PollEvent(&event))
    {
        // Handle relevant SDL events
        switch (event.type)
        {
        case SDL_QUIT:
            keepRenderLoopRunning = false;
        }
    }

    // Let the caller know if it should continue rendering, otherwise terminate
    return keepRenderLoopRunning;
}

int32_t Renderer::SetupSDL(SDL_Window** ppWindow, SDL_Renderer** ppRenderer, SDL_Texture** ppTexture)
{
    SDL_Init(SDL_INIT_VIDEO);

    if (CheckErrorSDL(!ppWindow, "Potiner to Window* was null\n")) return -1;

    *ppWindow = CreateCenteredWindow(g_kWindowWidth, g_kWindowHeight, g_kWindowTitle);

    if (CheckErrorSDL(!*ppWindow, "No Window. Aborting..."))
    {
        Shutdown(ppWindow, ppRenderer, ppTexture);

        return -1;
    }

    if (CheckErrorSDL(!ppRenderer, "Pointer to Renderer* was null\n")) return -1;

    *ppRenderer = CreateRenderer(*ppWindow, true);

    if (CheckErrorSDL(!ppRenderer, "No Renderer. Aborting..."))
    {
        Shutdown(ppWindow, ppRenderer, ppTexture);

        return -1;
    }

    if (CheckErrorSDL(!ppTexture, "Pointer to Texture* was null\n")) return -1;

    *ppTexture = CreateBackBufferTexture(*ppRenderer);

    if (CheckErrorSDL(!*ppTexture, "No back buffer Texture. Aborting..."))
    {
        Shutdown(ppWindow, ppRenderer, ppTexture);

        return -1;
    }

    return 0;
}

void Renderer::Shutdown(SDL_Window** ppWindow, SDL_Renderer** ppRenderer, SDL_Texture** ppTexture)
{
    // Free the Back Buffer
    if (ppTexture)
    {
        SDL_DestroyTexture(*ppTexture);
        *ppTexture = nullptr;
    }

    // Free the SDL renderer
    if (ppRenderer)
    {
        SDL_DestroyRenderer(*ppRenderer);
        *ppRenderer = nullptr;
    }

    // Free the SDL window
    if (ppWindow)
    {
        SDL_DestroyWindow(*ppWindow);
        *ppWindow = nullptr;
    }

    // Display render and timing information
    std::cout << "Total Frames:    " << totalFramesRendered << "\n";
    std::cout << "Total Time:      " << static_cast<double>(totalTicks) / SDL_GetPerformanceFrequency() << "s\n";
    std::cout << "Average FPS:     " << static_cast<double>(totalFramesRendered) * SDL_GetPerformanceFrequency() / totalTicks << "\n";
}

SDL_Texture* Renderer::CreateBackBufferTexture(SDL_Renderer* pRenderer)
{
    SDL_Texture* pTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, g_kRenderWidth, g_kRenderHeight);

    CheckErrorSDL(!pTexture, "Failed to create Back Buffer Texture\n");

    return pTexture;
}

SDL_Renderer* Renderer::CreateRenderer(SDL_Window* pWindow, bool hardwareAccelerated)
{
    if (hardwareAccelerated)
        return SDL_CreateRenderer(pWindow, g_kRenderDeviceFlags, SDL_RENDERER_ACCELERATED);
    else
        return SDL_CreateRenderer(pWindow, g_kRenderDeviceFlags, SDL_RENDERER_SOFTWARE);
}

SDL_Window* Renderer::CreateCenteredWindow(uint32_t width, uint32_t height, std::string title)
{
    // Get current device's Display Mode to calculate window position
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    // Calculate where the upper-left corner of a centered window will be
    const int32_t x = DM.w / 2 - width / 2;
    const int32_t y = DM.h / 2 - height / 2;

    // Create the SDL window
    SDL_Window* pWindow = SDL_CreateWindow(g_kWindowTitle, x, y, width, height,
        SDL_WINDOW_ALLOW_HIGHDPI);

    CheckErrorSDL(!pWindow, "Failed to create Window\n");

    return pWindow;
}