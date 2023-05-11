#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//��ȭ ���� ����
struct Movie {
	char title[15];
	int theater;
	int* ptr;
}; 

//���� ���� ����
struct BookInfo {
	char movie[15];
	int movieNum;
	int bookingNum;
	int* ptr;//�¼� ����Ű�� ������
	int rowNum;
	int columnNum;
	BookInfo* next;
};

struct Queue {
	BookInfo* front, * rear;
};

//�¼� �迭
int theater1_seat[5][6] = { 0, }; // 1�� �¼�
int theater4_seat[6][7] = { 0, }; // 4�� �¼�
int theater9_seat[7][8] = { 0, }; // 9�� �¼�


Movie movie1 = { "�� �� 4",1, theater1_seat[0] };
Movie movie2 = { "���ٿ��",4, theater4_seat[0] };
Movie movie3 = { "ų�� �θǽ�",9, theater9_seat[0] };

Queue que;

void Menu(); // �޴� ���
void ShowMovieList(); // ��ȭ ����Ʈ ���
void Book(Queue* q); // ��ȭ ����
void Update(Queue* q); //��ȭ ����
void AvailableSeats(); // �ܿ� �¼� Ȯ��
void print_queue(Queue* q); //���� ���� ���
bool is_reserved(int r, int c, int data); //�̹� ���ŵ� �¼����� �Ǻ�
bool is_full(int data);//�������� �Ǻ�

void init(Queue* q);
int is_empty(Queue* q);
void enqueue(Queue* q, int data, int row, int column);
void dequeue(Queue* q); //��ȭ ���� ���

int bookingNumber = 0; //�����ȣ

int main() {

	int num;
	
	init(&que);

	while (true) {

		Menu();
		printf("��ȣ�� �Է��ϼ��� >>");
		scanf_s("%d", &num); 

		if (num < 1 || num>6) {
			printf("�߸��� ��ȣ�Դϴ�.\n");
				continue;
		}

		switch (num) {
		case 1 : 
			ShowMovieList();
			break;
		case 2: 
			Book(&que);
			break;
		case 3:
			dequeue(&que);
			break;
		case 4:
			Update(&que);
			break;
		case 5:
			AvailableSeats();
			break;
		case 6:
			print_queue(&que);
			break;
		}
		
	}

}


void Menu() {

	printf("***************** MENU*****************\n\n");
	printf("1. ���� �� ���� ��ȭ\n");
	printf("2. ��ȭ ���� \n");
	printf("3. ��ȭ ���\n");
	printf("4. ��ȭ ����\n");
	printf("5. �ܿ� �¼� Ȯ��\n");
	printf("6. ���� ���� ��ȸ\n\n");
	printf("**************************************\n");

	return ;
}

void ShowMovieList() {
	
	printf("--------------------------------------\n");
	printf(" < ���� �� ���� ��ȭ >\n\n");
	printf("1. �� �� 4       %d��\n", movie1.theater);
	printf("2. ���ٿ��      %d��\n", movie2.theater);
	printf("3. ų�� �θǽ�   %d��\n\n", movie3.theater);
	printf("--------------------------------------\n");
	return;
}

void Book(Queue* q) {
	
	Queue* tmp = q;
	int n, column,row;
	
	while (true) {
		

		AvailableSeats();
		printf("\n���� ��ȭ ��ȣ�� �Է��ϼ��� >>");
		scanf_s("%d",&n);

		if (n < 1 || n>3) {
			printf("�߸��� ��ȣ�Դϴ�.\n");
			continue;
		}
		//���� ���� �Ǻ�
		if (is_full(n)) {
			continue;
		}

		else {
			printf("\n�¼��� �����ϼ���. (��,�� ���ʷ� �Է�) \n");
			printf("�� �Է� >>");
			scanf_s("%d", &row);
			printf("�� �Է� >>");
			scanf_s("%d", &column);

			// ���ŵ� �¼����� �Ǻ�
			if (is_reserved(row, column, n)) {
				continue;
			}

			enqueue(tmp, n, row, column);

			printf("\n ���Ű� �Ϸ�Ǿ����ϴ�.\n");

			break;
		}
		
	}
	return;
}



