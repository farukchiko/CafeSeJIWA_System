#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    char name[100];
    int price;
} Menu;

Menu heap[MAX];
int heapSize = 0;

void loadFromFile();
void saveToFile();
void insertMenu(char name[], int price);
void inputMenu();
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
void setColor(int color);
void clearScreen();

enum {
    CREAM = 14,   // Yellow
    BROWN = 6,    // Dark Yellow
    DARK = 8,     // Gray
    RESET = 7     // Default Gray
};

int main() {
    int choice;

    system("chcp 65001 > nul");
    loadFromFile();

    do {
        clearScreen();
        printMainMenu();

        if (scanf("%d", &choice) != 1) {
            clearScreen();
            setColor(DARK);
            pourCoffeeAnimation();
            printf("\nInput tidak valid. Masukkan angka.\n");
            setColor(RESET);
            clearInputBuffer();
            Sleep(1000);
            continue;
        }
        clearInputBuffer();

        clearScreen();
        pourCoffeeAnimation();

        switch (choice) {
            case 1:
                inputMenu();
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
                setColor(CREAM);
                printf("\nTerima kasih telah menggunakan Cafe SeJIWA System ☕\n");
                setColor(RESET);
                break;

            default:
                setColor(DARK);
                printf("Pilihan tidak valid!\n");
                setColor(RESET);
        }

        if (choice != 5) {
            printf("\nTekan Enter untuk kembali ke menu...");
            getchar();
        }

    } while (choice != 5);

    return 0;
}

void printMainMenu() {
    setColor(CREAM);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║        ☕  Cafe SeJIWA Menu Manager  ☕      ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  1. Tambah Menu                              ║\n");
    printf("║  2. Lihat Menu                               ║\n");
    printf("║  3. Edit Menu                                ║\n");
    printf("║  4. Hapus Menu                               ║\n");
    printf("║  5. Keluar                                   ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    setColor(BROWN);
    printf("\nPilih menu (1-5): ");
    setColor(RESET);
}

// Fungsi agar tidak case sensitif pada update menu
int compareIgnoreCase(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower(*a) != tolower(*b)) {
            return 0; // tidak sama
        }
        a++;
        b++;
    }
    return *a == *b;
}

// Fungsi hanya untuk memasukkan data ke heap (tanpa input)
void insertMenu(char name[], int price) {
    if (heapSize >= MAX) {
        setColor(DARK);
        printf("Menu penuh. Tidak bisa menambah lagi.\n");
        setColor(RESET);
        return;
    }
    Menu newMenu;
    strcpy(newMenu.name, name);
    newMenu.price = price;
    heap[heapSize] = newMenu;
    heapifyUp(heapSize);
    heapSize++;
}

