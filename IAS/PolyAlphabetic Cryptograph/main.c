#include <stdio.h>
#include <string.h>
#include "helper.h"

int main(){
	char key[256];
	char message[256];

	printf("Enter your message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    printf("Enter your key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    removeSpaces(message);
    removeSpaces(key);
    
	char *encryptedMessage = encrypt(message, key);
	printf("\nEncrypted Message = %s", encryptedMessage);

	char *decryptedMessage = decrypt(encryptedMessage, key);
	printf("\nDecrypted Message = %s", decryptedMessage);
	
	return 0;
}
