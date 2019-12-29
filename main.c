// C Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct event { // Struct for storing an event.
    char name[100]; // Name can be up to 100 char long

    int dayS; // dd
    int monthS; // mm
    int yearS; // yyyy

    int dayF; // dd
    int monthF; // mm
    int yearF; // yyyy

    int timeSM; // MM
    int timeSH; // HH

    int timeFM; // MM
    int timeFH; // HH

    int dayTil; // Not currently used. Made to store time until event.
    int monthTil;
    int yearTil;

    int booked; // Not currently used. Made to store bool value for if the user has booked the event or not.
    int transport; // Not currently used. Made to store bool value for if the user has booked transport for the event or not
    char mode[20]; // Not currently used. Made to store string for mode of transport booked.
};

// Function Prototypes.
void nameEvent(struct event *ev);
void dateEvent(struct event *ev);
void timeEvent(struct event *ev);
int createEvent(struct event ev[], int numSaved);
int displayEvents(struct event ev[], int numSaved);
int deleteEvent(struct event ev[], int numSaved);
void editEvent(struct event ev[], int numSaved);
struct event *sortEvents(struct event ev[], int numSaved);
int loadEvents(struct event ev[], int numSaved);
void saveEvents(struct event ev[], int numSaved);
int menu();
int options();
int getNumInp(int min, int max);
void getYNInp(char *test);
void line();

// Main
int main()
{
    int numSaved = 0; // Used to store the number of events currently saved.

    struct event events[1000]; // Array with lots of allocated memory for storage of events while program is running.

    numSaved = loadEvents(events, numSaved); // Set value of numSaved from file and load events from file

    int status = menu(); // Used as a state variable for deciding which menu etc to show.
    int subStatus = 0; // Used as a state variable for sub menus, e.g. event options.
    int disp = 1; // Used as a state variable for verifying there are events to display.

    while(status > 0){ // Run while user inputs value greater than 0 on menu prompt
        switch(status){ // Check value of status
            case 1: // Adding new event

                numSaved = c reateEvent(events, numSaved); // CreateEvent adds new event to events array and returns new value for numSaved

                status = menu(); // Menu prompt to get new status value

                break;

            case 2: // Displaying events

                memcpy(&events, sortEvents(events, numSaved), sizeof(struct event) * numSaved); // Sort the events, sortEvents return a sorted array of event structs, which is copied to the main events array

                disp = displayEvents(events, numSaved); // Display the events, function returns 0 if none to show, returns 1 if some shown
                if(disp == 0){ // No events to show
                    status = menu(); // Menu prompt to get new status value

                    break;
                }
                else if(disp == 1){ // Events to show

                    subStatus = options(); // Options menu prompt to get new substatus value

                    switch(subStatus){ // Check value of substatus
                        case 0: // Back to menu
                            status = menu(); // Menu prompt

                            break;

                        case 1: // Delete event
                            numSaved = deleteEvent(events, numSaved); // Delete event, function returns new value for numSaved

                            break;

                        case 2: // Edit event
                            editEvent(events, numSaved); // Edit event, function is void so no return

                            break;
                    }
                }

                break;

            case 3: // Save events to file
                saveEvents(events, numSaved); // Save events, function is void so no return

                FILE *numPtr = fopen("numEvents.txt", "w"); // Write number of saved events to file
                fprintf(numPtr,"%d", numSaved);
                fclose(numPtr);

                status = menu(); // Menu prompt

                break;

            case 4: // Manually load events from save file
                numSaved = loadEvents(events, numSaved); // Load events, function returns

                status = menu(); // Menu prompt

                break;
        }
    }

    printf("Exiting..."); //If status is less than 1 (hence 0), program exits.
    exit(0);

    return 0; // Stops compiler complaining about non void function
}

// Function for sorting given list of event structs and returning the sorted list.
struct event *sortEvents(struct event ev[], int numSaved){
    struct event *toSort = malloc(sizeof(struct event)* numSaved); //Define local toSort array and allocate correct amount of memory based on size of event struct and how many are saved

    for(int k=0;k<=numSaved-1; k++){ //Copy contents of given ev array to local toSort array
        toSort[k] = ev[k];
    }

