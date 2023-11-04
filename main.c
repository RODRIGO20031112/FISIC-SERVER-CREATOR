#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Chip {
    char id[20];
    int gb;
} Chip;

typedef struct Bar {
    char id[20];
    Chip *chips;
    int numChips;
} Bar;

typedef struct Plate {
    char id[20];
    Bar *bars;
    int numBars;
} Plate;

typedef struct CPU {
    char id[20];
    Plate *plates;
    int numPlates;
} CPU;

typedef struct Machine {
    char id[20];
    char state[20];
    CPU *cpus;
    int numCPUs;
    int damaged;
} Machine;

typedef struct System {
    char id[20];
    Machine *machines;
    int numMachines;
} System;

void freeMachine(Machine *machine) {
    for (int i = 0; i < machine->numCPUs; i++) {
        for (int j = 0; j < machine->cpus[i].numPlates; j++) {
            for (int k = 0; k < machine->cpus[i].plates[j].numBars; k++) {
                free(machine->cpus[i].plates[j].bars[k].chips);
            }
            free(machine->cpus[i].plates[j].bars);
        }
        free(machine->cpus[i].plates);
    }
    free(machine->cpus);
}

void createMachine(System *system) {
    Machine machine;

    printf("Put id in your machine: ");
    fgets(machine.id, sizeof(machine.id), stdin);

    machine.damaged = 0;

    printf("Enter the state symbol of your machine: ");
    fgets(machine.state, sizeof(machine.state), stdin);

    printf("How many CPUs do you want to create in the machine?: ");
    scanf("%d", &machine.numCPUs);

    getchar();

    machine.cpus = (CPU *)malloc(machine.numCPUs * sizeof(CPU));

    for (int i = 0; i < machine.numCPUs; i++) {
        printf("Put id in your CPU: ");
        fgets(machine.cpus[i].id, sizeof(machine.cpus[i].id), stdin);

        printf("How many plates do you want to create in CPU?: ");
        scanf("%d", &machine.cpus[i].numPlates);

        getchar();

        machine.cpus[i].plates = (Plate *)malloc(machine.cpus[i].numPlates * sizeof(Plate));

        for (int j = 0; j < machine.cpus[i].numPlates; j++) {
            printf("Put id in your plate: ");
            fgets(machine.cpus[i].plates[j].id, sizeof(machine.cpus[i].plates[j].id), stdin);

            printf("How many bars do you want to create in plate?: ");
            scanf("%d", &machine.cpus[i].plates[j].numBars);

            getchar();

            machine.cpus[i].plates[j].bars = (Bar *)malloc(machine.cpus[i].plates[j].numBars * sizeof(Bar));

            for (int k = 0; k < machine.cpus[i].plates[j].numBars; k++) {
                printf("Put id in your bar: ");
                fgets(machine.cpus[i].plates[j].bars[k].id, sizeof(machine.cpus[i].plates[j].bars[k].id), stdin);

                printf("How many chips do you want to create in bar?: ");
                scanf("%d", &machine.cpus[i].plates[j].bars[k].numChips);

                getchar();

                machine.cpus[i].plates[j].bars[k].chips = (Chip *)malloc(machine.cpus[i].plates[j].bars[k].numChips * sizeof(Chip));

                for (int l = 0; l < machine.cpus[i].plates[j].bars[k].numChips; l++) {
                    printf("Put id in your chip: ");
                    fgets(machine.cpus[i].plates[j].bars[k].chips[l].id, sizeof(machine.cpus[i].plates[j].bars[k].chips[l].id), stdin);

                    printf("What is the capacity of this chip ?: ");
                    scanf("%d", &machine.cpus[i].plates[j].bars[k].chips[l].gb);

                    getchar();
                }
            }
        }
    }

    system->numMachines++;
    system->machines = (Machine *)realloc(system->machines, system->numMachines * sizeof(Machine));
    system->machines[system->numMachines - 1] = machine;
}

void readMachine(const Machine *machine) {
    int machineCapacity = 0;

    if (machine->damaged != 0) {
        printf("\033[1;31m");
    }

    printf("Machine ID: %s", machine->id);
    printf("Machine ST: %s", machine->state);
    printf("Machine DM: %d\n", machine->damaged);

    for (int i = 0; i < machine->numCPUs; i++) {
        printf("|  CPU ID: %s", machine->cpus[i].id);

        for (int j = 0; j < machine->cpus[i].numPlates; j++) {
            printf("|    Plate ID: %s", machine->cpus[i].plates[j].id);

            for (int k = 0; k < machine->cpus[i].plates[j].numBars; k++) {
                printf("|      Bar ID: %s", machine->cpus[i].plates[j].bars[k].id);

                for (int l = 0; l < machine->cpus[i].plates[j].bars[k].numChips; l++) {
                    printf("|       Chip ID: %s|       Chip capacity: %dGB\n", machine->cpus[i].plates[j].bars[k].chips[l].id, machine->cpus[i].plates[j].bars[k].chips[l].gb);
                    machineCapacity += machine->cpus[i].plates[j].bars[k].chips[l].gb;
                }
            }
        }
    }

    printf("Machine capacity: %d GB\n", machineCapacity);

    if (machine->damaged != 0) {
        printf("\033[0m");
    }
}

