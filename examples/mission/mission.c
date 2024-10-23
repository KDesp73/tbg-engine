/**
 * Mission example file
 *
 * @file examples/mision.c
 * @author KDesp73
 * @date 24/10/2024
 */

#include "mission.h"
#include "game.h"

int main(int argc, char** argv) {
    // You should use enums for ids
    GAME.mission = mission_init("Mission name", "The description", "The resolution", 
        objective_init(0, "Objective 0"),
        objective_init(1, "Objective 1"),
        objective_init(2, "Objective 2"),
        NULL
    );
    
    mission_show(GAME.mission);

    mission_complete_objective(GAME.mission, 0);
    mission_complete_objective(GAME.mission, 1);
    mission_complete_objective(GAME.mission, 2);
    
    mission_show(GAME.mission);

    return 0;
}

