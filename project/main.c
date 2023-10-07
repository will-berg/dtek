/* main.c
   Main driver file for pong game on the PIC32
   Written by William Berg (willb) and Gustav Henningsson (ghenn).
   */
#include <pic32mx.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


// definitions taken from example project hello-display
#define DISPLAY_VDD PORTFbits.RF6
#define DISPLAY_VBATT PORTFbits.RF5
#define DISPLAY_COMMAND_DATA PORTFbits.RF4
#define DISPLAY_RESET PORTGbits.RG9

#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200


static const uint8_t const font[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 94, 0, 0, 0, 0,
	0, 0, 4, 3, 4, 3, 0, 0,
	0, 36, 126, 36, 36, 126, 36, 0,
	0, 36, 74, 255, 82, 36, 0, 0,
	0, 70, 38, 16, 8, 100, 98, 0,
	0, 52, 74, 74, 52, 32, 80, 0,
	0, 0, 0, 4, 3, 0, 0, 0,
	0, 0, 0, 126, 129, 0, 0, 0,
	0, 0, 0, 129, 126, 0, 0, 0,
	0, 42, 28, 62, 28, 42, 0, 0,
	0, 8, 8, 62, 8, 8, 0, 0,
	0, 0, 0, 128, 96, 0, 0, 0,
	0, 8, 8, 8, 8, 8, 0, 0,
	0, 0, 0, 0, 96, 0, 0, 0,
	0, 64, 32, 16, 8, 4, 2, 0,
	0, 62, 65, 73, 65, 62, 0, 0,
	0, 0, 66, 127, 64, 0, 0, 0,
	0, 0, 98, 81, 73, 70, 0, 0,
	0, 0, 34, 73, 73, 54, 0, 0,
	0, 0, 14, 8, 127, 8, 0, 0,
	0, 0, 35, 69, 69, 57, 0, 0,
	0, 0, 62, 73, 73, 50, 0, 0,
	0, 0, 1, 97, 25, 7, 0, 0,
	0, 0, 54, 73, 73, 54, 0, 0,
	0, 0, 6, 9, 9, 126, 0, 0,
	0, 0, 0, 102, 0, 0, 0, 0,
	0, 0, 128, 102, 0, 0, 0, 0,
	0, 0, 8, 20, 34, 65, 0, 0,
	0, 0, 20, 20, 20, 20, 0, 0,
	0, 0, 65, 34, 20, 8, 0, 0,
	0, 2, 1, 81, 9, 6, 0, 0,
	0, 28, 34, 89, 89, 82, 12, 0,
	0, 0, 126, 9, 9, 126, 0, 0,
	0, 0, 127, 73, 73, 54, 0, 0,
	0, 0, 62, 65, 65, 34, 0, 0,
	0, 0, 127, 65, 65, 62, 0, 0,
	0, 0, 127, 73, 73, 65, 0, 0,
	0, 0, 127, 9, 9, 1, 0, 0,
	0, 0, 62, 65, 81, 50, 0, 0,
	0, 0, 127, 8, 8, 127, 0, 0,
	0, 0, 65, 127, 65, 0, 0, 0,
	0, 0, 32, 64, 64, 63, 0, 0,
	0, 0, 127, 8, 20, 99, 0, 0,
	0, 0, 127, 64, 64, 64, 0, 0,
	0, 127, 2, 4, 2, 127, 0, 0,
	0, 127, 6, 8, 48, 127, 0, 0,
	0, 0, 62, 65, 65, 62, 0, 0,
	0, 0, 127, 9, 9, 6, 0, 0,
	0, 0, 62, 65, 97, 126, 64, 0,
	0, 0, 127, 9, 9, 118, 0, 0,
	0, 0, 38, 73, 73, 50, 0, 0,
	0, 1, 1, 127, 1, 1, 0, 0,
	0, 0, 63, 64, 64, 63, 0, 0,
	0, 31, 32, 64, 32, 31, 0, 0,
	0, 63, 64, 48, 64, 63, 0, 0,
	0, 0, 119, 8, 8, 119, 0, 0,
	0, 3, 4, 120, 4, 3, 0, 0,
	0, 0, 113, 73, 73, 71, 0, 0,
	0, 0, 127, 65, 65, 0, 0, 0,
	0, 2, 4, 8, 16, 32, 64, 0,
	0, 0, 0, 65, 65, 127, 0, 0,
	0, 4, 2, 1, 2, 4, 0, 0,
	0, 64, 64, 64, 64, 64, 64, 0,
	0, 0, 1, 2, 4, 0, 0, 0,
	0, 0, 48, 72, 40, 120, 0, 0,
	0, 0, 127, 72, 72, 48, 0, 0,
	0, 0, 48, 72, 72, 0, 0, 0,
	0, 0, 48, 72, 72, 127, 0, 0,
	0, 0, 48, 88, 88, 16, 0, 0,
	0, 0, 126, 9, 1, 2, 0, 0,
	0, 0, 80, 152, 152, 112, 0, 0,
	0, 0, 127, 8, 8, 112, 0, 0,
	0, 0, 0, 122, 0, 0, 0, 0,
	0, 0, 64, 128, 128, 122, 0, 0,
	0, 0, 127, 16, 40, 72, 0, 0,
	0, 0, 0, 127, 0, 0, 0, 0,
	0, 120, 8, 16, 8, 112, 0, 0,
	0, 0, 120, 8, 8, 112, 0, 0,
	0, 0, 48, 72, 72, 48, 0, 0,
	0, 0, 248, 40, 40, 16, 0, 0,
	0, 0, 16, 40, 40, 248, 0, 0,
	0, 0, 112, 8, 8, 16, 0, 0,
	0, 0, 72, 84, 84, 36, 0, 0,
	0, 0, 8, 60, 72, 32, 0, 0,
	0, 0, 56, 64, 32, 120, 0, 0,
	0, 0, 56, 64, 56, 0, 0, 0,
	0, 56, 64, 32, 64, 56, 0, 0,
	0, 0, 72, 48, 48, 72, 0, 0,
	0, 0, 24, 160, 160, 120, 0, 0,
	0, 0, 100, 84, 84, 76, 0, 0,
	0, 0, 8, 28, 34, 65, 0, 0,
	0, 0, 0, 126, 0, 0, 0, 0,
	0, 0, 65, 34, 28, 8, 0, 0,
	0, 0, 4, 2, 4, 2, 0, 0,
	0, 120, 68, 66, 68, 120, 0, 0,
};


