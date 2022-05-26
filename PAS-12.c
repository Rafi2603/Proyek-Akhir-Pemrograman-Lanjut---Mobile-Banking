#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <omp.h>
#include <dirent.h>
#include <time.h>

#define LIMIT 200
#define TRUE 1
#define FALSE 0

typedef struct{
	char nama[100];
	char username[100];
	char password[100];
	char pin[100];
	int day;
	int month;
	int year;
	char rekening[100];
	int saldo;
	//yang dibawah digunakan untuk menyimpan data dalam resi
	char time[100];
	char noTelp[100];
	char jenisTransaksi[100];
	int jumlah;
	int saldoawal;
	int kwh;
	char kategori[100];
} User;

typedef struct{
	int nominal;
	char noTelp[100];
	char time[100];
} emoney;

typedef struct{
	int biaya;
	int kwh;
	int daya;
	int air;
	char time[100];
	char kategori[100];
} bayar;

typedef struct node{
	User data;
	struct node* next; 
} Node;

typedef struct queue{
	int count;
	Node *front;
	Node *rear;
} Queue;

void login();
int isEmpty(char* filename);
void createFolder(User data, char* filename);
void menu(User listUser, char *filename);
void cekSaldo(User data);
void quickSort(User data[], int low, int high);
int partition(User data[], int low, int high);
void squickSort(User data[], int low, int high, char type);
int spartition(User data[], int low, int high, char type);
void swap(int *a, int *b);
void sswap(char *str1, char *str2);
void transfer(User data);
void resiTransfer(int jumlah, User data, User target);
void resiTopup(User data, emoney topup, int jumlah, char type);
void resiBayarPLN(User data, bayar pembayaran, int biaya);
void resiBayarPDAM(User data, bayar pembayaran, int biaya, char kategori[100]);
void changeData(User target, int jumlah);
void changeFile(char *file, char* file2);
void gantiPin(User data);
void topup(User listUser, char menu); //topup e-money (Gopay dan OVO)
void pembayaran(User listUser, char menu); //pembayaran tagihan listrik dan air (PLN dan PDAM)
Node *arrayToList(User arr[], int n);
void reverse(Node** head_ref);
void printHistory(Node *history);
void enqueue(Queue *queue, User value);
void dequeue(Queue *queue);


int main(){
	
	login();
	return 0;
}

void login(){
	FILE *file;
	User userData;						//meletakkan data user untuk di-register	
	User listUser[LIMIT] = {'\0', '\0'}; //meletakkan user yang ter-register
	
	int loginmenu = 0, i = 0, j, check = FALSE;
	char temp[100] = {'\0'};
	char *userfilename = "userdata.txt"; //file untuk meletakkan user yang telah ter-register
	
	char *userFile;					//nama file berdasarkan username user
	
	file = fopen (userfilename, "a+");
	if (file == NULL){
		printf("Error! Tidak dapat membuka file");
		exit(0);
	}
	fclose(file);
	
	while (TRUE){
	system("cls");
	printf("\n=================|HOME|=================\n");
		if (isEmpty(userfilename) == TRUE){
	        printf("\n\n1. | Register\n");
	        printf("0. | Keluar\n");
	    }
	    if (isEmpty(userfilename) == FALSE){
	        printf("\n\n1. | Register\n");
	        printf("2. | Login\n");
	        printf("0. | Keluar\n");
		}
		printf("Pilihan Anda: ");
		scanf("%d", &loginmenu);
		
		switch (loginmenu){
			case 0:
				printf("Terima kasih telah menggunakan program ini");
				exit(0);
			case 1:
				system("cls");
				file = fopen(userfilename, "a+");
				
				while (fscanf(file, " %[^\n]", listUser[i].nama) != -1){
			        fscanf(file, " %[^\n]", listUser[i].username);
			        fscanf(file, " %[^\n]", listUser[i].password);
			        fscanf(file, " %[^\n]", listUser[i].pin);
			        i++;
			    }
				
				
				printf("Masukkan Nama: ");
				scanf(" %[^\n]s", userData.nama);
				for (j = 0; j < i; j++){
					if (!strcmpi(listUser[j].nama, userData.nama)){
						check = TRUE;
						break;
					} 	
				}
				if (check){
					printf("Nama telah terdaftar\n");
					memset(userData.nama, 0, strlen(userData.nama));
					Sleep(1000);
					system("cls");
					break;
					}
				
				printf("Masukkan Username: ");
				scanf(" %[^\n]s", userData.username);
				
				while (TRUE){
					printf("Masukkan Password: ");
					scanf(" %[^\n]s", userData.password);
					printf("Masukkan Ulang Password: ");
					scanf(" %[^\n]s", temp);
					if (strcmp(userData.password, temp) == 0){
						break;
					} else {
						printf("Password tidak sesuai\n");
					}
				}
				
				fprintf(file, "%s\n", userData.nama);
	        	fprintf(file, "%s\n", userData.username);
	        	fprintf(file, "%s\n", userData.password);
				
				fclose(file);
				createFolder(userData, NULL);
				printf("\nRegistrasi berhasil. Kembali ke menu utama...");
				
				i = 0;
				memset(listUser, 0, LIMIT);
				Sleep(1000);
				break;
				
			case 2:
			    file = fopen(userfilename, "r");
			    char user[100]; 
				char pass[100];
			
			    i = 0;
			    int count = 0;
			    while (fscanf(file, " %[^\n]", listUser[i].nama) != -1){
			        fscanf(file, " %[^\n]", listUser[i].username);
			        fscanf(file, " %[^\n]", listUser[i].password);
			        i++;
			    }
			    fclose(file);
			    
			    i = 0;
			    printf("Masukkan Username: ");
				scanf(" %[^\n]s", user);
				printf("Masukkan Password: ");
				
				while (pass[i-1]!='\r'){
        			pass[i] = getch();
        			if (pass[i] != '\r'){
            			printf("*");
        			}
       				i++;
    			}
    			pass[i-1]= '\0';
				
				i = 0;
				while ((listUser[i].nama)[0] != '\0'){
					count++;
					i++;
				}
				
				for (i = 0; i < count; i++){
					if (strcmp(user, listUser[i].username) == 0){
						if (strcmp(pass, listUser[i].password) == 0){
							printf("\n\nLogin Sukses...");
							
							userFile = (char*) malloc (strlen(listUser[i].username));
							strcpy(userFile, listUser[i].username);
							strcat(userFile, "_data.txt");
							createFolder(listUser[i], userFile);
							
							Sleep(1000);
							system("cls");
							menu(listUser[i], userFile);
							break;
							}
						}	
					}
					if (strcmp(user, listUser[i].username) != 0 || strcmp(pass, listUser[i].password) != 0){
						printf("\n\nUsername Tidak ada atau Password salah\n");
						Sleep(1000);
					}
				break;
				
			default:
				printf("Pilihan Tidak Ada");
				Sleep(1000);
				break;
		}
	}
}

