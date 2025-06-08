#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX 100

typedef struct {
    char name[100];
    int price;
} Menu;

Menu heap[MAX];
int heapSize = 0;

// Definisi Warna ANSI
#define COLOR_CREAM "\x1b[38;2;245;222;179m"
#define COLOR_BROWN "\x1b[38;2;139;69;19m"
#define COLOR_DARK  "\x1b[38;2;101;67;33m"
#define COLOR_RESET "\x1b[0m"

// Deklarasi Fungsi (Prototypes)
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
void setColor(const char *colorCode);
void resetColor();
void clearScreen();
int compareIgnoreCase(const char *a, const char *b);

int main() {
    int choice;

    loadFromFile();

    do {
        clearScreen();
        printMainMenu();

        if (scanf("%d", &choice) != 1) {
            clearScreen();
            setColor(COLOR_DARK);
            pourCoffeeAnimation();
            printf("\nInput tidak valid. Masukkan angka.\n");
            resetColor();
            clearInputBuffer();
            usleep(1000000);
            continue;
        }
        clearInputBuffer();

        clearScreen();
        pourCoffeeAnimation();

        switch (choice) {
            case 1: inputMenu(); break;
            case 2: displayMenu(); break;
            case 3: updateMenu(); break; // saveToFile() dipindahkan ke dalam fungsi
            case 4: deleteMenu(); break; // saveToFile() dipindahkan ke dalam fungsi
            case 5:
                setColor(COLOR_CREAM);
                printf("\nTerima kasih telah menggunakan Cafe SeJIWA System ☕\n");
                resetColor();
                break;
            default:
                setColor(COLOR_DARK);
                printf("Pilihan tidak valid!\n");
                resetColor();
        }

        if (choice != 5) {
            printf("\nTekan Enter untuk kembali ke menu...");
            getchar();
        }

    } while (choice != 5);

    return 0;
}

void setColor(const char *colorCode) {
    printf("%s", colorCode);
}

void resetColor() {
    printf("%s", COLOR_RESET);
}

void clearScreen() {
    system("clear");
}

void printMainMenu() {
    setColor(COLOR_CREAM);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║        ☕  Cafe SeJIWA Menu Manager  ☕      ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  1. Tambah Menu                              ║\n");
    printf("║  2. Lihat Menu                               ║\n");
    printf("║  3. Edit Menu                                ║\n");
    printf("║  4. Hapus Menu                               ║\n");
    printf("║  5. Keluar                                   ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    setColor(COLOR_BROWN);
    printf("\nPilih menu (1-5): ");
    resetColor();
}

void loadingAnimation(const char *msg) {
    setColor(COLOR_CREAM);
    printf("%s", msg);
    fflush(stdout);
    for (int i = 0; i < 3; i++) {
        usleep(300000);
        printf(".");
        fflush(stdout);
    }
    printf("\n");
    resetColor();
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
        setColor(COLOR_CREAM);
        printf("%s\n", pour[i]);
        resetColor();
        usleep(250000);
    }
    setColor(COLOR_BROWN);
    printf("☕ Menyeduh kopi hangat...\n\n");
    resetColor();
    usleep(500000);
}

int compareIgnoreCase(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        a++;
        b++;
    }
    return tolower((unsigned char)*a) == tolower((unsigned char)*b);
}

void insertMenu(char name[], int price) {
    if (heapSize >= MAX) {
        setColor(COLOR_DARK);
        printf("Menu penuh. Tidak bisa menambah lagi.\n");
        resetColor();
        return;
    }
    Menu newMenu;
    strcpy(newMenu.name, name);
    newMenu.price = price;
    heap[heapSize] = newMenu;
    heapifyUp(heapSize);
    heapSize++;
}

