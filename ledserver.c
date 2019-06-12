// Einfacher Mirrirserver in C mit Threats
// In Ordner einfügen mit: gcc mirror-thread.c -o mirror-thread -Wall -lpthread -lwiringPi
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <wiringPi.h> 

#define BUFSIZE 1000
#define PORT 4000

#define LED1 25
#define LED2 24
#define LED3 23
#define LED4 18


// Warteschlange für ankommende Verbindung
#define QUEUE 3


void *led(void *arg)
{
	int clientfd = *(int *)arg; //typecast
	char einlesen[BUFSIZE];
	int eingabe;
		
	wiringPiSetupGpio();
	pinMode (LED1, OUTPUT);
	pinMode (LED2, OUTPUT);
	pinMode (LED3, OUTPUT);
	pinMode (LED4, OUTPUT);
	
	//Menü
	write(clientfd, "Herzlich Wilkommen am Led-Server!\n", 35);
	write(clientfd, "Bitte geben Sie eine Zahl zwischen 0 und 15 ein:\n", 50);
	
	while(1) {
		
		read(clientfd, &einlesen, sizeof(einlesen));
		
		eingabe = atoi (einlesen);
	
		if(eingabe == 0)
		{
			digitalWrite(LED1, 0);
			digitalWrite(LED2, 0);
			digitalWrite(LED3, 0);
			digitalWrite(LED4, 0);
		}
		if(eingabe == 1)
		{
			digitalWrite(LED1, 1);
			digitalWrite(LED2, 0);
			digitalWrite(LED3, 0);
			digitalWrite(LED4, 0);
		}
		if(eingabe == 2)
		{
			digitalWrite(LED1, 0);
			digitalWrite(LED2, 1);
			digitalWrite(LED3, 0);
			digitalWrite(LED4, 0);
		}
		if(eingabe == 3)
		{
			digitalWrite(LED1, 1);
			digitalWrite(LED2, 1);
			digitalWrite(LED3, 0);
			digitalWrite(LED4, 0);
		}
		if(eingabe == 4)
		{
			digitalWrite(LED1, 0);
			digitalWrite(LED2, 0);
			digitalWrite(LED3, 1);
			digitalWrite(LED4, 0);
		}
		if(eingabe == 5)
		{
			digitalWrite(LED1, 1);
			digitalWrite(LED2, 0);
			digitalWrite(LED3, 1);
			digitalWrite(LED4, 0);
		}
		if(eingabe == 6)
		{
			digitalWrite(LED1, 0);
			digitalWrite(LED2, 1);
			digitalWrite(LED3, 1);
			digitalWrite(LED4, 0);
		}
		if(eingabe == 7)
		{
			digitalWrite(LED1, 1);
			digitalWrite(LED2, 1);
			digitalWrite(LED3, 1);
			digitalWrite(LED4, 0);
		}
		if(eingabe == 8)
		{
			digitalWrite(LED1, 0);
			digitalWrite(LED2, 0);
			digitalWrite(LED3, 0);
			digitalWrite(LED4, 1);
		}
		if(eingabe == 9)
		{
			digitalWrite(LED1, 1);
			digitalWrite(LED2, 0);
			digitalWrite(LED3, 0);
			digitalWrite(LED4, 1);
		}
		if(eingabe == 10)
		{
			digitalWrite(LED1, 0);
			digitalWrite(LED2, 1);
			digitalWrite(LED3, 0);
			digitalWrite(LED4, 1);
		}
		if(eingabe == 11)
		{
			digitalWrite(LED1, 1);
			digitalWrite(LED2, 1);
			digitalWrite(LED3, 0);
			digitalWrite(LED4, 1);
		}
		if(eingabe == 12)
		{
			digitalWrite(LED1, 0);
			digitalWrite(LED2, 0);
			digitalWrite(LED3, 1);
			digitalWrite(LED4, 1);
		}
		if(eingabe == 13)
		{
			digitalWrite(LED1, 1);
			digitalWrite(LED2, 0);
			digitalWrite(LED3, 1);
			digitalWrite(LED4, 1);
		}
		if(eingabe == 14)
		{
			digitalWrite(LED1, 0);
			digitalWrite(LED2, 1);
			digitalWrite(LED3, 1);
			digitalWrite(LED4, 1);
		}
		if(eingabe == 15)
		{
			digitalWrite(LED1, 1);
			digitalWrite(LED2, 1);
			digitalWrite(LED3, 1);
			digitalWrite(LED4, 1);
		}
		if(*einlesen == 'q')
		{
			digitalWrite(LED1, 0);
			digitalWrite(LED2, 0);
			digitalWrite(LED3, 0);
			digitalWrite(LED4, 0);
			break;
		}
	} 
	close(clientfd);
	return NULL;
}


int main() 
{
	int server_socket, rec_socket;
	unsigned int len;
	struct sockaddr_in serverinfo, clientinfo;
	
	// Serversocket konfigurieren --> nimmt seine Connects entgegen
	server_socket = socket(AF_INET, SOCK_STREAM, 0); //TCP
	serverinfo.sin_family = AF_INET; //IPv4
	// hört auf allen Interfaces : 0.0.0.0
	serverinfo.sin_addr.s_addr = htonl(INADDR_ANY);
	serverinfo.sin_port = htons(PORT);
	
	//Verbinde Socket mit IP-Adresse und Port
	if(bind(server_socket, (struct sockaddr *)&serverinfo, sizeof(serverinfo)) != 0){
		printf("Fehler Socket\n");
		return 1; // Rückgabe Fehlercode
	}
	
	listen(server_socket, QUEUE); // Server wartet auf connect vom Client
	
	// Endlosschleife Server zur Abarbeitung der Client-Anfragen
	while(1) {		
		printf("Server wartet...\n");
		// Verbindung vom Client eingetroffen
		rec_socket = accept(server_socket, (struct sockaddr *)&clientinfo, &len);
		printf("Verbindung von %s:%d\n", inet_ntoa(clientinfo.sin_addr), ntohs(clientinfo.sin_port));
		
		pthread_t child; // Thread-Struktur
		if (pthread_create(&child, NULL, led, &rec_socket) != 0){
			perror("child error"); // Fehlerfall: Abbruch
			return 1;
		}
		else {	// Kind erzeugt:
			printf("Abgekoppelt!\n");
			pthread_detach(child); // abgekoppelt vom Hauptprozess
		}
	}
	return 0;
	
}	
