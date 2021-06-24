/*Proyek Akhir Pemrograman Lanjut 2021 Semester Genap
  dibuat oleh:
  1. Miranty Anjani Putri <2006468270>
  2. Nisrina Alifah Sauda <2006577523>
  3. Salma Dewi Taufiqoh <2006577473>
  
  Pemrograman Lanjut - 02
  di bawah bimbingan
  Dr. Ruki Harwahyu, ST. MT. MSc.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>
#include<windows.h>
#include<conio.h>

//Password admin = admin

//Deklarasi variabel global
int n; //Banyak daerah yang ingin diinput
int sudahInput = 0, batas[2] = {0}; //Banyak daerah yang sudah diinput, Jumlah daerah yang ingin diinput
long int votePaslon[4] = {0}; //Jumlah voting untuk setiap kandidat, maksimal paslon 4 pasangan
int banyakKandidat = 0, jumlahPemilihEligible = 0, golput = 0; /*Jumlah paslon, jumlah pemilih eligible secara skala besar (akumulasi jumlah pemilih per daerah), jumlah suara tidak sah (golput)*/
char kandidatPaslon[4][30]; //Variabel menyimpan nama pasangan calon pemimpin atau kandidat

//Struct node untuk menerima inputan suara
struct node {
	long int suara;
	struct node *next;
};

/*Deklarasi struct data daerah yang menyimpan nama daerah, jumlah pemilih legal di daerah, jumlah suara golput,
 jumlah suara sah paslon 1, 2, 3, 4*/
struct dataDaerah {
	char namaDaerah[50];
	long int paslon1, paslon2, paslon3, paslon4;
	long int jmlhPemilihPerDaerah, golput;
};

//Declare struct daerah
struct dataDaerah *daerah;

//Function untuk membuat node (menerima suara paslon per-inputan user)
struct node* newNode (int data, struct node* nextNode) {
	struct node* node = (struct node*)malloc(sizeof(struct node));
	node->suara = data;
	
	node->next = nextNode;
	
	return node;
}

//Function untuk membuat linked list (suara inputan user)
struct node* constructList(long int keys[], int n) {
	int i;
	struct node* head = NULL, *node = NULL;
	
	for (i = n - 1; i >= 0; i--) {
		node = newNode(keys[i], node);
		head = node;
	}
	
	return head;
}

//Function untuk mencetak linked list (suara inputan user)
void printList(struct node* head) {
	struct node* ptr = head;
	while (ptr != NULL) {
		printf("%d -> ", ptr->suara);
		ptr = ptr->next;
	}
	printf("Selesai");
}


//Function Prototype
void menu(); //fungsi menu utama program
void menuAdmin(); //fungsi menu untuk admin
void passwordAdmin(); //fungsi 'perlindungan' akses untuk admin
void pemilihan(); //fungsi untuk menerima inputan data untuk daerah
void thanks(); //tampilan terima kasih setiap user selesai input data daerah
void sortDaerah(); //fungsi untuk mengurutkan daerah pada tabel berdasarkan A-Z
void printFile(); //fungsi untuk menyimpan data yang sudah diinput user ke file txt
void show(); //fungsi untuk menampilkan data pemenang (menu utama nomor 3)
void tabel(); //fungsi untuk menampilkan data inputan serta pilihan manipulasi data yang admin dapat lakukan
/*Manipulasinya pengurutan berdasarkan daerah (A-Z), ubah inputan data, hapus data, melihat data sebelumnya, dan menyimpan data*/
void descending();//function untuk melakukan sorting pemenang
void display();//function untuk menampilkan hasil inputan suara per daerah
void dataSebelum(); //function untuk melakukan pembacaan file yang berisi input sebelumnya
void ubahData();//function untuk mengubah data input
void hapusData(); // function untuk menghapus data input
void thankYou(); //function display terima kasih ketika user ingin keluar dari program

int main (){
	menu();
}

