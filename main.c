/*
 * main.c: main file to a pong clone
 */

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "font.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 200

typedef struct {            /*   _____________  */
    float x;                /*  |            |  */
    float y;                /*  |            |  */
    float w;                /*  |            |  */
    float h;                /*  |____________|  */
} rect;

typedef struct {
    uint32_t lastTime;
    uint32_t currentTime;
    uint32_t deltaTime;
} deltaTime;

static struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
    SDL_Event event;
    int quit;
    rect p1, p2, ball;
    struct {
        float x, y;
    } ballVel;
    int score1, score2, wins1, wins2;
} state;

void newGame() {
    state.quit = 0;
    state.p1 = (rect) {5, SCREEN_HEIGHT / 2 - 20, 5, 40};
    state.p2 = (rect) {SCREEN_WIDTH - 10, SCREEN_HEIGHT / 2 - 20, 5, 40};
    state.ball = (rect) {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 5, 5};
    state.ballVel.x = 1.0f;
    state.ballVel.y = 1.0f;
    state.score1 = 0;
    state.score2 = 0;
    state.wins1 = 0;
    state.wins2 = 0;
    memset(state.pixels, 0, sizeof(state.pixels));
}

void player(int x, int y, uint32_t color) {
    // Draw player 1
    for(int i = 0; i < state.p1.w; i++) {
        for(int j = 0; j < state.p1.h; j++) {
            state.pixels[(x + i) + (y + j) * SCREEN_WIDTH] = color;
        }
    }
}

// NOTE: THIS ONLY WORKS FOR THIS GAME (I think) PLEASE DO NOT USE THIS FUNCTION OR COPY IT
bool checkCollision(rect pos1, rect pos2) {
    return pos1.x < pos2.x + pos2.w &&
           pos1.x + pos1.w > pos2.x &&
           pos1.y < pos2.y + pos2.h &&
           pos1.y + pos1.h > pos2.y;
}

void ball(int x, int y, uint32_t color) {
    // Draw ball
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            state.pixels[(x + i) + (y + j) * SCREEN_WIDTH] = color;
        }
    }
}

void update() {
    state.ball.x += state.ballVel.x;
    state.ball.y += state.ballVel.y;

    // breaks the game if the ball goes out of bounds
    // NOTE: THIS CODE BARELY WORKS
    if(state.ball.y <= 0 || state.ball.y >= SCREEN_HEIGHT - 5) {
        state.ballVel.y = -state.ballVel.y;
    }    if(state.ball.x <= 0 || state.ball.x >= SCREEN_WIDTH - 5) {
        state.ballVel.x = -state.ballVel.x;
    }
    if (checkCollision(state.ball, state.p1) || checkCollision(state.ball, state.p2)) {
        state.ballVel.x = -state.ballVel.x - 0.1f;
        // check if the ball hits the top or bottom of the paddle
        // NOTE: THIS CODE BARELY WORKS AND BUGS OUT
        if (state.ball.y < state.p1.y || state.ball.y > state.p1.y + state.p1.h) {
            state.ballVel.y = -state.ballVel.y;
        }
    }
}

void vline(int x, int y1, int y2, uint32_t color) {
    for (int y = y1; y < y2; y++) {
        state.pixels[y * SCREEN_WIDTH + x] = color;
    }
}

