/*
 * main.c: main file to a pong clone
 */

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 200

typedef struct {
    int x;
    int y;
    int w;
    int h;
} v2;

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
    v2 p1, p2, ball, ballVel;
    int score1, score2, wins1, wins2, ballSpeed;
} state;

void init() {
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
    state.quit = 0;
    state.p1.x = 5;
    state.p1.y = SCREEN_HEIGHT/2 - 20;
    state.p1.w = 5;
    state.p1.h = 40;
    state.p2.x = SCREEN_WIDTH - 10;
    state.p2.y = SCREEN_HEIGHT/2 - 20;
    state.p2.w = 5;
    state.p2.h = 40;
    state.ball.x = SCREEN_WIDTH/2;
    state.ball.y = SCREEN_HEIGHT/2;
    state.ball.w = 5;
    state.ball.h = 5;
    state.ballVel.x = 1;
    state.ballVel.y = 1;
    state.ballSpeed = 1;
    state.score1 = 0;
    state.score2 = 0;
    state.wins1 = 0;
    state.wins2 = 0;
}

void player1(int x, int y, uint32_t color) {
    // Draw player 1
    int x1 = 5;
    for(int i = 0; i < state.p1.w; i++) {
        for(int j = 0; j < state.p1.h; j++) {
            state.pixels[(x + i) + (state.p1.y + j) * SCREEN_WIDTH] = color;
        }
    }
}
void player2(int x, int y, uint32_t color) {
    // Draw player 2
    int x2 = SCREEN_WIDTH - 10;
    for(int i = 0; i < state.p2.w; i++) {
        for(int j = 0; j < state.p2.h; j++) {
            state.pixels[(x + i) + (state.p2.y + j) * SCREEN_WIDTH] = color;
        }
    }
}

bool checkCollision(v2 pos1, v2 pos2) {
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

    if(state.ball.y <= 0 || state.ball.y >= SCREEN_HEIGHT - 5) {
        state.ballVel.y = -state.ballVel.y;
    }    if(state.ball.x <= 0 || state.ball.x >= SCREEN_WIDTH - 5) {
        state.ballVel.x = -state.ballVel.x;
    }
    if (checkCollision(state.ball, state.p1) || checkCollision(state.ball, state.p2)) {
        state.ballVel.x = -state.ballVel.x;
    }
}

// number font data
const int font[10+27+1][8] = {
        {0x00, 0x00, 0x3C, 0x66, 0x6E, 0x76, 0x66, 0x3C},
        {0x00, 0x00, 0x18, 0x38, 0x18, 0x18, 0x18, 0x3C},
        {0x00, 0x00, 0x3C, 0x66, 0x0C, 0x18, 0x30, 0x7E},
        {0x00, 0x00, 0x3C, 0x66, 0x0C, 0x06, 0x66, 0x3C},
        {0x00, 0x00, 0x0C, 0x1C, 0x3C, 0x6C, 0x7E, 0x0C},
        {0x00, 0x00, 0x7E, 0x60, 0x7C, 0x06, 0x66, 0x3C},
        {0x00, 0x00, 0x3C, 0x66, 0x60, 0x7C, 0x66, 0x3C},
        {0x00, 0x00, 0x7E, 0x06, 0x0C, 0x18, 0x30, 0x30},
        {0x00, 0x00, 0x3C, 0x66, 0x3C, 0x66, 0x66, 0x3C},
        {0x00, 0x00, 0x3C, 0x66, 0x66, 0x3E, 0x06, 0x3C},
        // A
        {0x00, 0x00, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66},
        // B
        {0x00, 0x00, 0x3E, 0x66, 0x3E, 0x66, 0x66, 0x3E},
        // C
        {0x00, 0x00, 0x3C, 0x66, 0x60, 0x60, 0x66, 0x3C},
        // D
        {0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x3E},
        // E
        {0x00, 0x00, 0x7E, 0x60, 0x7C, 0x60, 0x60, 0x7E},
        // F
        {0x00, 0x00, 0x7E, 0x60, 0x7C, 0x60, 0x60, 0x60},
        // G
        {0x00, 0x00, 0x3C, 0x66, 0x60, 0x6E, 0x66, 0x3C},
        // H
        {0x00, 0x00, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66},
        // I
        {0x00, 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x3C},
        // J
        {0x00, 0x00, 0x0E, 0x06, 0x06, 0x06, 0x66, 0x3C},
        // K
        {0x00, 0x00, 0x66, 0x6C, 0x78, 0x78, 0x6C, 0x66},
        // L
        {0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E},
        // M
        {0x00, 0x00, 0xC6, 0xEE, 0xFE, 0xD6, 0xC6, 0xC6},
        // N
        {0x00, 0x00, 0x66, 0x76, 0x7E, 0x7E, 0x6E, 0x66},
        // O
        {0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C},
        // P
        {0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x60, 0x60},
        // Q
        {0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x6C, 0x36},
        // R
        {0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x6C, 0x66},
        // S
        {0x00, 0x00, 0x3C, 0x66, 0x38, 0x0C, 0x66, 0x3C},
        // T
        {0x00, 0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18},
        // U
        {0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C},
        // V
        {0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18},
        // W
        {0x00, 0x00, 0xC6, 0xC6, 0xD6, 0xFE, 0xEE, 0xC6},
        // X
        {0x00, 0x00, 0x66, 0x66, 0x3C, 0x3C, 0x66, 0x66},
        // Y
        {0x00, 0x00, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18},
        // Z
        {0x00, 0x00, 0x7E, 0x0C, 0x18, 0x30, 0x60, 0x7E},
        // !
        {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18},
        // :
        {0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18},
};