void menu(User listUser, char *filename){
    FILE *file; //deklarasi variable file bertipe pointer struct FILE
    
    int count, i;
    int day, month, year;
    
    char temp[100] = {'\0'};
    
    
	
	while(TRUE){
		system("cls");
		
        char menuChoice = '\0';
        char source[100] = ".//";
    
    	strcat(source, listUser.username);
		strcat(source, "//");
		strcat(source, filename);
        
        if (isEmpty(source) == TRUE){
        	printf("\n=================|MENU|=================\n");
    		printf("\nSelamat Datang %s\n\n", listUser.nama);
            printf("\nAnda baru disini, masukkan data diri Anda: \n");
            printf("Nama : %s\n", listUser.nama);
            
            while(TRUE){
            	printf("Masukkan Tanggal Lahir (DD/MM/YYYY) : ");
            	scanf ("%2d%2d%4d", &listUser.day, &listUser.month, &listUser.year);
            	if ((listUser.day > 0 && listUser.day <= 31) && (listUser.month > 0 && listUser.month <= 12)){
            		break;
				}
				else{
					printf("Tanggal Tidak Valid\n");
					Sleep(1000);
				}
			}
            printf("Masukkan No. Rekening: ");
            scanf(" %[0123456789]s", listUser.rekening);
            
            while (TRUE){
				printf("Masukkan PIN ATM (6 digit angka): ");
				scanf(" %[0123456789]", listUser.pin);
				
				printf("Konfirmasi PIN: ");
				scanf(" %[0123456789]", temp);
				
				if(((listUser.pin)[5] == '\0' || (listUser.pin)[6] != '\0') || strcmp(listUser.pin, temp)) {
					printf("PIN tidak valid atau PIN salah\n");
					memset(listUser.pin, 0, strlen(listUser.pin));
					memset(temp, 0, strlen(temp));
				}
				else{
					break;
				}	
			}
			printf("Masukkan jumlah saldo saat ini: ");
			scanf ("%d", &listUser.saldo);
					
			file = fopen(source, "a+");
		
			fprintf(file, "%s\n", listUser.nama);
	        fprintf(file, "%s\n", listUser.rekening);
	        fprintf(file, "%s\n", listUser.pin);
	        fprintf(file, "%d\n", listUser.saldo);
						
	        fclose(file);
					
			printf("Data berhasil disimpan\n");
			Sleep(1000);
			system("cls");
        }
        
        file = fopen(source, "r");
        
    	while (fscanf(file, " %[^\n]", listUser.nama) != -1){
			fscanf(file, " %[^\n]", listUser.rekening);
			fscanf(file, " %[^\n]", listUser.pin);
			fscanf(file, " %d", &listUser.saldo);
    	}
        
        printf("\n=================|MENU|=================\n");
    	printf("\nSelamat Datang %s\n\n", listUser.nama);
    
        printf("\n\nA. | Cek Saldo\n");
        printf("B. | Transfer\n");
		printf("C. | Top-Up\n");
		printf("D. | Bayar\n");
		printf("E. | Ganti PIN\n");
		printf("Q. | Keluar\n");
		
		printf("Pilihan Anda: ");
		scanf("%s", &menuChoice);
        
        switch (toupper(menuChoice)){
			case 'Q':
				printf("\nKeluar dari akun %s...\n", listUser.nama);
				Sleep(1000);
				system("cls");
				printf("Press any key to continue... ");
				getch();
				return;
		
			case 'A':
                system("cls");
                cekSaldo(listUser);
				break;
			
            case 'B':
                system("cls");
                transfer(listUser);
                break;
            case 'C':
				system("cls");
				topup(listUser, toupper(menuChoice));
				break;
				
			case 'D':
				system("cls");
				pembayaran(listUser, toupper(menuChoice));
				break;
            
            case 'E':
            	gantiPin(listUser);
                break;
            
            default:
                printf("Pilihan tidak ada");
                Sleep(1000);
				system("cls");
                break;
        }
    }
}

//mengecek apakah file ada atau tidak
int isEmpty(char* filename){ 

    FILE *file = fopen(filename, "a+");
    char sortChoice;
   
    if(file == NULL){
        printf("Tidak dapat membuka file %s.", filename);
        exit(0);
    }
    
    fseek(file, 0, SEEK_END); //memindahkan pointer file ke akhir data file
    int size = ftell(file);   //me-return size file
    fclose(file);

    if (size > 0) 			//size file > 0, maka file ada
        return FALSE;
    else
        return TRUE;		//size file == 0, maka file tidak ada
}

void createFolder(User data, char* filename){
	FILE *file;
	char *string = (char*) malloc(strlen(data.username));
	strcpy(string, data.username);
	mkdir(string);
	
	char source[100] = ".//";
	
	if (filename != NULL){
		strcat(source, string);
		strcat(source, "//");
		strcat(source, filename);
	
		file = fopen(source, "a+");
	
		if (file == NULL){
			printf("Unable to create file");
		}
			
		fclose(file);
	}	
}

