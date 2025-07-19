#ifndef HELPER_H
#define HELPER_H

void setMatrix(char m[26][26]);
void removeSpaces(char text[]);
char *encrypt(char message[], char key[]);
char *decrypt(char encryptedMessage[], char key[]);
int findIndex(char target);
void setUppercase(char message[], char key[]);

#endif