void Update(Queue* q) {

	BookInfo* p = q->front;
	int num ,data,r,c;

	print_queue(&que);
	printf("\n�����Ͻ� ���� ������ ���� ��ȣ�� �Է��ϼ��� >> ");
	scanf_s("%d", &num);
	
	while (p->bookingNum != num) {
		p = p->next;
	}
	while (true) {
		AvailableSeats();
		printf("�����Ͻ� ��ȭ�� ��ȣ�� �Է����ּ��� >>");
		scanf_s("%d", &data);
		if (is_full(data)) {
			continue;
		}
		printf("\n�¼��� �����ϼ���. (��,�� ���ʷ� �Է�) \n");
		printf("�� �Է� >>");
		scanf_s("%d", &r);
		printf("�� �Է� >>");
		scanf_s("%d", &c);
		if (is_reserved(r, c, data)) {
			continue;
		}
		else break;
	}
	
	// ���� ���� ������ �¼� �ʱ�ȭ
	switch (p->movieNum) {
	case 1:
		theater1_seat[p->rowNum - 1][p->columnNum - 1] = 0;
		break;
	case 2:
		theater4_seat[p->rowNum - 1][p->columnNum - 1] = 0;
		break;
	case 3:
		theater9_seat[p->rowNum - 1][p->columnNum - 1] = 0;
		break;
	}

	// ����� ���� ���� ����
	switch (data) {
	case 1:
		p->movieNum = data;
		strcpy_s(p->movie, movie1.title);
		p->ptr = &theater1_seat[r-1][c-1];
		theater1_seat[r-1][c-1] = 1;
		break;
	case 2:
		p->movieNum = data;
		strcpy_s(p->movie, movie2.title);
		p->ptr = &theater4_seat[r-1][c-1];
		theater4_seat[r-1][c-1] = 1;
		break;
	case 3:
		p->movieNum = data;
		strcpy_s(p->movie, movie3.title);
		p->ptr = &theater9_seat[r-1][c-1];
		theater9_seat[r-1][c-1] = 1;
		break;
	}
	p->rowNum = r;
	p->columnNum = c;
	
	printf("���Ű� ����Ǿ����ϴ�.\n");

	return;
}

void AvailableSeats() {

	printf("\n< �ܿ� �¼� >  ��: ���� ����  ��: ���� �Ұ� \n\n");

	printf("1. �� �� 4  : 1��\n\n");
	printf(" 1 2 3 4 5 6 ");
	for (int i = 0; i < 5; i++) {
		printf("\n");
		printf("%d", i+1);
		for (int j = 0; j < 6; j++) {
			if (theater1_seat[i][j] == 0) {
				printf("��");
			}
			else {
				printf("��");
			}
		}
		
	}
	

	printf("\n\n2. ���ٿ��  : 4��\n\n");
	printf(" 1 2 3 4 5 6 7 ");
	for (int i = 0; i < 6; i++) {
		printf("\n");
		printf("%d", i+1);
		for (int j = 0; j < 7; j++) {
			if (theater4_seat[i][j] == 0) {
				printf("��");
			}
			else {
				printf("��");
			}
		}
	}
	
	printf("\n\n3. ų���θǽ�  : 9��\n\n");
	printf(" 1 2 3 4 5 6 7 8");
	for (int i = 0; i < 7; i++) {
		printf("\n");
		printf("%d", i+1);
		for (int j = 0; j < 8; j++) {
			if (theater9_seat[i][j] == 0) {
				printf("��");
			}
			else {
				printf("��");
			}
		}
		
	}
	printf("\n");
	return;
}

void init(Queue* q) {

	q->front = q->rear = NULL;
}

int is_empty(Queue* q) {

	return(q->front == NULL);
}