void cekSaldo(User listUser){
	
	Node *history = NULL;
	
	Queue *dataHistory = (Queue*) malloc (sizeof(Queue));
	dataHistory->count = 0;
	dataHistory->front = NULL;
	dataHistory->rear = NULL;
	
	User data[LIMIT] = {'\0'};
	
	char *listFile[LIMIT] 		= {'\0'};
	char filename[LIMIT] 		= ".//";
	char tempfilename[LIMIT] 	= {'\0'};
	char datafile[LIMIT]		= {'\0'};
	
	strcpy(datafile, listUser.username);
	strcat(datafile, "_data.txt");
	
	FILE *file;
	
	int i = 0, j = 0, k;
	DIR *dir;
	struct dirent *en;
	dir = opendir(listUser.username); //open present directory
	if (dir) {
		while ((en = readdir(dir)) != NULL) {
			i++;
			if (strcmp(en->d_name, ".") && strcmp(en->d_name, "..") && strcmp(en->d_name, datafile)){
				listFile[j] = (char*) malloc (strlen(en->d_name) + 1);
				strcpy(listFile[j], en->d_name);
				j++;
			}
				
		}
		closedir(dir); //close all directory
	}
	strcat(filename, listUser.username);
	strcat(filename, "//");
	
	{
		for (k = 0; k < j; k++){
		
			{
			strcpy(tempfilename, filename);
			strcat(tempfilename, listFile[k]);
				
			file = fopen(tempfilename, "r");
			if (file == NULL){
				printf("\nFailed to open %s", tempfilename);
				exit(0);
			}
			
			while (fscanf(file, " %[^\n]", data[k].time) != -1){
				fscanf(file, " %[^\n]", data[k].jenisTransaksi);
				if (!strcmp(data[k].jenisTransaksi,"GoPay") || !strcmp(data[k].jenisTransaksi, "OVO")){
					fscanf(file, " %[^\n]", data[k].noTelp);
				}
				
				else if (!strcmp(data[k].jenisTransaksi,"PLN"))
					fscanf(file, " %d", &data[k].kwh);
				
				else if (!strcmp(data[k].jenisTransaksi,"PDAM"))
					fscanf(file, " %[^\n]", data[k].kategori);
				
				else{
					fscanf(file, " %[^\n]", data[k].rekening);
				}
				
				fscanf(file, " %d", &data[k].saldoawal);
				fscanf(file, " %d", &data[k].jumlah);
				fscanf(file, " %d", &data[k].saldo);
			}
			
			fclose(file);
			memset(tempfilename, 0, strlen(tempfilename));
				
			}
		}
	}
	
	int sortChoice = 0;
	
	
	printf("Sisa Saldo \t\t: %d\n", listUser.saldo);
	printf("==================================\n");
	
	printf("\n\nList Pilihan Sorting: \n");
	printf("'1' : berdasarkan waktu\n");
	printf("'2' : berdasarkan jenis\n");
	printf("'3' : berdasarkan nominal\n");
	printf("\n\nPilihan Sorting (0 untuk keluar) : ");
		
	scanf("%d", &sortChoice);
	system("cls");

	if (sortChoice == 1){
		/*membuat parallel region dengan banyak thread sebanyak nilai thread*/ 
		#pragma omp parallel 
		{
			/*hanya main thread yang dapat menjalankan code ini, thread ini akan membagi tugas ke thread lainnya
			   melalui #pragma omp task pada fungsi quickSort()*/
			#pragma omp master
				squickSort(data, 0, j - 1, 't'); 
		}		
	}
	
	else if (sortChoice == 2){
		/*membuat parallel region dengan banyak thread sebanyak nilai thread*/ 
		#pragma omp parallel 
		{
			/*hanya main thread yang dapat menjalankan code ini, thread ini akan membagi tugas ke thread lainnya
			   melalui #pragma omp task pada fungsi quickSort()*/
			#pragma omp master
				squickSort(data, 0, j - 1, 'k'); 
		}
	}
	
	else if (sortChoice == 3){
		/*membuat parallel region dengan banyak thread sebanyak nilai thread*/ 
		#pragma omp parallel 
		{
			/*hanya main thread yang dapat menjalankan code ini, thread ini akan membagi tugas ke thread lainnya
			   melalui #pragma omp task pada fungsi quickSort()*/
			#pragma omp master
				quickSort(data, 0, j - 1); 
		}
	}
	
	else{
		/*membuat parallel region dengan banyak thread sebanyak nilai thread*/ 
		#pragma omp parallel 
		{
			/*hanya main thread yang dapat menjalankan code ini, thread ini akan membagi tugas ke thread lainnya
			   melalui #pragma omp task pada fungsi quickSort()*/
			#pragma omp master
				squickSort(data, 0, j - 1, 't'); 
		}	
	}
	
	
		printf("Sisa Saldo \t\t: %d\n", listUser.saldo);
		printf("==================================\n");
		
		printf("\nList Transaksi: (Maksimum 10)\n\n");
		printf("\n ____________________________________________________________________ ");
		    printf("\n| %2s%1s |%14s%10s | %14s%1s | %7s%5s |", "No.", " " ,"Waktu Transaksi"," " ,"Jenis Transaksi"," " ,"Nominal"," ");
		    printf("\n|===================================================================|");
		    
		history = arrayToList(data, j);
		reverse(&history);
		
		while (history != NULL){
			enqueue(dataHistory, history->data);
			if (dataHistory->count > 10){
				dequeue(dataHistory);
			}
			history = history->next;
		}      
		reverse(&dataHistory->front);
		printHistory(dataHistory->front);

	printf("\n\nPress any key to continue...\n");
	getch();
	
	
	
	
}

void printHistory(Node *history){
	int k = 0;
	while (history != NULL){
		printf("\n| %2d%2s | %24s |", k+1, " " ,history->data.time);
		if (strcmp(history->data.jenisTransaksi, "GoPay") && strcmp(history->data.jenisTransaksi, "OVO") && strcmp(history->data.jenisTransaksi, "PLN") && strcmp(history->data.jenisTransaksi, "PDAM"))
			printf(" %16s |", "Transfer");
		else
			printf(" %16s |", history->data.jenisTransaksi);
			
		printf(" %12d |", history->data.jumlah);
		history = history->next;
		k++;
	}
	printf("\n|______|__________________________|__________________|______________|");
}

void reverse(Node** head_ref)
{
    Node* prev = NULL;
    Node* current = *head_ref;
    Node* next = NULL;
    while (current != NULL) {
        // Store next
        next = current->next;
 
        // Reverse current node's pointer
        current->next = prev;
 
        // Move pointers one position ahead.
        prev = current;
        current = next;
    }
    *head_ref = prev;
}

void insertEnding(Node **head, User data){
	Node *temp = (Node*) malloc(sizeof(Node));
	Node *last = *head;
	
	temp->data = data;
	temp->next = NULL;
	
	if (*head == NULL){
		*head = temp;
		return;
	}
	while (last->next != NULL)
		last = last->next;
	
	last->next = temp;
	return;
}

Node *arrayToList(User arr[], int n){
	int i;
    Node *root = NULL;
    for (i = 0; i < n; i++)
        insertEnding(&root, arr[i]);
        
   return root;
}

void enqueue(Queue *queue, User value){
    Node *temp = (Node*) malloc (sizeof(Node));
    temp->data = value;
    temp->next = NULL;
    
    if(isqueueempty(queue) == 0){
        queue->rear->next = temp;
        queue->rear = temp;
    } 
	else
        queue->front = queue->rear = temp;
    queue->count++;
}

int isqueueempty(Queue *queue){
    if (queue->rear == NULL){
    	return 1;
	} 
	else
		return 0;
}

void dequeue(Queue *queue){
    Node *temp;
    User out = queue->front->data;
    
    temp = queue->front;
    queue->front = queue->front->next;
    queue->count--;
    
    free(temp);
}

int partition(User data[], int low, int high) {
  
	int j;
	int pivot = data[high].jumlah;
	
		
	int i = (low - 1);

	for (j = low; j < high; j++) {
		if (data[j].jumlah >= pivot) {
    		i++;
    		sswap(data[i].time, data[j].time);
    		sswap(data[i].rekening, data[j].rekening);
    		sswap(data[i].jenisTransaksi, data[j].jenisTransaksi);
    		sswap(data[i].noTelp, data[j].noTelp);
    		
    		swap(&data[i].saldoawal, &data[j].saldoawal);
			swap(&data[i].jumlah, &data[j].jumlah);
			swap(&data[i].saldo, &data[j].saldo);
		}
	}
	sswap(data[i + 1].time, data[high].time);
	sswap(data[i + 1].time, data[high].time);
	sswap(data[i + 1].rekening, data[high].rekening);
    sswap(data[i + 1].jenisTransaksi, data[high].jenisTransaksi);
    sswap(data[i + 1].noTelp, data[high].noTelp);
    		
    swap(&data[i + 1].saldoawal, &data[high].saldoawal);
	swap(&data[i + 1].jumlah, &data[high].jumlah);
	swap(&data[i + 1].saldo, &data[high].saldo);
	
	return (i + 1);
}

void quickSort(User data[], int low, int high) {
	if (low < high) {
    
    	int pivotIndex = partition(data, low, high);
    	
    	#pragma omp task
    	{
    		quickSort(data, low, pivotIndex - 1);
		}
		#pragma omp task
		{
    		quickSort(data, pivotIndex + 1, high);
		}
	}
}

