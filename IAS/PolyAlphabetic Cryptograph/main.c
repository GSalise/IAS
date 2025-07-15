#include <stdio.h>
#include "helper.h"

int main(){
	char key[256];
	char message[256];
//	
//	printf("Enter your key: ");
//	scanf("%s", key);
//	printf("\n%s - KEY", key);

	printf("Enter your message: ");
	scanf("%255s", message);
	
	printf("\nEnter your key: ");
	scanf("%255s", key);
	
	char *encryptedMessage = encrypt(message, key);
	printf("\nEncrypted Message = %s", encryptedMessage);

	char *decryptedMessage = decrypt(encryptedMessage, key);
	printf("\nDecrypted Message = %s", decryptedMessage);
	
	return 0;
}
