#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// Grid constant
#define GRID_W 41   // Columns
#define GRID_H 21   // Rows
#define ORIGIN_X (GRID_W / 2)     // Center horizontally
#define ORIGIN_Y (GRID_H - 2)     // Near bottom

// Structs
typedef struct {
    double angle;       // Rotation angle in degrees
    char name[20];      // Joint name
} Joint;

typedef struct {
    double length;      // Length of segment
    Joint joint;        // Joint at the end of the segment
} Link;

typedef struct {
    Link *links;        // Arrey of links
    int num_links;      // Number of links
} Arm;

typedef struct {
    double x;
    double y;
} Vec2;

// Functions
void print_arm(Arm arm);
double deg2rad(double degrees);
void jointPositions(const Arm* arm, Vec2* positions);
void buildArm(Arm* arm);
void clear_grid(char grid[GRID_H][GRID_W]);
void draw_axes(char grid[GRID_H][GRID_W]);
void print_grid(char grid[GRID_H][GRID_W]);
void world_to_grid(double x, double y, int *gc, int *gr, double scale);
void draw_line(char grid[GRID_H][GRID_W], int c0, int r0, int c1, int r1, char ch);
void draw_arm_ascii(const Arm* arm, const Vec2* pos, double scale);

// Main
int main(void) {
    Arm arm = {0};

    buildArm(&arm);

    // Compute and display positions
    Vec2 positions[arm.num_links + 1];
    jointPositions(&arm, positions);

    printf("\nJoint positions:\n");
    for (int i = 0; i <= arm.num_links; i++) {
        printf("    Joint %d: (%.2f, %.2f)\n", i, positions[i].x, positions[i].y);
    }

    // Print end effektor position
    printf("\nEnd effektor position: (%.2f, %.2f)\n", positions[arm.num_links].x,
           positions[arm.num_links].y);
    free(arm.links);

    draw_arm_ascii(&arm, positions, 0.8);
    return 0;
}

void print_arm(Arm arm) {
    printf("=================================\n");
    printf("    Robotic Arm Configuration\n");
    printf("=================================\n\n");
    printf("Total links: %d\n\n", arm.num_links);

    for (int i = 0; i < arm.num_links; i++) {
        printf("Link %d:\n", i + 1);
        printf("    Length: %.2f units\n", arm.links[i].length);
        printf("    Joint: %s\n", arm.links[i].joint.name);
        printf("    Angle: %.2f°\n\n", arm.links[i].joint.angle);
    }

    printf("=================================\n");
}

// Convert degrees to radians
double deg2rad(double degrees) {
    return degrees * M_PI / 180.0;
}

// Compute joint positions
void jointPositions(const Arm* arm, Vec2* positions) {
    double theta = 0.0;
    positions[0].x = 0.0;
    positions[0].y = 0.0;

    for (int i = 0; i < arm->num_links; i++) {
        theta += deg2rad(arm->links[i].joint.angle);
        positions[i + 1].x = positions[i].x + arm->links[i].length * cos(theta);
        positions[i + 1].y = positions[i].y + arm->links[i].length * sin(theta);
        }
}

// Build arm from input
void buildArm(Arm* arm) {
    // Number of links
    do {
        printf("Enter number of links (1 - 3): ");
        scanf("%d", &arm->num_links);
        if (arm->num_links < 1 || arm->num_links > 3)
            printf("Please enter a number between 1 and 3.\n");
    } while (arm->num_links < 1 || arm->num_links > 3);

    // Allocate memory
    arm->links = malloc(arm->num_links * sizeof(Link));
    if (arm->links == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    // Input data
    for (int i = 0; i < arm->num_links; i++) {
        printf("\n-- Link %d --\n", i + 1);

        do {
            printf("Enter length: ");
            scanf("%lf", &arm->links[i].length);
            if (arm->links[i].length <= 0)
                printf("Length must be positive.\n");
        } while (arm->links[i].length <= 0);

        printf("Enter angle (degrees): ");
        scanf("%lf", &arm->links[i].joint.angle);

        sprintf(arm->links[i].joint.name, "joint %d", i +1);
    }
}

// Grid utilities start
void clear_grid(char grid[GRID_H][GRID_W]) {
    for (int r = 0; r < GRID_H; r++)
        for (int c = 0; c < GRID_W; c++)
            grid[r][c] = ' ';
}

void draw_axes(char grid[GRID_H][GRID_W]) {
    // x-axis
    for (int c = 0; c < GRID_W; c++) grid[ORIGIN_Y][c] = '-';
    // y-axis
    for (int r = 0; r < GRID_H; r++) grid[ORIGIN_X][r] = '|';
    grid[ORIGIN_Y][ORIGIN_X] = '+';     // origin
}

void print_grid(char grid[GRID_H][GRID_W]) {
    // top border
    putchar('\n');
    for (int c = 0; c < GRID_W + 2; c++) putchar('#');
    putchar('\n');

    for (int r = 0; r < GRID_H; r++) {
        putchar('#');
        for (int c = 0; c < GRID_W; c++) putchar(grid[r][c]);
        putchar('#');
        putchar('\n');
    }

    for (int c = 0; c < GRID_W + 2; c++) putchar('#');
    putchar('\n');
}
// Grid utilities end

// World (double x,y) -> grid (col, row)
void world_to_grid(double x, double y, int *gc, int *gr, double scale) {
    // flip y for screen rows 
    *gc = ORIGIN_X + (int)lround(x * scale);
    *gr = ORIGIN_Y - (int) lround(y * scale);
}

void draw_line(char grid[GRID_H][GRID_W], int c0, int r0, int c1, int r1, char ch) {
    int dc = abs(c1 - c0), dr = -abs(r1 - r0);
    int sc = (c0 < c1) ? 1 : -1;
    int sr = (r0 < r1) ? 1 : -1;
    int err = dc + dr;

    while (1) {
        if (r0 >= 0 && r0 < GRID_H && c0 >= 0 && c0 < GRID_W)
            if (grid[r0][c0] == ' ') grid[r0][c0] = ch;     // Don't overwrite joint if already in place

            if (c0 == c1 && r0 == r1) break;
            int e2 = 2 * err;
            if (e2 >= dr) { err += dr; c0 += sc; }
            if (e2 <= dc) { err += dc; r0 += sr; }
    }
}

void draw_arm_ascii(const Arm* arm, const Vec2* pos, double scale) {
    char grid[GRID_H][GRID_W];
    clear_grid(grid);
    draw_axes(grid);

    // Plot links as line between joints
    for (int i = 0; i < arm->num_links; i++) {
        int c0, r0, c1, r1;
        world_to_grid(pos[i].x, pos[i].y, &c0, &r0, scale);     // Joint i
        world_to_grid(pos[i + 1].x, pos[i + 1].y, &c1, &r1, scale); // Joint i + 1
        draw_line(grid, c0, r0, c1, r1, '.');
    }

    // Plot joint and end effektor
    for (int i = 0; i <= arm->num_links; i++) {
        int c, r; world_to_grid(pos[i].x, pos[i].y, &c, &r, scale);
        if (r >= 0 && r < GRID_H && c >= 0 && c < GRID_W) {
            grid[r][c] = (i == 0) ? 'o' : (i == arm->num_links ? 'X' : '*');
        }
    }

    print_grid(grid);
}