// Fungsi untuk input data menu dari user (interaktif)
void inputMenu() {
    char name[100];
    int price;
    char choice;

    do {
        if (heapSize >= MAX) {
            setColor(DARK);
            printf("Menu penuh. Tidak bisa menambah lagi.\n");
            setColor(RESET);
            return;
        }

        setColor(BROWN);
        printf("\nMasukkan Nama Menu : ");
        setColor(RESET);
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;

        if (strlen(name) == 0) {
            setColor(DARK);
            printf("Nama tidak boleh kosong!\n");
            setColor(RESET);
            continue;
        }

        setColor(BROWN);
        printf("Masukkan Harga Menu : ");
        setColor(RESET);
        if (scanf("%d", &price) != 1 || price <= 0) {
            setColor(DARK);
            printf("Harga tidak valid!\n");
            setColor(RESET);
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        insertMenu(name, price);
        saveToFile();
        loadingAnimation("Menyimpan");

        do {
            setColor(BROWN);
            printf("\nIngin menambah menu lagi? ☕\n(Y untuk lanjut / T untuk kembali)\n: ");
            setColor(RESET);
            scanf(" %c", &choice);
            clearInputBuffer();
            choice = toupper(choice);
        } while (choice != 'Y' && choice != 'T');

    } while (choice == 'Y');
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
    setColor(BROWN);
    printf("\n╔═════════════════════════════════════════════╗\n");
    printf("║         📋 Daftar Menu Cafe SeJIWA          ║\n");
    printf("╠════╦════════════════════════╦═══════════════╣\n");
    printf("║ No ║ Nama Menu              ║ Harga         ║\n");
    printf("╠════╬════════════════════════╬═══════════════╣\n");
    setColor(RESET);

    if (heapSize == 0) {
        setColor(DARK);
        printf("║ %-36s ║\n", "Belum ada menu yang tersedia.");
        setColor(RESET);
        printf("╚════╩════════════════════════╩════════════════╝\n");
        return;
    }

    for (int i = 0; i < heapSize; i++) {
        setColor(BROWN);
        printf("║ %2d ║ %-22s ║ Rp %-10d ║\n", i + 1, heap[i].name, heap[i].price);
    }
    setColor(RESET);
    printf("╚════╩════════════════════════╩═══════════════╝\n");
}


void deleteMenu() {
    if (heapSize == 0) {
        setColor(DARK);
        printf("Tidak ada menu untuk dihapus.\n");
        setColor(RESET);
        return;
    }

    int index;
    char ulang;

    do {
        clearScreen();
        loadingAnimation("Memuat Menu");
        displayMenu();
        setColor(BROWN);
        printf("\nPilih nomor menu yang ingin dihapus: ");
        setColor(RESET);

        if (scanf("%d", &index) != 1 || index < 1 || index > heapSize) {
            clearInputBuffer();
            setColor(DARK);
            printf("Pilihan tidak valid!\n");
            setColor(RESET);

            do {
                setColor(BROWN);
                printf("\nIngin mencoba lagi? ☕\n(Y untuk ulang / T untuk kembali ke menu utama)\n: ");
                setColor(RESET);
                scanf(" %c", &ulang);
                clearInputBuffer();
                ulang = toupper(ulang);
            } while (ulang != 'Y' && ulang != 'T');

            if (ulang == 'T') return;
            continue;
        }

        clearInputBuffer();
        index--;

        setColor(CREAM);
        printf("\nMenghapus menu: %s - Rp %d\n", heap[index].name, heap[index].price);
        setColor(RESET);

        heap[index] = heap[heapSize - 1];
        heapSize--;

        for (int i = (heapSize / 2) - 1; i >= 0; i--) {
            heapifyDown(i);
        }

        saveToFile();
        loadingAnimation("Menghapus");

        do {
            setColor(BROWN);
            printf("\nIngin menghapus menu lain? ☕\n(Y untuk lanjut / T untuk kembali ke menu utama)\n: ");
            setColor(RESET);
            scanf(" %c", &ulang);
            clearInputBuffer();
            ulang = toupper(ulang);
        } while (ulang != 'Y' && ulang != 'T');

    } while (ulang == 'Y');
}

void updateMenu() {
    if (heapSize == 0) {
        setColor(DARK);
        printf("Tidak ada menu untuk diedit.\n");
        setColor(RESET);
        return;
    }

    int index;
    char ulang;

    do {
        clearScreen();
        loadingAnimation("Memuat Menu");
        displayMenu();
        setColor(BROWN);
        printf("\nPilih nomor menu yang ingin diedit: ");
        setColor(RESET);

        if (scanf("%d", &index) != 1 || index < 1 || index > heapSize) {
            clearInputBuffer();
            setColor(DARK);
            printf("Pilihan tidak valid!\n");
            setColor(RESET);

            do {
                setColor(BROWN);
                printf("\nIngin mencoba lagi? ☕\n(Y untuk ulang / T untuk kembali ke menu utama)\n: ");
                setColor(RESET);
                scanf(" %c", &ulang);
                clearInputBuffer();
                ulang = toupper(ulang);
            } while (ulang != 'Y' && ulang != 'T');

            if (ulang == 'T') return;
            continue;
        }

        clearInputBuffer();
        index--;

        char newName[100];
        int newPrice;

        setColor(BROWN);
        printf("Nama baru: ");
        setColor(RESET);
        fgets(newName, sizeof(newName), stdin);
        newName[strcspn(newName, "\n")] = 0;

        if (strlen(newName) == 0) {
            setColor(DARK);
            printf("Nama tidak boleh kosong.\n");
            setColor(RESET);
            return;
        }

        // Validasi nama sama
        if (compareIgnoreCase(newName, heap[index].name)) {
            setColor(DARK);
            printf("Nama produk yang di edit harus berbeda!\n");
            setColor(RESET);

            do {
                setColor(BROWN);
                printf("\nIngin mencoba lagi? ☕\n(Y untuk ulang / T untuk kembali ke menu utama)\n: ");
                setColor(RESET);
                scanf(" %c", &ulang);
                clearInputBuffer();
                ulang = toupper(ulang);
            } while (ulang != 'Y' && ulang != 'T');

            if (ulang == 'T') return;
            else continue;
        }

        setColor(BROWN);
        printf("Harga baru: ");
        setColor(RESET);
        if (scanf("%d", &newPrice) != 1 || newPrice <= 0) {
            setColor(DARK);
            printf("Harga tidak valid!\n");
            setColor(RESET);
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        // Update data langsung (tidak perlu cek harga sama)
        strcpy(heap[index].name, newName);
        heap[index].price = newPrice;

        for (int i = (heapSize / 2) - 1; i >= 0; i--) {
            heapifyDown(i);
        }

        saveToFile();
        loadingAnimation("Memperbarui");

        do {
            setColor(BROWN);
            printf("\nIngin mengedit menu lain? ☕\n(Y untuk lanjut / T untuk kembali ke menu utama)\n: ");
            setColor(RESET);
            scanf(" %c", &ulang);
            clearInputBuffer();
            ulang = toupper(ulang);
        } while (ulang != 'Y' && ulang != 'T');

    } while (ulang == 'Y');
}

void saveToFile() {
    FILE *fp = fopen("menu.txt", "w");
    if (!fp) {
        setColor(DARK);
        printf("Gagal menyimpan data ke file.\n");
        setColor(RESET);
        return;
    }
    for (int i = 0; i < heapSize; i++) {
        fprintf(fp, "%s;%d\n", heap[i].name, heap[i].price);
    }
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("menu.txt", "r");
    if (!fp) return;

    heapSize = 0;

    char line[150];
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ";");
        char name[100];
        int price = 0;

        if (token) strcpy(name, token);
        token = strtok(NULL, ";\n");
        if (token) price = atoi(token);

        if (strlen(name) > 0 && price > 0) {
            insertMenu(name, price);
        }
    }
    fclose(fp);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void loadingAnimation(const char *msg) {
    setColor(CREAM);
    printf("%s", msg);
    fflush(stdout);
    for (int i = 0; i < 3; i++) {
        Sleep(300);
        printf(".");
        fflush(stdout);
    }
    printf("\n");
    setColor(RESET);
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
        setColor(CREAM);
        printf("%s\n", pour[i]);
        setColor(RESET);
        Sleep(250);
    }
    setColor(BROWN);
    printf("☕ Menyeduh kopi hangat...\n\n");
    setColor(RESET);
    Sleep(500);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearScreen() {
    system("cls");
}
