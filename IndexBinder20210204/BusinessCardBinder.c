//BusinessCardBinder.c
#include"BusinessCardBinder.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)


#if 0
int main(int argc, char* argv[]) {
	BusinessCardBinder businessCardBinder;
	BusinessCard* index;
	BusinessCard* (*indexes);
	Long count;
	BusinessCard businessCard;
	Long i = 0;
	BusinessCard* previous = NULL;

	BusinessCardBinder_Create(&businessCardBinder);
	
	strcpy(businessCard.personal.name, "�赵��");
	strcpy(businessCard.personal.rankName, "����");
	strcpy(businessCard.personal.telephoneNumber, "01011112222");
	strcpy(businessCard.personal.emailAddress, "kimdo@welco.com");
	strcpy(businessCard.company.name, "�ٷκ�������");
	strcpy(businessCard.company.address, "����� ���ʱ� ȿ�ɷ�");
	strcpy(businessCard.company.telephoneNumber, "0262507226");
	strcpy(businessCard.company.faxNumber, "05022173100");
	strcpy(businessCard.company.url, "www.welco.or.kr");
	index = Put(&businessCardBinder, businessCard);
	printf("%s %s %s %s %s %s %s %s %s\n", index->personal.name, index->personal.rankName,
		index->personal.telephoneNumber, index->personal.emailAddress, index->company.name,
		index->company.address, index->company.telephoneNumber, index->company.faxNumber, 
		index->company.url);
	
	strcpy(businessCard.personal.name, "��ȿ��");
	strcpy(businessCard.personal.rankName, "�븮");
	strcpy(businessCard.personal.telephoneNumber, "01027234371");
	strcpy(businessCard.personal.emailAddress, "taxmap@empal.com");
	strcpy(businessCard.company.name, "����ȸ��������");
	strcpy(businessCard.company.address, "��⵵ ������ �߿���");
	strcpy(businessCard.company.telephoneNumber, "0317323042");
	strcpy(businessCard.company.faxNumber, "0317323046");
	strcpy(businessCard.company.url, "www.semu.com");
	index = Put(&businessCardBinder, businessCard);
	printf("%s %s %s %s %s %s %s %s %s\n", index->personal.name, index->personal.rankName,
		index->personal.telephoneNumber, index->personal.emailAddress, index->company.name,
		index->company.address, index->company.telephoneNumber, index->company.faxNumber,
		index->company.url);

	strcpy(businessCard.personal.name, "�赵��");
	strcpy(businessCard.personal.rankName, "����");
	strcpy(businessCard.personal.telephoneNumber, "01048760946");
	strcpy(businessCard.personal.emailAddress, "venus@ngtech.co.kr");
	strcpy(businessCard.company.name, "������ũ�����");
	strcpy(businessCard.company.address, "����� ���α� ����2��");
	strcpy(businessCard.company.telephoneNumber, "0220822930");
	strcpy(businessCard.company.faxNumber, "0220822933");
	strcpy(businessCard.company.url, "www.ngtech.co.kr");
	index = Put(&businessCardBinder, businessCard);
	printf("%s %s %s %s %s %s %s %s %s\n", index->personal.name, index->personal.rankName,
		index->personal.telephoneNumber, index->personal.emailAddress, index->company.name,
		index->company.address, index->company.telephoneNumber, index->company.faxNumber,
		index->company.url);

	strcpy(businessCard.personal.name, "����ȫ");
	strcpy(businessCard.personal.rankName, "�̻�");
	strcpy(businessCard.personal.telephoneNumber, "01047232982");
	strcpy(businessCard.personal.emailAddress, "hong@mainsoft.co.kr");
	strcpy(businessCard.company.name, "���μ���Ʈ");
	strcpy(businessCard.company.address, "��⵵ �Ⱦ�� ���ȱ�");
	strcpy(businessCard.company.telephoneNumber, "0313440101");
	strcpy(businessCard.company.faxNumber, "0313440220");
	strcpy(businessCard.company.url, "www.mainsoft.co.kr");
	index = Put(&businessCardBinder, businessCard);
	printf("%s %s %s %s %s %s %s %s %s\n", index->personal.name, index->personal.rankName,
		index->personal.telephoneNumber, index->personal.emailAddress, index->company.name,
		index->company.address, index->company.telephoneNumber, index->company.faxNumber,
		index->company.url);

	Find(&businessCardBinder, "�赵��", &indexes, &count);
	while (i < count) {
		printf("%s %s %s %s %s %s %s %s %s\n", indexes[i]->personal.name, indexes[i]->personal.rankName,
			indexes[i]->personal.telephoneNumber, indexes[i]->personal.emailAddress, indexes[i]->company.name,
			indexes[i]->company.address, indexes[i]->company.telephoneNumber, indexes[i]->company.faxNumber,
			indexes[i]->company.url);
		i++;
	}

	index = FindCompanyName(&businessCardBinder, "���μ���Ʈ");
	printf("%s %s %s %s %s\n", index->company.name, index->company.address, index->company.telephoneNumber,
      index->company.faxNumber, index->company.url);

	index = BusinessCardBinder_First(&businessCardBinder);
	index = BusinessCardBinder_Next(&businessCardBinder);
	businessCard = PutOut(&businessCardBinder, index);
	printf("%s %s %s %s %s %s %s %s %s\n", businessCard.personal.name, businessCard.personal.rankName,
		businessCard.personal.telephoneNumber, businessCard.personal.emailAddress, businessCard.company.name,
		businessCard.company.address, businessCard.company.telephoneNumber, businessCard.company.faxNumber,
		businessCard.company.url);

	index = BusinessCardBinder_First(&businessCardBinder);
	while (index != previous) {
		printf("%s %s %s %s %s %s %s %s %s\n", index->personal.name, index->personal.rankName,
			index->personal.telephoneNumber, index->personal.emailAddress, index->company.name,
			index->company.address, index->company.telephoneNumber, index->company.faxNumber,
			index->company.url);
		previous = index;
		index = BusinessCardBinder_Next(&businessCardBinder);
	}


	BusinessCardBinder_Destroy(&businessCardBinder);

	return 0;
}
#endif

