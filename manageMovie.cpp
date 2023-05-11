#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//영화 정보 저장
struct Movie {
	char title[15];
	int theater;
	int* ptr;
}; 

//예약 정보 저장
struct BookInfo {
	char movie[15];
	int movieNum;
	int bookingNum;
	int* ptr;//좌석 가리키는 포인터
	int rowNum;
	int columnNum;
	BookInfo* next;
};

struct Queue {
	BookInfo* front, * rear;
};

//좌석 배열
int theater1_seat[5][6] = { 0, }; // 1관 좌석
int theater4_seat[6][7] = { 0, }; // 4관 좌석
int theater9_seat[7][8] = { 0, }; // 9관 좌석


Movie movie1 = { "존 윅 4",1, theater1_seat[0] };
Movie movie2 = { "리바운드",4, theater4_seat[0] };
Movie movie3 = { "킬링 로맨스",9, theater9_seat[0] };

Queue que;

void Menu(); // 메뉴 출력
void ShowMovieList(); // 영화 리스트 출력
void Book(Queue* q); // 영화 예매
void Update(Queue* q); //영화 변경
void AvailableSeats(); // 잔여 좌석 확인
void print_queue(Queue* q); //예매 내역 출력
bool is_reserved(int r, int c, int data); //이미 예매된 좌석인지 판별
bool is_full(int data);//매진인지 판별

void init(Queue* q);
int is_empty(Queue* q);
void enqueue(Queue* q, int data, int row, int column);
void dequeue(Queue* q); //영화 예매 취소

int bookingNumber = 0; //예약번호

int main() {

	int num;
	
	init(&que);

	while (true) {

		Menu();
		printf("번호를 입력하세요 >>");
		scanf_s("%d", &num); 

		if (num < 1 || num>6) {
			printf("잘못된 번호입니다.\n");
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
	printf("1. 현재 상영 중인 영화\n");
	printf("2. 영화 예매 \n");
	printf("3. 영화 취소\n");
	printf("4. 영화 변경\n");
	printf("5. 잔여 좌석 확인\n");
	printf("6. 예매 내역 조회\n\n");
	printf("**************************************\n");

	return ;
}

void ShowMovieList() {
	
	printf("--------------------------------------\n");
	printf(" < 현재 상영 중인 영화 >\n\n");
	printf("1. 존 윅 4       %d관\n", movie1.theater);
	printf("2. 리바운드      %d관\n", movie2.theater);
	printf("3. 킬링 로맨스   %d관\n\n", movie3.theater);
	printf("--------------------------------------\n");
	return;
}

void Book(Queue* q) {
	
	Queue* tmp = q;
	int n, column,row;
	
	while (true) {
		

		AvailableSeats();
		printf("\n상영할 영화 번호를 입력하세요 >>");
		scanf_s("%d",&n);

		if (n < 1 || n>3) {
			printf("잘못된 번호입니다.\n");
			continue;
		}
		//매진 여부 판별
		if (is_full(n)) {
			continue;
		}

		else {
			printf("\n좌석을 선택하세요. (행,열 차례로 입력) \n");
			printf("행 입력 >>");
			scanf_s("%d", &row);
			printf("열 입력 >>");
			scanf_s("%d", &column);

			// 예매된 좌석인지 판별
			if (is_reserved(row, column, n)) {
				continue;
			}

			enqueue(tmp, n, row, column);

			printf("\n 예매가 완료되었습니다.\n");

			break;
		}
		
	}
	return;
}



void Update(Queue* q) {

	BookInfo* p = q->front;
	int num ,data,r,c;

	print_queue(&que);
	printf("\n변경하실 예매 내역의 예매 번호를 입력하세요 >> ");
	scanf_s("%d", &num);
	
	while (p->bookingNum != num) {
		p = p->next;
	}
	while (true) {
		AvailableSeats();
		printf("변경하실 영화의 번호를 입력해주세요 >>");
		scanf_s("%d", &data);
		if (is_full(data)) {
			continue;
		}
		printf("\n좌석을 선택하세요. (행,열 차례로 입력) \n");
		printf("행 입력 >>");
		scanf_s("%d", &r);
		printf("열 입력 >>");
		scanf_s("%d", &c);
		if (is_reserved(r, c, data)) {
			continue;
		}
		else break;
	}
	
	// 이전 예매 내역의 좌석 초기화
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

	// 변경된 예매 내역 저장
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
	
	printf("예매가 변경되었습니다.\n");

	return;
}

void AvailableSeats() {

	printf("\n< 잔여 좌석 >  □: 예매 가능  ■: 예매 불가 \n\n");

	printf("1. 존 윅 4  : 1관\n\n");
	printf(" 1 2 3 4 5 6 ");
	for (int i = 0; i < 5; i++) {
		printf("\n");
		printf("%d", i+1);
		for (int j = 0; j < 6; j++) {
			if (theater1_seat[i][j] == 0) {
				printf("□");
			}
			else {
				printf("■");
			}
		}
		
	}
	

	printf("\n\n2. 리바운드  : 4관\n\n");
	printf(" 1 2 3 4 5 6 7 ");
	for (int i = 0; i < 6; i++) {
		printf("\n");
		printf("%d", i+1);
		for (int j = 0; j < 7; j++) {
			if (theater4_seat[i][j] == 0) {
				printf("□");
			}
			else {
				printf("■");
			}
		}
	}
	
	printf("\n\n3. 킬링로맨스  : 9관\n\n");
	printf(" 1 2 3 4 5 6 7 8");
	for (int i = 0; i < 7; i++) {
		printf("\n");
		printf("%d", i+1);
		for (int j = 0; j < 8; j++) {
			if (theater9_seat[i][j] == 0) {
				printf("□");
			}
			else {
				printf("■");
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

void enqueue(Queue* q, int data, int row, int column) { //data는 영화 번호

	BookInfo* newnode = (BookInfo*)malloc(sizeof(BookInfo));
	newnode -> rowNum = row;
	newnode -> columnNum = column;
	newnode -> next = NULL;

	int r = row-1;
	int c = column-1;
	
	//제목, 좌석번호 저장
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

	//예약번호 저장
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
		printf("삭제할 내역이 없습니다.\n");
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
		printf("예매번호 : %d 내역이 취소 되었습니다.\n",data);

		return;
	}
}

void print_queue(Queue* q) {

	BookInfo* p;
	printf("< 예매 내역 >\n");

	if (q->front == NULL) {
		printf("예매 내역이 없습니다.\n");
		return;
	}

	for (p = q->front; p!= NULL; p = p->next) {
		printf("-------------------------------------\n");
		printf("예매번호 : %d \n", p->bookingNum);
		printf("영화 : %s\n", p->movie);
		printf("좌석번호 : %d행 %d열\n", p->rowNum, p->columnNum);
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
			printf("\n***이미 예매된 자석입니다.***\n");
			return 1;
		}
	case 2:
		if (theater4_seat[row - 1][column - 1] == 1) {
			printf("\n***이미 예매된 자석입니다.***\n");
			return 1;
		}
	case 3:
		if (theater9_seat[row - 1][column - 1] == 1) {
			printf("\n***이미 예매된 자석입니다.***\n");
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
			printf("매진된 영화입니다.\n");
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
			printf("매진된 영화입니다.\n");
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
			printf("매진된 영화입니다.\n");
			return 1;
		}
		return 0;
	}
	

}