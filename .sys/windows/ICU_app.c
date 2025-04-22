#include <windows.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

#define WINDOW_TITLE "Error openning app"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define WHITE_COLOR (SDL_Color){255, 255, 255, 255}
#define BLUE_COLOR (SDL_Color){137, 180, 250, 255}
#define BUBBLE_RADIUS 15

#define TEXT_SIZE 100
#define TEXT_STR "SDL"
#define TEXT_VEL 3

struct App {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    SDL_Event event;
    bool is_running;
};

struct Text {
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_FRect rect;
};

bool text_new(struct Text **text, SDL_Renderer *renderer) {
    *text = calloc(1, sizeof(struct Text));
    if (*text == NULL) {
        return false;
    }
    struct Text *t = *text;

    t->renderer = renderer;

    TTF_Font *font = TTF_OpenFont("JetBrainsMonoNL-Regular.ttf", size);
    if (!font) {
        return NULL;
    }

    SDL_Surface *surf = TTF_RenderText_Blended(font, "TESTSSS", WHITE_COLOR);
    if (!surf) {
        return false;
    }

    t->rect.w = 0;
    t->rect.h = 0;

    t->texture = SDL_CreateTextureFromSurface(t->renderer, surf);
    SDL_DestroySurface(surf);
    surf = NULL;
    if (!t->texture) {
        return false;
    }

    SDL_FreeSurface(surface);
    TTF_CloseFont(font);

    return true;
}

void text_draw(const struct Text *t) {
    SDL_RenderCopyF(t->renderer, t->texture, NULL, &t->rect);
}

void text_free(struct Text **t) {
    if (*t != NULL) {
        struct SimpleText *st = *t;
        if (st->texture)
            SDL_DestroyTexture(st->texture);
        free(*simple_text);
        *simple_text = NULL;
    }
}

bool app_init_sdl(struct App *app) {
    TTF_Init();
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return false;
    }

    if (!load_font) {
        return false;
    }

    app->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!app->window) {
        return false;
    }

    app->renderer = SDL_CreateRenderer(app->window, NULL);
    if (!app->renderer) {
        return false;
    }

    return true;
}

bool app_new(struct App **app) {
    *app = calloc(1, sizeof(struct App));
    if (*app == NULL) {
        return false;
    }
    struct App *_app = *app;

    if (!app_init_sdl(_app)) {
        return false;
    }

    _app->is_running = true;

    return true;
}

void app_events(struct App *app) {
    while (SDL_PollEvent(&app->event)) {
        switch (app->event.type) {
        case SDL_EVENT_QUIT:
            app->is_running = false;
            break;

        default:
            break;
        }
    }
}

void app_free(struct App **app) {
    if (*app) {
        struct App *_app = *app;

        if (_app->renderer) {
            SDL_DestroyRenderer(_app->renderer);
            _app->renderer = NULL;
        }
        if (_app->window) {
            SDL_DestroyWindow(_app->window);
            _app->window = NULL;
        }

        SDL_Quit();
        free(_app);

        _app = NULL;
        *app = NULL;
    }
}

bool app_run(struct App *app) {
    while (app->is_running) {
        app_events(app);

        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);
        text_draw()

        SDL_RenderPresent(app->renderer);

        SDL_Delay(16);
    }

    return false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    CreateProcess("x86_x64.exe", NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    bool exit_status = EXIT_FAILURE;
    struct App *app = NULL;

    if (app_new(&app)) {
        if (app_run(app)) {
            exit_status = EXIT_SUCCESS;
        }
    }

    app_free(&app);

    return exit_status;
}
