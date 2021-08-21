//BusinessCardBinder.h
#ifndef _BUSINESSCARDBINDER_H
#define _BUSINESSCARDBINDER_H
#include"LinkedList.h"
typedef signed long Long;
typedef struct _personal {
	char name[11];
	char rankName[16];
	char telephoneNumber[12];
	char emailAddress[32];
}Personal;

typedef struct _company {
	char name[32];
	char address[64];
	char telephoneNumber[12];
	char faxNumber[12];
	char url[64];
}Company;

typedef struct _businessCard {
	Personal personal;
	Company company;
}BusinessCard;

typedef struct _businessCardBinder {
	LinkedList businessCards;
	Long length;
	BusinessCard* current;
}BusinessCardBinder;

void BusinessCardBinder_Create(BusinessCardBinder* businessCardBinder);
Long Load(BusinessCardBinder* businessCardBinder);
BusinessCard* BusinessCardBinder_Put(BusinessCardBinder* businessCardBinder, BusinessCard businessCard);
void BusinessCardBinder_Find(BusinessCardBinder* businessCardBinder, char(*name), BusinessCard** (*indexes), Long* count);
BusinessCard* FindCompanyName(BusinessCardBinder* businessCardBinder, char(*companyName));
BusinessCard BusinessCardBinder_PutOut(BusinessCardBinder* businessCardBinder, BusinessCard* index);
BusinessCard* Move(BusinessCardBinder* businessCardBinder, BusinessCard* index);
BusinessCard* BusinessCardBinder_First(BusinessCardBinder* businessCardBinder);
BusinessCard* BusinessCardBinder_Previous(BusinessCardBinder* businessCardBinder);
BusinessCard* BusinessCardBinder_Next(BusinessCardBinder* businessCardBinder);
BusinessCard* BusinessCardBinder_Last(BusinessCardBinder* businessCardBinder);
Long Save(BusinessCardBinder* businessCardBinder);
void BusinessCardBinder_Destroy(BusinessCardBinder* businessCardBinder);
int BusinessCardBinder_CompareNames(void* one, void* other);
int BusinessCardBinder_CompareCompanyNames(void* one, void* other);
int BusinessCardBinder_CompareBusinessCards(void* one, void* other);

#endif//_BUSINESSCARDBINDER_H