int spartition(User data[], int low, int high, char type) {
  
	int j;
	char pivot[100];
	
	if (type == 't'){
		strcpy(pivot, data[high].time);
	  
		int i = (low - 1);
	
		for (j = low; j < high; j++) {
			if (strcmp(data[j].time, pivot) <= 0) {
	    		i++;
	    		sswap(data[i].time, data[j].time);
	    		sswap(data[i].rekening, data[j].rekening);
	    		sswap(data[i].jenisTransaksi, data[j].jenisTransaksi);
	    		sswap(data[i].noTelp, data[j].noTelp);
	    		
	    		swap(&data[i].saldoawal, &data[j].saldoawal);
				swap(&data[i].jumlah, &data[j].jumlah);
				swap(&data[i].saldo, &data[j].saldo);
			}
		}
		sswap(data[i + 1].time, data[high].time);
		sswap(data[i + 1].time, data[high].time);
		sswap(data[i + 1].rekening, data[high].rekening);
	    sswap(data[i + 1].jenisTransaksi, data[high].jenisTransaksi);
	    sswap(data[i + 1].noTelp, data[high].noTelp);
	    		
	    swap(&data[i + 1].saldoawal, &data[high].saldoawal);
		swap(&data[i + 1].jumlah, &data[high].jumlah);
		swap(&data[i + 1].saldo, &data[high].saldo);
		
		return (i + 1);
	}

	if (type == 'k'){
		strcpy(pivot, data[high].time);
	  
		int i = (low - 1);
	
		for (j = low; j < high; j++) {
			if (strcmp(data[j].jenisTransaksi, pivot) <= 0) {
	    		i++;
	    		sswap(data[i].time, data[j].time);
	    		sswap(data[i].rekening, data[j].rekening);
	    		sswap(data[i].jenisTransaksi, data[j].jenisTransaksi);
	    		sswap(data[i].noTelp, data[j].noTelp);
	    		
	    		swap(&data[i].saldoawal, &data[j].saldoawal);
				swap(&data[i].jumlah, &data[j].jumlah);
				swap(&data[i].saldo, &data[j].saldo);
			}
		}
		sswap(data[i + 1].time, data[high].time);
		sswap(data[i + 1].time, data[high].time);
		sswap(data[i + 1].rekening, data[high].rekening);
	    sswap(data[i + 1].jenisTransaksi, data[high].jenisTransaksi);
	    sswap(data[i + 1].noTelp, data[high].noTelp);
	    		
	    swap(&data[i + 1].saldoawal, &data[high].saldoawal);
		swap(&data[i + 1].jumlah, &data[high].jumlah);
		swap(&data[i + 1].saldo, &data[high].saldo);
		
		return (i + 1);
	}
}

void squickSort(User data[], int low, int high, char type) {
	if (low < high) {
    
    	int pivotIndex = spartition(data, low, high, type);
    	
    	#pragma omp task
    	{
    		squickSort(data, low, pivotIndex - 1, type);
		}
		#pragma omp task
		{
    		squickSort(data, pivotIndex + 1, high, type);
		}
	}
}

void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

void sswap(char *str1, char *str2) {
	char *temp = (char *)malloc((strlen(str1) + 1) * sizeof(char));
	
	strcpy(temp, str1);
	strcpy(str1, str2);
	strcpy(str2, temp);
	free(temp);
}

void transfer(User data){
	int i = 0, j, k, jumlah;
	char rekening[100] 	= {'\0'};
	char source[100];
	char pinCheck[100] 	= {'\0'};
	
	int check = FALSE;
	
	User target;
	User listUser[LIMIT] = {'\0', '\0'};
	
	FILE *file;
	
	file = fopen("userdata.txt", "r");
	
	while (fscanf(file, " %[^\n]", listUser[i].nama) != -1){
		fscanf(file, " %[^\n]", listUser[i].username);
		fscanf(file, " %[^\n]", listUser[i].password);
		i++;
	}
	
	fclose(file);
	
	for (j = 0; j <= i; j++){
		strcpy(source, ".//");
		strcat(source, listUser[j].username);
		strcat(source, "//");
		strcat(source, listUser[j].username);
		strcat(source, "_data.txt");
		
		file = fopen(source, "r");
		while (fscanf(file, " %[^\n]", listUser[j].nama) != -1){
			fscanf(file, " %[^\n]", listUser[j].rekening);
			fscanf(file, " %[^\n]", listUser[j].pin);
			fscanf(file, " %d", &listUser[j].saldo);
			//k++;
		}
		memset(source, 0, strlen(source));	
		
		fclose(file);
	}
	
	while (TRUE){
		printf("Masukkan Rekening Tujuan : ");
		scanf(" %[0123456789]s", rekening);
		
		for (j = 0; j <= i; j++){
			if (strcmp(listUser[j].rekening, rekening) == 0){
				check = TRUE;
				break;
			}
		}
		if (check){
			strcpy(target.nama, listUser[j].nama);
			strcpy(target.rekening, listUser[j].rekening);
			strcpy(target.username, listUser[j].username);
			strcpy(target.pin, listUser[j].pin);
			target.saldo = listUser[j].saldo;
			break;
		}
			
		else{
			printf("Rekening Tujuan Tidak Ada\n");
			Sleep(1000);
			system("cls");
		}	
	}
	printf("Rekening Ditemukan\n");
	printf("Nama\t\t: %s\n", target.nama);
	printf("No.Rekening\t: %s\n", target.rekening);
	
	printf("Masukkan Nominal Transfer: ");
	scanf("%d", &jumlah);
	
	while (jumlah > data.saldo){
		printf("Saldo Anda tidak mencukupi, masukkan ulang nomimal transfer\n");
		printf("Masukkan Nominal Transfer: ");
		scanf("%d", &jumlah);
	}
	
	int masuk = jumlah * -1;
	
	k = 0;
	while (k < 3){
		printf("Masukkan PIN\t: ");
		scanf(" %[0123456789]s", pinCheck);
		if (!strcmp(pinCheck, data.pin)){
			changeData(target, masuk);
			printf("Sedang memproses...\n\n");
			Sleep(1000);
			changeData(data, jumlah);
			system("cls");
			printf("\nTransaksi Berhasil!\n");
			Sleep(1000);
			resiTransfer(jumlah, data, target);
			
			printf("\n\nPress any key to continue...");
			getch();
			break;
		}
		else{
			printf("PIN salah, silahkan masukkan PIN yang benar\n");
			k++;
		}
		
	}if (k == 3){
		printf("PIN Anda telah diblokir\n");
		Sleep(1000);
		login();
	}
}

void changeData(User target, int jumlah){
	FILE *file;
	char source[100];
	char sourceTemp[100];
	
	strcpy(source, ".//");
	strcat(source, target.username);
	strcat(source, "//");
	
	strcpy(sourceTemp, source); //buat path ke file temp
	strcat(sourceTemp, target.username);
	strcat(sourceTemp, "_dataTemp.txt");
	
	strcat(source, target.username);
	strcat(source, "_data.txt");
	
	file = fopen(source, "w");
	fprintf(file, "%s\n", target.nama);
	fprintf(file, "%s\n", target.rekening);
	fprintf(file, "%s\n", target.pin);
	fprintf(file, "%d\n", (target.saldo - jumlah));
	fclose(file);
}