void BusinessCardBinder_Create(BusinessCardBinder* businessCardBinder) {
	LinkedList_Create(&businessCardBinder->businessCards);
	businessCardBinder->length = 0;
	businessCardBinder->current = NULL;
}

Long Load(BusinessCardBinder* businessCardBinder) {
	BusinessCard businessCard;
	Personal personal;
	Company company;
	Node* node;
	Long index;
	Long indexCompany;
	FILE* filePersonals;
	FILE* fileCompanies;
	size_t flagPersonal;
	size_t flagCompany;

	filePersonals = fopen("Personals.dat", "rb");
	fileCompanies = fopen("Companies.dat", "rb");
	if (filePersonals != NULL && fileCompanies != NULL) {
		//1.1 ��ġ, ���� ����, ���� ����, ���� ��ȭ��ȣ, ���� �̸��� �ּҸ� �д´�.
		flagPersonal = fread(&index, sizeof(Long), 1, filePersonals);
		flagPersonal = fread(&personal, sizeof(Personal), 1, filePersonals);
		//1. ������ ���� �ƴѵ��� �ݺ��Ѵ�.
		while (!feof(filePersonals) && flagPersonal != 0) {
			indexCompany = 1;
			//1.2.1 ȸ���̸�, �ּ�, ��ȭ��ȣ, �ѽ���ȣ, url�� �д´�.
			fseek(fileCompanies, 0L, SEEK_SET);
			flagCompany = fread(&company, sizeof(Company), 1, fileCompanies);
			//1.2 ����(Companies)�� ���� �ƴϰ�, index�� indexCompany���� Ŭ���� �ݺ��Ѵ�.
			while (!feof(fileCompanies) && flagCompany != 0 && index > indexCompany) {
				indexCompany++;
				flagCompany = fread(&company, sizeof(Company), 1, fileCompanies);
			}
			//1.2.2 ��ġ�� ȸ���̸� ���Կ� ���´�.
			businessCard.company = company;
			businessCard.personal = personal;
			//1.3 ���Կ� �ȴ´�.
			node = LinkedList_AppendFromTail(&businessCardBinder->businessCards, &businessCard, sizeof(BusinessCard));
			//1.4 ������ ����.
			businessCardBinder->length++;
			businessCardBinder->current = (BusinessCard*)(node + 1);
			//1.1 ��ġ, ���� ����, ���� ����, ���� ��ȭ��ȣ, ���� �̸��� �ּҸ� �д´�.
			flagPersonal = fread(&index, sizeof(Long), 1, filePersonals);
			flagPersonal = fread(&personal, sizeof(Personal), 1, filePersonals);
		}
	}
	//2. ������ ����Ѵ�.
	return businessCardBinder->length;
	//3. ������.
}

