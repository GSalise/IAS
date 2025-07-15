#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "helper.h"


void setMatrix(char m[26][26]){
	char alphabet[26];
	int a = 0;

	for (int i = 0; i < 26; i++) {
	    alphabet[i] = 'A' + i;
	}
	
	for(int i = 0; i < 26; i++){	
		for(int j = 0; j < 26; j++){
			m[i][j] = alphabet[(i + j) % 26];
		}
	}
}

void removeSpaces(char text[]){
	int i = 0, j = 0;
	
	while(text[i]){
		if(text[i] != ' '){
			text[j++] = text[i];
		}
		i++;
	}	
	
	text[j] = '/0';
}

void setUppercase(char message[], char key[]){
	for(int i = 0; i < strlen(message); i++){
		message[i] = toupper(message[i]);
	}
	for(int i = 0; i < strlen(key); i++){
		key[i] = toupper(key[i]);
	}
}

char *encrypt(char message[], char key[]){
	char matrix[26][26];
	setMatrix(matrix);
	
	char extendedKey[256];
	char *encryptedMessage = malloc(sizeof(char) * 256);
	int messageLength = strlen(message);

	for(int i = 0; i < messageLength; i++){
		extendedKey[i] = key[i % strlen(key)];
	}
	
	for(int a = 0; a < messageLength; a++){
		encryptedMessage[a] = matrix[findIndex(extendedKey[a])][findIndex(message[a])];
	}
	
	encryptedMessage[messageLength] = '\0';
	
	return encryptedMessage;
}

char *decrypt(char encryptedMessage[], char key[]){
	char matrix[26][26];
	setMatrix(matrix);
	
	char extendedKey[256];
	char *decryptedMessage = malloc(sizeof(char) * 256);
	int messageLength = strlen(encryptedMessage);

	for(int i = 0; i < messageLength; i++){
		extendedKey[i] = key[i % strlen(key)];
	}
	
	for(int i = 0; i < messageLength; i++){
		for(int j = 0; j < 26; j++){
			if(encryptedMessage[i] == matrix[findIndex(extendedKey[i])][j]){
				decryptedMessage[i] = matrix[0][j];
			}
		}
	}
	
	
	
	
	decryptedMessage[messageLength] = '\0';
	
	return decryptedMessage;
}

int findIndex(char target){
	char alphabet[26];

	for (int i = 0; i < 26; i++) {
	    alphabet[i] = 'a' + i;
	    
	    if(alphabet[i] == target){
	    	return i;
		}
	}
}




