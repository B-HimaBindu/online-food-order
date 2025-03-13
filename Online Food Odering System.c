#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct MenuItem {
    char name[50];
    float price;
    struct MenuItem* next;
} MenuItem;

typedef struct Restaurant {
    char name[50];
    MenuItem* menu;
    struct Restaurant* next;
} Restaurant;

typedef struct OrderItem {
    char itemName[50];
    int quantity;
    float price;
    struct OrderItem* next;
} OrderItem;

typedef struct Order {
    int orderId;
    char restaurantName[50];
    OrderItem* items;
    struct Order* next;
} Order;


Restaurant* restaurantList = NULL;
Order* orderQueue = NULL;
int orderIdCounter = 1;


void addRestaurant();
void addMenuItem();
void displayRestaurants();
void displayMenu(char* restaurantName);
void placeOrder();
void displayOrders();
Restaurant* findRestaurant(char* restaurantName);
void freeRestaurantList();
void freeOrderQueue();

int main() {
    int choice;
    char restaurantName[50];

    do {
        printf("\nOnline Food Ordering System\n");
        printf("1. Add Restaurant\n2. Add Menu Item\n3. Display Restaurants\n4. Display Menu\n5. Place Order\n6. Display Orders\n7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1: addRestaurant(); break;
            case 2: addMenuItem(); break;
            case 3: displayRestaurants(); break;
            case 4:
                printf("Enter restaurant name: ");
                fgets(restaurantName, sizeof(restaurantName), stdin);
                restaurantName[strcspn(restaurantName, "\n")] = 0;
                displayMenu(restaurantName);
                break;
            case 5: placeOrder(); break;
            case 6: displayOrders(); break;
            case 7: freeRestaurantList(); freeOrderQueue(); exit(0);
            default: printf("Invalid choice.\n");
        }
    } while (1);

    return 0;
}

void addRestaurant() {
    Restaurant* newRestaurant = (Restaurant*)malloc(sizeof(Restaurant));
    printf("Enter restaurant name: ");
    fgets(newRestaurant->name, sizeof(newRestaurant->name), stdin);
    newRestaurant->name[strcspn(newRestaurant->name, "\n")] = 0;
    newRestaurant->menu = NULL;
    newRestaurant->next = restaurantList;
    restaurantList = newRestaurant;
    printf("Restaurant added successfully.\n");
}

void addMenuItem() {
    char restaurantName[50];
    printf("Enter restaurant name: ");
    fgets(restaurantName, sizeof(restaurantName), stdin);
    restaurantName[strcspn(restaurantName, "\n")] = 0;

    Restaurant* restaurant = findRestaurant(restaurantName);
    if (!restaurant) {
        printf("Restaurant not found.\n");
        return;
    }

    MenuItem* newItem = (MenuItem*)malloc(sizeof(MenuItem));
    printf("Enter menu item name: ");
    fgets(newItem->name, sizeof(newItem->name), stdin);
    newItem->name[strcspn(newItem->name, "\n")] = 0;
    printf("Enter price: ");
    scanf("%f", &newItem->price);
    getchar(); 
    newItem->next = restaurant->menu;
    restaurant->menu = newItem;
    printf("Menu item added successfully.\n");
}

void displayRestaurants() {
    Restaurant* current = restaurantList;
    if (!current) {
        printf("No restaurants available.\n");
        return;
    }
    printf("\nRestaurants:\n");
    while (current) {
        printf("- %s\n", current->name);
        current = current->next;
    }
}

void displayMenu(char* restaurantName) {
    Restaurant* restaurant = findRestaurant(restaurantName);
    if (!restaurant) {
        printf("Restaurant not found.\n");
        return;
    }

    MenuItem* current = restaurant->menu;
    if (!current) {
        printf("Menu is empty.\n");
        return;
    }

    printf("\nMenu for %s:\n", restaurant->name);
    while (current) {
        printf("- %s: %.2f\n", current->name, current->price);
        current = current->next;
    }
}

void placeOrder() {
    char restaurantName[50];
    printf("Enter restaurant name: ");
    fgets(restaurantName, sizeof(restaurantName), stdin);
    restaurantName[strcspn(restaurantName, "\n")] = 0;

    Restaurant* restaurant = findRestaurant(restaurantName);
    if (!restaurant) {
        printf("Restaurant not found.\n");
        return;
    }

    Order* newOrder = (Order*)malloc(sizeof(Order));
    newOrder->orderId = orderIdCounter++;
    strcpy(newOrder->restaurantName, restaurant->name);
    newOrder->items = NULL;
    newOrder->next = orderQueue;
    orderQueue = newOrder;

    char itemName[50];
    int quantity;
    float total = 0;

    printf("Enter items (type 'done' when finished):\n");
    while (1) {
        printf("Item name: ");
        fgets(itemName, sizeof(itemName), stdin);
        itemName[strcspn(itemName, "\n")] = 0;

        if (strcmp(itemName, "done") == 0) {
            break;
        }

        MenuItem* menuItem = restaurant->menu;
        while (menuItem) {
            if (strcmp(menuItem->name, itemName) == 0) {
                printf("Quantity: ");
                scanf("%d", &quantity);
                getchar(); 

                OrderItem* newOrderItem = (OrderItem*)malloc(sizeof(OrderItem));
                strcpy(newOrderItem->itemName, menuItem->name);
                newOrderItem->quantity = quantity;
                newOrderItem->price = menuItem->price * quantity;
                newOrderItem->next = newOrder->items;
                newOrder->items = newOrderItem;

                total += newOrderItem->price;
                break;
            }
            menuItem = menuItem->next;
        }
        if (!menuItem) {
            printf("Item not found.\n");
        }
    }
    printf("Order placed successfully. Total: %.2f\n", total);
}

void displayOrders() {
    Order* current = orderQueue;
    if (!current) {
        printf("No orders placed yet.\n");
        return;
    }

    printf("\nOrders:\n");
    while (current) {
        printf("Order ID: %d, Restaurant: %s\n", current->orderId, current->restaurantName);
        OrderItem* item = current->items;
        while (item) {
            printf("  - %s: %d x %.2f = %.2f\n", item->itemName, item->quantity, item->price / item->quantity, item->price);
            item = item->next;
        }
        current = current->next;
    }
}

Restaurant* findRestaurant(char* restaurantName) {
    Restaurant* current = restaurantList;
    while (current) {
        if (strcmp(current->name, restaurantName) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void freeRestaurantList() {
    Restaurant* current = restaurantList;
    while (current) {
        Restaurant* temp = current;
        MenuItem* menuItem = current->menu;
        while(menuItem){
            MenuItem* tempMenu = menuItem;
            menuItem = menuItem->next;
            free(tempMenu);
        }
        current = current->next;
        free(temp);
    }
    restaurantList = NULL;
}

void freeOrderQueue(){
    Order* current = orderQueue;
    while(current){
        Order* temp = current;
        OrderItem* item = current->items;
        while(item){
            OrderItem* tempItem = item;
            item = item->next;
            free(tempItem);
        }
        current = current->next;
        free(temp);
    }
    orderQueue = NULL;
}