    //Bubble sort algorithm
    struct event temp; //Need a temporary event struct for use when swapping elements
    for(int i=0; i<= numSaved-1; i++){ //Loop for the number of events saved-1
        for(int j=0; j<= numSaved - i - 2; j++){ //Last elements are already sorted so don't need to check them
            if(toSort[j].yearS > toSort[j+1].yearS){ //Different years
                temp = toSort[j]; //swap elements
                toSort[j] = toSort[j+1];
                toSort[j+1] = temp;
            }
            else if(toSort[j].yearS == toSort[j+1].yearS){ //Same years
                if(toSort[j].monthS > toSort[j+1].monthS){ //Different months
                    temp = toSort[j]; //swap elements
                    toSort[j] = toSort[j+1];
                    toSort[j+1] = temp;
                }
                else if(toSort[j].monthS == toSort[j+1].monthS){ //Same months
                    if(toSort[j].dayS > toSort[j+1].dayS){//Different days
                        temp = toSort[j]; //swap elements
                        toSort[j] = toSort[j+1];
                        toSort[j+1] = temp;
                    }
                    else if(toSort[j].dayS == toSort[j+1].dayS){//Same days
                        if(toSort[j].timeSH > toSort[j+1].timeSH){//different hours
                            temp = toSort[j]; //swap elements
                            toSort[j] = toSort[j+1];
                            toSort[j+1] = temp;
                        }
                        else if(toSort[j].timeSH == toSort[j+1].timeSH){//same hours
                            if(toSort[j].timeSM > toSort[j+1].timeSM){//different mins
                                temp = toSort[j]; //swap elements
                                toSort[j] = toSort[j+1];
                                toSort[j+1] = temp;
                            }
                            //if else, then event is at the exact same date/time
                        }
                    }
                }
            }
        }
    }
    return toSort; //return the array which has now been sorted
}

// Function to edit a chosen event
void editEvent(struct event ev[], int numSaved){
    printf("> Select event to edit: "); // Prompt user to input an event to edit
    int toEdit = getNumInp(1,numSaved);
    line();

    while(toEdit == -1){ // Input validation, getNumInp  returns -1 if invalid input
        printf("> Select event to edit: ");
        toEdit = getNumInp(1,numSaved);
        line();
    }

    printf("Choose edit to make: \n"); // Print options for user to choose from
    printf("1. Name.\n");
    printf("2. Date.\n");
    printf("3. Time.\n");
    line();
    printf("> Select option: "); // Prompt user to input an edit to make
    int edit = getNumInp(1,3);
    line();

    while(edit == -1){ // Input validation, getNumInp returns -1 if invalid input
        printf("Choose edit to make: \n");
        printf("1. Name.\n");
        printf("2. Date.\n");
        printf("3. Time.\n");
        line();
        printf("> Select option: ");
        edit = getNumInp(1,3);
        line();
    }

    switch(edit){ // Check value of 'edit'
        case 1: // Edit name
            nameEvent(&ev[toEdit-1]);
            break;

        case 2: // Edit date
            dateEvent(&ev[toEdit-1]);
            break;

        case 3: // Edit time
            timeEvent(&ev[toEdit-1]);
            break;
    }
}

// Function for deleting an event
int deleteEvent(struct event ev[], int numSaved){
    printf("> Enter event number to delete: \n"); // Prompt user to enter an event to delete
    int toDel = getNumInp(1,numSaved);

    while(toDel == -1){ // Input validation
        printf("> Enter event number to delete: \n");
        toDel = getNumInp(1,numSaved);
    }

    for(int i=toDel-1; i<= numSaved-1; i++){ // Loops from index of item to delete to end and replaces each value with its following one, hence removing the starting value
        ev[i] = ev[i+1];
    }

    return numSaved - 1; // Subtract 1 from numSaved and return
}

