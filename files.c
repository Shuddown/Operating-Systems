#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <math.h>

#define BUF_SIZE 4096

char *input(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	vprintf(msg, args);
	va_end(args);
	static char buf[4096];
	fgets(buf, BUF_SIZE, stdin);
	buf[strlen(buf) - 1] = '\0';
	return buf;
}

/* Block stuff, modify */
typedef struct Block {
	int id;
	bool in_use;
	char filename[64];
	union {
		int next;
		int block_table[1024];
	};
} Block;

typedef struct DirEnt {
	char filename[64];
	int start;
	int end;
} DirEnt;

typedef void (*AllocStrategy)(Block *disk, int num_blocks, DirEnt *entry, int size);


bool has_space(Block *disk, int num_blocks, int size, bool contiguous) {
	int free_blocks = 0;
	for(int i = 0; i < num_blocks; i++) {
		if(!disk[i].in_use) {
			free_blocks += 1;
			if(free_blocks == size) return true;
		} else if(contiguous) {
			free_blocks = 0;
		}
	}
	return false;
}

bool is_contig_free(Block disk[], int num_blocks, int start_block, int size) {
	for(int i = start_block; i < start_block + size; i++) {
		if(disk[i].in_use) return false;
	}
	return true;
}

int get_free_block(Block disk[], int num_blocks, int contig_size) {
	while(true) {
		int block = rand() % num_blocks;
		if(is_contig_free(disk, num_blocks, block, contig_size)) return block;		
	}
}

void contiguous(Block disk[], int num_blocks, DirEnt *entry, int size) {
	if(!has_space(disk, num_blocks, size, true)) {
		printf("Not enough space to allocate for file %s, skipping...\n", entry->filename);
		return;
	}
	int start_block = get_free_block(disk, num_blocks, size);
	for(int i = start_block; i < size; i++) {
		disk[i].in_use = true;
		disk[i].id = i;
		strcpy(disk[i].filename, entry->filename);
	}
	entry->start = start_block;
	entry->end = start_block + size - 1;
}


void linked(Block disk[], int num_blocks, DirEnt *entry, int size) {
	if(!has_space(disk, num_blocks, size, false)) {
		printf("Not enough space to allocate for file %s, skipping...\n", entry->filename);
		return;
	}
	
	int cur_block = get_free_block(disk, num_blocks, 1); 
	entry->start = cur_block;
	for(int i = 0; i < size; i++) {
		strcpy(disk[cur_block].filename, entry->filename);
		disk[cur_block].id = cur_block;
		disk[cur_block].in_use = true;
		int next_block;
		if(i == size - 1) {
			next_block = -1;
			entry->end = cur_block;
		} else {
			next_block = get_free_block(disk, num_blocks, 1);
		} 
		disk[cur_block].next = next_block;
		cur_block = next_block;
	}
}

void indexed(Block disk[], int num_blocks, DirEnt *entry, int size) {
	if(!has_space(disk, num_blocks, size+1, false)) {
		printf("Not enough space to allocate for file %s, skipping...\n", entry->filename);
		return;
	}
	
	int index_block = get_free_block(disk, num_blocks, 1); 
	entry->start = index_block;
	strcpy(disk[index_block].filename, entry->filename);
	disk[index_block].id = index_block;
	disk[index_block].in_use = true;
	int *block_table = disk[index_block].block_table;
	for(int i = 0; i < size; i++) {
		int cur_block = get_free_block(disk, num_blocks, 1);
		block_table[i] = cur_block;
		strcpy(disk[cur_block].filename, entry->filename);
		disk[cur_block].id = cur_block;
		disk[cur_block].in_use = true;
	}
	block_table[size] = -1;
}


int main() {
	srand(time(NULL));
	int num_blocks = atoi(input("Enter num blocks: "));
	int block_size = atoi(input("Enter block size: "));
	Block *disk = (Block *)calloc(num_blocks, sizeof(Block));
	AllocStrategy strategies[] = {contiguous, linked, indexed};
	printf("File Allocation Techniques\n");
	printf("1. Contiguous\n");
	printf("2. Linked\n");
	printf("3. Indexed\n");
	int strat_index = atoi(input("Enter your option: "));
	AllocStrategy strategy = strategies[strat_index - 1];
	int num_files = atoi(input("Enter the number of files: "));
	DirEnt *entries = (DirEnt *)calloc(num_files, sizeof(DirEnt));
	for(int i = 0; i < num_files; i++) {
		strcpy(entries[i].filename, input("Enter The name of file %d: ", i+1));
		int size = atoi(input("Enter the size of the file %d: ", i+1));
		int num_file_blocks = ceil((double)size / block_size);
		strategy(disk, num_blocks, &entries[i], num_file_blocks);
	}
	
	switch(strat_index) {
		case 1:
			printf("\nDirectory Structure\n\n");
			printf("%-15s%-7s%-7s\n", "File Name", "Start", "Length");
			for(int i = 0; i < num_files; i++) {
				printf("%-15s%-7d%-7d\n", entries[i].filename, entries[i].start, entries[i].end - entries[i].start + 1);
			}
			break;
		case 2:
			printf("\nDirectory Structure\n\n");
			printf("%-15s%-7s%-7s\n", "File Name", "Start", "End");
			for(int i = 0; i < num_files; i++) {
				printf("%-15s%-7d%-7d\n", entries[i].filename, entries[i].start, entries[i].end);
			}
			printf("\nIndividual File Listings\n\n");
			for(int i = 0; i < num_files; i++) {
				printf("Blocks for file '%s': ", entries[i].filename);
				int cur_block = entries[i].start;
				while(cur_block != -1) {
					printf("%d ", cur_block);
					cur_block = disk[cur_block].next;
				}
				printf("\n");
			}
			break;
		case 3:
			printf("\nDirectory Structure\n\n");
			printf("%-15s%-15s\n", "File Name", "Index Block");
			for(int i = 0; i < num_files; i++) {
				printf("%-15s%-15d\n", entries[i].filename, entries[i].start);
			}
			printf("\nIndex Tables\n\n");
			for(int i = 0; i < num_files; i++) {
				printf("Index for file '%s': ", entries[i].filename);
				int index_block = entries[i].start;
				int *index = disk[index_block].block_table;
				for(int i = 0; index[i] != -1; i++) {
					printf("%d ", index[i]);
				}
				printf("\n"); 
			}
			break;
			
	}
	return 0;
}