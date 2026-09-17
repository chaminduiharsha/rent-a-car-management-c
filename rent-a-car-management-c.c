/* SWIFT WHEELS - RENT A CAR MANAGEMENT SYSTEM (REFACTORED - FIXED INPUT LOOP)
    IN1101 Programming Fundamentals | Assignment  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/*  CONSTANTS    */

#define MAX_CUSTOMERS  100
#define MAX_CARS       100
#define MAX_RENTALS    100
#define MAX_INVOICES   100
#define MAX_BOOKINGS   100
#define RATE_PER_KM    40.0
#define FREE_KM_PER_DAY 100
#define ADVANCE_PERCENT 0.50

 /* Entity type constants for generic CRUD operations */

#define ENTITY_CUSTOMER 1
#define ENTITY_CAR      2
#define ENTITY_RENTAL   3
#define ENTITY_INVOICE  4
#define VIEW_AVAILABLE  5
#define VIEW_ACTIVE     6





/*  STRUCTURES  */

typedef struct
 {  int    custID;
    char   name[60];
    char   phone[20];
    char   licenseNo[20];
    char   email[60];
    int    isActive;
}   Customer ;


typedef struct
{   int    carID;
    char   brand[40];
    char   model[40];
    char   plateNo[20];
    char   category[20];
    double ratePerDay;
    int    odometer;
    int    isAvailable;
    int    isActive;
}  Car;


typedef struct
 {
    int    rentalID;
    int    custID;
    int    carID;
    char   pickupDate[20];
    char   returnDate[20];
    int    bookedDays;
    int    startOdometer;
    int    endOdometer;
    double advancePaid;
    int    status;
}  Rental;


typedef struct
{   int  invoiceID;
    int  rentalID;
    double baseCost;
    double extraKmCost;
    double lateFee;
    double totalDue;
    double advancePaid;
    double balance;
    int    isPaid;
} Invoice;



typedef struct
 {  int  logID;
    char  timestamp[30];
    char  action[100];
    int  referenceID;
}  AdminLog ;




/*  GLOBAL DATA ARRAYS */

Customer customers[MAX_CUSTOMERS];      //array to store all customer
Car      cars[MAX_CARS];                //array to store all cars
Rental   rentals[MAX_RENTALS];
Invoice  invoices[MAX_INVOICES];
AdminLog logs[MAX_BOOKINGS];




int custCount = 0;             //total customers in system
int carCount = 0;
int rentalCount = 0;
int invoiceCount= 0;
int logCount= 0;




/*   UTILITY FUNCTIONS (5) */

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);    // function is used to discard unwanted characters (including the newline character) from the standard input buffer

}

void getCurrentDateTime(char *buffer, int size)
 {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);                              /* 1. Get current date and time */
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

void getCurrentDate(char *buffer, int size)
 {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);                            /* 2. Get current date only */
    strftime(buffer, size, "%Y-%m-%d", t);
}


void printLine(char c, int n)
 {
    for (int i = 0; i < n; i++) putchar(c);                 /* 3. Print separator line */
    putchar('\n');
}

void addLog(const char *action, int refID)
 {
    if (logCount >= MAX_BOOKINGS) return;
    AdminLog *l = &logs[logCount++];
    l->logID = logCount;
    getCurrentDateTime(l->timestamp, sizeof(l->timestamp));            /* 4. Add log entry */
    strncpy(l->action, action, sizeof(l->action) - 1);
    l->referenceID = refID;
}


/*   HELPER FUNCTIONS (2) */



 int findCustomerIndex(int id)
 {
    for (int i = 0; i < custCount; i++)
        if (customers[i].custID == id && customers[i].isActive)
            return i;    /* 1. Find customer index by ID */
    return -1;
}


int findCarIndex(int id) {
    for (int i = 0; i < carCount; i++)
        if (cars[i].carID == id && cars[i].isActive)           /* 2. Find car index by ID */
return i;
    return -1;
}




/*  MAIN MENU-PART 1 */



void addCustomer()
{
    if (custCount >= MAX_CUSTOMERS)
        {
        printf("  [!] Customer database full.\n");
        return;
        }
    Customer *c = &customers[custCount];
    c->custID   = custCount + 1001;
    c->isActive = 1;

    printf("\n  --- Add Customer ---\n");

    printf("  Name        : ");
    fgets(c->name, sizeof(c->name), stdin);
    c->name[strcspn(c->name, "\n")] = '\0';

    printf("  Phone       : ");
    fgets(c->phone, sizeof(c->phone), stdin);
    c->phone[strcspn(c->phone, "\n")] = '\0';

    printf("  License No  : ");
    fgets(c->licenseNo, sizeof(c->licenseNo), stdin);
    c->licenseNo[strcspn(c->licenseNo, "\n")] = '\0';

    printf("  Email       : ");
    fgets(c->email, sizeof(c->email), stdin);
    c->email[strcspn(c->email, "\n")] = '\0';

    custCount++;
    char msg[100];
    sprintf(msg, "Customer added: %s (ID %d)", c->name, c->custID);
    addLog(msg, c->custID);
    printf("  [OK] Customer registered with ID: %d\n", c->custID);



}
      /* VIEW ALL type menus */