BusinessCard* BusinessCardBinder_Put(BusinessCardBinder* businessCardBinder, BusinessCard businessCard) {
	Node* node;

	node = LinkedList_AppendFromTail(&businessCardBinder->businessCards, &businessCard, sizeof(BusinessCard));
	businessCardBinder->length++;
	businessCardBinder->current = (BusinessCard*)(node + 1);

	return businessCardBinder->current;
}

void BusinessCardBinder_Find(BusinessCardBinder* businessCardBinder, char(*name), BusinessCard** (*indexes), Long* count) {
	Node* (*indexes_);
	Long i = 0;
	LinkedList_LinearSearchDuplicate(&businessCardBinder->businessCards, name, &indexes_, count, 
		BusinessCardBinder_CompareNames);
	*indexes = (BusinessCard * (*))calloc(businessCardBinder->length, sizeof(BusinessCard*));
	while (i < *count) {
		(*indexes)[i] = (BusinessCard*)(indexes_[i] + 1);
		i++;
	}
	if (indexes_ != NULL) {
		free(indexes_);
		indexes_ = NULL;
	}
}

BusinessCard* FindCompanyName(BusinessCardBinder* businessCardBinder, char(*companyName)) {
	Node* node;
	BusinessCard* index;

	node = LinkedList_LinearSearchUnique(&businessCardBinder->businessCards, companyName, 
		BusinessCardBinder_CompareCompanyNames);
	if (node != NULL) {
		index = (BusinessCard*)(node + 1);
	}
	else {
		index = NULL;
	}
	return index;
}

BusinessCard BusinessCardBinder_PutOut(BusinessCardBinder* businessCardBinder, BusinessCard* index) {
	Node* node;
	BusinessCard businessCard;

	node = LinkedList_LinearSearchUnique(&businessCardBinder->businessCards, index, BusinessCardBinder_CompareBusinessCards);
	LinkedList_GetAt(&businessCardBinder->businessCards, node, &businessCard, sizeof(BusinessCard));
	LinkedList_Delete(&businessCardBinder->businessCards, node);
	businessCardBinder->length--;
	if (businessCardBinder->businessCards.current != NULL) {
		businessCardBinder->current = (BusinessCard*)(businessCardBinder->businessCards.current + 1);
	}
	else {
		businessCardBinder->current = NULL;
	}

	return businessCard;
}

BusinessCard* Move(BusinessCardBinder* businessCardBinder, BusinessCard* index) {
	Node* node;

	node = LinkedList_LinearSearchUnique(&businessCardBinder->businessCards, index, BusinessCardBinder_CompareBusinessCards);
	businessCardBinder->businessCards.current = node;
	businessCardBinder->current = index;

	return businessCardBinder->current;
}

BusinessCard* BusinessCardBinder_First(BusinessCardBinder* businessCardBinder) {
	Node* node;

	node = LinkedList_First(&businessCardBinder->businessCards);
    businessCardBinder->current = (BusinessCard*)(node + 1);

	return businessCardBinder->current;
}

BusinessCard* BusinessCardBinder_Previous(BusinessCardBinder* businessCardBinder) {
	Node* node;

	node = LinkedList_Previous(&businessCardBinder->businessCards);
	businessCardBinder->current = (BusinessCard*)(node + 1);

	return businessCardBinder->current;
}

