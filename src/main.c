#include <stdio.h>
#include <string.h>

// Structs
typedef struct {
    double angle;       // Rotation angle in degrees
    char name[20];      // Joint name
} Joint;

typedef struct {
    double lenght;      // Lenght of segment
    Joint joint;        // Joint at the end of the segment
} Link;

typedef struct {
    Link *links;        // Arrey of links
    int num_links;      // Number of links
} Arm;

// Functions
void print_arm(Arm arm);

// Main
int main(void) {
    Arm arm;

    arm.num_links = 2;

    // Array of 2 links
    Link links[2];

    // Link 1
    links[0].lenght = 10.0;
    strcpy(links[0].joint.name, "Joint 1");
    links[0].joint.angle = 45.0;

    // Link 2
    links[1].lenght = 7.0;
    strcpy(links[1].joint.name, "Joint 2");
    links[1].joint.angle = 30.0;

    // Point arm.links to the array
    arm.links = links;

    print_arm(arm);
    return 0;
}

void print_arm(Arm arm) {
    printf("=================================\n");
    printf("    Robotic Arm Configuration\n");
    printf("=================================\n\n");
    printf("Total links: %d\n\n", arm.num_links);

    for (int i = 0; i < arm.num_links; i++) {
        printf("Link %d:\n", i + 1);
        printf("    Lenght: %.2f units\n", arm.links[i].lenght);
        printf("    Joint: %s\n", arm.links[i].joint.name);
        printf("    Angle: %.2f\n\n", arm.links[i].joint.angle);
    }

    printf("=================================\n");
}