/*1.2.view all customer,2.2.view all cars,2.3.view available cars,3.5.view all rentals*/



void viewAll(int entityType)

{
    if (entityType == ENTITY_CUSTOMER)
        { printf("\n  %-6s %-25s %-15s %-15s %-30s\n","ID","Name","Phone","License","Email");
          printLine('-', 95);

       int found = 0;
        for (int i = 0; i < custCount; i++) {
            if (!customers[i].isActive)
                continue;
            Customer *c = &customers[i];

            printf("  %-6d %-25s %-15s %-15s %-30s\n",c->custID, c->name, c->phone, c->licenseNo, c->email);
            found++;
        }
        if (!found)
            printf("  No customers found.\n");
    }

    else if (entityType == ENTITY_CAR)
   {
        printf("\n  %-6s %-12s %-15s %-10s %-8s %-10s %-8s\n","ID", "Brand", "Model", "Plate", "Rate/Day", "Odometer", "Status");
        printLine('-', 80);
        int found = 0;

        for (int i = 0; i < carCount; i++) {
            if (!cars[i].isActive)
                continue;
            Car *c = &cars[i];
            printf("  %-6d %-12s %-15s %-10s %-8.0f %-8d %-8s\n",c->carID, c->brand, c->model, c->plateNo,

                c->ratePerDay, c->odometer,
                c->isAvailable ? "AVAIL" : "RENTED");

            found++;
        }
        if (!found) printf("  No vehicles found.\n");
    }


    else if (entityType == VIEW_AVAILABLE) {
        printf("\n  === Available Vehicles ===\n");
        printf("  %-6s %-12s %-15s %-10s %-10s\n","ID", "Brand", "Model", "Plate", "Rate/Day");
        printLine('-', 60);
        int found = 0;

        for (int i = 0; i < carCount; i++) {
            Car *c = &cars[i];

            if (!c->isActive || !c->isAvailable)
                continue;
            printf("  %-6d %-12s %-15s %-10s Rs %-8.0f\n",c->carID, c->brand, c->model, c->plateNo, c->ratePerDay);
            found++;
        }
        if (!found) printf("  No available vehicles.\n");
    }


    else if (entityType == ENTITY_RENTAL)
        {
         const char *statusStr[] = {"PRE-BOOKED", "ACTIVE", "RETURNED", "CANCELLED"};

         printf("\n  %-8s %-8s %-6s %-12s %-12s %-6s %-10s\n","RentID","CustID","CarID","Pickup","Return","Days","Status");
         printLine('-', 75);

        for (int i = 0; i < rentalCount; i++) {
            Rental *r = &rentals[i];
            printf("  %-8d %-8d %-6d %-12s %-12s %-6d %-10s\n",r->rentalID, r->custID, r->carID,r->pickupDate
                   , r->returnDate, r->bookedDays,statusStr[r->status]);
        }
        if (!rentalCount) printf("  No rentals found.\n");
      }
    }




/* 1.3.search customers,2.4.search vehicles */

void searchEntity(int entityType) {
    if (entityType == ENTITY_CUSTOMER) {
        int id;
        printf("  Enter Customer ID: ");
        scanf("%d", &id);
        clearInputBuffer();
        int idx = findCustomerIndex(id);
        if (idx == -1)
            { printf("  [!] Customer not found.\n"); return; }
        Customer *c = &customers[idx];
        printf("\n  ID       : %d\n", c->custID);
        printf("  Name     : %s\n",  c->name);
        printf("  Phone    : %s\n",  c->phone);
        printf("  License  : %s\n",  c->licenseNo);
        printf("  Email    : %s\n",  c->email);
    }


    else if (entityType == ENTITY_CAR) {
        int id;
        printf("  Enter Car ID: ");
        scanf("%d", &id);
        clearInputBuffer();
        int idx = findCarIndex(id);
        if (idx == -1) { printf("  [!] Car not found.\n"); return; }

        Car *c = &cars[idx];

        printf("\n  ID        : %d\n",  c->carID);
        printf("  Vehicle   : %s %s\n", c->brand, c->model);
        printf("  Plate     : %s\n",  c->plateNo);
        printf("  Category  : %s\n",  c->category);
        printf("  Rate/Day  : Rs %.2f\n", c->ratePerDay);
        printf("  Odometer  : %d km\n", c->odometer);
        printf("  Status    : %s\n",  c->isAvailable ? "Available" : "Rented Out");
    }
}


/*1.4.update customer,2.5.update vehicle rate*/

