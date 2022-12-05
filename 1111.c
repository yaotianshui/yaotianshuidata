#include<windows.h>
#include<mysql.h>
#include<stdio.h>
#pragma comment(lib, "libmysql.lib") 
#pragma warning(disable:4996)

MYSQL mysql;
MYSQL_RES* result;
MYSQL_ROW row;
char  ch[2];

void ShowAll();
void AddBook();
void ModifyBook();
void DelteBook();
void QueryBook();

void ShowMenu()
{
	system("cls");
	printf("\n\n\n\n\n");
	printf("\t|***************************************\n");
	printf("\t|  Welcome To Lemon Books System       |\n");
	printf("\t|***************************************\n");
	printf("\t|\t 1 - 모든 도서 정보 표시            |\n");
	printf("\t|\t 2 - 추가 도서 정보                 |\n");
	printf("\t|\t 3 - 도서 정보 삭제                 |\n");
	printf("\t|\t 4 - 도서 정보 조회                 |\n");
	printf("\t|\t 6 - 종료                           |\n");
	printf("\t|***************************************\n");
	printf("\t|  ENTER YOUR CHOICE(1-6)              |\n");

}

int main()
{
	int n;
	mysql_init(&mysql);
	ShowMenu();
	scanf("%d", &n);

	while (n)
	{
		switch (n)
		{
		case 1:
			ShowAll();
			break;

		case 2:
			AddBook();
			break;

		case 3:
			ModifyBook();
			break;

		case 4:
			DelteBook();
			break;

		case 5:
			QueryBook();
			break;

		case 6:
			exit(0);

		default:
			break;
		}

		scanf("%d", &n);
	}

}

void inquire()
{
	printf("\t메인 메뉴가 뜬다고요?（y/n）");
	scanf("%s", ch);

	if (strcmp(ch, "Y") == 0 || strcmp(ch, "y") == 0)
	{
		ShowMenu();
	}
	else
	{
		exit(0);
	}
}


void ShowAll()
{
	// 데이터베이스 연결
	if (!mysql_real_connect(&mysql, "192.168.46.6", "root", "pqlcyl", "dp_books", 0, NULL, 0))
	{
		printf("\n\t Can not connect db_books!\n");
	}
	else
	{
		// 데이터베이스 연결 성공!
		if (mysql_query(&mysql, "select * from tb_book"))
		{
			printf("\n\t Query tb_book failed!\n");
		}
		else
		{
			/*조회실패*/
			result = mysql_store_result(&mysql);
			if (mysql_num_rows(result) > 0)
			{
				printf("\t***************************************\n");
				printf("\t            기록 정보 보이기               \n");
				printf("\t***************************************\n");
				printf("\t 도서번호     도서명     저자     출판사    \n");
				printf("\t***************************************\n");
				while (row = mysql_fetch_row(result))
				{
					fprintf(stdout, "\t %s      %s      %s     %s  \n", row[0], row[1], row[2], row[3]);

				}
				printf("\t***************************************\n");

			}
			else
			{
				printf("\n\t기록 정보 없음");
			}
			mysql_close(&mysql);

		}

	}

}

