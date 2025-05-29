#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

struct menu {
    int price;
    char item[100];
} m;

FILE *fp;

int mainmenu();
void adminmenu();
void additem();
void deleteitem();
void showmenu();
void usermenu();
void bill();
void getPassword(char *password);

int main() {
    int menu_value = mainmenu();
    if (menu_value == 1)
        adminmenu();
    else
        usermenu();
    return 0;
}

int mainmenu() {
    system("cls");
    int n;
    printf("|************* Ristorante ******************|\n");
    printf("|********** OPTIONS ************************|\n\n");
    printf("\t\t\t\t1. ADMIN\n");
    printf("\t\t\t\t2. USER\n");
    printf("Enter your choice: ");
    scanf("%d", &n);
    return n;
}

void getPassword(char *password) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == '\r') {
            password[i] = '\0';
            break;
        } else if (ch == '\b' && i > 0) {
            i--;
            printf("\b \b");
        } else {
            password[i++] = ch;
            printf("");
        }
    }
}

void adminmenu() {
    int n, x;
    char ch[6];
    const char password[6] = "admin";

    printf("\nEnter the password: ");
    getPassword(ch);
    if (strcmp(ch, password) != 0) {
        system("cls");
        printf("\nIncorrect Password!! Press 1 to return back");
        scanf("%d", &n);
        if (n == 1)
            mainmenu();
        else
            adminmenu();
    } else {
        system("cls");
        printf("1. Add Item\n");
        printf("2. Delete Item\n");
        printf("3. Display menu\n");
        printf("4. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &x);
        if (x == 1)
            additem();
        else if (x == 2)
            deleteitem();
        else if (x == 3)
            showmenu();
        else
            mainmenu();
    }
}

void additem() {
    int n;
    system("cls");
    fp = fopen("menu1.txt", "a");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    getchar();
    printf("Enter item name: ");
    fgets(m.item, 100, stdin);
    m.item[strcspn(m.item, "\n")] = 0;
    printf("Enter item price: ");
    scanf("%d", &m.price);
    fwrite(&m, sizeof(m), 1, fp);
    fclose(fp);
    printf("Enter 1 to add more and 2 to go back: ");
    scanf("%d", &n);
    if (n == 1)
        additem();
    else {
        system("cls");
        adminmenu();
    }
}

void showmenu() {
    int count = 0;
    fp = fopen("menu1.txt", "r");
    if (!fp) {
        printf("Menu file not found!\n");
        return;
    }
    printf("SN \t\t ITEMS \t\t\t\t Price\n");
    while (fread(&m, sizeof(m), 1, fp)) {
        count++;
        printf("%d\t\t %s \t\t\t\t %d\n", count, m.item, m.price);
    }
    fclose(fp);
}

void deleteitem() {
    FILE *temp;
    int num, c = 0, flag = 0;
    showmenu();
    printf("\nEnter the item number to delete: ");
    scanf("%d", &num);
    fp = fopen("menu1.txt", "r");
    temp = fopen("temp.txt", "w");
    while (fread(&m, sizeof(m), 1, fp)) {
        c++;
        if (c == num) {
            flag = 1;
            continue;
        }
        fwrite(&m, sizeof(m), 1, temp);
    }
    fclose(fp);
    fclose(temp);
    if (flag) {
        remove("menu1.txt");
        rename("temp.txt", "menu1.txt");
        printf("Item deleted successfully!\n");
    } else {
        printf("Invalid item number.\n");
    }
}

void usermenu() {
    int c, n;
    char ch;
    FILE *f;
    showmenu();
    do {
        f = fopen("bill.txt", "a");
        fp = fopen("menu1.txt", "r");
        printf("Enter your choice: ");
        scanf("%d", &n);
        c = 0;
        while (fread(&m, sizeof(m), 1, fp)) {
            c++;
            if (c == n) {
                fwrite(&m, sizeof(m), 1, f);
            }
        }
        fclose(fp);
        fclose(f);
        printf("Do you want to add more? (y/n): ");
        scanf(" %c", &ch);
    } while (ch == 'y' || ch == 'Y');
    system("cls");
    bill();
}

void bill() {
    fp = fopen("bill.txt", "r");
    int total = 0, count = 0;
    if (!fp) {
        printf("No items in the bill!\n");
        return;
    }
    printf("\t\tBILL\n");
    printf("SN\t\tItems\t\tPrices\n");
    while (fread(&m, sizeof(m), 1, fp)) {
        count++;
        total += m.price;
        printf("%d\t\t%s\t\t%d\n", count, m.item, m.price);
    }
    printf("TOTAL = %d\n", total);
    fclose(fp);
    fp=fopen("bill.txt","w");
    fclose(fp);
}