void updateEntity(int entityType)

 {
    if (entityType == ENTITY_CUSTOMER) {
        int id;
        printf("  Enter Customer ID to update: ");
         scanf("%d", &id);
        clearInputBuffer();
        int idx = findCustomerIndex(id);
        if (idx == -1)
            { printf("  [!] Customer not found.\n"); return; }

        Customer *c = &customers[idx];
        printf("  New Phone (current: %s): ", c->phone);
        fgets(c->phone, sizeof(c->phone), stdin);
        c->phone[strcspn(c->phone, "\n")] = '\0';
        printf("  New Email (current: %s): ", c->email);
        fgets(c->email, sizeof(c->email), stdin);
        c->email[strcspn(c->email, "\n")] = '\0';

        printf("  [OK] Customer updated.\n");
        addLog("Customer updated", id);
    }


    else if (entityType == ENTITY_CAR) {
        int id;
        printf("  Enter Car ID to update: ");
        scanf("%d", &id);
        clearInputBuffer();
        int idx = findCarIndex(id);
          if (idx == -1)
            { printf("  [!] Car not found.\n"); return; }


        printf("  New Rate/Day (current Rs %.2f): ", cars[idx].ratePerDay);
        scanf("%lf", &cars[idx].ratePerDay);
        clearInputBuffer();
        printf("  [OK] Rate updated.\n");
        addLog("Car rate updated", id);
    }
}



/* 1.5.delete customer,2.6.delete/decommision car*/

void deleteEntity(int entityType)


 {
    if (entityType == ENTITY_CUSTOMER) {
        int id;
        printf("  Enter Customer ID to delete: ");
        scanf("%d", &id);
        clearInputBuffer();
        int idx = findCustomerIndex(id);
        if (idx == -1) { printf("  [!] Customer not found.\n"); return; }
        customers[idx].isActive = 0;
        printf("  [OK] Customer removed.\n");
        addLog("Customer deleted", id);
    }
    else if (entityType == ENTITY_CAR)


    {
        int id;
        printf("  Enter Car ID to decommission: ");
        scanf("%d", &id);
        clearInputBuffer();
        int idx = findCarIndex(id);
        if (idx == -1)
            { printf("  [!] Car not found.\n");
            return; }
        if (!cars[idx].isAvailable)
   {
            printf("  [!] Cannot remove a rented car.\n");
            return;
    }
        cars[idx].isActive = 0;
        printf("  [OK] Vehicle decommissioned.\n");
        addLog("Car decommissioned", id);
    }
}


/* MAIN MENU-other parts in PART 2*/
/*2.1.add vehicle */



void addCar() {
    if (carCount >= MAX_CARS)
        { printf("  [!] Fleet full.\n");
             return; }
    Car *c = &cars[carCount];
    c->carID      = carCount + 2001;
    c->isAvailable = 1;
    c->isActive    = 1;

    printf("\n  --- Add Vehicle ---\n");
    printf("  Brand       : ");
    fgets(c->brand, sizeof(c->brand), stdin);
    c->brand[strcspn(c->brand, "\n")] = '\0';

    printf("  Model       : ");
    fgets(c->model, sizeof(c->model), stdin);
    c->model[strcspn(c->model, "\n")] = '\0';

    printf("  Plate No    : ");
    fgets(c->plateNo, sizeof(c->plateNo), stdin);
    c->plateNo[strcspn(c->plateNo, "\n")] = '\0';

    printf("  Category    : ");
    fgets(c->category, sizeof(c->category), stdin);
    c->category[strcspn(c->category, "\n")] = '\0';

    printf("  Rate/Day(Rs): ");
    scanf("%lf", &c->ratePerDay);
    clearInputBuffer();

    printf("  Odometer(km): ");
    scanf("%d",  &c->odometer);
    clearInputBuffer();    // function is used to discard unwanted characters (including the newline character) from the standard input buffer

    carCount++;
    char msg[120];
    sprintf(msg, "Car added: %s %s [%s] ID %d", c->brand, c->model, c->plateNo, c->carID);
    addLog(msg, c->carID);
    printf("  [OK] Vehicle added with ID: %d\n", c->carID);
}



/*MAIN MENU-other parrts in PART 3*/
/*3.1.make pre book*/