void render_font(int x, int y, int number) {
    int *font_data = (int *) &font[number];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int color = (font_data[i] >> (7 - j)) & 1;
            for (int k = 0; k < 9; k++) {
                for (int l = 0; l < 9; l++) {
                    state.pixels[(y * 9 + i) * SCREEN_WIDTH + x * 9 + j] = color ? 0xFFFFFFFF : 0x00000000;
                }
            }
        }
    }
}

void vline(int x, int y1, int y2, uint32_t color) {
    for (int y = y1; y < y2; y++) {
        state.pixels[y * SCREEN_WIDTH + x] = color;
    }
}

void render() {
    player1(state.p1.x, state.p1.y, 0xff00ffff);
    player2(state.p2.x, state.p2.y, 0xff00ffff);
    update();
    ball(state.ball.x, state.ball.y, 0xff00ffff);
    render_font((SCREEN_WIDTH / 2) - 2, 0, state.score1);
    render_font((SCREEN_WIDTH / 2) + 1, 0, state.score2);
    render_font(0, 0, 32);
    render_font(1, 0, 18);
    render_font(2, 0, 23);
    render_font(3, 0, 28);
    render_font(4, 0, 37);
    render_font(5, 0, state.wins1);
    // player 2 wins
    render_font(SCREEN_WIDTH - 362, 0, 32);
    render_font(SCREEN_WIDTH - 361, 0, 18);
    render_font(SCREEN_WIDTH - 360, 0, 23);
    render_font(SCREEN_WIDTH - 359, 0, 28);
    render_font(SCREEN_WIDTH - 358, 0, 37);
    render_font(SCREEN_WIDTH - 357, 0, state.wins2);

    vline((SCREEN_WIDTH / 2) - 1, 0, SCREEN_HEIGHT, 0xFFFFFFFF);
}

void gameOver() {
    if (state.score1 == 5) {
        state.wins1++;
        SDL_Log("Player 1 wins!\n");
    } else if (state.score2 == 5) {
        state.wins2++;
        SDL_Log("Player 2 wins!\n");
    }
    state.p1.y = SCREEN_HEIGHT / 2 - 20;
    state.p2.y = SCREEN_HEIGHT / 2;
    state.ball.x = SCREEN_WIDTH / 2;
    state.ball.y = SCREEN_HEIGHT / 2;
    state.ballVel.x = 1;
    state.ballVel.y = 1;
    state.score1 = 0;
    state.score2 = 0;
}

int main(int argc, char *argv[]) {

    init();
    uint32_t lastTime = SDL_GetTicks();
    uint32_t currentTime = 0;
    uint32_t deltaTime = 0;

    while (!state.quit) {
        SDL_Event ev;
        while(SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:
                    state.quit = true;
                    goto end;
            }
        }

        currentTime = SDL_GetTicks();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Display FPS in the title
        char title[256];
        sprintf(title, "DEMO - FPS: %f", 1000.0f / deltaTime);
        SDL_SetWindowTitle(state.window, title);

        uint8_t *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_W] && state.p1.y > 0) {
            state.p1.y -= 2;
        }
        if (keys[SDL_SCANCODE_S] && state.p1.y < SCREEN_HEIGHT - state.p1.h) {
            state.p1.y += 2;
        }
        if (keys[SDL_SCANCODE_UP] && state.p2.y > 0) {
            state.p2.y -= 2;
        }
        if (keys[SDL_SCANCODE_DOWN] && state.p2.y < SCREEN_HEIGHT - state.p2.h) {
            state.p2.y += 2;
        }
        if (keys[SDL_SCANCODE_ESCAPE]) {
            state.quit = true;
        }
        if (state.ball.x <= 0) {
            state.score2++;
            state.ball.x = SCREEN_WIDTH / 2;
            state.ball.y = SCREEN_HEIGHT / 2;
            if(state.score2 == 5) {
                gameOver();
            }
        }
        if (state.ball.x >= SCREEN_WIDTH - 5) {
            state.score1++;
            state.ball.x = SCREEN_WIDTH / 2;
            state.ball.y = SCREEN_HEIGHT / 2;
            if(state.score1 == 5) {
                gameOver();
            }
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