BusinessCard* BusinessCardBinder_Next(BusinessCardBinder* businessCardBinder) {
	Node* node;

	node = LinkedList_Next(&businessCardBinder->businessCards);
	businessCardBinder->current = (BusinessCard*)(node + 1);

	return businessCardBinder->current;
}

BusinessCard* BusinessCardBinder_Last(BusinessCardBinder* businessCardBinder) {
	Node* node;

	node = LinkedList_Last(&businessCardBinder->businessCards);
	businessCardBinder->current = (BusinessCard*)(node + 1);

	return businessCardBinder->current;
}

Long Save(BusinessCardBinder* businessCardBinder) {
	BusinessCard businessCard;
	Company company;
	Node* node;
	Node* previous = NULL;
	Long index;
	FILE* fileCompanies;
	FILE* filePersonals;
	size_t flagCompany;

	filePersonals = fopen("Personals.dat", "wb");
	fileCompanies = fopen("Companies.dat", "wb+");
	if (filePersonals != NULL && fileCompanies != NULL) { //������ ���������� ��������
		node = LinkedList_First(&businessCardBinder->businessCards);
		//1.1 ������ ���� �ƴѵ��� �ݺ��Ѵ�.
		while (node != previous) {
			//1.1.1 ȸ���̸�, �ּ�, ��ȭ��ȣ, �ѽ���ȣ, url�� �д´�.
			index = 1;
			LinkedList_GetAt(&businessCardBinder->businessCards, node, &businessCard, sizeof(BusinessCard));
			fseek(fileCompanies, 0L, SEEK_SET);
			flagCompany = fread(&company, sizeof(Company), 1, fileCompanies);
			//1.1.2 ������ ���� �ƴϰ�, ���� ȸ�簡 �ƴѵ��� �ݺ��Ѵ�.
			while (!feof(fileCompanies) && flagCompany != 0 
				&& strcmp(businessCard.company.name, company.name) != 0) {
				index++;
				flagCompany = fread(&company, sizeof(Company), 1, fileCompanies);
			}
			//1.1.3 ���Ͽ� ���� ȸ���̸�,
			if (flagCompany == 0) {
				//1.1.3.1 ȸ���̸�, �ּ�, ��ȭ��ȣ, fax��ȣ, url�� ���´�.
				fwrite(&businessCard.company, sizeof(Company), 1, fileCompanies);
			}
			//1.1.4 ��ġ, ���� ����, ����, ��ȭ��ȣ, �̸��� �ּҸ� Personals���Ͽ� ���´�.
			fwrite(&index, sizeof(Long), 1, filePersonals);
			fwrite(&businessCard.personal, sizeof(Personal), 1, filePersonals);
			previous = node;
			node = LinkedList_Next(&businessCardBinder->businessCards);
		}
		fclose(filePersonals);
		fclose(fileCompanies);
	}
	//2. ������ ����Ѵ�.
	return businessCardBinder->length;
	//3. ������.
}
void BusinessCardBinder_Destroy(BusinessCardBinder* businessCardBinder) {
	LinkedList_Destroy(&businessCardBinder->businessCards);
}

int BusinessCardBinder_CompareNames(void* one, void* other) {
	BusinessCard* one_ = (BusinessCard*)one;
	char(*other_) = (char(*))other;
	int ret;

	ret = strcmp(one_->personal.name, other_);
	return ret;
}

int BusinessCardBinder_CompareCompanyNames(void* one, void* other) {
	BusinessCard* one_ = (BusinessCard*)one;
	char(*other_) = (char(*))other;
	int ret;

	ret = strcmp(one_->company.name, other_);
	return ret;
}

int BusinessCardBinder_CompareBusinessCards(void* one, void* other) {
	BusinessCard* one_ = (BusinessCard*)one;
	BusinessCard* other_ = (BusinessCard*)other;
	int ret;

	if (one_ == other_) {
		ret = 0;
	}
	else {
		ret = -1;
	}
	return ret;

}