void makePreBooking()

 {
    if (rentalCount >= MAX_RENTALS)
        { printf("  [!] Rental log full.\n");
       return; }

    printf("\n  --- Pre-Booking / Reservation ---\n");
    viewAll(VIEW_AVAILABLE);

    int custID, carID, days;
    printf("\n  Enter Customer ID  : ");
    scanf("%d", &custID);
    clearInputBuffer();
     if (findCustomerIndex(custID) == -1)
        { printf("  [!] Customer not found.\n"); return; }

    printf("  Enter Car ID       : ");
    scanf("%d", &carID);
    clearInputBuffer();
    int ci = findCarIndex(carID);
    if (ci == -1 || !cars[ci].isAvailable)
        {printf("  [!] Car not available.\n"); return;}

    printf("  Number of Days     : ");
    scanf("%d", &days);
    clearInputBuffer();
    if (days <= 0)
        { printf("  [!] Invalid days.\n"); return; }

    Rental *r        = &rentals[rentalCount];
    r->rentalID      = rentalCount + 3001;
    r->custID        = custID;
    r->carID         = carID;
    r->bookedDays    = days;
    r->startOdometer = cars[ci].odometer;
    r->endOdometer   = 0;
    r->status        = 0;

    double estimatedTotal = cars[ci].ratePerDay * days;
    r->advancePaid = estimatedTotal * ADVANCE_PERCENT;

    strcpy(r->pickupDate, "NOT PICKED UP");
    strcpy(r->returnDate, "NOT RETURNED");

    rentalCount++;
    char msg[120];
    sprintf(msg, "Pre-booking made: Rental %d | Cust %d | Car %d | %d days",r->rentalID, custID, carID, days);
    addLog(msg, r->rentalID);

    printf("\n                                      \n");
    printf("          PRE-BOOKING CONFIRMED      \n");
    printf("                                       \n");
    printf("   Rental ID      : %-15d  \n", r->rentalID);
    printf("    Booked Days    : %-15d  \n", days);
    printf("    Est. Total     : Rs %-12.2f  \n", estimatedTotal);
    printf("    Advance (50%%) : Rs %-12.2f  \n", r->advancePaid);
    printf("  ===================================\n");
    printf("  [!] Please collect Rs %.2f advance from customer.\n", r->advancePaid);
}



/*3.2.pickup car*/

void pickupCar()

{
    int rentalID;
    printf("  Enter Rental ID to activate pickup: ");
    scanf("%d", &rentalID);
    clearInputBuffer();

    Rental *r = NULL;
    for (int i = 0; i < rentalCount; i++)
        if (rentals[i].rentalID == rentalID) { r = &rentals[i]; break; }

    if (!r) { printf("  [!] Rental not found.\n"); return; }
    if (r->status != 0) { printf("  [!] Rental is not in pre-booked state.\n"); return; }

    int ci = findCarIndex(r->carID);
    if (ci == -1 || !cars[ci].isAvailable) {
        printf("  [!] Car not available for pickup.\n"); return;
    }

    getCurrentDate(r->pickupDate, sizeof(r->pickupDate));
    r->status = 1;
    cars[ci].isAvailable = 0;

    char msg[120];
    sprintf(msg, "Car picked up: Rental %d | Car %d | Date %s",
            r->rentalID, r->carID, r->pickupDate);
    addLog(msg, r->rentalID);

    printf("  [OK] Pickup confirmed on %s\n", r->pickupDate);
    printf("  [OK] Start odometer: %d km\n",  r->startOdometer);
}

/*3.3.return car,4.1.create invoice*/

void returnCar()

{
    int rentalID, endOdo;
    printf("  Enter Rental ID   : ");
    scanf("%d", &rentalID);
    clearInputBuffer();

    Rental *r = NULL;
    for (int i = 0; i < rentalCount; i++)
        if (rentals[i].rentalID == rentalID && rentals[i].status == 1)
            { r = &rentals[i]; break; }

    if (!r) { printf("  [!] Active rental not found.\n"); return; }

    printf("  Enter Current Odometer (km): ");
    scanf("%d", &endOdo);
    clearInputBuffer();
    if (endOdo < r->startOdometer) { printf("  [!] Odometer cannot be less than start.\n"); return; }

    r->endOdometer = endOdo;
    getCurrentDate(r->returnDate, sizeof(r->returnDate));
    r->status = 2;

    int ci = findCarIndex(r->carID);
    if (ci != -1) {
        cars[ci].odometer    = endOdo;
        cars[ci].isAvailable = 1;
    }

    int    kmDriven       = r->endOdometer - r->startOdometer;
    double ratePerDay     = (ci != -1) ? cars[ci].ratePerDay : 0;
    int    bookedDays     = r->bookedDays;
    int    freeKmTotal    = FREE_KM_PER_DAY * bookedDays;
    int    extraKm        = (kmDriven > freeKmTotal) ? (kmDriven - freeKmTotal) : 0;

    double baseCost       = ratePerDay * bookedDays;
    double extraKmCost    = extraKm * RATE_PER_KM;
    double totalDue       = baseCost + extraKmCost;
    double balance        = totalDue - r->advancePaid;

    Invoice *inv     = &invoices[invoiceCount];
    inv->invoiceID   = invoiceCount + 4001;
    inv->rentalID    = rentalID;
    inv->baseCost    = baseCost;
    inv->extraKmCost = extraKmCost;
    inv->lateFee     = 0;
    inv->totalDue    = totalDue;
    inv->advancePaid = r->advancePaid;
    inv->balance     = balance;
    inv->isPaid      = (balance <= 0) ? 1 : 0;
    invoiceCount++;

    char msg[120];
    sprintf(msg, "Car returned: Rental %d | Invoice %d | Balance Rs %.2f",
            rentalID, inv->invoiceID, balance);
    addLog(msg, rentalID);

    printf("\n  ======================================\n");
    printf("              RETURN SUMMARY            \n");
    printf("  ======================================\n");
    printf("    Return Date     : %-18s  \n", r->returnDate);
    printf("    KM Driven       : %-18d  \n", kmDriven);
    printf("    Free KM Allowed : %-18d  \n", freeKmTotal);
    printf("    Extra KM        : %-18d  \n", extraKm);
    printf("    Base Cost       : Rs %-15.2f  \n", baseCost);
    printf("    Extra KM Cost   : Rs %-15.2f  \n", extraKmCost);
    printf("    Total Due       : Rs %-15.2f  \n", totalDue);
    printf("    Advance Paid    : Rs %-15.2f  \n", r->advancePaid);
    printf("    Balance Due     : Rs %-15.2f  \n", balance);
    printf("  ======================================\n");

    if (balance > 0)
        printf("  [!] Collect Rs %.2f from customer.\n", balance);
    else if (balance < 0)
        printf("  [!] Refund Rs %.2f to customer.\n", -balance);
    else
        printf("  [OK] Fully settled.\n");
}



