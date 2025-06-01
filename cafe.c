#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 100

#define CREAM   "\x1b[38;2;245;222;179m"
#define BROWN   "\x1b[38;2;139;69;19m"
#define DARK    "\x1b[38;2;101;67;33m"
#define RESET   "\x1b[0m"

typedef struct {
    char name[100];
    int price;
} Menu;

Menu heap[MAX];
int heapSize = 0;

void loadFromFile();
void saveToFile();
void insertMenu(char name[], int price);
void heapifyUp(int index);
void heapifyDown(int index);
void displayMenu();
void deleteMenu();
void updateMenu();
void swap(Menu *a, Menu *b);
void clearInputBuffer();
void pourCoffeeAnimation();
void loadingAnimation(const char *msg);
void printMainMenu();

int main() {
    int choice;
    char name[100];
    int price;

    loadFromFile();

    do {
        system("clear");
        printMainMenu();

        if (scanf("%d", &choice) != 1) {
            printf(DARK "\nInput tidak valid. Masukkan angka.\n" RESET);
            clearInputBuffer();
            sleep(1);
            continue;
        }
        clearInputBuffer();

        system("clear");
        pourCoffeeAnimation();

        switch (choice) {
            case 1:
                printf(BROWN "Masukkan Nama Menu : " RESET);
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                if (strlen(name) == 0) {
                    printf(DARK "Nama tidak boleh kosong!\n" RESET);
                    break;
                }

                printf(BROWN "Masukkan Harga Menu : " RESET);
                if (scanf("%d", &price) != 1 || price <= 0) {
                    printf(DARK "Harga tidak valid!\n" RESET);
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();

                insertMenu(name, price);
                saveToFile();
                loadingAnimation("Menyimpan");
                break;

            case 2:
                displayMenu();
                break;

            case 3:
                updateMenu();
                saveToFile();
                break;

            case 4:
                deleteMenu();
                saveToFile();
                break;

            case 5:
                printf(CREAM "\nTerima kasih telah menggunakan Cafe SeJIWA System ☕\n" RESET);
                break;

            default:
                printf(DARK "Pilihan tidak valid!\n" RESET);
        }

        if (choice != 5) {
            printf("\nTekan Enter untuk kembali ke menu...");
            getchar();
        }

    } while (choice != 5);

    return 0;
}

void printMainMenu() {
    printf(CREAM "╔══════════════════════════════════════════════╗\n");
    printf("║        ☕  " RESET BROWN "Cafe SeJIWA Menu Manager" RESET CREAM "  ☕      ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  " RESET "1. Tambah Menu                             " CREAM " ║\n");
    printf("║  " RESET "2. Lihat Menu                              " CREAM " ║\n");
    printf("║  " RESET "3. Edit Menu                               " CREAM " ║\n");
    printf("║  " RESET "4. Hapus Menu                              " CREAM " ║\n");
    printf("║  " RESET "5. Keluar                                  " CREAM " ║\n");
    printf("╚══════════════════════════════════════════════╝\n" RESET);
    printf(BROWN "\nPilih menu (1-5): " RESET);
}

void insertMenu(char name[], int price) {
    if (heapSize >= MAX) {
        printf(DARK "Menu penuh. Tidak bisa menambah lagi.\n" RESET);
        return;
    }
    Menu newMenu;
    strcpy(newMenu.name, name);
    newMenu.price = price;
    heap[heapSize] = newMenu;
    heapifyUp(heapSize);
    heapSize++;
}

void heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[parent].price < heap[index].price) {
            swap(&heap[parent], &heap[index]);
            index = parent;
        } else break;
    }
}

void heapifyDown(int index) {
    int largest = index;
    int left = index * 2 + 1;
    int right = index * 2 + 2;

    if (left < heapSize && heap[left].price > heap[largest].price)
        largest = left;
    if (right < heapSize && heap[right].price > heap[largest].price)
        largest = right;

    if (largest != index) {
        swap(&heap[index], &heap[largest]);
        heapifyDown(largest);
    }
}

void swap(Menu *a, Menu *b) {
    Menu temp = *a;
    *a = *b;
    *b = temp;
}

void displayMenu() {
    printf(CREAM "\n--- Daftar Menu Cafe ---\n\n" RESET);
    if (heapSize == 0) {
        printf(DARK "Belum ada menu yang tersedia.\n" RESET);
        return;
    }
    for (int i = 0; i < heapSize; i++) {
        printf(BROWN "%2d. %-20s - Rp %d\n" RESET, i + 1, heap[i].name, heap[i].price);
    }
}

void deleteMenu() {
    if (heapSize == 0) {
        printf(DARK "Tidak ada menu untuk dihapus.\n" RESET);
        return;
    }

    displayMenu();
    int index;
    printf(BROWN "\nPilih nomor menu yang ingin dihapus: " RESET);
    if (scanf("%d", &index) != 1 || index < 1 || index > heapSize) {
        printf(DARK "Pilihan tidak valid!\n" RESET);
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    index--;
    printf(CREAM "\nMenghapus menu: %s - Rp %d\n" RESET, heap[index].name, heap[index].price);

    heap[index] = heap[heapSize - 1];
    heapSize--;

    for (int i = (heapSize / 2) - 1; i >= 0; i--) {
        heapifyDown(i);
    }

    loadingAnimation("Menghapus");
}

void updateMenu() {
    if (heapSize == 0) {
        printf(DARK "Tidak ada menu untuk diedit.\n" RESET);
        return;
    }

    displayMenu();
    int index;
    printf(BROWN "\nPilih nomor menu yang ingin diedit: " RESET);
    if (scanf("%d", &index) != 1 || index < 1 || index > heapSize) {
        printf(DARK "Pilihan tidak valid!\n" RESET);
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    index--;

    char name[100];
    int price;

    printf(BROWN "Nama baru: " RESET);
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    if (strlen(name) == 0) {
        printf(DARK "Nama tidak boleh kosong.\n" RESET);
        return;
    }

    printf(BROWN "Harga baru: " RESET);
    if (scanf("%d", &price) != 1 || price <= 0) {
        printf(DARK "Harga tidak valid!\n" RESET);
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    strcpy(heap[index].name, name);
    heap[index].price = price;

    for (int i = (heapSize / 2) - 1; i >= 0; i--) {
        heapifyDown(i);
    }

    loadingAnimation("Memperbarui");
}

void saveToFile() {
    FILE *fp = fopen("menu.txt", "w");
    for (int i = 0; i < heapSize; i++) {
        fprintf(fp, "%s;%d\n", heap[i].name, heap[i].price);
    }
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("menu.txt", "r");
    if (!fp) return;

    char line[150];
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ";");
        char name[100];
        int price;

        if (token) strcpy(name, token);
        token = strtok(NULL, ";\n");
        if (token) price = atoi(token);

        insertMenu(name, price);
    }
    fclose(fp);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void loadingAnimation(const char *msg) {
    printf(CREAM "%s", msg);
    fflush(stdout);
    for (int i = 0; i < 3; i++) {
        usleep(300000);
        printf(".");
        fflush(stdout);
    }
    printf("\n" RESET);
}

void pourCoffeeAnimation() {
    const char *pour[] = {
        "     ( (",
        "      ) )",
        "    ........",
        "    |      |]",
        "    \\      /",
        "     `----'"
    };
    for (int i = 0; i < 6; i++) {
        printf(CREAM "%s\n" RESET, pour[i]);
        usleep(250000);
    }
    printf(BROWN "☕ Menyeduh kopi hangat...\n\n" RESET);
    usleep(500000);
}