//Function ini dibuat oleh Miranty Anjani Putri
void menu() {
	system("CLS");
	printf("\n----------------------------------------------------------------------------\n");
    printf("                Selamat datang pada Program Penghitung Suara                    ");
	printf("\n----------------------------------------------------------------------------\n");
    printf("\n              1. Input User                                                   ");
    printf("\n              2. Admin                                                        ");
    printf("\n              3. Hasil Voting                                                 ");
    printf("\n              0. Exit														    ");
    printf("\n----------------------------------------------------------------------------\n"); 
	printf("\n\n\t\t Pilihan Anda: ");
	
	switch(getch()) {
	fflush(stdin);
	case '1':
		system("CLS");
		// call function input user
    	pemilihan();
   		break;
	case '2':
		system("CLS");
		// call function agar admin bisa log-in
		passwordAdmin();
		break;
	case '3':
    	system("CLS");
		// cal function hasil
		descending();
		break;
	case '0':
		system("CLS");
		//thankYou();
    thankYou();
		exit(0);
  	default: //Jika Input salah maka panggil kembali Modular menu untuk menerima input kembali
    printf("\n\aMasukkan Input yang benar !");
    printf("\nTekan (Enter) untuk memasukkan input kembali....");
    getch();
    system("CLS");
    menu();
  }
}

//function input user
void pemilihan () {
	long int votePerDaerah[4] = {0};
	long int sum = 0;
	int x = banyakKandidat;
	int i, j, k;
	
	fflush(stdin);
	if (batas[0] == batas[1]) {
		printf("\n---------------------------------------------------------\n");
		printf("\n	Sebanyak %d data daerah telah diinput	\n", n);
		printf("\n---------------------------------------------------------\n");
		printf("\nTekan Enter untuk kembali ke menu...");
		getch();
		system("CLS");
		menu();
	}
	
  //Input data suara berdasarkan daerah
	Input:
	system("CLS");
	printf("\n\t\t\t\t--------------------------------------\n");
	printf("\n\t\t			    DATA DAERAH					\n");
	printf("\n\t\t\t\t--------------------------------------\n");
	printf("\n");
	printf("\t\t\t\t	Masukkan nama daerah: \n");
	printf("\t\t\t\t	");
	scanf(" %[^\n]", (daerah+sudahInput)->namaDaerah);
	printf("\t\t\t\t	Masukkan jumlah pemilih eligible: \n");
	printf("\t\t\t\t	");
	scanf("%ld", &(daerah+sudahInput)->jmlhPemilihPerDaerah);
	
  //menampilkan kandidat calon yang telah diatur oleh Admin
	printf("\n\t\t\t\t	Berikut daftar pasangan calon pemimpin: \n");
	if (x == 1) {
		for (i = 0; i < x+1; i++) {
			printf("\n\t\t\t\t	%d. ", i+1);
			printf("%s", kandidatPaslon[i]);
		}
	}
	else {
		for (i = 0; i < x; i++) {
			printf("\n\t\t\t\t	%d. ", i+1);
			printf("%s", kandidatPaslon[i]);
		}
	}

	//meminta jumlah suara untuk masing-masing paslon
	for (j = 0; j < x; j++) {
		printf("\n\n\t\t\t\tJumlah suara sah paslon ke-%d : ", j+1);
		scanf("%d", &votePerDaerah[j]);votePaslon[i] += votePerDaerah[j];
	}
	
	for (i = 0; i < x; i++) {
		votePaslon[i] += votePerDaerah[i];
	}
	
  //implementasi multithreading untuk mempercepat proses penjumlahan pada array
	#pragma omp parallel for reduction(+:sum)
	for (k = 0; k < 4; k++) {
		sum += votePerDaerah[k];
	}
	
	if(banyakKandidat == 1 || banyakKandidat == 2) {
		struct node* head = constructList(votePerDaerah, 2);
			
		//Memasukkan data voting ke struct masing-masing daerah
		(daerah+sudahInput)->paslon1 = votePerDaerah[0];
		(daerah+sudahInput)->paslon2 = votePerDaerah[1];
		(daerah+sudahInput)->paslon3 = 0;
		(daerah+sudahInput)->paslon4 = 0;
		daerah[sudahInput].golput = daerah[sudahInput].jmlhPemilihPerDaerah - sum;
		printf("\nData yang anda masukkan : \n");
		printList(head);
	}
	else if(banyakKandidat == 3) {
		struct node* head = constructList(votePerDaerah, 3);
		
		(daerah+sudahInput)->paslon1 = votePerDaerah[0];
		(daerah+sudahInput)->paslon2 = votePerDaerah[1];
		(daerah+sudahInput)->paslon3 = votePerDaerah[2];
		(daerah+sudahInput)->paslon4 = 0;
		(daerah+sudahInput)->golput = ((daerah+sudahInput)->jmlhPemilihPerDaerah - sum);
		printf("\nData yang anda masukkan : \n");
		printList(head);
	}
    else if(banyakKandidat == 4) {
    	struct node* head = constructList(votePerDaerah, 4);
		
		(daerah+sudahInput)->paslon1 = votePerDaerah[0];
		(daerah+sudahInput)->paslon2 = votePerDaerah[1];
		(daerah+sudahInput)->paslon3 = votePerDaerah[2];
		(daerah+sudahInput)->paslon4 = votePerDaerah[3];
		(daerah+sudahInput)->golput = (daerah+sudahInput)->jmlhPemilihPerDaerah - sum;
		printf("\nData yang anda masukkan : \n");
		printList(head);
	}
	
	printf("\n\nTekan (Enter) untuk melanjutkan....");
	getch();
    sudahInput++; //increment jumlah daerah yang sudah vote
    batas[0] = sudahInput;
    thanks();
}