void updateMachine(System *system) {
    char targetId[20];

    printf("Enter the ID of the machine you want to update: ");
    fgets(targetId, sizeof(targetId), stdin);

    for (int i = 0; i < system->numMachines; i++) {
        if (strcmp(targetId, system->machines[i].id) == 0) {
            printf("Update the state symbol of your machine: ");
            fgets(system->machines[i].state, sizeof(system->machines[i].state), stdin);

            printf("Do you want to update the exams for this machine? (1 for Yes, 0 for No): ");
            scanf("%d", &system->machines[i].damaged);

            printf("Update the number of CPUs: ");
            scanf("%d", &system->machines[i].numCPUs);
            getchar();  

            system->machines[i].cpus = (CPU *)realloc(system->machines[i].cpus, system->machines[i].numCPUs * sizeof(CPU));

            for (int j = 0; j < system->machines[i].numCPUs; j++) {
                printf("Update the number of plates for CPU %d: ", j + 1);
                scanf("%d", &system->machines[i].cpus[j].numPlates);
                getchar();  

                system->machines[i].cpus[j].plates = (Plate *)realloc(system->machines[i].cpus[j].plates, system->machines[i].cpus[j].numPlates * sizeof(Plate));

                for (int k = 0; k < system->machines[i].cpus[j].numPlates; k++) {
                    printf("Update the number of bars for Plate %d in CPU %d: ", k + 1, j + 1);
                    scanf("%d", &system->machines[i].cpus[j].plates[k].numBars);
                    getchar();  

                    system->machines[i].cpus[j].plates[k].bars = (Bar *)realloc(system->machines[i].cpus[j].plates[k].bars, system->machines[i].cpus[j].plates[k].numBars * sizeof(Bar));

                    for (int l = 0; l < system->machines[i].cpus[j].plates[k].numBars; l++) {
                        printf("Update the number of chips for Bar %d in Plate %d in CPU %d: ", l + 1, k + 1, j + 1);
                        scanf("%d", &system->machines[i].cpus[j].plates[k].bars[l].numChips);
                        getchar();  

                        system->machines[i].cpus[j].plates[k].bars[l].chips = (Chip *)realloc(system->machines[i].cpus[j].plates[k].bars[l].chips, system->machines[i].cpus[j].plates[k].bars[l].numChips * sizeof(Chip));

                        for (int m = 0; m < system->machines[i].cpus[j].plates[k].bars[l].numChips; m++) {
                            printf("Update the capacity of Chip %d for Bar %d in Plate %d in CPU %d: ", m + 1, l + 1, k + 1, j + 1);
                            scanf("%d", &system->machines[i].cpus[j].plates[k].bars[l].chips[m].gb);
                            getchar();  
                        }
                    }
                }
            }

            printf("Machine with ID %s updated successfully.\n", targetId);
            return;
        }
    }

    printf("Machine with ID %s not found.\n", targetId);
}

void deleteMachine(System *system) {
    char targetId[20];

    printf("Enter the ID of the machine you want to delete: ");
    fgets(targetId, sizeof(targetId), stdin);

    for (int i = 0; i < system->numMachines; i++) {
        if (strcmp(targetId, system->machines[i].id) == 0) {

            for (int j = i; j < system->numMachines - 1; j++) {
                system->machines[j] = system->machines[j + 1];
            }

            system->numMachines--;

            printf("Machine with ID %s deleted successfully.\n", targetId);
            return;
        }
    }

    printf("Machine with ID %s not found.\n", targetId);
}

void damagedMachine(System *system) {
    char targetId[20];

    printf("Enter the ID of the machine you want to update: ");
    fgets(targetId, sizeof(targetId), stdin);

    for (int i = 0; i < system->numMachines; i++) {
        if (strcmp(targetId, system->machines[i].id) == 0) {

            system->machines[i].damaged = 1;

            system->machines[i].cpus = (CPU *)realloc(system->machines[i].cpus, system->machines[i].numCPUs * sizeof(CPU));

            printf("Machine with ID %s updated successfully.\n", targetId);
            return;
        }
    }

    printf("Machine with ID %s not found.\n", targetId);
}

void repairMachine(System *system) {
    char targetId[20];

    printf("Enter the ID of the machine you want to update: ");
    fgets(targetId, sizeof(targetId), stdin);

    for (int i = 0; i < system->numMachines; i++) {
        if (strcmp(targetId, system->machines[i].id) == 0) {

            system->machines[i].damaged = 0;

            system->machines[i].cpus = (CPU *)realloc(system->machines[i].cpus, system->machines[i].numCPUs * sizeof(CPU));

            printf("Machine with ID %s updated successfully.\n", targetId);
            return;
        }
    }

    printf("Machine with ID %s not found.\n", targetId);
}

