#include "se.h"

void main() {
	element search;
	char filename[FILENAME], filetemp[10];
	int i = 0;

	// document1.txt~document(DOC_NUM).txt���� �ݺ��ϸ鼭 �ؽ�
	for (i = 1; i <= DOC_NUM; i++) { 
		strcpy(filename, "document");
		itoa(i, filetemp, 10);
		strcat(filetemp, ".txt");
		strcat(filename, filetemp);
		build_hash(filename,i);
	}
	// ��� ���
	printf("-----------���� ��� ------------\n");
	printf("Total number of documents : %d\n", DOC_NUM);
	printf("Total number of indexed words : %d\n", wcount);
	printf("Total number of comparison : %d\n", icomparison);
	printf("---------------------------------\n");
	printf("\n\n�˻��� �ܾ �Է����ּ��� : ");
	scanf("%s", search.data);
	printf("-----------�˻� ��� ------------\n");
	hash_chain_find(search, hash_table);
	printf("---------------------------------\n");
	printf("Total number of comparison : %d\n", scomparison);

}

// �ؽ� ����
void build_hash(char fname[], int fnum){
	char line[1000];
	char *tok;
	FILE *ifp;
	element tmp;
	int i = 0;

	// ���� �ȿ����� ����ó��
	if ((ifp = fopen(fname, "r")) == NULL) {
		printf("No such file ! \n");
		exit(1);
	}
	// �� �پ� �Է� -> DEL(������)�� ������ �߶� tok�� ���� -> �ҹ��ڷ� ��ȯ -> ü�̴� �ؽ� �߰�
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
// ü�̴� �ؽ�
void hash_chain_add(element item, list *ht[], int fnum){
	int hash_value = hash(item.data);
	list *ptr;
	list *node_before = NULL, *node = ht[hash_value];
	// ����� ó������ ������ �ݺ�
	for (; node; node_before = node, node = node->link) {
		// ���ڿ� ��++
		icomparison++;
		// item�� data(�ܾ�)�� ���ٸ� �ߺ�
		if (!strcmp(node->item.data, item.data)) {
			node->item.filenum[fnum]++; // ���� ��ġ ����
			return;
		}
	}
	// �ݺ����� ���������� �ߺ��� ���ٸ� ó�� ������ �ܾ��̹Ƿ� �ܾ� ���� ���� �� ��� ����, ����
	wcount++;
	ptr = (list *)malloc(sizeof(list));
	ptr->item = item;
	memset((ptr->item.filenum), 0, sizeof(int)*(DOC_NUM + 1)); // ������ ����� filnum�迭 0���� �ʱ�ȭ
	ptr->link = NULL;
	ptr->item.filenum[fnum]++; // ���� ��ġ ����
	if (node_before)
		node_before->link = ptr;
	else
		ht[hash_value] = ptr;
}
// �ؽ� Ű ����
int hash(char data[]) {
	return transform(data) % TABLE_SIZE;
}
// folding(�� �ڸ����� ����)
int transform(char *data) {
	int number = 0;
	while (*data)
		number += (*data++);
	return number;
}
// �˻���
void hash_chain_find(element item, list *ht[]){
	list *node;
	int max;

	int hash_value = hash(item.data); // data�� hash_value�� ã�Ƽ� ����
	for (node = ht[hash_value]; node; node = node->link) { // �ش� �ؽ� ĭ �ݺ�
		if (!strcmp(node->item.data, item.data)) { // ���� �ִ��� Ž��
			scomparison++;
			for (int i = 1; i <= DOC_NUM; i++)
				printf("%d ", node->item.filenum[i]);
			printf("\n");

			// ��ġ����Ʈ�Լ�(�ִ밪, ã�����ϴ� �ܾ� ����)
			// ���ϰ��� -1�̶�� �� �̻� ���°��̹Ƿ� break
			for (int i = 1; i <= DOC_NUM; i++) {
				max = maxnum(node->item.filenum);
				if (max == -1)
					break;
				search_print(max, item.data, node->item.filenum[max]);
				// max�� �ش��ϴ� filnum[i]���� ����� �������Ƿ� 0����
				node->item.filenum[max] = 0;
				printf("\n");
			}
		}
	}
}

// �ִ밪 ã�� �Լ�(������ ã�� ��ġ���� ����� ���ؼ�)
int maxnum(int filenum[]) {
	int max=filenum[1], maxi = 1;
	for (int i = 2; i <= DOC_NUM; i++) {
		if (filenum[i] > max) {
			max = filenum[i];
			maxi = i;
		}
	}
	// max�� 0�̶�� -1�������� �� �̻� ������ �˸�
	if (max == 0)
		return -1;
	return maxi;
}

// ��º�(�˻�)
void search_print(int fnum, char search[], int wnum) {
	char line[1000];
	char *tok;
	FILE *ifp;
	char filename[FILENAME], filetemp[10];
	int i = 0, cnt = 0, j;
	
	// ���� �̸� ����
	strcpy(filename, "document");
	itoa(fnum, filetemp, 10);
	strcat(filetemp, ".txt");
	strcat(filename, filetemp);

	printf("<%s> %s : %d\n", filename, search, wnum);

	// ���� �ȿ����� ����ó��
	if ((ifp = fopen(filename, "r")) == NULL) {
		printf("No such file ! \n");
		exit(1);
	}
	// �� �پ� �Է� -> DEL(������)�� ������ �߶� tok�� ���� -> �ҹ��ڷ� ��ȯ -> �˻�
	while (fgets(line, 1000, ifp) != NULL) {
		tok = strtok(line, DEL);
		while (tok != NULL) {
			scomparison++;
			i++;
			for (int j = 0; j < strlen(tok); j++) {
				tok[j] = tolower(tok[j]);
			}
			// ���ٸ� ����Ʈ
			if (!strcmp(search, tok)) {
				// i��(�˻� �ܾ� ���� �ܾ��� ��)�� 3 �̻��̶�� ...�� �Բ� �ܾ�� ���
				if (i > 3) {
					printf("... ");
					for (j = 4; j >= -2; j--) {
						printf("%s ", voca[fnum][i - j]);
					}
					printf("... \n");
				}
				// 3�� �ȵȴٸ� ...���� �ܾ�� ���
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