// Function for loading events from save file
int loadEvents(struct event ev[], int numSaved){
    FILE *numPtr = fopen("numEvents.txt", "r"); // Get updated number of events from save file
    fscanf(numPtr, "%d", &numSaved);

    FILE *evPtr; // Open save file for events
    evPtr = fopen("savedEvents.txt", "rb");

    if(numPtr == NULL){
        perror("Critical error cannot find 'numEvents.txt'");
        printf("Exiting...\n");
        line();
        exit(0);
    }

    if(evPtr == NULL){
        perror("Failed to open 'savedEvents.txt'");
        printf("Returning to Menu...\n");
        line();

        return(0);
    }
    else{
        for(int i=0; i<= numSaved-1; i++){ // Read all saved events from file and add to array
        fread(&ev[i], sizeof(struct event), 1, evPtr);
        }

        printf("%d Events successfully loaded.\n", numSaved);
        line();

        fclose(evPtr); // Close the opened files
        fclose(numPtr);

        return numSaved; // Return the updated number of saved events
    }
}

// Function for saving events to save file
void saveEvents(struct event ev[], int numSaved){
    FILE *evPtr; // Open save file for writing
    evPtr = fopen("savedEvents.txt", "wb");

    if(evPtr == NULL){
        perror("Failed to open file: ");
        printf("Returning to Menu...\n");
        line();

        return(0);
    }
    else{
        for(int i=0; i<= numSaved-1; i++){ // Write all structs from array to file
            fwrite(&ev[i], sizeof(struct event), 1, evPtr);
        }
        fclose(evPtr);

        printf("Events successfully saved to file. Returning to menu...\n");
        line();
    }
}

// Function to get and validate an integer input
int getNumInp(int min, int max){
    int rtn; // Stores return of the scanf function
    int val; // Stores the user input value

    rtn = scanf("%d", &val); // Scanf returns 0 if integer is not entered
        if(rtn == EOF){ // User generated EOF
            printf("Input cancelled by user.\n");
            return -1;
        }
        else if(rtn == 0){ // Invalid input
            printf("You must enter a numerical value.\n");
            fflush(stdin);
            return -1;
        }
        else if(val < min || val > max){ // Input out of range
            printf("Input not in valid range.\n");
            fflush(stdin);
            return -1;
        }
        else{ // Valid input
            fflush(stdin);
            return val;
        }
}

// Function for getting a character based boolean input
void getYNInp(char *test){
    scanf("%c", test); // Get char input from user
    fflush(stdin);

    while(*test != 'N' || *test != 'n' || *test != 'Y' || *test != 'y'){ // If input isn't a valid Y/N answer
        if(*test == 'N' || *test == 'n' || *test == 'Y' || *test == 'y'){ // For some reason this is needed because the while wasn't breaking properly without it
            break;
        }
        printf("> Invalid input, try again: "); // Invalid input, re-prompt
        scanf("%c", test);
        fflush(stdin);
    }
}

// Function for drawing a line in the console, makes it easier to draw one than copy-pasting the printf repeatedly.
void line(){
    printf("----------------------------------------------\n");
}

// Function for displaying the main menu and getting an input
int menu(){
    printf("MENU\n"); // Display menu
    line();
    printf("1. Add new event.\n");
    printf("2. View events.\n");
    printf("3. Save events.\n");
    printf("4. Load events.\n");
    printf("0. Exit.\n");
    line();
    printf("> Select option: "); // Prompt for and get input
    int stat;
    stat = getNumInp(0,4);
    line();

    while(stat == -1){ // Validate input
        printf("> Select option: ");
        stat = getNumInp(0,4);
        line();
    }

    return stat; // Return the value of user input
}

// Function for displaying 'event options' menu
int options(){
    printf("Event Options\n"); // Display menu
    line();
    printf("1. Delete an event.\n");
    printf("2. Edit an event.\n");
    printf("0. Back to menu.\n");
    line();
    printf("> Select option: "); // Prompt user for and get input
    int stat;
    stat = getNumInp(0,2);
    line();

    while(stat == -1){ // Validate input
        printf("> Select option: ");
        stat = getNumInp(0,2);
        line();
    }

    return stat; // Return the value of user input
}

// Function for calling all appropriate functions to create an event
int createEvent(struct event ev[], int numSaved){
    nameEvent(&ev[numSaved]); // Getting event name
    dateEvent(&ev[numSaved]); // Getting event date
    timeEvent(&ev[numSaved]); // Getting event time

    numSaved ++; // Add 1 to the numSaved value and return it
    return numSaved;
}

// Function for setting the name of an event
void nameEvent(struct event *ev){
    printf("> Enter a name for event: "); // Prompt the user for a name input
    gets((*ev).name);
    line();
}

