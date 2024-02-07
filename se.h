#pragma once
#pragma warning(disable:4996)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#define FILENAME 20 // 파일 이름
#define MAX_LENGTH 20 // 단어의 최대 길이
#define DOC_NUM 9 // 문서 개수
#define DEL "\n\t \"',.?[]1234567890" // 구분자
#define	TABLE_SIZE	26 // 테이블 사이즈(알파벳 개수)

typedef struct{
	char data[MAX_LENGTH];
	int filenum[DOC_NUM + 1]; // 단어의 파일 위치를 저장
} element; // data 값

typedef struct list{
	element item;
	list *link;
}list;
list *hash_table[TABLE_SIZE]; // 해시 테이블

void build_hash(char [], int); // 해시 생성
void hash_chain_add(element, list**, int); // 체이닝 해시
int hash(char[]); // 해시 키 설정
int transform(char[]); // folding
void hash_chain_find(element item, list *ht[]);

int maxnum(int[]); // 파일 순서 정하기
void search_print(int, char[], int); // 출력부(검색)

int wcount = 0; // 단어 개수
int icomparison = 0; // 색인 비교 연산
int scomparison = 0; // 검색 비교 연산
char voca[DOC_NUM][10000][MAX_LENGTH]; // 단어 저장 배열