void thanks() {
	system("CLS");
	printf("\n-------------------------------------------------------------\n");
	printf("\n		TERIMA KASIH TELAH MENGINPUT DATA\n");
	printf("\n-------------------------------------------------------------\n");
	printf("\n\n\nTekan (Enter) untuk voting selanjutnya.....");
    getch();
    menu();
}

void passwordAdmin() {
	char password[6] = {"admin"}; //password yang harus dimasukkan oleh admin
	char pass[15];
	int i, j = 0, k;
	int log = 0;
	
	Password:
		system("CLS");
		printf("\n----------------------------------------------------------\n");
		printf("\n		SILAHKAN MASUKKAN PASSWORD							\n");
		printf("\n----------------------------------------------------------\n");
		printf("\n\t	>>> Password: ");
		for (i = 0; i < 5; i++) {
			pass[i] = getch();
			printf("%c", '*');
		}
		pass[i] = '\0';
		
    //melakukan pengecekan input password Admin
		if((strcmp(pass, password)) != 0) {
			j++;
			printf("\nPassword yang Anda masukkan salah. Tekan enter untuk mengulang kembali.\n");
			if(j == 3) {
				printf("\nAnda telah salah memasukkan password sebanyak 3 kali! Mohon menunggu 5 detik!");
				printf("\a");
				for (k = 1; k <= 5; k++) {
					printf("\n%d", k);
					Sleep(1000);
				}
					passwordAdmin();
			}
				getch();
				goto Password;			
		}
		
		else {
			printf("\n\nLOGIN SUCCESFULL");
			for (k = 0; k < 6; k++) {
				printf(".");
				Sleep(200);
			}
				printf("\nTekan Enter!");
				getch();
				menuAdmin();
		}
}

// function admin
void menuAdmin() {
	int n, i;
	char kosong[] = {"Kubu kosong"};
	system("CLS");
	
	system("CLS");
	printf("\n-------------------------------------------------------------\n");
	printf("\n\t\t\t MENU ADMIN					\n");
	printf("\n-------------------------------------------------------------\n");
	printf("\n	1. Memasukkan batas daerah untuk diinput	                  \n");
	printf("\n	2. Menampilkan data voting				                        	\n");
	printf("\n	0. Kembali ke menu							                            \n");
  printf("\n\n  Note: Mohon isi batas daerah terlebih dahulu (pilihan 1).   ");
	printf("\n-------------------------------------------------------------\n");
	printf("\n\t\t\t>>>Pilihan Anda: ");
	
	switch(getch()) {
		fflush(stdin);
		case '1':
			printf("\n\nMasukkan BATAS DAERAH untuk data yang ingin diinput: ");
			scanf("%d", &n);
			batas[1] = n;
			if (batas[1] < batas[0]) {
        	//pengecekan batas daerah
				printf("\n\nJumlah daerah telah melebihi batas yang telah dimasukkan...");
				printf("\nTekan Enter untuk memasukkan input kembali...");
				getch();
				menuAdmin();
			}
			printf("\nMAKSIMAL input PASANGAN CALON adalah 4.\nMasukkan JUMLAH PASANGAN CALON: ");
			scanf("%d", &banyakKandidat);
      		//error handling pada input banyak kandidat
			if(banyakKandidat < 1 || banyakKandidat > 4) {
				printf("\nAnda hanya dapat memasukkan maksimal 4 pasangan calon..");
				printf("\nSilahkan masukkan angka 1-4: ");
				scanf("%d", &banyakKandidat);
			}
			printf("\n");
			if (banyakKandidat == 1) {
				banyakKandidat = 2;
				printf("Masukkan nama pasangan calon ke-1: ");
				scanf(" %[^\n]", kandidatPaslon[0]);
				strcpy(kandidatPaslon[1], kosong);
			}
			else {
				for (i = 0; i < banyakKandidat; i++) {
					printf("Masukkan nama pasangan calon ke-%d: ", i+1);
					scanf(" %[^\n]", kandidatPaslon[i]);
				}
			}
			break;
		case '2':
			tabel();
			break;
		case '0':
			menu();
			break;
		default:
			printf("\nMasukkan input yang benar!");
			printf("\nTekan Enter untuk memasukkan input kembali...");
			getch();
			menuAdmin();
	}
	daerah = (struct dataDaerah*)malloc(n * sizeof(struct dataDaerah));
	menu();
}