void render() {
    // TODO: Make a better font system
    player(state.p1.x, state.p1.y, 0xff00ffff);
    player(state.p2.x, state.p2.y, 0xff00ffff);
    update();
    ball(state.ball.x, state.ball.y, 0xff00ffff);
    render_font(state.pixels, (SCREEN_WIDTH / 2) - 2, 0, state.score1, SCREEN_WIDTH);
    render_font(state.pixels, (SCREEN_WIDTH / 2) + 1, 0, state.score2, SCREEN_WIDTH);
    render_font(state.pixels, 0, 0, 32, SCREEN_WIDTH);
    render_font(state.pixels, 1, 0, 18, SCREEN_WIDTH);
    render_font(state.pixels, 2, 0, 23, SCREEN_WIDTH);
    render_font(state.pixels, 3, 0, 28, SCREEN_WIDTH);
    render_font(state.pixels, 4, 0, 37, SCREEN_WIDTH);
    render_font(state.pixels, 5, 0, state.wins1, SCREEN_WIDTH);
    // player 2 wins
    render_font(state.pixels, SCREEN_WIDTH - 362, 0, 32, SCREEN_WIDTH);
    render_font(state.pixels, SCREEN_WIDTH - 361, 0, 18, SCREEN_WIDTH);
    render_font(state.pixels, SCREEN_WIDTH - 360, 0, 23, SCREEN_WIDTH);
    render_font(state.pixels, SCREEN_WIDTH - 359, 0, 28, SCREEN_WIDTH);
    render_font(state.pixels, SCREEN_WIDTH - 358, 0, 37, SCREEN_WIDTH);
    render_font(state.pixels, SCREEN_WIDTH - 357, 0, state.wins2, SCREEN_WIDTH);

    vline((SCREEN_WIDTH / 2) - 1, 0, SCREEN_HEIGHT, 0xFFFFFFFF);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
    }

    state.window =
            SDL_CreateWindow(
                    "DEMO",
                    SDL_WINDOWPOS_CENTERED_DISPLAY(0),
                    SDL_WINDOWPOS_CENTERED_DISPLAY(0),
                    1280,
                    720,
                    SDL_WINDOW_ALLOW_HIGHDPI);
    state.renderer =
            SDL_CreateRenderer(state.window, -1, SDL_RENDERER_PRESENTVSYNC);

    state.texture =
            SDL_CreateTexture(
                    state.renderer,
                    SDL_PIXELFORMAT_ARGB8888,
                    SDL_TEXTUREACCESS_STREAMING,
                    SCREEN_WIDTH,
                    SCREEN_HEIGHT);
    float lastTime = (float) SDL_GetTicks();
    float currentTime = 0;
    float deltaTime = 0;
    newGame();
    while (!state.quit) {
        // Update every frame
        currentTime = (float) SDL_GetTicks();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        SDL_Event ev;
        while(SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:
                    state.quit = true;
                    goto end;
            }
        }


        // Display FPS in the title
        // char title[256];
        // sprintf(title, "DEMO - FPS: %f", 1000.0f / deltaTime);
        // SDL_SetWindowTitle(state.window, title);

        const uint8_t *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_W] && state.p1.y > 0) {
            state.p1.y -= 0.1f * deltaTime;
        }
        if (keys[SDL_SCANCODE_S] && state.p1.y < SCREEN_HEIGHT - state.p1.h) {
            state.p1.y += 0.1f * deltaTime;
        }
        if (keys[SDL_SCANCODE_UP] && state.p2.y > 0) {
            state.p2.y -= 0.1f * deltaTime;
        }
        if (keys[SDL_SCANCODE_DOWN] && state.p2.y < SCREEN_HEIGHT - state.p2.h) {
            state.p2.y += 0.1f * deltaTime;
        }
        if (keys[SDL_SCANCODE_ESCAPE]) {
            state.quit = true;
        }
        // DEBUG KEYS
        if (keys[SDL_SCANCODE_1]) {
            state.score1 = 5;
        }
        if (keys[SDL_SCANCODE_2]) {
            state.score2 = 5;
        }
        if (state.ball.x <= 0) {
            state.score2++;
            state.ball.x = SCREEN_WIDTH / 2;
            state.ball.y = SCREEN_HEIGHT / 2;
        }
        if (state.ball.x >= SCREEN_WIDTH - 5) {
            state.score1++;
            state.ball.x = SCREEN_WIDTH / 2;
            state.ball.y = SCREEN_HEIGHT / 2;
        }

        if(state.score2 == 5) {
            fprintf(stdout, "Player 2 wins!\n");
            newGame();
        }
        if(state.score1 == 5) {
            fprintf(stdout, "Player 1 wins!\n");
            newGame();
        }

        memset(state.pixels, 0, sizeof(state.pixels));
        render();

        SDL_UpdateTexture(state.texture, NULL, state.pixels, SCREEN_WIDTH * 4);
        SDL_RenderCopyEx(
                state.renderer,
                state.texture,
                NULL,
                NULL,
                0.0,
                NULL,
                SDL_FLIP_NONE);
        SDL_RenderPresent(state.renderer);
    }
    end:

    SDL_DestroyTexture(state.texture);
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
    return 0;
}