void inputMenu() {
    char name[100];
    int price;
    char choice;

    do {
        if (heapSize >= MAX) {
            setColor(COLOR_DARK);
            printf("Menu penuh. Tidak bisa menambah lagi.\n");
            resetColor();
            return;
        }

        setColor(COLOR_BROWN);
        printf("\nMasukkan Nama Menu : ");
        resetColor();
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;

        if (strlen(name) == 0) {
            setColor(COLOR_DARK);
            printf("Nama tidak boleh kosong!\n");
            resetColor();
            continue;
        }

        setColor(COLOR_BROWN);
        printf("Masukkan Harga Menu : ");
        resetColor();
        if (scanf("%d", &price) != 1 || price <= 0) {
            setColor(COLOR_DARK);
            printf("Harga tidak valid!\n");
            resetColor();
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        insertMenu(name, price);
        saveToFile();
        loadingAnimation("Menyimpan");

        do {
            setColor(COLOR_BROWN);
            printf("\nIngin menambah menu lagi? ☕\n(Y untuk lanjut / T untuk kembali)\n: ");
            resetColor();
            scanf(" %c", &choice);
            clearInputBuffer();
            choice = toupper(choice);
        } while (choice != 'Y' && choice != 'T');

    } while (choice == 'Y');
}

void heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        // Menggunakan Max-Heap berdasarkan harga
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

// ==========================================================
// BAGIAN YANG DIPERBAIKI: Fungsi displayMenu
// Warna diubah menjadi cokelat dan diterapkan ke seluruh tabel
// ==========================================================
void displayMenu() {
    // Atur warna menjadi cokelat untuk seluruh blok di bawah ini
    setColor(COLOR_BROWN);

    printf("\n--- Daftar Menu Cafe ---\n\n");

    if (heapSize == 0) {
        // Pesan ini bisa menggunakan warna lain jika diinginkan
        setColor(COLOR_DARK);
        printf("Belum ada menu yang tersedia.\n");
        resetColor(); // Reset khusus untuk pesan ini saja
        return;
    }

    // Seluruh tabel ini sekarang akan berwarna cokelat
    printf("╔═════════════════════════════════════════════╗\n");
    printf("║         📋 Daftar Menu Cafe SeJIWA          ║\n");
    printf("╠════╦════════════════════════╦═══════════════╣\n");
    printf("║ No ║ Nama Menu              ║ Harga         ║\n");
    printf("╠════╬════════════════════════╬═══════════════╣\n");

    // Karena ini Max-Heap, menu termahal akan di atas.
    // Jika ingin menampilkan terurut, perlu algoritma sorting terpisah.
    // Untuk saat ini, kita tampilkan sesuai urutan di array.
    for (int i = 0; i < heapSize; i++) {
        printf("║ %2d ║ %-22s ║ Rp %-10d ║\n", i + 1, heap[i].name, heap[i].price);
    }
    printf("╚════╩════════════════════════╩═══════════════╝\n");

    // Kembalikan warna ke default setelah semua selesai dicetak
    resetColor();
}


// ==========================================================
// BAGIAN YANG DIPERBAIKI: Fungsi deleteMenu
// Logika perbaikan heap disederhanakan menjadi lebih benar dan efisien
// ==========================================================
void deleteMenu() {
    if (heapSize == 0) {
        setColor(COLOR_DARK);
        printf("Tidak ada menu untuk dihapus.\n");
        resetColor();
        return;
    }

    int index;
    char ulang;

    do {
        clearScreen();
        loadingAnimation("Memuat Menu");
        displayMenu();
        setColor(COLOR_BROWN);
        printf("\nPilih nomor menu yang ingin dihapus: ");
        resetColor();

        if (scanf("%d", &index) != 1 || index < 1 || index > heapSize) {
            clearInputBuffer();
            setColor(COLOR_DARK);
            printf("Pilihan tidak valid!\n");
            resetColor();

            do {
                setColor(COLOR_BROWN);
                printf("\nIngin mencoba lagi? ☕\n(Y untuk ulang / T untuk kembali ke menu utama)\n: ");
                resetColor();
                scanf(" %c", &ulang);
                clearInputBuffer();
                ulang = toupper(ulang);
            } while (ulang != 'Y' && ulang != 'T');

            if (ulang == 'T') return;
            continue;
        }

        clearInputBuffer();
        index--; // Ubah dari 1-based ke 0-based index

        setColor(COLOR_CREAM);
        printf("\nMenghapus menu: %s - Rp %d\n", heap[index].name, heap[index].price);
        resetColor();

        // Pindahkan elemen terakhir ke posisi yang dihapus
        heap[index] = heap[heapSize - 1];
        heapSize--;

        // [FIX] Cukup panggil heapifyDown pada elemen yang dipindahkan
        if (heapSize > 0) {
            heapifyDown(index);
        }

        saveToFile();
        loadingAnimation("Menghapus");

        do {
            setColor(COLOR_BROWN);
            printf("\nIngin menghapus menu lain? ☕\n(Y untuk lanjut / T untuk kembali ke menu utama)\n: ");
            resetColor();
            scanf(" %c", &ulang);
            clearInputBuffer();
            ulang = toupper(ulang);
        } while (ulang != 'Y' && ulang != 'T');

    } while (ulang == 'Y');
}

// ==========================================================
// BAGIAN YANG DIPERBAIKI: Fungsi updateMenu
// Memperbaiki bug kritis, validasi, dan logika heap
// ==========================================================
void updateMenu() {
    if (heapSize == 0) {
        setColor(COLOR_DARK);
        printf("Tidak ada menu untuk diedit.\n");
        resetColor();
        return;
    }

    int index;
    char ulang;

    do {
        clearScreen();
        loadingAnimation("Memuat Menu");
        displayMenu();
        setColor(COLOR_BROWN);
        printf("\nPilih nomor menu yang ingin diedit: ");
        resetColor();

        if (scanf("%d", &index) != 1 || index < 1 || index > heapSize) {
            clearInputBuffer();
            setColor(COLOR_DARK);
            printf("Pilihan tidak valid!\n");
            resetColor();
            // Loop untuk coba lagi
            do {
                setColor(COLOR_BROWN);
                printf("\nIngin mencoba lagi? ☕\n(Y untuk ulang / T untuk kembali ke menu utama)\n: ");
                resetColor();
                scanf(" %c", &ulang);
                clearInputBuffer();
                ulang = toupper(ulang);
            } while (ulang != 'Y' && ulang != 'T');
            if (ulang == 'T') return;
            continue;
        }
        clearInputBuffer();
        index--; // Ubah ke 0-based index

        char newName[100];
        int newPrice;

        setColor(COLOR_BROWN);
        printf("Nama baru (sekarang: %s): ", heap[index].name);
        resetColor();
        fgets(newName, sizeof(newName), stdin);
        newName[strcspn(newName, "\n")] = 0;

        if (strlen(newName) == 0) {
            setColor(COLOR_DARK);
            printf("Nama tidak boleh kosong. Menggunakan nama lama.\n");
            strcpy(newName, heap[index].name); // Jika kosong, pakai nama lama
            resetColor();
        }

        setColor(COLOR_BROWN);
        printf("Harga baru (sekarang: %d): ", heap[index].price);
        resetColor();
        if (scanf("%d", &newPrice) != 1 || newPrice <= 0) {
            setColor(COLOR_DARK);
            printf("Harga tidak valid! Proses update dibatalkan.\n");
            resetColor();
            clearInputBuffer();
            continue; // Kembali ke awal loop do-while update
        }
        clearInputBuffer();

        // [FIX] Simpan harga lama untuk perbandingan
        int oldPrice = heap[index].price;

        // [FIX] Bug kritis: Terapkan perubahan ke array heap
        strcpy(heap[index].name, newName);
        heap[index].price = newPrice;

        // [FIX] Gunakan logika heap yang benar untuk update
        if (newPrice > oldPrice) {
            heapifyUp(index);
        } else if (newPrice < oldPrice) {
            heapifyDown(index);
        }
        // Jika harga sama, tidak perlu heapify

        saveToFile();
        loadingAnimation("Memperbarui");
        printf("Menu berhasil diperbarui!\n");

        do {
            setColor(COLOR_BROWN);
            printf("\nIngin mengedit menu lain? ☕\n(Y untuk lanjut / T untuk kembali ke menu utama)\n: ");
            resetColor();
            scanf(" %c", &ulang);
            clearInputBuffer();
            ulang = toupper(ulang);
        } while (ulang != 'Y' && ulang != 'T');

    } while (ulang == 'Y');
}

void saveToFile() {
    FILE *fp = fopen("menu.txt", "w");
    if (!fp) {
        setColor(COLOR_DARK);
        printf("Gagal menyimpan data ke file.\n");
        resetColor();
        return;
    }
    for (int i = 0; i < heapSize; i++) {
        fprintf(fp, "%s;%d\n", heap[i].name, heap[i].price);
    }
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("menu.txt", "r");
    if (!fp) return; // Jika file belum ada, abaikan

    heapSize = 0; // Reset heap sebelum load

    char line[150];
    while (fgets(line, sizeof(line), fp)) {
        // Hapus newline di akhir jika ada
        line[strcspn(line, "\n")] = 0;
        
        char *token = strtok(line, ";");
        char name[100];
        int price = 0;

        if (token) {
            strcpy(name, token);
            token = strtok(NULL, ";");
            if (token) {
                price = atoi(token);
            }
        }
        
        if (strlen(name) > 0 && price > 0) {
            // Langsung insert tanpa heapify, lalu build heap di akhir
            if(heapSize < MAX){
                strcpy(heap[heapSize].name, name);
                heap[heapSize].price = price;
                heapSize++;
            }
        }
    }
    fclose(fp);
    
    // Build heap dari data yang sudah di-load
    for(int i = (heapSize / 2) - 1; i >= 0; i--){
        heapifyDown(i);
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}