//function untuk memperlihatkan data inputan user dan menu manipulasi data yang dapat mereka lakukan
void tabel() {
	int i, input, x;
	system("CLS");
	
	display();
	printf("\n\nPilih metode pengurutan yang diinginkan: ");
	printf("\n1. Urutkan berdasarkan nama daerah");
	printf("\n2. Update data");
	printf("\n3. Hapus data");
	printf("\n4. Simpan data");
	printf("\n5. Lihat data sebelumnya");
	printf("\n0. Kembali ke menu admin");
	printf("\nInput: ");
	scanf("%d", &input);
	
	if (input == 1) {
		sortDaerah();
	}
	else if (input == 2) {
		//Function ubah data
		ubahData();
	}
	else if (input == 3) {
		//Function hapus data
		hapusData();
	}
	else if (input == 4) {
		//Function simpan data
		printFile();
	}
	else if (input == 5) {
		//Function membaca file sebelumnya
		dataSebelum();
	}
	else if (input == 0) {
		menuAdmin();
	}
	else {
		printf("\nInput Anda salah! Tekan Enter untuk memilih kembali!");
		getch();
		tabel();
	}
	
	printf("Tekan Enter untuk melanjutkan...");
	getch();
	menuAdmin();
}

//Function untuk mengurutkan daerah yang sudah diinput A-Z
void sortDaerah() {
	char namaTemp[4][20];
	long int jmlhPemilihTemp, suaraTemp[4], golputTemp;
	int swapped, i, j;
	for (i = 0; i < sudahInput; i++) {
		for (j = i+1; j < sudahInput; j++) {
			if((strcmp((daerah+i)->namaDaerah, (daerah+i+1)->namaDaerah)) > 0) {
				//Swap Nama
				strcpy(namaTemp[i], (daerah+i)->namaDaerah);
				strcpy((daerah+i)->namaDaerah, (daerah+i+1)->namaDaerah);
				strcpy((daerah+i+1)->namaDaerah, namaTemp[i]);

				//Swap Jumlah Pemilih
				jmlhPemilihTemp = (daerah+i)->jmlhPemilihPerDaerah;
				(daerah+i)->jmlhPemilihPerDaerah = (daerah+i+1)->jmlhPemilihPerDaerah;
				(daerah+i+1)->jmlhPemilihPerDaerah = jmlhPemilihTemp;
					
				//Swap suara paslon
				suaraTemp[0] = (daerah+i)->paslon1;
				(daerah+i)->paslon1 = (daerah+i+1)->paslon1;
				(daerah+i+1)->paslon1 = suaraTemp[0];
					
				suaraTemp[1] = (daerah+i)->paslon2;
				(daerah+i)->paslon2 = (daerah+i+1)->paslon2;
				(daerah+i+1)->paslon2 = suaraTemp[1];
				
				suaraTemp[2] = (daerah+i)->paslon3;
				(daerah+i)->paslon3 = (daerah+i+1)->paslon3;
				(daerah+i+1)->paslon3 = suaraTemp[2];
					
				suaraTemp[3] = (daerah+i)->paslon4;
				(daerah+i)->paslon4 = (daerah+i+1)->paslon4;
				(daerah+i+1)->paslon4 = suaraTemp[3];
					
				//Swap data golput
        		golputTemp = (daerah+i)->golput;
				(daerah+i)->golput = (daerah+i+1)->golput;
				(daerah+i+1)->golput = golputTemp;
			}
		}
	}
	tabel();
}