/*3.4.cancel prebook */

void cancelBooking()
{
    int id;
    printf("  Enter Rental ID to cancel: ");
    scanf("%d", &id);
    clearInputBuffer();
    for (int i = 0; i < rentalCount; i++)
    {
        if (rentals[i].rentalID == id && rentals[i].status == 0)
            {
            rentals[i].status = 3;
            printf("  [OK] Pre-booking %d cancelled. Refund advance if collected.\n", id);
            addLog("Pre-booking cancelled", id);
            return;
           }
    }
    printf("  [!] Pre-booking not found or already active.\n");
}

/*4.1.view invoice*/



void viewInvoice()

{
    int rentalID;
    printf("  Enter Rental ID: ");
    scanf("%d", &rentalID);
    clearInputBuffer();

    for (int i = 0; i < invoiceCount; i++)
    {
        Invoice *inv = &invoices[i];
        if (inv->rentalID != rentalID)
         continue;
        printf("\n  ======================================\n");
        printf("               INVOICE #%-5d           \n", inv->invoiceID);
        printf("  ======================================\n");
        printf("    Rental ID      : %-18d  \n", inv->rentalID);
        printf("    Base Cost      : Rs %-15.2f  \n", inv->baseCost);
        printf("    Extra KM Cost  : Rs %-15.2f  \n", inv->extraKmCost);
        printf("    Late Fee       : Rs %-15.2f  \n", inv->lateFee);
        printf("    Total Due      : Rs %-15.2f  \n", inv->totalDue);
        printf("    Advance Paid   : Rs %-15.2f  \n", inv->advancePaid);
        printf("    Balance        : Rs %-15.2f  \n", inv->balance);
        printf("    Status         : %-18s  \n", inv->isPaid ? "PAID" : "OUTSTANDING");
        printf("  ======================================\n");
          return;
    }
    printf("  [!] Invoice not found for Rental %d.\n", rentalID);
}



/*4.2.Manage invoice*/


void manageBilling()
{
    int choice;
    printf("\n  1. Mark Invoice as Paid\n");
    printf("  2. Apply Late Fee\n");
    printf("  Choice: ");

    scanf("%d", &choice);
    clearInputBuffer();

    if (choice == 1) {
        int rentalID;
        printf("  Enter Rental ID to mark as paid: ");
        scanf("%d", &rentalID);
        clearInputBuffer();
        for (int i = 0; i < invoiceCount; i++)
            {
            if (invoices[i].rentalID == rentalID)
            {
                invoices[i].isPaid  = 1;
                invoices[i].balance = 0;
                printf("  [OK] Invoice marked as PAID.\n");
                addLog("Invoice marked paid", rentalID);
                return;
            }
        }
        printf("  [!] Invoice not found.\n");
    }
    else if (choice == 2)
  {
        int rentalID;
        double fee;
        printf("  Enter Rental ID  : ");
        scanf("%d", &rentalID);
        clearInputBuffer();
        printf("  Late Fee Amount  : Rs ");
        scanf("%lf", &fee);
        clearInputBuffer();
        for (int i = 0; i < invoiceCount; i++) {
            if (invoices[i].rentalID == rentalID) {
                invoices[i].lateFee  += fee;
                invoices[i].totalDue += fee;
                invoices[i].balance  += fee;
                printf("  [OK] Late fee Rs %.2f applied.\n", fee);
                addLog("Late fee applied", rentalID);
                return;
            }
        }
        printf("  [!] Invoice not found.\n");
    }
}


/*4.3.Revenue summary*/


