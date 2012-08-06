/*
 * File:   security.c
 * Author: Antonio Ranieri 
 */

#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pbc.h>
#include "sec_config.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include "utma_weak.h"
#include "common.h"
#include <netinet/in.h>
#include <netdb.h>

pthread_t sock_connection;
char vis_msg[18];

PubTriple *pubT;
Key *key;
Sk * sk;
Pk * pk;
Pub * pub;
pairing_t pairing;
uint8_t* buffer;
int fd, c, size,errore;
struct termios oldtio, newtio;
char * message,* cifrato,* randomizzato;
long operazioni;
//This Function read the file with parameter
int SocketDescriptor= 0;

#ifdef VISUAL
void CloseSocket(int sock)
{
  close(sock);
  return;
}

int CreateSocket(char* Destinazione, int Porta)
{
  struct sockaddr_in temp; 
  struct hostent *h;
  int sock;
  //int errore;
  
  //Tipo di indirizzo
  temp.sin_family=AF_INET;
  temp.sin_port=htons(Porta);
  h=gethostbyname(Destinazione);
  if (h==0) 
  {
    printf("Gethostbyname failed\n");
    return -1;
  }
  bcopy(h->h_addr,&temp.sin_addr,h->h_length);
  //Creazione socket. 
  sock=socket(AF_INET,SOCK_STREAM,0);
  //Connessione del socket. Esaminare errore per compiere azioni
  //opportune in caso di errore.
  errore=connect(sock, (struct sockaddr*) &temp, sizeof(temp));

  return sock;
}

int SendMessage(int sock, uint8_t* Messaggio)
{
 // printf("Client: %s\n",Messaggio);  
  int writt_bytes = send(sock,Messaggio,strlen(Messaggio),MSG_NOSIGNAL);
  if (writt_bytes<0)
  {
	
    printf("Impossible to send message.\n");
//    CloseSocket(sock);
    return -1;
  }
  else
// printf("Messagge sent.\n");
  return writt_bytes;
}
#endif

char* read_file(char* filename) {

	#ifdef PARAM_DEBUG
	printf("File to open: %s \n", filename);
	#endif
	FILE *fp;
	fp = fopen(filename, "r");
	if(fp<=0){
	printf("ERROR NO FILE TO LOAD. ADD IT IN THE PARAM FOLDER\nExiting...\n");
	exit(1);
	}
	#ifdef PARAM_DEBUG
	printf("File opened: %s \n", filename);
	#endif
	char* buffer = NULL;
	if (fp) {
		int fd = fileno(fp);
		struct stat buf;
		fstat(fd, &buf);
		int size = buf.st_size;
		#ifdef PARAM_DEBUG
		printf("file size is %d.\n", size);
		#endif
		buffer = malloc(sizeof(char) * size + 1);
		size_t readChars = fread(buffer, size, sizeof(char), fp);
		buffer[strlen(buffer)] = '\0';
	#ifdef PARAM_DEBUG
		printf("file content is: \n%s\n", buffer);
	} else {
		printf("An error occurred while opening the file.\n");
	#endif
	}
	fclose(fp);
	return buffer;
}

void pairing_setup() {
	// Init pairing
	pairing_init_set_str(pairing, read_file("./param/a.param"));
	// Generate and store Public Parameters
	pubT = GenPub(&pairing);
	buffer = pubtriple2bytes(pubT->pub.pairing, pubT);
	store(buffer, pubtriple_size(pairing), "pubt");
	free(buffer);
	printf("store pubt\n");
	// Generate and store  Key
	key = GenKey(&pubT->pub, &pubT->msk);
	buffer = key2bytes(pubT->pub.pairing, key);
	//store(buffer, key_size(pairing), "key");
	free(buffer);
	printf("store key\n");
	//Store PK
	buffer = pk2bytes(pubT->pub.pairing, &(key->pk));
	store(buffer, key_size(pairing), "pk");
	pk = bytes2pk(pairing, buffer);
	free(buffer);
	printf("store pk\n");
	//Store SK
	buffer = sk2bytes(pubT->pub.pairing, &(key->sk));
	store(buffer, sk_size(pairing), "sk");
	sk = bytes2sk(pairing, buffer);
	free(buffer);
	printf("store sk\n");
	//Store Pub
	buffer = pub2bytes(pubT->pub.pairing, &(pubT->pub));
	//store(buffer, pub_size(pairing), "pub");
	free(buffer);
	printf("store pub\n");
}