void topup(User listUser, char menu){
	emoney topup;	
	menu_topup:
	printf("E-money: \n");
	printf("G. Gopay\n");
	printf("O. Ovo\n");
	printf("Pilihan Anda: ");
	scanf("%s", &menu);
		
	if(toupper(menu) == 'G'){
        printf("Masukkan Nomor Telepon: ");
        scanf(" %[0123456789]s", topup.noTelp);
        printf("Masukkan Nominal Uang: ");
        scanf("%d", &topup.nominal);
        if(listUser.saldo > topup.nominal){
        	printf("Apakah Anda Yakin Untuk Top Up Gopay Sebesar %d dengan Nomor %s?\n", topup.nominal, topup.noTelp);
        	printf("Y. YES\n");
        	printf("N. NO\n");
        	printf("Pilihan Anda: ");
			scanf("%s", &menu);
        	if(menu == 'Y'){
				printf("Sedang memproses...\n\n");
				Sleep(1000);
				changeData(listUser, topup.nominal);
				system("cls");
        		printf("Transaksi Top Up Gopay Berhasil...\n");
        		Sleep(1000);
        		resiTopup(listUser, topup, topup.nominal, 'G');
        		printf("\nPress any key to continue...\n");
        		getch();
			} else if(menu == 'N'){
				system("cls");
				goto menu_topup;
			} else{
				printf("Pilihan tidak ada");
				Sleep(1000);
				system("cls");
				goto menu_topup;
			}	
		}
		if(listUser.saldo < topup.nominal){
			printf("Maaf Saldo Anda Tidak Cukup...\n");
			Sleep(1000);
			printf("Silakan Melakukan Pengisian Saldo\n");
			system("pause");
		}
	}
	if(toupper(menu) == 'O'){
		printf("Masukkan Nomor Telepon: ");
        scanf(" %[0123456789]s", topup.noTelp);
        printf("Masukkan Nominal Uang: ");
        scanf("%d", &topup.nominal);
        if(listUser.saldo > topup.nominal){
        	printf("Apakah Anda Yakin Untuk Top Up Sebesar %d dengan Nomer %s?\n", topup.nominal, topup.noTelp);
        	printf("Y. YES\n");
        	printf("N. NO\n");
        	printf("Pilihan Anda: ");
			scanf("%s", &menu);
        	if(menu == 'Y'){
				printf("Sedang memproses...\n\n");
				Sleep(1000);
				system("cls");
        		printf("Transaksi Top Up OVO Berhasil...\n");
        		changeData(listUser, topup.nominal);
        		Sleep(1000);
        		resiTopup(listUser, topup, topup.nominal, 'O');
        		printf("\nPress any key to continue...\n");
        		getch();
        		
			} else if(menu == 'N'){
				system("cls");
				goto menu_topup;
			} else{
				printf("Pilihan tidak ada");
				Sleep(1000);
				system("cls");
				goto menu_topup;
			}	
		}
		if(listUser.saldo < topup.nominal){
			printf("Maaf Saldo Anda Tidak Cukup...\n");
			Sleep(1000);
			printf("Silakan Melakukan Pengisian Saldo\n");
			system("pause");
		}
	}
}