bool inGame;
int scoreToReach = 5;

char textbuffer[4][16];

// 2D array matrix representing the OLED-display, with 32 rows(y) and 128 columns(x)
uint8_t two_d_array[32][128];
uint8_t num_of_rows = sizeof(two_d_array)/sizeof(two_d_array[0]);
uint8_t num_of_columns = sizeof(two_d_array[0])/sizeof(two_d_array[0][0]);
// 1D array representation that can actually be used to show something on the display
uint8_t one_d_array[512];
int one_d_length = sizeof(one_d_array)/sizeof(one_d_array[0]);


struct Player {
    char name[20];
    int score;
};

// Ball values
struct Ball {
    float height, width, xSpeed, ySpeed, xPos, yPos;
};

// Paddle values
struct Paddle {
    float height, width, speed, xPos, yPos;
};

/* Initialize players, the paddles, and the ball.
Unmentioned variables will be zero by default */
struct Player player1 = {.name = 'L', .score = 0};
struct Player player2 = {.name = 'R', .score = 0};

struct Paddle paddle1 = {.height = 7, .width = 2, .speed = 1};
struct Paddle paddle2 = {.height = 7, .width = 2, .speed = 1};

struct Ball ball = {.height = 2, .width = 2, .xSpeed = 0.4, .ySpeed = 0.4};


void delay(int cyc) {
	int i;
	for (i = cyc; i > 0; i--);
}

uint8_t spi_send_recv(uint8_t data) {
	while (!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while (!(SPI2STAT & 0x01));
	return SPI2BUF;
}

void display_init(void) {
	DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
	delay(10);
	DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
	delay(1000000);

	spi_send_recv(0xAE);
	DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;
	delay(10);
	DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
	delay(10);

	spi_send_recv(0x8D);
	spi_send_recv(0x14);

	spi_send_recv(0xD9);
	spi_send_recv(0xF1);

	DISPLAY_VBATT_PORT &= ~DISPLAY_VBATT_MASK;
	delay(10000000);

	spi_send_recv(0xA1);
	spi_send_recv(0xC8);

	spi_send_recv(0xDA);
	spi_send_recv(0x20);

	spi_send_recv(0xAF);
}

void display_image(int x, const uint8_t *data) {
	int i, j;

	for (i = 0; i < 4; i++) {
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));

		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
		/* This part of the code was changed to fit the entire screen. j counted up to 32 in the
        sample code, which we had to change to 128 to get control of the entire screen.
        We also removed the inversion (~) of the data. */
		for (j = 0; j < num_of_columns; j++)
			spi_send_recv(data[i*num_of_columns + j]);
	}
}