void load_pairing_param(int func) {
	
	pairing_init_set_str(pairing, read_file("param/a.param"));

	//Load Pubt
	buffer = malloc(sizeof(PubTriple));
	load(buffer, pub_size(pairing), "pubt");
	pubT = bytes2pubtriple(pairing, buffer);
	free(buffer);
	switch (func) {
	case 2: {
		buffer = malloc(key_size(pairing));
		load(buffer, pub_size(pairing), "pk");
		pk = bytes2pk(pairing, buffer);
		free(buffer);
	}
		break;
	case 3: {
		buffer = malloc(key_size(pairing));
		load(buffer, pub_size(pairing), "sk");
		sk = bytes2sk(pairing, buffer);
		free(buffer);
	}
		break;
	case 4: {printf("nothing to load \n");
	}
		break;
	case 5: {
		buffer = malloc(key_size(pairing));
		load(buffer, pub_size(pairing), "pk");
		pk = bytes2pk(pairing, buffer);
		free(buffer);

		buffer = malloc(key_size(pairing));
		load(buffer, pub_size(pairing), "sk");
		sk = bytes2sk(pairing, buffer);
		free(buffer);

	}
		break;

	}

}

void prepare_uart() {
	//Store acrtual configuration in oldtio struct
	tcgetattr(fd, &oldtio);
	//reset configuration of newtio
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD | CRTSCTS;
	newtio.c_iflag = 0;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 10;
	cfsetospeed(&newtio, BAUDRATE);
	cfsetispeed(&newtio, BAUDRATE);
	//Open The Uart
	fd = open(UART, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
	//Error in opening UART
	if (fd < 0) {
		perror(UART);
		printf("Error opening uart\n");
		//exit(-1);
	}
	//configure new settings
	tcflush(fd, TCIFLUSH);
	tcflush(fd, TCOFLUSH);
	//set new configurations
	//tcsetattr(fd,TCSANOW,&newtio);
	int status = TIOCM_RTS | TIOCM_DTR;
	ioctl(fd, TIOCMSET, &status);
	status = 0;
	ioctl(fd, TIOCMSET, &status);
	sleep(1);

}

int read_bb(uint8_t * msgtp, uint8_t * msg_id, uint8_t * req_id,	uint8_t * payload_len, uint8_t * payload) {

	int end = 0, index = 1, read_ret,res_read;
	uint8_t newbuff[160], c[1], temp[1];

	res_read = read(fd, newbuff, 1);

	if ((newbuff[0] == 16) && (res_read == 1)) {
		read_ret = read(fd,c,1); while(read_ret !=1 ){read_ret = read(fd,c,1);}if(c[0]==3) return 3;else newbuff[index]=c[0];index++;
		while (end == 0) {
			read_ret = read(fd, c, 1);
			if (read_ret != 1) {
				continue;
			}
			if (c[0] == 16) {
				temp[0] = c[0];
				int check = 0;
				while (check != 1) {
					check = read(fd, c, 1);
				}
				if (c[0] == 3) {
					end = 1;
					newbuff[index] = temp[0];
					index++;
				}
			}
			newbuff[index] = c[0];
			index++;
		}
		memcpy(&msgtp[0], &newbuff[2], 1);
		if (msgtp[0] == 104) {
			
			#ifdef VISUAL
			if((newbuff[4]==86)&&(newbuff[5]==73)&&(newbuff[6]==83)){
			//printf("-----------------------\n");
		


			uint8_t coord[4];
			vis_msg[0]=86;
			vis_msg[1]=73;
			vis_msg[2]=83;
			vis_msg[3]=32;
			vis_msg[4]=45;
	
			int s_x=0 , s_y = 0 ,s_id = 0;			
			int i =0;

			while (newbuff[i+ 9]!=45){
			//printf(".\n");
			vis_msg[i+5]=newbuff[i+9];
			s_id++;	
			i++;		
			}
			vis_msg[i+5]=newbuff[i+9];
			//printf("sid : %d\n",s_id);
			i =0;

			while (newbuff[i+ 9 +s_id+1]!=45){
			//printf("+\n");
			vis_msg[i+5+ s_id+1]=newbuff[i+9+s_id+1];
			s_x++;	
			i++;		
			}
			vis_msg[i+5+s_id+1]=newbuff[i+9+s_id+1];
			//printf("sz : %d\n",s_x);
			i=0;

			while (newbuff[i+ 9 +s_id+s_x+2]!=45){
			//printf("-\n");
			vis_msg[i+5+ s_id+s_x+2]=newbuff[i+9+s_id+s_x+2];
			s_y++;
			i++;			
			}
			vis_msg[i+5+s_id+s_x+2]=newbuff[i+9+s_id+s_x+2];
			//printf("sy : %d\n",s_y);
	
			vis_msg[s_id+s_x+s_y+8]=10;
			SendMessage(SocketDescriptor,vis_msg);
			}

			#endif

			#ifdef SENSOR_DEBUG
			int count;
			printf("\n");
			for (count = 3; count < index - 2; count++)
			printf("%c", newbuff[count]);
			printf("\n");
			#endif

			return 2;
		}

		else {
			memcpy(&msg_id[0], &newbuff[3], 1);
			memcpy(&req_id[0], &newbuff[4], 2);
			payload_len[0] = index;
			//memcpy(&payload_len[0],&newbuff[6],1);
			memcpy(&payload[0], &newbuff[7], index);
			return 1;
		}

	}

	else
		return 0;
}

int uart_write(int fd, uint8_t *buff, int len) {
	int i, y, count, count2, additem = 0, a;
	uint8_t dle[1], stx[1], etx[1], msgtype[1], buff_final[150];

	dle[0] = 16;
	stx[0] = 2;
	etx[0] = 3;
	msgtype[0] = 10;

	memcpy(&buff_final[0], dle, 1);
	memcpy(&buff_final[1], stx, 1);
	memcpy(&buff_final[2], msgtype, 1);

	for (i = 3; i < len; i++) {
		if (buff[i] == 16) {
			buff_final[i + additem] = 16;
			additem++;
		}
		buff_final[i + additem] = buff[i];
	}

	memcpy(&buff_final[len + additem], dle, 1);
	memcpy(&buff_final[len + 1 + additem], etx, 1);
	a = write(fd, buff_final, len + additem + 2);

	#ifdef PC_DEBUG
	int j;
	printf("\nWrite on uart :\n");
	for (j = 0; j < len + additem + 2; j++) {
		printf("%u ", buff_final[j]);
	}
	#endif
	return a;
}

uint8_t * encrypt(uint8_t * payload, uint8_t* req_id) {
	uint8_t* test;
	int dle_number = 0, i, jc;
	int flag = 1, flag_2 = 1;
	#ifdef REQUEST_REPLY_DEBUG
	printf("\nReceived encrypt request\n");
	#endif
	uint8_t encrypt_reply[114];
	uint8_t msg_en_reply[1];
	msg_en_reply[0] = 140;

	uint8_t payload_en_len[1];
	payload_en_len[0] = CT_SIZE;
	uint8_t req_id_null[2];

	PlainText *plainText = fromString(pubT->pub.pairing, payload);
	// Encrypt ID read from UART and Write it in a buffer
	CipherText *ciphertext = rE(&pubT->pub, pk, plainText);
	buffer = ciphertext2bytes(pubT->pub.pairing, ciphertext);

	while (flag) {

		for (i = 0; i < 40; i++) {
			if (buffer[i] == 16)
				dle_number++;
		}
		if (dle_number < 20)
			flag = 0;
		else {
			CipherText *ciphertext =
					bytes2ciphertext(pubT->pub.pairing, buffer);
			ciphertext = Randomize(&pubT->pub, ciphertext);
			//Store New CipherText in Buffer
			buffer = ciphertext2bytes(pubT->pub.pairing, ciphertext);
		}
	}

	for (c = 0; c < CT_SIZE-1; c++)
		if ((buffer[c] == 0) && (buffer[c + 1] == 0)) {
			free(buffer);
			encrypt(payload, req_id);
			return;
		}

	memcpy(&encrypt_reply[3], msg_en_reply, 1);
	memcpy(&encrypt_reply[4], req_id, 2);
	memcpy(&encrypt_reply[6], payload_en_len, 1);
	memcpy(&encrypt_reply[7], buffer, CT_SIZE);

	uart_write(fd, encrypt_reply, CT_SIZE+7);

	#ifdef REQUEST_REPLY_DEBUG
	printf("\nSend Encription reply\n");
	#endif

	clear_ciphertext(ciphertext);
	clear_plaintext(plainText);
	free(buffer);

}

uint8_t * decrypt(uint8_t * payload, uint8_t * req_id) {

	#ifdef REQUEST_REPLY_DEBUG
	printf("\nReceived decrypt request\n");
	#endif

	uint8_t msg_dec_reply[1];
	msg_dec_reply[0] = 130;
	uint8_t payload_dec_len[1];
	payload_dec_len[0] = 2;
	uint8_t decrypt_reply[9];

	//Load cipherId from buffer
	CipherText *ciphertext = bytes2ciphertext(pubT->pub.pairing, payload);
	// Decrypt the chiperID and put it in a buffer
	PlainText *pt = rD(&pubT->pub, sk, ciphertext);
	buffer = toString(pubT->pub.pairing, pt);
	// Write on UART ID Decrypted

	memcpy(&decrypt_reply[3], msg_dec_reply, 1);
	memcpy(&decrypt_reply[4], req_id, 2);
	memcpy(&decrypt_reply[6], payload_dec_len, 1);
	memcpy(&decrypt_reply[7], buffer, 2);

	uart_write(fd, decrypt_reply, 9);
	clear_ciphertext(ciphertext);
	clear_plaintext(pt);
	free(buffer);

	#ifdef REQUEST_REPLY_DEBUG
	printf("\n Send decription reply\n");
	#endif
}

uint8_t * randomize(uint8_t * payload, uint8_t * req_id) {
	int dle_number = 0, i,flag=1;


	#ifdef REQUEST_REPLY_DEBUG
	printf("\nReceived randomization request\n");
	#endif
	uint8_t msg_ra_reply[1], payload_ra_len[1], randomize_reply[90];
	msg_ra_reply[0] = 150;
	payload_ra_len[0] = CT_SIZE;

	//Generate CipherText from buffer
	CipherText *ciphertext = bytes2ciphertext(pubT->pub.pairing, payload);
	//Randomize CipherText
	ciphertext = Randomize(&pubT->pub, ciphertext);
	//Store New CipherText in Buffer
	buffer = ciphertext2bytes(pubT->pub.pairing, ciphertext);
	//Write New CipherText on UART


	while (flag) {

		for (i = 0; i < 40; i++) {
			if (buffer[i] == 16)
				dle_number++;
		}
		if (dle_number < 20)
			flag = 0;
		else {
			CipherText *ciphertext =
					bytes2ciphertext(pubT->pub.pairing, buffer);
			ciphertext = Randomize(&pubT->pub, ciphertext);
			//Store New CipherText in Buffer
			buffer = ciphertext2bytes(pubT->pub.pairing, ciphertext);
		}
	}

	for (c = 0; c < CT_SIZE-1; c++)
		if ((buffer[c] == 0) && (buffer[c + 1] == 0)) {
			free(buffer);
			randomize(payload, req_id);
			return;
		}


	memcpy(&randomize_reply[3], msg_ra_reply, 1);
	memcpy(&randomize_reply[4], req_id, 2);
	memcpy(&randomize_reply[6], payload_ra_len, 1);
	memcpy(&randomize_reply[7], buffer, CT_SIZE);

	uart_write(fd, randomize_reply, CT_SIZE+7);
	#ifdef REQUEST_REPLY_DEBUG
	printf("\nSend randomization reply\n");
	#endif
	clear_ciphertext(ciphertext);
	free(buffer);
}

#ifdef VISUAL
void* sub_visualization(void * vis_msg)
{	
	//++printf("----X------------------X---------------X----\n");
	int ret_write=0;

	SocketDescriptor = CreateSocket("127.0.0.1",1745);	
	while(SocketDescriptor == -1)
	{	
		//printf("Socket Descriptor : %d ",SocketDescriptor);
		SocketDescriptor = CreateSocket("127.0.0.1",1745);	
	}
	
	//printf("Socket Descriptor : %d ",SocketDescriptor);


	while(1){

	/*if(SendMessage(SocketDescriptor,vis_msg)==-1)
	{
	ret_write++;
	printf(" %d\n",ret_write);
	}*/
	//printf(" check : %s \n",vis_msg);
	if(ret_write > 5 )
	{
		SocketDescriptor = CreateSocket("127.0.0.1",1745);
		
		if (errore != -1) ret_write = 0;	
	}
	usleep(10000);	
	}	
}
#endif

int main(int argc, char** argv) {

	int prova = 1;
	int prova2 = 60;

	char x_coord[3];
	char y_coord[3];
	x_coord[2]=prova%10;
	x_coord[1]=(prova/10)%10;
	x_coord[0]=((prova/10)/10)%10;

	y_coord[2]=prova2%10;
	y_coord[1]=(prova2/10)%10;
	y_coord[0]=((prova2/10)/10)%10;

	//printf("%d %d %d %d %d %d ",x_coord[0],x_coord[1],x_coord[2],y_coord[0],y_coord[1],y_coord[2]);
	

	vis_msg[0]=86;
	vis_msg[1]=73;
	vis_msg[2]=83;
	vis_msg[3]=32;
	vis_msg[4]=45;
	vis_msg[5]=99;
	vis_msg[6]=99;
	vis_msg[7]=45;
	vis_msg[8]=x_coord[0]+48;
	vis_msg[9]=x_coord[1]+48;
	vis_msg[10]=x_coord[2]+48;
	vis_msg[11]=45;
	vis_msg[12]=y_coord[0]+48;
	vis_msg[13]=y_coord[1]+48;
	vis_msg[14]=y_coord[2]+48;
	vis_msg[15]=45;
	vis_msg[16]=10;
	vis_msg[17]=0;
//printf("\nStringa %s \n",vis_msg);
#ifdef VISUAL
     	if ( pthread_create( &sock_connection, NULL, sub_visualization, (void*) vis_msg) )
	{
		printf("Can't create thr_b\n");
		//exit(1);
	}
#endif
	//while(1){vis_msg[0]=(vis_msg[0]+10);vis_msg[1]=(vis_msg[1]+10);	sleep(1);};
	
	prepare_uart();
	int i, load, flag;
	printf(
			"Press :\n1 to generate new Key \n2 to load Encription Key\n3 to load Decription Key\n4 to load Randomization key\n5 to load all (Just for Testing)\n");
	scanf("%d", &load);

	if ((load < 1) || (load > 5)) {
		printf("Invalid choice. Exit\n");
		//exit(0);
	}

	if (load == 1)
	{
		pairing_setup();
		return (EXIT_SUCCESS);
	}
	else
	{
		load_pairing_param(load);
	}
	uint8_t msgtp[1], msg_id[1], req_id[2], payload_len[1], payload[150];

	#ifdef PC_DEBUG
	printf("Uart ready\n");
	#endif
	

	while (1) {
	
		
		flag = read_bb(msgtp, msg_id, req_id, payload_len, payload);
		if (flag == 1) {

			if (msg_id[0] == 110)
				encrypt(payload, req_id);
			else if (msg_id[0] == 100)
				decrypt(payload, req_id);
			else if (msg_id[0] == 120)
				randomize(payload, req_id);
		} else if (flag == 0) {
		} else if (flag == 2) {
		}else if (flag==3){
	}
	
	}
	#ifdef VISUAL
//	CloseSocket(SocketDescriptor);
	#endif
	return (EXIT_SUCCESS);
}