// Function for setting the date of an event
void dateEvent(struct event *ev){
    printf("> Enter start date of Event (numerical format)\n");

    printf(">   Day (dd): "); // Prompt user to enter day
    (*ev).dayS = getNumInp(1,31);
    while((*ev).dayS == -1){ // Validate input
        printf(">   Day (dd): ");
        (*ev).dayS = getNumInp(1,31);
    }

    printf(">   Month (mm): "); // Prompt user to enter a month
    (*ev).monthS = getNumInp(1,12);
    while((*ev).monthS == -1){ // Validate input
        printf(">   Month (mm): ");
        (*ev).monthS = getNumInp(1,12);
    }

    printf(">   Year (yyyy): "); // Prompt user to enter a year
    (*ev).yearS = getNumInp(1000,9999);
    while((*ev).yearS == -1){ // Validate input
        printf(">   Year (yyyy): ");
        (*ev).yearS = getNumInp(1000,9999);
    }

    char same = 'Y'; // Stores boolean character input

    line();
    printf("> Does the event end on the same date? (Y/N): "); // Prompt user for and get input
    getYNInp(&same);
    line();

    if(same == 'N' || same == 'n'){ // Event ends of different date to start
        printf("> Enter finishing date of Event (numerical format)\n");

        printf(">   Day (dd): "); // Prompt user for day input
        (*ev).dayF = getNumInp(1,31);
        while((*ev).dayF == -1){ // Validate input
            printf(">   Day (dd): ");
            (*ev).dayF = getNumInp(1,31);
        }

        printf(">   Month (mm): "); // Prompt user for month input
        (*ev).monthF = getNumInp(1,12);
        while((*ev).monthF == -1){ // Validate input
            printf(">   Month (mm): ");
            (*ev).monthF = getNumInp(1,12);
        }

        printf(">   Year (yyyy): "); // Prompt user for year input
        (*ev).yearF = getNumInp(1000,9999);
        while((*ev).yearF == -1){
            printf(">   Year (yyyy): "); // Validate input
            (*ev).yearF = getNumInp(1000,9999);
        }
        line();
    }
    else if(same == 'Y' || same == 'y'){ // End date is same as start
        (*ev).dayF = (*ev).dayS;
        (*ev).monthF = (*ev).monthS;
        (*ev).yearF = (*ev).yearS;
    }
}

// Function for setting the time of an event
void timeEvent(struct event *ev){
    char known; // Stores boolean character input
    printf("> Do you know the starting time? (Y/N) "); // Prompt user for and get Y/N input for if start time is known
    getYNInp(&known);
    line();

    if(known == 'Y' || known =='y'){ // Start time is known
        printf("> Enter start hour of event (HH): "); // Prompt user for and get hour input
        (*ev).timeSH = getNumInp(0,23);
        line();

        while((*ev).timeSH == -1){ // Validate input
            printf("> Enter start hour of event (HH): ");
            (*ev).timeSH = getNumInp(0,23);
            line();
        }

        printf("> Enter start minute of event (MM): "); // Prompt user for and get minute input
        (*ev).timeSM = getNumInp(0,59);
        line();

        while((*ev).timeSM == -1){ // Validate input
            printf("> Enter start minute of event (MM): ");
            (*ev).timeSM = getNumInp(0,59);
            line();
        }
    }
    else if(known == 'N' || known == 'n'){ // Start time not known
        (*ev).timeSH = -1;
        (*ev).timeSM = -1;
    }

    printf("> Do you know the ending time? (Y/N) "); // Prompt user for input for if end time is known
    getYNInp(&known);
    line();

    if(known == 'Y' || known == 'y'){ // End time known
        printf("> Enter end hour of event (HH): "); // Prompt user for and get hour input
        (*ev).timeFH = getNumInp(0,23);
        line();

        while((*ev).timeFH == -1){ // Validate input
            printf("> Enter end hour of event (HH): ");
            (*ev).timeFH = getNumInp(0,23);
            line();
        }

        printf("> Enter end minute of event(MM): "); // Prompt user for and get hour input
        (*ev).timeFM = getNumInp(0,59);
        line();

        while((*ev).timeFM == -1){ // Validate input
            printf("> Enter end minute of event (MM): ");
            (*ev).timeFM = getNumInp(0,59);
            line();
        }
    }
    else if(known == 'N' || known == 'n'){ // End time not known
        (*ev).timeFM = -1;
        (*ev).timeFH = -1;
    }
}