void revenueSummary()
{
    double total = 0, collected = 0, outstanding = 0;
    for (int i = 0; i < invoiceCount; i++)
        {
        total       += invoices[i].totalDue;
        collected   += invoices[i].advancePaid + (invoices[i].isPaid ? invoices[i].balance : 0);
        outstanding += invoices[i].isPaid ? 0 : invoices[i].balance;
       }
    printf("\n  === Revenue Summary ===\n");
    printf("  Total Revenue Due  : Rs %.2f\n", total);
    printf("  Total Collected    : Rs %.2f\n", collected);
    printf("  Outstanding        : Rs %.2f\n", outstanding);
    printf("\n  === Outstanding Payments ===\n");
    printf("  %-8s %-8s %-12s\n", "InvID", "RentID", "Balance");
    printLine('-', 35);

    int found = 0;
    for (int i = 0; i < invoiceCount; i++) {
        if (invoices[i].isPaid)
            continue;
        printf("  %-8d %-8d Rs %-10.2f\n",invoices[i].invoiceID, invoices[i].rentalID, invoices[i].balance);
        found++;
    }
    if (!found) printf("  All invoices settled!\n");
}

/*5.1.system dashboard*/


void generateDashboard()

 {
    int availCars = 0, rentedCars = 0, activeRentals = 0, preBooked = 0;
    for (int i = 0; i < carCount; i++)
    {
        if (!cars[i].isActive) continue;
        if (cars[i].isAvailable) availCars++; else rentedCars++;
    }
    for (int i = 0; i < rentalCount; i++)
     {
        if (rentals[i].status == 1) activeRentals++;
        if (rentals[i].status == 0) preBooked++;
    }

    printf("\n  ===================================\n");
    printf("          SYSTEM DASHBOARD           \n");
    printf("  ===================================\n");
    printf("    Total Customers  : %-13d  \n", custCount);
    printf("    Total Vehicles   : %-13d  \n", carCount);
    printf("    Available Cars   : %-13d  \n", availCars);
    printf("    Rented Cars      : %-13d  \n", rentedCars);
    printf("    Active Rentals   : %-13d  \n", activeRentals);
    printf("    Pre-Bookings     : %-13d  \n", preBooked);
    printf("    Total Invoices   : %-13d  \n", invoiceCount);
    printf("  ===================================\n");
}


/*5.2.view admin logs*/


void viewAdminLog() {
    printf("\n  %-5s %-22s %-6s  %s\n", "Log", "Timestamp", "RefID", "Action");
    printLine('-', 80);
    for (int i = 0; i < logCount; i++) {
        AdminLog *l = &logs[i];
        printf("  %-5d %-22s %-6d  %s\n",
               l->logID, l->timestamp, l->referenceID, l->action);
    }
    if (!logCount) printf("  No log entries.\n");

    int ch;
    printf("  Clear logs? (1=Yes, 0=No): ");
    scanf("%d", &ch);
    clearInputBuffer();
    if (ch == 1) {
        logCount = 0;
        printf("  [OK] Admin logs cleared.\n");
    }
}


/*5.3.vehicle rental report*/


void generateVehicleReport()
{
    int count[MAX_CARS] = {0};
    for (int i = 0; i < rentalCount; i++)
        {
        int ci = findCarIndex(rentals[i].carID);
        if (ci != -1) count[ci]++;
    }
    printf("\n  === Vehicle Rental Frequency ===\n");
    printf("  %-6s %-15s %-15s %-10s\n", "CarID", "Brand", "Model", "Times");
    printLine('-', 55);

    for (int i = 0; i < carCount; i++) {
        if (!cars[i].isActive)
            continue;
        printf("  %-6d %-15s %-15s %-10d\n",cars[i].carID, cars[i].brand, cars[i].model, count[i]);
    }
}


/*5.4.Search and display rentals by customer*/


void searchRentalByCustomer() {
    int custID;
    printf("  Enter Customer ID: ");
    scanf("%d", &custID);
    clearInputBuffer();
    const char *statusStr[] = {"PRE-BOOKED", "ACTIVE", "RETURNED", "CANCELLED"};
    int found = 0;
    for (int i = 0; i < rentalCount; i++) {
        if (rentals[i].custID != custID) continue;
        Rental *r = &rentals[i];
        printf("  Rental %-5d | Car %-5d | %-10s | Days %-3d | %s\n",
               r->rentalID, r->carID, r->pickupDate, r->bookedDays,
               statusStr[r->status]);
        found++;
    }
    if (!found) printf("  No rentals found for customer %d.\n", custID);
}




/*  CUSTOMER MANAGEMENT MENU */


void menuCustomer() {
    int ch;
    do {
        printf("\n==============================\n");
        printf("      CUSTOMER MANAGEMENT   \n");
        printf("  ==========================\n");
        printf("    1. Add Customer          \n");
        printf("    2. View All Customers    \n");
        printf("    3. Search Customer       \n");
        printf("    4. Update Customer       \n");
        printf("    5. Delete Customer       \n");
        printf("    0. Back                  \n");
        printf("                             \n");
        printf("  Choice: ");
        scanf("%d", &ch);
        clearInputBuffer();
        switch (ch) {
            case 1: addCustomer(); break;
            case 2: viewAll(ENTITY_CUSTOMER); break;
            case 3: searchEntity(ENTITY_CUSTOMER); break;
            case 4: updateEntity(ENTITY_CUSTOMER); break;
            case 5: deleteEntity(ENTITY_CUSTOMER); break;
            case 0: break;
            default: printf("  Invalid option.\n");
        }
    } while (ch != 0);
}