void pembayaran(User listUser, char menu){
	bayar pembayaran;
	int pilihan;
	float kwhIndex;
	
	printf("P. PLN\n");
	printf("Q. PDAM\n");
	printf("Pilihan Anda: ");
	scanf("%s", &menu);
	
	if(toupper(menu) == 'P'){
		printf("900 VA\n");
		printf("1300 VA\n");
		printf("2200 VA\n");
		printf("3500 VA s.d 5500 VA\n");
		printf("6600 VA\n");
		printf("Pilihan Anda (Nasukkan nilai daya): ");
		scanf("%d", &pembayaran.daya);
		
		switch(pembayaran.daya){
			case 900:
				kwhIndex = 1352;
				break;
				
			case 1300:
				kwhIndex = 1444.70;
				break;
				
			case 2200:
				kwhIndex = 1444.70;
				break;
				
			case 3500 ... 5500:
				kwhIndex = 1444.70;
				break;
			
			case 6600:
				kwhIndex = 1444.70;
				break;
			
			default:
				printf("Pilihan Tidak Ada");
				break;
		}
		
		printf("Harga Per KWH: Rp %f\n", kwhIndex);
		printf("Masukkan Penggunaan KWH Anda: ");
		scanf("%d", &pembayaran.kwh);
		pembayaran.biaya = pembayaran.kwh * kwhIndex;
			
		printf("Total Biaya dengan Daya %d VA dan KWH %d: %d\n", pembayaran.daya, pembayaran.kwh, pembayaran.biaya);
		system("pause");
		if(listUser.saldo > pembayaran.biaya){	
			printf("Lanjutkan Pembayaran..?:\n");
        	printf("Y. YES\n");
        	printf("N. NO\n");
        	printf("Pilihan Anda: ");
			scanf("%s", &menu);
				
        	if(toupper(menu) == 'Y'){
        			printf("Sedang memproses...\n\n");
					Sleep(1000);
					changeData(listUser, pembayaran.biaya);
					system("cls");
        			printf("Pembayaran PLN Daya %d Berhasil...\n", pembayaran.daya);
        			Sleep(1000);
        			resiBayarPLN(listUser, pembayaran, pembayaran.biaya);
        			printf("\nPress any key to continue...\n");
        			getch();
        			return;
        			
			} else if(toupper(menu) == 'N'){
					system("cls");
					return;
			} else{
					printf("Pilihan tidak ada");
					Sleep(1000);
					system("cls");
				}
			}
			if(listUser.saldo < pembayaran.biaya){
				printf("Maaf Saldo Anda Tidak Cukup...\n");
				Sleep(1000);
				printf("Silakan Melakukan Pengisian Saldo\n");
				system("pause");
			}
		} 
	if(toupper(menu) == 'Q'){
		printf("Kategori Pelanggam: \n");
		printf("1. Rumah Tangga I\n");
		printf("2. Rumah Tangga II\n");
		printf("3. Rumah Tangga III\n");
		printf("4. Rumah Tangga IV\n");
		printf("5. Rumah Tangga V\n");
		printf("6. Keterangan Kategori\n");
		printf("Pilihan Anda: ");
		scanf("%d", &pilihan);
		if(pilihan == 1){
			strcpy(pembayaran.kategori, "Rumah Tangga I");
			printf("Kategori Pelanggan: Rumah Tangga I\n");
			printf("Masukkan Besar Pemakaian Air (M3) : ");
			scanf("%d", &pembayaran.air);
			if(pembayaran.air >= 1 && pembayaran.air <= 10){
				pembayaran.biaya = pembayaran.air * 1550;
			} else if(pembayaran.air >= 11 && pembayaran.air <= 20){
				pembayaran.biaya = pembayaran.air * 1950;
			} else if(pembayaran.air >= 21 && pembayaran.air <= 30){
				pembayaran.biaya = pembayaran.air * 3650;
			} else if(pembayaran.air >= 31){
				pembayaran.biaya = pembayaran.air * 4500;
			}
			printf("Total Pembayaran PDAM Kategori Rumah Tangga I: %d\n", pembayaran.biaya);
			system("pause");
			if(listUser.saldo > pembayaran.biaya){
				printf("Lanjutkan Pembayaran..?:\n");
        		printf("Y. YES\n");
        		printf("N. NO\n");
        		printf("Pilihan Anda: ");
				scanf("%s", &menu);
        		if(toupper(menu) == 'Y'){
        			printf("Sedang memproses...\n\n");
					Sleep(1000);
					changeData(listUser, pembayaran.biaya);
					system("cls");
        			printf("Pembayaran PDAM Berhasil...\n");
        			Sleep(1000);
        			resiBayarPDAM(listUser, pembayaran, pembayaran.biaya, pembayaran.kategori);
        			printf("\nPress any key to continue...\n");
        			getch();
				} else if(toupper(menu) == 'N'){
					system("cls");
				} else{
					printf("Pilihan tidak ada");
					Sleep(1000);
					system("cls");
				}
			}
			if(listUser.saldo < pembayaran.biaya){
				printf("Maaf Saldo Anda Tidak Cukup...\n");
				Sleep(1000);
				printf("Silakan Melakukan Pengisian Saldo\n");
				system("pause");
			}
		} else if(pilihan == 2){
			strcpy(pembayaran.kategori, "Rumah Tangga II");
			printf("Kategori Pelanggan: Rumah Tangga II\n");
			printf("Masukkan Besar Pemakaian Air (M3) : ");
			scanf("%d", &pembayaran.air);
			if(pembayaran.air >= 1 && pembayaran.air <= 10){
				pembayaran.biaya = pembayaran.air * 2170;
			} else if(pembayaran.air >= 11 && pembayaran.air <= 20){
				pembayaran.biaya = pembayaran.air * 2950;
			} else if(pembayaran.air >= 21 && pembayaran.air <= 30){
				pembayaran.biaya = pembayaran.air * 4150;
			} else if(pembayaran.air >= 31){
				pembayaran.biaya = pembayaran.air * 5250;
			}
			printf("Total Biaya Pembayaran PDAM Kategori Rumah Tangga II: %d\n", pembayaran.biaya);
			system("pause");
			if(listUser.saldo > pembayaran.biaya){
				printf("Lanjutkan Pembayaran..?:\n");
        		printf("Y. YES\n");
        		printf("N. NO\n");
        		printf("Pilihan Anda: ");
				scanf("%s", &menu);
        		if(toupper(menu) == 'Y'){
        			printf("Sedang memproses...\n\n");
					Sleep(1000);
					changeData(listUser, pembayaran.biaya);
					system("cls");
        			printf("Pembayaran PDAM Berhasil...\n");
        			Sleep(1000);
        			resiBayarPDAM(listUser, pembayaran, pembayaran.biaya, pembayaran.kategori);
        			printf("\nPress any key to continue...\n");
        			getch();
				} else if(toupper(menu) == 'N'){
					system("cls");
				} else{
					printf("Pilihan tidak ada");
					Sleep(1000);
					system("cls");
				}
			}
			if(listUser.saldo < pembayaran.biaya){
				printf("Maaf Saldo Anda Tidak Cukup...\n");
				Sleep(1000);
				printf("Silakan Melakukan Pengisian Saldo\n");
				system("pause");
			}
			printf("");
		} else if(pilihan == 3){
			strcpy(pembayaran.kategori, "Rumah Tangga III");
			printf("Kategori Pelanggan: Rumah Tangga III\n");
			printf("Masukkan Besar Pemakaian Air (M3)");
			scanf("%d", &pembayaran.air);
			if(pembayaran.air >= 1 && pembayaran.air <= 10){
				pembayaran.biaya = pembayaran.air * 3000;
			} else if(pembayaran.air >= 11 && pembayaran.air <= 20){
				pembayaran.biaya = pembayaran.air * 4000;
			} else if(pembayaran.air >= 21 && pembayaran.air <= 30){
				pembayaran.biaya = pembayaran.air * 5000;
			} else if(pembayaran.air >= 31){
				pembayaran.biaya = pembayaran.air * 6500;
			}
			printf("Total Biaya Pembayaran PDAM Kategori Rumah Tangga III: %d\n", pembayaran.biaya);
			system("pause");
			if(listUser.saldo > pembayaran.biaya){
				printf("Lanjutkan Pembayaran..?:\n");
        		printf("Y. YES\n");
        		printf("N. NO\n");
        		printf("Pilihan Anda: ");
				scanf("%s", &menu);
        		if(toupper(menu) == 'Y'){
        			printf("Sedang memproses...\n\n");
					Sleep(1000);
					changeData(listUser, pembayaran.biaya);
					system("cls");
        			printf("Pembayaran PDAM Berhasil...\n");
        			Sleep(1000);
        			resiBayarPDAM(listUser, pembayaran, pembayaran.biaya, pembayaran.kategori);
        			printf("\nPress any key to continue...\n");
        			getch();
				} else if(toupper(menu) == 'N'){
					system("cls");
				} else{
					printf("Pilihan tidak ada");
					Sleep(1000);
					system("cls");
				}
			}
			if(listUser.saldo < pembayaran.biaya){
				printf("Maaf Saldo Anda Tidak Cukup...\n");
				Sleep(1000);
				printf("Silakan Melakukan Pengisian Saldo\n");
				system("pause");
			}
		}else if(pilihan == 4){
			strcpy(pembayaran.kategori, "Rumah Tangga IV");
			printf("Kategori Pelanggan: Rumah Tangga IV\n");
			printf("Masukkan Besar Pemakaian Air (M3)");
			scanf("%d", &pembayaran.air);
			if(pembayaran.air >= 1 && pembayaran.air <= 10){
				pembayaran.biaya = pembayaran.air * 4000;
			} else if(pembayaran.air >= 11 && pembayaran.air <= 20){
				pembayaran.biaya = pembayaran.air * 5000;
			} else if(pembayaran.air >= 21 && pembayaran.air <= 30){
				pembayaran.biaya = pembayaran.air * 6000;
			} else if(pembayaran.air >= 31){
				pembayaran.biaya = pembayaran.air * 7000;
			}
			printf("Total Biaya Pembayaran PDAM Kategori Rumah Tangga IV: %d\n", pembayaran.biaya);
			system("pause");
			if(listUser.saldo > pembayaran.biaya){
				printf("Lanjutkan Pembayaran..?:\n");
        		printf("Y. YES\n");
        		printf("N. NO\n");
        		printf("Pilihan Anda: ");
				scanf("%s", &menu);
        		if(toupper(menu) == 'Y'){
        			printf("Sedang memproses...\n\n");
					Sleep(1000);
					changeData(listUser, pembayaran.biaya);
					system("cls");
        			printf("Pembayaran PDAM Berhasil...\n");
        			Sleep(1000);
        			resiBayarPDAM(listUser, pembayaran, pembayaran.biaya, pembayaran.kategori);
        			printf("\nPress any key to continue...\n");
        			getch();
				} else if(toupper(menu) == 'N'){
					system("cls");
				} else{
					printf("Pilihan tidak ada");
					Sleep(1000);
					system("cls");
				}
			}
			if(listUser.saldo < pembayaran.biaya){
				printf("Maaf Saldo Anda Tidak Cukup...\n");
				Sleep(1000);
				printf("Silakan Melakukan Pengisian Saldo\n");
				system("pause");
			}
		} else if(pilihan == 5){
			strcpy(pembayaran.kategori, "Rumah Tangga V");
			printf("Kategori Pelanggan: Rumah Tangga V\n");
			printf("Masukkan Besar Pemakaian Air (M3)");
			scanf("%d", &pembayaran.air);
			if(pembayaran.air >= 1 && pembayaran.air <= 10){
				pembayaran.biaya = pembayaran.air * 5000;
			} else if(pembayaran.air >= 11 && pembayaran.air <= 20){
				pembayaran.biaya = pembayaran.air * 6000;
			} else if(pembayaran.air >= 21 && pembayaran.air <= 30){
				pembayaran.biaya = pembayaran.air * 7500;
			} else if(pembayaran.air >= 31){
				pembayaran.biaya = pembayaran.air * 8500;
			}
			printf("Total Biaya Pembayaran PDAM Kategori Rumah Tangga V: %d\n", pembayaran.biaya);
			system("pause");
			if(listUser.saldo > pembayaran.biaya){
				printf("Lanjutkan Pembayaran..?:\n");
        		printf("Y. YES\n");
        		printf("N. NO\n");
        		printf("Pilihan Anda: ");
				scanf("%s", &menu);
        		if(toupper(menu) == 'Y'){
        			printf("Sedang memproses...\n\n");
					Sleep(1000);
					changeData(listUser, pembayaran.biaya);
					system("cls");
        			printf("Pembayaran PDAM Berhasil...\n");
        			Sleep(1000);
        			resiBayarPDAM(listUser, pembayaran, pembayaran.biaya, pembayaran.kategori);
        			printf("\nPress any key to continue...\n");
        			getch();
				} else if(toupper(menu) == 'N'){
					system("cls");
				} else{
					printf("Pilihan tidak ada");
					Sleep(1000);
					system("cls");
				}
			}
			if(listUser.saldo < pembayaran.biaya){
				printf("Maaf Saldo Anda Tidak Cukup...\n");
				Sleep(1000);
				printf("Silakan Melakukan Pengisian Saldo\n");
				system("pause");
			}
		} else if(pilihan == 6){
			printf("1. Rumah Tangga I\n");
			printf("-Tingkat Konsumsi Air (M3): 1 - 10\n");
			printf("Tarif (Rp): 1.550\n");
			printf("-Tingkat Konsumsi Air (M3): 11 - 20\n");
			printf("Tarif (Rp): 1.950\n");
			printf("-Tingkat Konsumsi Air (M3): 21 - 30\n");
			printf("Tarif (Rp): 3.650\n");
			printf("-Tingkat Konsumsi Air (M3): > 31\n");
			printf("Tarif (Rp): 4.500\n");
			printf("\n");
			printf("2. Rumah Tangga II\n");
			printf("-Tingkat Konsumsi Air (M3): 1 - 10\n");
			printf("Tarif (Rp): 2.170\n");
			printf("-Tingkat Konsumsi Air (M3): 11 - 20\n");
			printf("Tarif (Rp): 2.950\n");
			printf("-Tingkat Konsumsi Air (M3): 21 - 30\n");
			printf("Tarif (Rp): 4.150\n");
			printf("-Tingkat Konsumsi Air (M3): > 31\n");
			printf("Tarif (Rp): 5.250\n");
			printf("\n");
			printf("3. Rumah Tangga III\n");
			printf("-Tingkat Konsumsi Air (M3): 1 - 10\n");
			printf("Tarif (Rp): 3.000\n");
			printf("-Tingkat Konsumsi Air (M3): 11 - 20\n");
			printf("Tarif (Rp): 4.000\n");
			printf("-Tingkat Konsumsi Air (M3): 21 - 30\n");
			printf("Tarif (Rp): 5.000\n");
			printf("-Tingkat Konsumsi Air (M3): > 31\n");
			printf("Tarif (Rp): 6.500\n");
			printf("\n");
			printf("4. Rumah Tangga IV\n");
			printf("-Tingkat Konsumsi Air (M3): 1 - 10\n");
			printf("Tarif (Rp): 4.000\n");
			printf("-Tingkat Konsumsi Air (M3): 11 - 20\n");
			printf("Tarif (Rp): 5.000\n");
			printf("-Tingkat Konsumsi Air (M3): 21 - 30\n");
			printf("Tarif (Rp): 6.000\n");
			printf("-Tingkat Konsumsi Air (M3): > 31\n");
			printf("Tarif (Rp): 7.000\n");
			printf("\n");
			printf("5. Rumah Tangga V\n");
			printf("-Tingkat Konsumsi Air (M3): 1 - 10\n");
			printf("Tarif (Rp): 5.000\n");
			printf("-Tingkat Konsumsi Air (M3): 11 - 20\n");
			printf("Tarif (Rp): 6.000\n");
			printf("-Tingkat Konsumsi Air (M3): 21 - 30\n");
			printf("Tarif (Rp): 7.500\n");
			printf("-Tingkat Konsumsi Air (M3): > 31\n");
			printf("Tarif (Rp): 8.500\n");
			printf("\n");
			system("pause");
			system("cls");
		} else{
			printf("Pilihan tidak ada");
			Sleep(1000);
			system("cls");
		}
	}
}