//Function untuk menampilkan data yang sudah diinput
void display() {
	int i;
	char header[8][20] = {"No.", "Nama Daerah", "Jumlah Pemilih", "Suara Paslon 1", "Suara Paslon 2", "Suara Paslon 3", "Suara Paslon 4", "Golput"};
	
	printf("\n=======================================================================================================================================\n");
    printf("%-5s%-20s%-20s%-20s%-20s%-20s%-20s%-20s", header[0], header[1], header[2], header[3], header[4], header[5], header[6], header[7]);
    printf("\n=======================================================================================================================================\n");
	for (i = 0; i < sudahInput; i++) {
    	printf("\n");
		printf("%-5d%-20s%-20d", i+1, (daerah+i)->namaDaerah, (daerah+i)->jmlhPemilihPerDaerah);
    	printf("%-20d%-20d%-20d%-20d%-20d", (daerah+i)->paslon1, (daerah+i)->paslon2, (daerah+i)->paslon3, (daerah+i)->paslon4, (daerah+i)->golput);
	}
}

void dataSebelum()
{
    char p[4];	// variabel untuk menyimpan data
    char header[8][20] = {"No.", "Nama Daerah", "Jumlah Pemilih", "Suara Paslon 1", "Suara Paslon 2", "Suara Paslon 3", "Suara Paslon 4", "Golput"};
	long int a,b,c,d,e,f, g; // variabel untuk menyimpan data
	FILE *fileptr;

    system("CLS");
    printf("\n=======================================================================================================================================\n");
    printf("%-5s%-20s%-20s%-20s%-20s%-20s%-20s%-20s", header[0], header[1], header[2], header[3], header[4], header[5], header[6], header[7]);
    printf("\n=======================================================================================================================================\n");

	if ((fileptr = fopen ("D:\\hasil_vote.txt", "r"))==NULL)
    {
    	printf("Error, data sebelumnya tidak ditemukan");
    	exit(1);
	}
	else {
		//Membuka lalu membaca file jika untuk voting yang telah dilakukan sebelumnya(jika sudah dilakukan)
		while(!feof(fileptr)) {
			fscanf(fileptr, "%d %s %ld %ld %ld %ld %ld %ld\n", &g, p, &a, &b, &c, &d, &e, &f);
			printf("%-5d%-20s%-20ld%-20ld%-20ld%-20ld%-20ld%-20ld\n", g, p, a, b, c, d, e, f);
		}
	}

	fclose(fileptr);

	printf("Tekan {Enter) untuk kembali ke tabel vote sekarang...");
	getch();
	tabel();
}

//Function untuk menyimpan data yang diinput pada file txt
void printFile() {
	int i;
	FILE *fileptr;
	fileptr = fopen("D:\\hasil_vote.txt","w+");
	if(fileptr == NULL)
	{
		printf("Error, Tidak dapat dijalankan..");
		tabel();
	}
	else
	{
		for(i=0; i<sudahInput; i++) {
			fprintf(fileptr, "%d %s %ld %ld %ld %ld %ld %ld\n", i+1, (daerah+i)->namaDaerah, (daerah+i)->jmlhPemilihPerDaerah, (daerah+i)->paslon1, (daerah+i)->paslon2, (daerah+i)->paslon3, (daerah+i)->paslon4, (daerah+i)->golput);
		}
	}
	fclose(fileptr);
}

//function hasil
void descending (){//function untuk mengurutkan kandidat mana yang akan menjadi pemenang
	int i,j, input;
	long int temp;
	char swap[4][50];
	
	for (i=0; i< banyakKandidat ; ++i){
		for (j=i+1 ; j<banyakKandidat ; ++j){
			if (votePaslon[i] < votePaslon[j]){
				temp = votePaslon[i];
				votePaslon[i]=votePaslon[j];
				votePaslon[j]=temp;
				
				strcpy(swap[0], kandidatPaslon[i]);
            	strcpy(kandidatPaslon[i], kandidatPaslon[j]);
            	strcpy(kandidatPaslon[j], swap[0]);
			}
		}
	}
	show();
	
	printf("\n\nApakah Anda ingin menyelesaikan Program?\n ");
	printf("(Y/N)\n\n");
	scanf(" %c",&input);

	if(input=='y' || input =='Y')
	{
		printFile();
		thanks();
	}
	else
		menu();
}