// Function for displaying all events
int displayEvents(struct event ev[], int numSaved){
    if(numSaved == 0){ // No events currently loaded
        printf("No existing events. Returning to menu...\n");
        line();
        return(0);
    }
    else{
        for(int i=0; i<=numSaved-1;i++){ // Loop through all events
            printf("Event %d: %s\n", i+1, ev[i].name); // Display details of event

            if(ev[i].dayS >= 10 && ev[i].monthS >= 10){ // Display start date with appropriate leading zeros
                printf("-Start Date: %d/%d/%d\n", ev[i].dayS, ev[i].monthS, ev[i].yearS);
            }
            else if(ev[i].dayS < 10 && ev[i].monthS >= 10){
                printf("-Start Date: 0%d/%d/%d\n", ev[i].dayS, ev[i].monthS, ev[i].yearS);
            }
            else if(ev[i].dayS >= 10 && ev[i].monthS < 10){
                printf("-Start Date: %d/0%d/%d\n", ev[i].dayS, ev[i].monthS, ev[i].yearS);
            }
            else if(ev[i].dayS < 10 && ev[i].monthS < 10){
                printf("-Start Date: 0%d/0%d/%d\n", ev[i].dayS, ev[i].monthS, ev[i].yearS);
            }

            if(ev[i].timeSH == -1){ // Event does have known start time
                printf("-Start Time unknown\n");
            }
            else if(ev[i].timeSH >= 10 && ev[i].timeSM >=10){ //Event has start time, add appropriate leading zeros
                printf("-Start Time: %d:%d\n", ev[i].timeSH, ev[i].timeSM);
            }
            else if(ev[i].timeSH < 10 && ev[i].timeSM >=10){
                printf("-Start Time: 0%d:%d\n", ev[i].timeSH, ev[i].timeSM);
            }
            else if(ev[i].timeSH >= 10 && ev[i].timeSM < 10){
                printf("-Start Time: %d:0%d\n", ev[i].timeSH, ev[i].timeSM);
            }
            else if(ev[i].timeSH < 10 && ev[i].timeSM < 10){
                printf("-Start Time: 0%d:0%d\n", ev[i].timeSH, ev[i].timeSM);
            }

            if(ev[i].dayF >= 10 && ev[i].monthF >= 10){ // Display end date with appropriate leading zeros
                printf("-End Date: %d/%d/%d\n", ev[i].dayF, ev[i].monthF, ev[i].yearF);
            }
            else if(ev[i].dayF < 10 && ev[i].monthF >= 10){
                printf("-End Date: 0%d/%d/%d\n", ev[i].dayF, ev[i].monthF, ev[i].yearF);
            }
            else if(ev[i].dayF >= 10 && ev[i].monthF < 10){
                printf("-End Date: %d/0%d/%d\n", ev[i].dayF, ev[i].monthF, ev[i].yearF);
            }
            else if(ev[i].dayF < 10 && ev[i].monthF < 10){
                printf("-End Date: 0%d/0%d/%d\n", ev[i].dayF, ev[i].monthF, ev[i].yearF);
            }

            if(ev[i].timeFH == -1){ // Event doesn't have known end time
                printf("-End Time unknown\n");
            }
            else if(ev[i].timeFH >= 10 && ev[i].timeFM >=10){ // Event has known end time, add appropriate leading zeros
                printf("-End Time: %d:%d\n", ev[i].timeFH, ev[i].timeFM);
            }
            else if(ev[i].timeFH < 10 && ev[i].timeFM >=10){
                printf("-End Time: 0%d:%d\n", ev[i].timeFH, ev[i].timeFM);
            }
            else if(ev[i].timeFH >= 10 && ev[i].timeFM < 10){
                printf("-End Time: %d:0%d\n", ev[i].timeFH, ev[i].timeFM);
            }
            else if(ev[i].timeFH < 10 && ev[i].timeFM < 10){
                printf("-End Time: 0%d:0%d\n", ev[i].timeFH, ev[i].timeFM);
            }

            line();
        }
        return(1);
    }
}