/*  VEHICLE MANAGEMENT MENU */


void menuVehicle() {
    int ch;
    do {
        printf("\n\n");
        printf("     VEHICLE MANAGEMENT    \n");
        printf("  =============================\n");
        printf("   1. Add Vehicle          \n");
        printf("    2. View All Vehicles    \n");
        printf("    3. View Available Cars  \n");
        printf("    4. Search Vehicle       \n");
        printf("    5. Update Vehicle Rate  \n");
        printf("    6. Decommission Vehicle \n");
        printf("    0. Back                 \n");
        printf("                             \n");
        printf("  Choice: ");
        scanf("%d", &ch);
        clearInputBuffer();
        switch (ch) {
            case 1: addCar(); break;
            case 2: viewAll(ENTITY_CAR); break;
            case 3: viewAll(VIEW_AVAILABLE); break;
            case 4: searchEntity(ENTITY_CAR); break;
            case 5: updateEntity(ENTITY_CAR); break;
            case 6: deleteEntity(ENTITY_CAR); break;
            case 0: break;
            default: printf("  Invalid option.\n");
        }
    } while (ch != 0);
}

/*  RENTAL AND PREBO0KING MENU */


void menuRental() {
    int ch;
    do {
        printf("\n  ==============================\n");
        printf("     RENTAL & PRE-BOOKING       \n");
        printf("  ==============================\n");
        printf("    1. Make Pre-Booking         \n");
        printf("    2. Pickup Car (Activate)    \n");
        printf("    3. Return Car               \n");
        printf("    4. Cancel Pre-Booking       \n");
        printf("    5. View All Rentals         \n");
        printf("    0. Back                     \n");
        printf("  ==============================\n");
        printf("  Choice: ");
        scanf("%d", &ch);
        clearInputBuffer();
        switch (ch) {

            case 1: makePreBooking();
            break;

            case 2: pickupCar();
             break;

            case 3: returnCar();
            break;

            case 4: cancelBooking();
            break;

            case 5: viewAll(ENTITY_RENTAL);
            break;

            case 0:
            break;
            default: printf("  Invalid option.\n");
        }
    } while (ch != 0);
}

/*  BILLING AND PAYEMENT MENU */


void menuBilling() {
    int ch;
    do {
        printf("\n  ==============================\n");
        printf("      BILLING & PAYMENTS        \n");
        printf("  ==============================\n");
        printf("    1. View Invoice             \n");
        printf("    2. Manage Invoice (Pay/Fee) \n");
        printf("    3. Revenue Summary          \n");
        printf("    0. Back                     \n");
        printf("  ==============================\n");
        printf("  Choice: ");
        scanf("%d", &ch);
        clearInputBuffer();
        switch (ch)
         {
            case 1: viewInvoice();
             break;
            case 2:manageBilling();
             break;
            case 3: revenueSummary();
            break;
            case 0:
            break;

            default: printf("  Invalid option.\n");
        }
    } while (ch != 0);
}

/*  REPORTS AND ADMIN MENU */


void menuReports() {
    int ch;
    do {
        printf("\n  ==================================\n");
        printf("         REPORTS & ADMIN            \n");
        printf("  ==================================\n");
        printf("    1. System Dashboard             \n");
        printf("    2. View Admin Logs              \n");
        printf("    3. Vehicle Rental Report        \n");
        printf("    4. Search Customer Rentals      \n");
        printf("    0. Back                         \n");
        printf("  ==================================\n");
        printf("  Choice: ");
        scanf("%d", &ch);
        clearInputBuffer();
        switch (ch)
         {
            case 1: generateDashboard(); break;
            case 2: viewAdminLog(); break;
            case 3: generateVehicleReport(); break;
            case 4: searchRentalByCustomer(); break;
            case 0: break;
            default: printf("  Invalid option.\n");
        }
    } while (ch != 0);
}


 /* LOAD DEMO DATA */