void display_string(int line, char *s) {
	int i;
	if (line < 0 || line >= 4)
		return;
	if (!s)
		return;

	for (i = 0; i < 16; i++)
		if (*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

void display_update(void) {
	int i, j, k;
	int c;
	for (i = 0; i < 4; i++) {
		DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;

		for (j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if (c & 0x80)
				continue;

			for (k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

/* Initialization function for I/O controls */
void ioinit(void) {
    /* Set up peripheral bus clock */
    OSCCON &= ~0x180000;
    OSCCON |= 0x080000;

    /* Set up output pins */
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    TRISECLR = 0xFF;
    PORTE = 0x0;

    /* Output pins for display signals */
    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;

    /* Set up input pins */
    TRISDSET = 0xE0; // Buttons 2-4
    TRISFSET = 0x2;  // Button 1

    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;

    /* Clear SPIROV*/
    SPI2STATCLR &= ~0x40;
    /* Set CKP = 1, MSTEN = 1; */
    SPI2CON |= 0x60;

    /* Turn on SPI */
    SPI2CONSET = 0x8000;
}


/* Set all elements to zero in the 1D and 2D array */
void clearDisplay(void) {
    uint8_t row, column;
    int8_t x = -1;
    for (row = 0; row < num_of_rows; row++) {
        x++;
        for(column = 0; column < num_of_columns; column++) {
            two_d_array[row][column] = 0;
            if (x < 4) {
                one_d_array[x * num_of_columns + column] = 0;
            }
        }
    }
}

/* Function for drawing something in a 2d array representation of the display */
void setDisplayMatrix(int xPos, int yPos, int xLength, int yLength) {
    uint8_t row, column;
    for (row = 0; row < num_of_rows; row++) {
        for (column = 0; column < num_of_columns; column++) {
            if (column >= xPos && column <= (xPos + xLength) && row >= yPos && row <= (yPos + yLength)) {
                two_d_array[row][column] = 1;
            }
        }
    }
}

/* Function for translating the matrix representation to a 1d array with 512 points
point zero is in the top left of the display, 511 in the bottom right */
void fromMatrixToArray(void) {
    uint8_t page, column, row, one_d_byte;
    /* This will go page by page and column by column (128 columns and 8 rows for a page), addessing each pixel on
    the display at a time. 128 * 8 = 1024 pixels for each page. 1024 * 4 = 4096 = 512 * 8. Each column in each
    page represents 8 pixels, or one byte in the 512 byte 1d array */
    for (page = 0; page < 4; page++) {

        for (column = 0; column < num_of_columns; column++) {
            // Assume all pixels should be off
            one_d_byte = 0;

            for (row = 0; row < 8; row++) {
                // If the current pixel should be lit, set that bit to 1 in the one_d_byte
                if (two_d_array[row + (8 * page)][column] == 1) {
                    one_d_byte |= (1 << row);
                }
            }
            /* Set the byte with the correct bit values for the pixels
            specified by the two_d_array*/
            one_d_array[page * num_of_columns + column] = one_d_byte;
        }
    }
}

/* returns status of push-buttons, BTN1 is LSB and
BTN4 is MSB of the 4 bits returned */
int getButtons(void) {
    int buttons24 = (PORTD >> 4);
    buttons24 &= 0xE;
    int button1 = (PORTF >> 1);
    button1 &= 0x1;
    int buttonState = buttons24 | button1;
    return buttonState;
}

/* Redraw the game with initial values */
void resetGame(void) {
	clearDisplay();
    // Starting values for the paddles and the ball are set
    paddle1.xPos = 0 + paddle1.width;
    paddle1.yPos = num_of_rows/2 - paddle1.height/2;

    paddle2.xPos = (num_of_columns - 1) - paddle2.width;
    paddle2.yPos = num_of_rows/2 - paddle2.height/2;

    ball.xPos = num_of_columns/2;
    ball.yPos = num_of_rows/2;

    // Display the image with corret positions
    setDisplayMatrix(paddle1.xPos, paddle1.yPos, paddle1.width, paddle1.height);
    setDisplayMatrix(paddle2.xPos, paddle2.yPos, paddle2.width, paddle2.height);
    setDisplayMatrix(ball.xPos, ball.yPos, ball.width, ball.height);
    fromMatrixToArray();
    display_image(0, one_d_array);
}

/* Initialize the game by drawing the board and setting the inGame var to true */
void gameInit(void) {
    resetGame();
    inGame = true;
}

/* Update ball position and check for ball contact with the walls */
void update(void) {
    // Update ball position
    clearDisplay();
    ball.xPos += ball.xSpeed;
    ball.yPos += ball.ySpeed;

    // check for if the ball bounces on the top or bottom walls
    if ((ball.yPos + ball.height) > (num_of_rows - 1) || ball.yPos < 0) {
        ball.ySpeed *= -1;
    }
}

/* Reset player scores and toggle the inGame variable */
void gameOver(void) {
    inGame = false;
    player1.score = 0;
    player2.score = 0;
}

/* Check if a player scored a goal and if a player has won */
void goal(void) {
    if (ball.xPos < 0) {
        player1.score++;
        resetGame();
    }
    if (ball.xPos > num_of_columns) {
        player1.score++;
        resetGame();
    }
    // If either player has reached the winning score, end the game
    if (player1.score == scoreToReach || player2.score == scoreToReach) {
        gameOver();
    }
}

/* Check if a paddle and the ball have come in contact and if
a bounce should be registered */
void paddleBounce(void) {
    // paddle1 = left player
    if (ball.xPos >=  paddle1.xPos && ball.xPos <= (paddle1.xPos + paddle1.width) && (ball.yPos + ball.height) >= paddle1.yPos && (ball.yPos + ball.height) <= (paddle1.yPos + paddle1.height)) {
        ball.xSpeed *= -1;
    }
    // paddle2 = right player
    if ((ball.xPos + ball.width) >= paddle2.xPos && (ball.xPos + ball.width) <= (paddle2.xPos + paddle2.width) && (ball.yPos + ball.height) >= paddle2.yPos && (ball.yPos + ball.height) <= (paddle2.yPos + paddle2.height)) {
        ball.xSpeed *= -1;
    }
}

/* Move player 2 paddle up (BTN1 is pressed) */
void paddle2Up(void) {
    int buttonState = getButtons();
    if (paddle2.yPos >= (0) && (buttonState == 1 || buttonState == 5 || buttonState == 9)) {
        paddle2.yPos -= paddle2.speed;
        update();
    }
}

/* Move player 2 paddle down (BTN2 is pressed) */
void paddle2Down(void) {
    int buttonState = getButtons();
    if (paddle2.yPos <= (num_of_rows - paddle2.height) && (buttonState == 2 || buttonState == 6 || buttonState == 10)) {
        paddle2.yPos += paddle2.speed;
        update();
    }
}

/* Move player 1 paddle up (BTN3 is pressed) */
void paddle1Up(void) {
    int buttonState = getButtons();
    if (paddle1.yPos >= (0) && (buttonState == 4 || buttonState == 5 || buttonState == 6)) {
        paddle1.yPos -= paddle1.speed;
        update();
    }
}

/* Move player 1 paddle down (BTN4 is pressed) */
void paddle1Down(void) {
    int buttonState = getButtons();
    if (paddle1.yPos <= (num_of_rows - paddle1.height) && (buttonState == 8 || buttonState == 9 || buttonState == 10)) {
        paddle1.yPos += paddle1.speed;
        update();
    }
}

/* Check if any player wants to move their paddle */
void movePlayer(void) {
    paddle2Up();
    paddle2Down();
    paddle1Up();
    paddle1Down();
}


int main(void) {
    // Run all necessary initializations
    ioinit();
    display_init();
    gameInit();

    while (inGame) {
        setDisplayMatrix(paddle1.xPos, paddle1.yPos, paddle1.width, paddle1.height);
        setDisplayMatrix(paddle2.xPos, paddle2.yPos, paddle2.width, paddle2.height);
        setDisplayMatrix(ball.xPos, ball.yPos, ball.width, ball.height);
        fromMatrixToArray();
        display_image(0, one_d_array);
        // Constantly check for buttonpresses, paddlebounces and goals
        movePlayer();
        paddleBounce();
        goal();
        update();
    }
    display_string(1, "Game Over!");
    display_string(2, "Press any button");
    display_string(3, "to restart game");
    display_update();
    delay(5000000);
    while (true) {
        if (getButtons() != 0) {
            main();
        }
    }
    return 0;
}