void exportMachine(const Machine *machine, const char *outputFilename) {
    int machineCapacity = 0;
    FILE *file = stdout;  

    if (outputFilename != NULL) {
        file = fopen(outputFilename, "w");
        if (file == NULL) {
            fprintf(stderr, "Error opening file: %s\n", outputFilename);
            return;
        }
    }

    fprintf(file, "Machine ID: %s", machine->id);
    fprintf(file, "Machine ST: %s", machine->state);
    fprintf(file, "Machine DM: %d\n", machine->damaged);

    for (int i = 0; i < machine->numCPUs; i++) {
        fprintf(file, "|  CPU ID: %s", machine->cpus[i].id);

        for (int j = 0; j < machine->cpus[i].numPlates; j++) {
            fprintf(file, "|    Plate ID: %s", machine->cpus[i].plates[j].id);

            for (int k = 0; k < machine->cpus[i].plates[j].numBars; k++) {
                fprintf(file, "|      Bar ID: %s", machine->cpus[i].plates[j].bars[k].id);

                for (int l = 0; l < machine->cpus[i].plates[j].bars[k].numChips; l++) {
                    fprintf(file, "|       Chip ID: %s|       Chip capacity: %dGB\n",
                            machine->cpus[i].plates[j].bars[k].chips[l].id,
                            machine->cpus[i].plates[j].bars[k].chips[l].gb);
                            machineCapacity += machine->cpus[i].plates[j].bars[k].chips[l].gb;
                }
            }
        }
    }

    fprintf(file, "Machine capacity: %d GB\n", machineCapacity);
    printf("Fisic Server export sucessfully!\n");

    if (outputFilename != NULL) {
        fclose(file);
    }
}

int main() {
    System system;
    system.numMachines = 0;
    system.machines = NULL;

    int loginOption;

    do {
        printf("How do you want to log in?\n");
        printf("1 - Employee\n");
        printf("2 - Manager\n");
        printf("3 - Exit\n");
        printf("Option: ");
        scanf("%d", &loginOption);

        getchar();  

        switch (loginOption) {
            case 1: {
                char employeeEmail[20] = "employee@gmail.com\n";
                char employeePassword[20] = "123456789\n";
                int employeeOption;

                char email[50];
                char password[50];

                printf("Enter your email: ");
                fgets(email, sizeof(email), stdin);

                printf("Enter your password: ");
                fgets(password, sizeof(password), stdin);

                if (strcmp(email, employeeEmail) == 0 && strcmp(password, employeePassword) == 0) {
                    printf("Welcome employee@gmail.com\n");

                    do {
                        printf("1 - Read Machine\n");
                        printf("2 - Damaged machine\n");
                        printf("3 - Repair machine\n");
                        printf("4 - Export machine\n");
                        printf("5 - Go back\n");
                        printf("6 - Exit\n");
                        printf("Option: ");
                        scanf("%d", &employeeOption);

                        getchar();
                        switch (employeeOption) {
                            case 1:
                            for (int i = 0; i < system.numMachines; i++) readMachine(&system.machines[i]);
                            break;
                        case 2:
                            damagedMachine(&system);
                            break;
                        case 3:
                            repairMachine(&system);
                            break;
                        case 4:
                            for (int i = 0; i < system.numMachines; i++) exportMachine(&system.machines[i], "FISIC-SERVER.txt");  
                            break;
                        case 5:     
                            break;
                        case 6:
                            return 0;        
                            break;
                        default:
                            printf("Opção inválida\n");
                        }
                    } while (employeeOption != 5);
                } else {
                    printf("Wrong email or password\n");
                }
                break;
            }
            case 2: {
                char managerEmail[20] = "manager@gmail.com\n";
                char managerPassword[20] = "987654321\n";
                int managerOption;

                char email[50];
                char password[50];

                printf("Enter your email: ");
                fgets(email, sizeof(email), stdin);

                printf("Enter your password: ");
                fgets(password, sizeof(password), stdin);

                if (strcmp(email, managerEmail) == 0 && strcmp(password, managerPassword) == 0) {
                    printf("Welcome manager@gmail.com\n");

                    do {
                        printf("1 - Create machine\n");
                        printf("2 - Read machine\n");
                        printf("3 - Update machine\n");
                        printf("4 - Delete machine\n");
                        printf("5 - Go back\n");
                        printf("6 - Exit\n");
                        printf("Option: ");
                        scanf("%d", &managerOption);

                        getchar();
                        switch (managerOption) {
                            case 1:
                            createMachine(&system);
                            break;
                        case 2:
                            for (int i = 0; i < system.numMachines; i++) readMachine(&system.machines[i]);
                            break;
                        case 3:
                            updateMachine(&system);
                            break;
                        case 4:
                            deleteMachine(&system);
                            break;
                        case 5:
                            break;
                        case 6:
                            return 0;
                            break;
                        default:
                            printf("Opção inválida\n");
                        }
                    } while (managerOption != 5);
                } else {
                    printf("Wrong email or password\n");
                }
                break;
            }
            case 3:
                return 0;  
            default:
                printf("Opção inválida\n");
        }
    } while (loginOption != 3);

    return 0;
}