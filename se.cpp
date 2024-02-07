#include "se.h"

void main() {
	element search;
	char filename[FILENAME], filetemp[10];
	int i = 0;

	// document1.txt~document(DOC_NUM).txt까지 반복하면서 해시
	for (i = 1; i <= DOC_NUM; i++) { 
		strcpy(filename, "document");
		itoa(i, filetemp, 10);
		strcat(filetemp, ".txt");
		strcat(filename, filetemp);
		build_hash(filename,i);
	}
	// 결과 출력
	printf("-----------색인 결과 ------------\n");
	printf("Total number of documents : %d\n", DOC_NUM);
	printf("Total number of indexed words : %d\n", wcount);
	printf("Total number of comparison : %d\n", icomparison);
	printf("---------------------------------\n");
	printf("\n\n검색할 단어를 입력해주세요 : ");
	scanf("%s", search.data);
	printf("-----------검색 결과 ------------\n");
	hash_chain_find(search, hash_table);
	printf("---------------------------------\n");
	printf("Total number of comparison : %d\n", scomparison);

}

// 해시 생성
void build_hash(char fname[], int fnum){
	char line[1000];
	char *tok;
	FILE *ifp;
	element tmp;
	int i = 0;

	// 파일 안열릴시 예외처리
	if ((ifp = fopen(fname, "r")) == NULL) {
		printf("No such file ! \n");
		exit(1);
	}
	// 한 줄씩 입력 -> DEL(구분자)가 나오면 잘라서 tok에 저장 -> 소문자로 변환 -> 체이닝 해시 추가
	while (fgets(line,1000,ifp) != NULL) {
		tok = strtok(line, DEL);
		while (tok != NULL) {
			strcpy(voca[fnum][i], tok);
			i++;
			for (int j = 0; j < strlen(tok); j++) {
				tok[j] = tolower(tok[j]);
			}
			strcpy(tmp.data, tok);
			hash_chain_add(tmp,hash_table, fnum);
			tok = strtok(NULL, DEL);
		}
	}
	fclose(ifp);
}
// 체이닝 해시
void hash_chain_add(element item, list *ht[], int fnum){
	int hash_value = hash(item.data);
	list *ptr;
	list *node_before = NULL, *node = ht[hash_value];
	// 노드의 처음부터 끝까지 반복
	for (; node; node_before = node, node = node->link) {
		// 문자열 비교++
		icomparison++;
		// item의 data(단어)가 같다면 중복
		if (!strcmp(node->item.data, item.data)) {
			node->item.filenum[fnum]++; // 파일 위치 저장
			return;
		}
	}
	// 반복문이 끝날때까지 중복이 없다면 처음 들어오는 단어이므로 단어 개수 증가 및 노드 생성, 저장
	wcount++;
	ptr = (list *)malloc(sizeof(list));
	ptr->item = item;
	memset((ptr->item.filenum), 0, sizeof(int)*(DOC_NUM + 1)); // 생성된 노드의 filnum배열 0으로 초기화
	ptr->link = NULL;
	ptr->item.filenum[fnum]++; // 파일 위치 저장
	if (node_before)
		node_before->link = ptr;
	else
		ht[hash_value] = ptr;
}
// 해시 키 설정
int hash(char data[]) {
	return transform(data) % TABLE_SIZE;
}
// folding(각 자릿값을 더함)
int transform(char *data) {
	int number = 0;
	while (*data)
		number += (*data++);
	return number;
}
// 검색부
void hash_chain_find(element item, list *ht[]){
	list *node;
	int max;

	int hash_value = hash(item.data); // data의 hash_value를 찾아서 저장
	for (node = ht[hash_value]; node; node = node->link) { // 해당 해시 칸 반복
		if (!strcmp(node->item.data, item.data)) { // 값이 있는지 탐색
			scomparison++;
			for (int i = 1; i <= DOC_NUM; i++)
				printf("%d ", node->item.filenum[i]);
			printf("\n");

			// 서치프린트함수(최대값, 찾고자하는 단어 전달)
			// 리턴값이 -1이라면 더 이상 없는것이므로 break
			for (int i = 1; i <= DOC_NUM; i++) {
				max = maxnum(node->item.filenum);
				if (max == -1)
					break;
				search_print(max, item.data, node->item.filenum[max]);
				// max에 해당하는 filnum[i]값은 사용을 마쳤으므로 0으로
				node->item.filenum[max] = 0;
				printf("\n");
			}
		}
	}
}

// 최대값 찾는 함수(여러번 찾은 위치부터 출력을 위해서)
int maxnum(int filenum[]) {
	int max=filenum[1], maxi = 1;
	for (int i = 2; i <= DOC_NUM; i++) {
		if (filenum[i] > max) {
			max = filenum[i];
			maxi = i;
		}
	}
	// max가 0이라면 -1리턴으로 더 이상 없음을 알림
	if (max == 0)
		return -1;
	return maxi;
}

// 출력부(검색)
void search_print(int fnum, char search[], int wnum) {
	char line[1000];
	char *tok;
	FILE *ifp;
	char filename[FILENAME], filetemp[10];
	int i = 0, cnt = 0, j;
	
	// 파일 이름 구성
	strcpy(filename, "document");
	itoa(fnum, filetemp, 10);
	strcat(filetemp, ".txt");
	strcat(filename, filetemp);

	printf("<%s> %s : %d\n", filename, search, wnum);

	// 파일 안열릴시 예외처리
	if ((ifp = fopen(filename, "r")) == NULL) {
		printf("No such file ! \n");
		exit(1);
	}
	// 한 줄씩 입력 -> DEL(구분자)가 나오면 잘라서 tok에 저장 -> 소문자로 변환 -> 검색
	while (fgets(line, 1000, ifp) != NULL) {
		tok = strtok(line, DEL);
		while (tok != NULL) {
			scomparison++;
			i++;
			for (int j = 0; j < strlen(tok); j++) {
				tok[j] = tolower(tok[j]);
			}
			// 같다면 프린트
			if (!strcmp(search, tok)) {
				// i값(검색 단어 이전 단어의 수)가 3 이상이라면 ...과 함께 단어들 출력
				if (i > 3) {
					printf("... ");
					for (j = 4; j >= -2; j--) {
						printf("%s ", voca[fnum][i - j]);
					}
					printf("... \n");
				}
				// 3이 안된다면 ...없이 단어들 출력
				else {
					for (j = 4; j >= -2; j--) {
						if (!strcmp(voca[fnum][i - j], ""));
						else
							printf("%s ", voca[fnum][i - j]);
					}
					printf("... \n");
				}
				
				cnt++;
				if (cnt == wnum)
					return;
			}
			tok = strtok(NULL, DEL);
		}
	}
	fclose(ifp);
}