void enqueue(Queue* q, int data, int row, int column) { //data�� ��ȭ ��ȣ

	BookInfo* newnode = (BookInfo*)malloc(sizeof(BookInfo));
	newnode -> rowNum = row;
	newnode -> columnNum = column;
	newnode -> next = NULL;

	int r = row-1;
	int c = column-1;
	
	//����, �¼���ȣ ����
	switch (data) {
	case 1: 
		newnode->movieNum = data;
		strcpy_s(newnode->movie, movie1.title);
		newnode->ptr = &theater1_seat[r][c];
		theater1_seat[r][c] = 1;
		break;
	case 2: 
		newnode->movieNum = data;
		strcpy_s(newnode->movie, movie2.title);
		newnode->ptr = &theater4_seat[r][c];
		theater4_seat[r][c] = 1;
		break;
	case 3:
		newnode->movieNum = data;
		strcpy_s(newnode->movie, movie3.title);
		newnode->ptr = &theater9_seat[r][c];
		theater9_seat[r][c] = 1;
		break;
	}

	//�����ȣ ����
	bookingNumber += 1;
	newnode->bookingNum = bookingNumber;
	
	
		
	if (is_empty(q)) {
		q->front = newnode;
		q->rear = newnode;
	}
	else {
		q->rear->next = newnode;
		q->rear = newnode;
	}
}

void dequeue(Queue* q) { 

	BookInfo* temp;
	int data,num;

	if (is_empty(q)) {
		printf("������ ������ �����ϴ�.\n");
	}

	else {
		temp = q->front;
		data = temp->bookingNum;

		num = temp->movieNum;

		switch (num) {
		case 1:
			theater1_seat[temp->rowNum-1][temp->columnNum-1] = 0;
			break;
		case 2:
			theater4_seat[temp->rowNum-1][temp->columnNum-1] = 0;
			break;
		case 3:
			theater9_seat[temp->rowNum-1][temp->columnNum-1] = 0;
			break;
		}
		q->front = temp->next;

		if (q->front == NULL) {
			q->rear = NULL;
		}

		free(temp);
		printf("���Ź�ȣ : %d ������ ��� �Ǿ����ϴ�.\n",data);

		return;
	}
}

void print_queue(Queue* q) {

	BookInfo* p;
	printf("< ���� ���� >\n");

	if (q->front == NULL) {
		printf("���� ������ �����ϴ�.\n");
		return;
	}

	for (p = q->front; p!= NULL; p = p->next) {
		printf("-------------------------------------\n");
		printf("���Ź�ȣ : %d \n", p->bookingNum);
		printf("��ȭ : %s\n", p->movie);
		printf("�¼���ȣ : %d�� %d��\n", p->rowNum, p->columnNum);
		printf("-------------------------------------\n");
	}
	
	
	return;
	
}

bool is_reserved(int r, int c ,int data) {

	int n = data;
	int row = r;
	int column = c;

	switch (n) {
	case 1:
		if (theater1_seat[row - 1][column - 1] == 1) {
			printf("\n***�̹� ���ŵ� �ڼ��Դϴ�.***\n");
			return 1;
		}
	case 2:
		if (theater4_seat[row - 1][column - 1] == 1) {
			printf("\n***�̹� ���ŵ� �ڼ��Դϴ�.***\n");
			return 1;
		}
	case 3:
		if (theater9_seat[row - 1][column - 1] == 1) {
			printf("\n***�̹� ���ŵ� �ڼ��Դϴ�.***\n");
			return 1;
		}
	}
	return 0;
}

bool is_full(int data) {
	int sum = 0;

	switch (data) {
	case 1: 
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 6; j++) {
				if (theater1_seat[i][j] == 1) {
					sum += 1;
				}
			}
		}
		if (sum == 30) {
			printf("������ ��ȭ�Դϴ�.\n");
			return 1;
		}
		return 0;

	case 2:
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 7; j++) {
				if (theater1_seat[i][j] == 1) {
					sum += 1;
				}
			}
		}
		if (sum == 42) {
			printf("������ ��ȭ�Դϴ�.\n");
			return 1;
		}
		return 0;

	case 3:
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 8; j++) {
				if (theater1_seat[i][j] == 1) {
					sum += 1;
				}
			}
		}
		if (sum == 56) {
			printf("������ ��ȭ�Դϴ�.\n");
			return 1;
		}
		return 0;
	}
	

}