void gantiPin(User data){
	FILE *file;
	
	char filename[100] = {'\0'};
	strcpy(filename, ".//");
	strcat(filename, data.username);
	strcat(filename, "//");
	strcat(filename, data.username);
	strcat(filename, "_data.txt");
	
	file = fopen(filename, "a+");
	
	char checkPin[100] = {'\0'};
	char newPin[100] = {'\0'};
	char temp[100] = {'\0'};
	
	int check;
	
	while (fscanf(file, " %[^\n]", data.nama) != -1){
		fscanf(file, " %[^\n]", data.rekening);
		fscanf(file, " %[^\n]", data.pin);
		fscanf(file, " %d", &data.saldo);
	}
	
	fclose(file);
	
	while(TRUE){
		printf("Masukkan PIN lama Anda : ");
		scanf(" %[0123456789]s", checkPin);
	
		if (!strcmp(checkPin, data.pin)){
			break;
		}
		else{
			printf("PIN salah, silahkan masukkan PIN yang benar\n");
		}
	}
	 while (TRUE){
	 	system("cls");
		printf("Masukkan PIN ATM (6 digit angka): ");
		scanf(" %[0123456789]", newPin);
				
		printf("Konfirmasi PIN: ");
		scanf(" %[0123456789]", temp);
				
			if(((newPin)[5] == '\0' || (newPin)[6] != '\0') || strcmp(newPin, temp)) {
				printf("PIN tidak valid atau PIN salah\n");
				memset(newPin, 0, strlen(newPin));
				memset(temp, 0, strlen(temp));
			}
			else{
				file = fopen(filename, "w");
				
				fprintf(file, "%s\n", data.nama);
				fprintf(file, "%s\n", data.rekening);
				fprintf(file, "%s\n", newPin);
				fprintf(file, "%d\n", data.saldo);
				
				fclose(file);
				break;
			}	
		}
		
		printf("PIN berhasil diganti.\n");
		Sleep(1000);
}

