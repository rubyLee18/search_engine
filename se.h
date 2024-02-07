#pragma once
#pragma warning(disable:4996)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#define FILENAME 20 // ���� �̸�
#define MAX_LENGTH 20 // �ܾ��� �ִ� ����
#define DOC_NUM 9 // ���� ����
#define DEL "\n\t \"',.?[]1234567890" // ������
#define	TABLE_SIZE	26 // ���̺� ������(���ĺ� ����)

typedef struct{
	char data[MAX_LENGTH];
	int filenum[DOC_NUM + 1]; // �ܾ��� ���� ��ġ�� ����
} element; // data ��

typedef struct list{
	element item;
	list *link;
}list;
list *hash_table[TABLE_SIZE]; // �ؽ� ���̺�

void build_hash(char [], int); // �ؽ� ����
void hash_chain_add(element, list**, int); // ü�̴� �ؽ�
int hash(char[]); // �ؽ� Ű ����
int transform(char[]); // folding
void hash_chain_find(element item, list *ht[]);

int maxnum(int[]); // ���� ���� ���ϱ�
void search_print(int, char[], int); // ��º�(�˻�)

int wcount = 0; // �ܾ� ����
int icomparison = 0; // ���� �� ����
int scomparison = 0; // �˻� �� ����
char voca[DOC_NUM][10000][MAX_LENGTH]; // �ܾ� ���� �迭