//function untuk menampilkan hasil sorting dari function descending
void show(){
	int i;
	char header[3][30] = {"No.", "Nama Pemenang", "Suara Sah"};
	
	printf("\n===============================================================================================================\n");
    printf("%-5s%-20s%-20s", header[0], header[1], header[2], header[3]);
    printf("\n===============================================================================================================\n");
	for (i = 0; i < banyakKandidat; i++) {
    	printf("\n");
		printf("%-5d%-20s%-20d", i+1, kandidatPaslon[i], votePaslon[i]);
	}
	
	printf ("\n");
	printf ("\n\t\t\t !!! PEMENANG : %20s !!!\n", strupr(kandidatPaslon[0]));
	printf ("\n");
}

//Function untuk hapus data input satu daerah
void hapusData () {
    int i, j, hapus, pilihan;

    printf("\n\nNomor daerah yang ingin dihapus : ");
    scanf("%d", &hapus);
    pilihan = hapus-1;

    if(hapus >= sudahInput + 1) {
        printf("\nPenghapusan tidak dapat dilakukan !");
        printf("\nTekan (enter) untuk melanjutkan...");
        getch();
        tabel();
    }
	else {
		jumlahPemilihEligible -= (daerah+pilihan)->jmlhPemilihPerDaerah;
		golput -= (daerah+pilihan)->golput;
		votePaslon[0] -= (daerah+pilihan)->paslon1;
		votePaslon[1] -= (daerah+pilihan)->paslon2;
		votePaslon[2] -= (daerah+pilihan)->paslon3;
		votePaslon[3] -= (daerah+pilihan)->paslon4;

        for(i=hapus-1; i<sudahInput-1; i++ )
        {
            strcpy((daerah+i)->namaDaerah, (daerah+i+1)->namaDaerah);
			(daerah+i)->jmlhPemilihPerDaerah = (daerah+i+1)->jmlhPemilihPerDaerah;
			(daerah+i)->golput = (daerah+i+1)->golput;
			(daerah+i)->paslon1 = (daerah+i+1)->paslon1;
			(daerah+i)->paslon2 = (daerah+i+1)->paslon2;
			(daerah+i)->paslon3 = (daerah+i+1)->paslon3;
			(daerah+i)->paslon4 = (daerah+i+1)->paslon4;
        }
    }
    batas[0]--;
    sudahInput--;
    tabel();
}

//Function untuk mengubah data daerah yang diinput user
void ubahData(){
	int i, j, ubah, pilihan;
	long int a, b, c, d, e, f;
	char temp[50];
	
	printf("\n\nNomor daerah yang ingin diubah: ");
	scanf("%d", &ubah);
	pilihan = ubah-1;
	
	if(ubah >= sudahInput + 1){
		printf("\nUbah data tidak dapat dilakukan !");
		printf("\nTekan (enter) untuk melanjutkan...");
		getch();
		tabel();
	}else{
		printf("Masukkan nama daerah yang baru: ");
		scanf(" %[^\n]", temp);
		printf("Masukkan jumlah pemilih yang baru: ");
		scanf("%d", &a);
		printf("Masukkan jumlah golput yang baru: ");
		scanf("%d", &b);
		printf("Masukkan jumlah suara sah paslon 1 yang baru: ");
		scanf("%d", &c);
		printf("Masukkan jumlah suara sah paslon 2 yang baru: ");
		scanf("%d", &d);
		printf("Masukkan jumlah suara sah paslon 3 yang baru: ");
		scanf("%d", &e);
		printf("Masukkan jumlah suara sah paslon 4 yang baru: ");
		scanf("%d", &f);
		for(i=pilihan-1; i<sudahInput-1; i++ ){
			strcpy((daerah+i)->namaDaerah, temp);
			(daerah+i)->jmlhPemilihPerDaerah = a;
			(daerah+i)->golput = b;
			(daerah+i)->paslon1 = c;
			(daerah+i)->paslon2 = d;
			(daerah+i)->paslon3 = e;
			(daerah+i)->paslon4 = f;
		}
	}
	tabel();
}

//Function display terima kasih ketika user ingin menyelesaikan program
void thankYou(){
	system("cls");
	printf("\n\t\t\t\t|-------------------------------------------------|\n");
	printf("\t\t\t\t||                                               ||\n");
	printf("\t\t\t\t||   Terima Kasih Sudah Menggunakan Program Ini! ||\n");
	printf("\t\t\t\t||                                               ||\n");
	printf("\t\t\t\t|-------------------------------------------------|\n");
	Sleep(500);
	exit(0);
}
