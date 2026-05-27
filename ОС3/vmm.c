#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 256
#define NUM_PAGES 256
#define NUM_FRAMES 256
#define TLB_SIZE 16
#define PHYS_MEM_SIZE (NUM_FRAMES * PAGE_SIZE)

char phys_mem[PHYS_MEM_SIZE];        // физическая память
int page_table[NUM_PAGES];           // -1 = нет фрейма
int tlb_page[TLB_SIZE];              // номер страницы в TLB
int tlb_frame[TLB_SIZE];             // соответствующий фрейм
int tlb_valid[TLB_SIZE];             // валидна ли запись
int tlb_index = 0;                   // для FIFO

int next_free_frame = 0;             // следующий свободный фрейм
int page_faults = 0;
int tlb_hits = 0;
int total = 0;

FILE *backing;

void init() {
    for (int i = 0; i < NUM_PAGES; i++) page_table[i] = -1;
    for (int i = 0; i < TLB_SIZE; i++) tlb_valid[i] = 0;
    backing = fopen("BACKING_STORE.bin", "rb");
    if (!backing) { perror("BACKING_STORE.bin"); exit(1); }
}

// Поиск в TLB
int tlb_lookup(int page_num) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb_valid[i] && tlb_page[i] == page_num) {
            tlb_hits++;
            return tlb_frame[i];
        }
    }
    return -1;
}

// Обновление TLB (FIFO)
void tlb_update(int page_num, int frame_num) {
    tlb_page[tlb_index] = page_num;
    tlb_frame[tlb_index] = frame_num;
    tlb_valid[tlb_index] = 1;
    tlb_index = (tlb_index + 1) % TLB_SIZE;
}

// Обработка page fault
int handle_page_fault(int page_num) {
    page_faults++;
    if (next_free_frame >= NUM_FRAMES) {
        fprintf(stderr, "Нет свободных фреймов!\n");
        exit(1);
    }
    int frame = next_free_frame++;
    // Читаем страницу из BACKING_STORE
    fseek(backing, page_num * PAGE_SIZE, SEEK_SET);
    fread(&phys_mem[frame * PAGE_SIZE], 1, PAGE_SIZE, backing);
    page_table[page_num] = frame;
    return frame;
}

// Трансляция логического адреса в физический
int translate(int logical) {
    logical &= 0xFFFF;                // оставляем 16 бит
    int page_num = (logical >> 8) & 0xFF;   // старшие 8 бит
    int offset   = logical & 0xFF;         // младшие 8 бит

    int frame = tlb_lookup(page_num);
    if (frame == -1) {
        frame = page_table[page_num];
        if (frame == -1) {
            frame = handle_page_fault(page_num);
        }
        tlb_update(page_num, frame);
    }
    return frame * PAGE_SIZE + offset;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s addresses.txt\n", argv[0]);
        return 1;
    }
    init();
    FILE *addr = fopen(argv[1], "r");
    if (!addr) { perror("addresses.txt"); return 1; }

    int logical;
    while (fscanf(addr, "%d", &logical) == 1) {
        total++;
        int physical = translate(logical);
        signed char value = phys_mem[physical];
        printf("Virtual address: %d Physical address: %d Value: %d\n",
               logical, physical, value);
    }
    fclose(addr);
    fclose(backing);

    printf("\nСтатистика:\n");
    printf("Всего обращений: %d\n", total);
    printf("TLB hits: %d (%.2f%%)\n", tlb_hits, (float)tlb_hits / total * 100);
    printf("Page faults: %d (%.2f%%)\n", page_faults, (float)page_faults / total * 100);
    return 0;
}