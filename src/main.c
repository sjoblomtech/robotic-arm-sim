#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

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

// Main
int main(void) {
    Arm arm;

    arm.num_links = 2;

    // Array of 2 links
    Link links[2];

    // Link 1
    links[0].length = 10.0;
    strcpy(links[0].joint.name, "Joint 1");
    links[0].joint.angle = 45.0;

    // Link 2
    links[1].length = 7.0;
    strcpy(links[1].joint.name, "Joint 2");
    links[1].joint.angle = 30.0;

    // Point arm.links to the array
    arm.links = links;

    // Compute and display positions
    Vec2 positions[arm.num_links + 1];
    jointPositions(&arm, positions);

    printf("\nJoint positions:\n");
    for (int i = 0; i <= arm.num_links; i++) {
        printf("    Joint %d: (%.2f, %.2f)\n", i, positions[i].x, positions[i].y);
    }

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
        printf("    Angle: %.2f\n\n", arm.links[i].joint.angle);
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