void resiTransfer(int jumlah, User data, User target){
	
	char timeTransfer[100] = {'\0'};
	char *filename = (char*) malloc(LIMIT);
	char hour[100] = {'\0'};
	
	time_t rawtime;
	struct tm *info;
	time(&rawtime);
	info = localtime(&rawtime);
	strcpy(timeTransfer, asctime(info));
	
	sprintf(hour, "%02d;%02d;%02d", info->tm_hour, info->tm_min, info->tm_sec);
	
	FILE *file;
	
			printf("\nWaktu transaksi : %s\n", timeTransfer);
			printf("PENERIMA:\n");
			printf("%s - %s\n", target.nama, target.rekening);
			
			printf("\nDetail: \n");
			printf("\n======================\n\n");
			printf("Saldo Awal\t: %d\n", data.saldo);
			printf("Total Transaksi\t: %d\n", jumlah);
			printf("Sisa Saldo\t: %d", (data.saldo - jumlah));
			
			strcpy(filename, ".//");
			strcat(filename, data.username);
			strcat(filename, "//");
			//strcat(filename, timeTransfer);
			strcat(filename, "transfer-");
			strcat(filename, target.username);
			strcat(filename, "-");
			strcat(filename, target.rekening);
			strcat(filename, "-");
			strcat(filename, hour);
			strcat(filename, ".txt");
			
			file = fopen(filename, "w");
			if (file == NULL)
				printf("\nFailed to open %s\n", filename);
			
			fprintf(file ,"%s", timeTransfer);
			fprintf(file ,"%s\n", target.nama);
			fprintf(file ,"%s\n", target.rekening);
			fprintf(file ,"%d\n", data.saldo);
			fprintf(file ,"%d\n", jumlah);
			fprintf(file ,"%d\n", (data.saldo - jumlah));
			
			fclose(file);	
}

void resiTopup(User data, emoney topup, int jumlah, char type){
	
	char timeTransfer[100] = {'\0'};
	char *filename = (char*) malloc(LIMIT);
	char hour[100] = {'\0'};
	
	time_t rawtime;
	struct tm *info;
	time(&rawtime);
	info = localtime(&rawtime);
	strcpy(timeTransfer, asctime(info));
	
	sprintf(hour, "%02d;%02d;%02d", info->tm_hour, info->tm_min, info->tm_sec);
	
	FILE *file;
	
	char *namaJasa = (char*) malloc(LIMIT);
	if (type == 'G')
		strcpy(namaJasa, "GoPay");
	if (type == 'O')
		strcpy(namaJasa, "OVO");
	
	printf("\nWaktu transaksi : %s\n", timeTransfer);
	printf("NAMA E-MONEY:\n");
	printf("%s - %s\n", namaJasa, topup.noTelp);
			
	printf("\nDetail: \n");
	printf("\n======================\n\n");
	printf("Saldo Awal\t: %d\n", data.saldo);
	printf("Total Transaksi\t: %d\n", jumlah);
	printf("Sisa Saldo\t: %d", (data.saldo - jumlah));
		
	strcpy(filename, ".//");
	strcat(filename, data.username);
	strcat(filename, "//");
	strcat(filename, "topup-");
	strcat(filename, namaJasa);
	strcat(filename, "-");
	strcat(filename, topup.noTelp);
	strcat(filename, "-");
	strcat(filename, hour);
	strcat(filename, ".txt");
			
	file = fopen(filename, "w");
	if (file == NULL)
		printf("\nFailed to open %s\n", filename);
			
	fprintf(file ,"%s", timeTransfer);
	fprintf(file ,"%s\n", namaJasa);
	fprintf(file ,"%s\n", topup.noTelp);
	fprintf(file ,"%d\n", data.saldo);
	fprintf(file ,"%d\n", jumlah);
	fprintf(file ,"%d\n", (data.saldo - jumlah));
			
	fclose(file);	
}

void resiBayarPDAM(User data, bayar pembayaran, int biaya, char kategori[100]){
	
	char timeTransfer[100] = {'\0'};
	char *filename = (char*) malloc(LIMIT);
	char hour[100] = {'\0'};
	char bayarAir[100];
	
	sprintf(bayarAir,"%d" ,pembayaran.air);
	
	time_t rawtime;
	struct tm *info;
	time(&rawtime);
	info = localtime(&rawtime);
	strcpy(timeTransfer, asctime(info));
	
	sprintf(hour, "%02d;%02d;%02d", info->tm_hour, info->tm_min, info->tm_sec);
	
	FILE *file;
	
	printf("\nWaktu transaksi : %s\n", timeTransfer);
	printf("JENIS PEMBAYARAN:\n");
	printf("PDAM - %s\n", kategori);
	printf("\nDetail: \n");
	printf("\n======================\n\n");
	printf("Saldo Awal\t: %d\n", data.saldo);
	printf("Total Transaksi\t: %d\n", biaya);
	printf("Sisa Saldo\t: %d", (data.saldo - biaya));
		
	strcpy(filename, ".//");
	strcat(filename, data.username);
	strcat(filename, "//");
	strcat(filename, "pembayaran-");
	strcat(filename, "PDAM-");
	strcat(filename, bayarAir);
	strcat(filename, "-");
	strcat(filename, hour);
	strcat(filename, ".txt");
			
	file = fopen(filename, "w");
	if (file == NULL)
		printf("\nFailed to open %s\n", filename);
			
	fprintf(file ,"%s", timeTransfer);
	fprintf(file ,"PDAM\n");
	fprintf(file, "%s\n", kategori);
	fprintf(file ,"%d\n", data.saldo);
	fprintf(file ,"%d\n", biaya);
	fprintf(file ,"%d\n", (data.saldo - biaya));
			
	fclose(file);	
}

void resiBayarPLN(User data, bayar pembayaran, int biaya){
	
	char timeTransfer[100] = {'\0'};
	char *filename = (char*) malloc(LIMIT);
	char hour[100] = {'\0'};
	char skwh[100];
	
	time_t rawtime;
	struct tm *info;
	time(&rawtime);
	info = localtime(&rawtime);
	strcpy(timeTransfer, asctime(info));
	
	sprintf(hour, "%02d;%02d;%02d", info->tm_hour, info->tm_min, info->tm_sec);
	
	FILE *file;
	
	printf("\nWaktu transaksi : %s\n", timeTransfer);
	printf("JENIS PEMBAYARAN:\n");
	printf("PLN - %d KWH\n", pembayaran.daya);
	printf("\nDetail: \n");
	printf("\n======================\n\n");
	printf("Saldo Awal\t: %d\n", data.saldo);
	printf("Total Transaksi\t: %d\n", biaya);
	printf("Sisa Saldo\t: %d", (data.saldo - biaya));
		
	sprintf(skwh, "%d", pembayaran.kwh);
	strcpy(filename, ".//");
	strcat(filename, data.username);
	strcat(filename, "//");
	strcat(filename, "pembayaran-");
	strcat(filename, "PLN");
	strcat(filename, "-");
	strcat(filename, skwh);
	strcat(filename, "-");
	strcat(filename, hour);
	strcat(filename, ".txt");
			
	file = fopen(filename, "w");
	if (file == NULL)
		printf("\nFailed to open %s\n", filename);
			
	fprintf(file ,"%s", timeTransfer);
	fprintf(file ,"PLN\n");
	fprintf(file ,"%d\n", pembayaran.kwh);
	fprintf(file ,"%d\n", data.saldo);
	fprintf(file ,"%d\n", biaya);
	fprintf(file ,"%d\n", (data.saldo - biaya));
			
	fclose(file);	
}