void loadDemoData()
 {
    strcpy(customers[0].name, "Ashan Perera");
    strcpy(customers[0].phone, "0771234567");
    strcpy(customers[0].licenseNo, "LIC-001-A");
    strcpy(customers[0].email, "ashan@email.com");
    customers[0].custID = 1001; customers[0].isActive = 1;



    strcpy(customers[1].name, "Nimali Fernando");
    strcpy(customers[1].phone, "0759876543");
    strcpy(customers[1].licenseNo, "LIC-002-B");
    strcpy(customers[1].email, "nimali@email.com");
    customers[1].custID = 1002; customers[1].isActive = 1;



    strcpy(customers[2].name, "Kasun Jayawardena");
    strcpy(customers[2].phone, "0712345678");
    strcpy(customers[2].licenseNo, "LIC-003-C");
    strcpy(customers[2].email, "kasun@email.com");
    customers[2].custID = 1003; customers[2].isActive = 1;
    custCount = 3;



    strcpy(cars[0].brand, "Toyota"); strcpy(cars[0].model, "Prius");
    strcpy(cars[0].plateNo, "CAA-1234"); strcpy(cars[0].category, "Sedan");
    cars[0].carID = 2001; cars[0].ratePerDay = 4500;
    cars[0].odometer = 32000; cars[0].isAvailable = 0; cars[0].isActive = 1;



    strcpy(cars[1].brand, "Honda"); strcpy(cars[1].model, "Vezel");
    strcpy(cars[1].plateNo, "CBB-5678"); strcpy(cars[1].category, "SUV");
    cars[1].carID = 2002; cars[1].ratePerDay = 6000;
    cars[1].odometer = 15000; cars[1].isAvailable = 1; cars[1].isActive = 1;



    strcpy(cars[2].brand, "Suzuki"); strcpy(cars[2].model, "Alto");
    strcpy(cars[2].plateNo, "CCC-9999"); strcpy(cars[2].category, "Hatchback");
    cars[2].carID = 2003; cars[2].ratePerDay = 3000;
    cars[2].odometer = 8000; cars[2].isAvailable = 1; cars[2].isActive = 1;



    strcpy(cars[3].brand, "Nissan"); strcpy(cars[3].model, "X-Trail");
    strcpy(cars[3].plateNo, "CDD-4321"); strcpy(cars[3].category, "SUV");
    cars[3].carID = 2004; cars[3].ratePerDay = 7500;
    cars[3].odometer = 22000; cars[3].isAvailable = 0; cars[3].isActive = 1;
    carCount = 4;



    rentals[0].rentalID = 3001; rentals[0].custID = 1001; rentals[0].carID = 2001;
    strcpy(rentals[0].pickupDate, "2026-05-28");
    strcpy(rentals[0].returnDate, "NOT RETURNED");
    rentals[0].bookedDays = 5; rentals[0].startOdometer = 32000;
    rentals[0].endOdometer = 0;
    rentals[0].advancePaid = 4500 * 5 * 0.5;
    rentals[0].status = 1;



    rentals[1].rentalID = 3002; rentals[1].custID = 1002; rentals[1].carID = 2004;
    strcpy(rentals[1].pickupDate, "NOT PICKED UP");
    strcpy(rentals[1].returnDate, "NOT RETURNED");
    rentals[1].bookedDays = 3; rentals[1].startOdometer = 22000;
    rentals[1].endOdometer = 0;
    rentals[1].advancePaid = 7500 * 3 * 0.5;
    rentals[1].status = 0;


    rentals[2].rentalID = 3003; rentals[2].custID = 1003; rentals[2].carID = 2003;
    strcpy(rentals[2].pickupDate, "2026-05-20");
    strcpy(rentals[2].returnDate, "2026-05-24");
    rentals[2].bookedDays = 4; rentals[2].startOdometer = 8000;
    rentals[2].endOdometer = 8520;
    rentals[2].advancePaid = 3000 * 4 * 0.5;
    rentals[2].status = 2;
    rentalCount = 3;



    addLog("System initialised with demo data", 0);
    addLog("Demo: Ashan rented Toyota Prius (active)", 3001);
    addLog("Demo: Nimali pre-booked Nissan X-Trail", 3002);
    addLog("Demo: Kasun returned Suzuki Alto | Balance Rs 10800", 3003);
}


/*  MAIN FUNCTION */


int main()
{
    loadDemoData();

    printf("\n");
    printLine('=', 55);
    printf("      SWIFT WHEELS - RENT A CAR SYSTEM\n");
    printf("      IN1101 Programming Fundamentals\n");
    printLine('=', 55);
    printf(" Demo data also loaded.Explore the full workflow!\n");

    int choice;
    do {
        printf("\n                                  \n");
        printf("            MAIN MENU               \n");
        printf("                                     \n");
        printf("    1. Customer Management          \n");
        printf("    2. Vehicle Management           \n");
        printf("    3. Rental & Pre-Booking         \n");
        printf("    4. Billing & Payments           \n");
        printf("    5. Reports & Admin              \n");
        printf("    0. Exit                         \n");
        printf("                                    \n");
        printf("  Choice: ");
        scanf("%d",&choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1: menuCustomer();
        break;
        case 2: menuVehicle();
        break;
        case 3: menuRental();
        break;
        case 4: menuBilling();
        break;
        case 5: menuReports();
        break;
        case 0:
                 printf("\n  Thank you for using Swift Wheels!\n\n");
                   break;
            default:
                printf("Invalid option.Try again.\n");
        }
    }   while (choice!=0);

    return 0;
}