void AddBook()
{
	int rowcount;
	char ID[10];
	char bookname[50];
	char author[50];
	char bookconcern[100];


	char* sql;
	char dest[220] = { " " };

	if (!mysql_real_connect(&mysql, "192.168.46.6", "root", "pqlcyl", "dp_books", 0, NULL, 0))
	{
		printf("\n\t Can not connect db_books!\n");
	}
	else
	{
		printf("\t***************************************\n");
		printf("\t             도서 정보 추가                \n");
		printf("\t***************************************\n");

		if (mysql_query(&mysql, "select * from tb_book"))
		{
			printf("\n\t Query tb_book failed!\n");
		}
		else
		{
			/*쿼리 실패*/
			result = mysql_store_result(&mysql);
			rowcount = mysql_num_rows(result);
			row = mysql_fetch_row(result);
			printf("\t ID:");
			scanf("%s", ID);
			if (mysql_num_rows(result) != NULL)
			{
				do
				{
					if (!strcmp(ID, row[0]))
					{
						printf("\t기록이 존재합니다！");
						getch();
						mysql_free_result(result);
						mysql_close(&mysql);
						return;
					}
				} while (row = mysql_fetch_row(result));
			}


			printf("\t 도서명：");
			scanf("%s", &bookname);
			sql = "insert into tb_book(ID, bookname, author, bookconcern) values(";
			strcat(dest, sql);
			strcat(dest, "'");
			strcat(dest, ID);
			strcat(dest, "','");
			//strcat(dest, &bookname);
			strcat(dest, bookname);
			printf("\t 작자： ");
			scanf("%s", &author);
			strcat(dest, "','");
			//strcat(dest, &author);
			strcat(dest, author);
			printf("\t 출판사： ");
			scanf("%s", &bookconcern);
			strcat(dest, "','");
			strcat(dest, bookconcern);
			//strcat(dest, &bookconcern);
			strcat(dest, "')");
			printf("%s\n", dest);

			if (mysql_query(&mysql, dest) != 0)
			{
				fprintf(stderr, "레코드를 삽입할 수 없음！\n", mysql_error(&mysql));
				printf("%s\n", mysql_error(&mysql));
			}
			else
			{
				printf("\t 삽입 성공！\n");
			}
			mysql_free_result(result);
		}
		inquire();
	}
}

void ModifyBook()
{
	char ID[10];
	char dest[100] = { " " };
	char dest1[220] = { " " };
	char* bookname;
	char* author;
	char* bookconcern;
	char* sql;


	if (!mysql_real_connect(&mysql, "192.168.46.6", "root", "pqlcyl", "dp_books", 0, NULL, 0))
	{
		printf("\n\t Can not connect db_books!\n");
	}
	else
	{
		printf("수정할 도서 번호를 입력하십시오\n");
		scanf("%s", ID);

		sql = "select * from tb_book where id ='";
		strcat(dest, sql);
		strcat(dest, ID);
		strcat(dest, "'");

		if (mysql_query(&mysql, dest))
		{
			printf("\n tb_book 데이터 쿼리 실패！\n");
		}
		else
		{
			result = mysql_store_result(&mysql);
			if (mysql_num_rows(result) != NULL)
			{
				printf("\t검색 기록 정보, 표시 여부？(Y/N)");
				scanf("%s", ch);

				if (strcmp(ch, "Y") == 0 || strcmp(ch, "y") == 0)
				{
					printf("\t***************************************\n");
					printf("\t            기록 정보 보이기               \n");
					printf("\t***************************************\n");
					printf("\t 도서번호     도서명     저자     출판사    \n");
					printf("\t***************************************\n");
					while (row = mysql_fetch_row(result))
					{
						fprintf(stdout, "\t %s      %s      %s     %s  \n", row[0], row[1], row[2], row[3]);

					}

					printf("\t***************************************\n");
					printf("\t Modify? (y/n) \n");
					scanf("%s", ch);
					if (strcmp(ch, "Y") == 0 || strcmp(ch, "y") == 0)
					{
						printf("\t 도서명：");
						scanf("%s", &bookname);
						sql = "update tb_book set bookname= '";
						strcat(dest1, sql);
						strcat(dest1, &bookname);
						printf("\t 작자： ");
						scanf("%s", &author);
						strcat(dest1, "', author= '");
						strcat(dest1, &author);
						printf("\t 출판사： ");
						scanf("%s", &bookconcern);
						strcat(dest1, "', bookconcern='");
						strcat(dest1, &bookconcern);
						strcat(dest1, "' where ID= '");
						strcat(dest1, ID);
						strcat(dest1, "'");
						printf("dest = %s\n", dest);
						printf("dest1 =%s\n", dest1);

						if (mysql_query(&mysql, dest1) != 0)
						{
							fprintf(stderr, "기록을 수정할 수 없다！\n", mysql_error(&mysql));
							printf("%s\n", mysql_error(&mysql));
						}
						else
						{
							printf("\t 수정 성공！\n");
						}
					}
				}
			}
			else
			{
				printf("수정할 정보를 찾을 수 없습니다");
			}
		}
		mysql_free_result(result);
	}
	mysql_close(&mysql);
	inquire();
}