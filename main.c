#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WORDLENGTH 25
#define MAX_PATIENTS 10000

struct patient {
    long long int ssnr; // ssnr : Social security number
    char name[WORDLENGTH];
    char lastName[WORDLENGTH];
    int imRef[10];               
    int numOfIm;
};

typedef struct patient Patient;

int loadPatientsFromFile(Patient patientArray[], int *numOfPatients);
void dataManager(Patient patientArray[], int *numOfPatients);
Patient createPatient(long long int ssnr, char name[], char lastName[], int imRef[], int numOfIm);
int ssnrIsUnique(Patient patientArray[], int *numOfPatients, long long int ssnr);
int registerPatients(Patient patientArray[], int *numOfPatients);
void printPatient(Patient p);
void printPatients(Patient patientArray[], int *numOfPatients);
int searchPatient(Patient patientArray[], int *numOfPatients);
void addImage(Patient patientArray[], int *numOfPatients);
int compareName(Patient p1, Patient p2);
int compareSsnr(Patient p1, Patient p2);
void sortByName(Patient patientArray[], int *numOfPatients);
void sortBySsnr(Patient patientArray[], int *numOfPatients);
void removePatient(Patient patientArray[], int *numOfPatients);
void removeImage(Patient patientArray[], int *numOfPatients);
void saveArrayToFile(Patient patientArray[], int *numOfPatients);
char *ifHighThenLow(char string[]);
int ssnrControll(long long int ssnr);
void theMainMenueTitle(void);
void printTitel(void);
void searchMenueTitle(void);
int sumOfTwoDigits(int remainer);

int main(void){
    Patient patientArray[MAX_PATIENTS];
    int numOfPatients = 0;
    numOfPatients = loadPatientsFromFile(patientArray,&numOfPatients);  
    dataManager(patientArray, &numOfPatients);
    return 0;
}

int loadPatientsFromFile(Patient patientArray[], int *numOfPatients){
    char file[WORDLENGTH];
    FILE *fPointer;
    printf("Enter the name of the file for patients: ");
    scanf("%s",file);
    fPointer = fopen(file,"r");
    if(fPointer != NULL){
        long long int ssnr;
        char name[WORDLENGTH],lastName[WORDLENGTH];
        int imRef[10], numOfIm = 0;
        while(fscanf(fPointer,"%I64u %s %s %d ",&ssnr,name,lastName,&numOfIm) == 4){
            for (int i = 0; i < numOfIm; i++)
                fscanf(fPointer,"%d",&imRef[i]);
            patientArray[*numOfPatients] = createPatient(ssnr,name,lastName,imRef,numOfIm);
            (*numOfPatients)++;
        }
        fclose(fPointer);
    }else{
        printf("The file does not exists!\n");
        printf("Creating the file %s\n", file);
    }
    printf("current number of patients: ");
    printf("%d\n",*numOfPatients);
    return *numOfPatients;
}

void dataManager(Patient patientArray[], int *numOfPatients){
    while (1){
        theMainMenueTitle();
        int category;
        if (scanf("%d", &category) !=1)
            break;
        if (category == 0)
            registerPatients(patientArray, &(*numOfPatients));
        else if(category == 1)
            printPatients(patientArray, &(*numOfPatients));
        else if(category == 2)
            searchPatient(patientArray, &(*numOfPatients));
        else if(category == 3)
            addImage(patientArray, &(*numOfPatients));
        else if(category == 4)
            {
            printf("Enter N to sort by name, S to sort by social security number: ");
            char sortBy;
            scanf(" %c",&sortBy);
            fflush(stdin);
            if (sortBy == 'n' || sortBy == 'N'){
                sortByName(patientArray, &(*numOfPatients));
            }else if(sortBy == 's' || sortBy == 'S'){
                sortBySsnr(patientArray, &(*numOfPatients));
            }else{
                printf("\aWrong input!\n");
            }
        }else if(category == 5)
            removePatient(patientArray, &(*numOfPatients));
        else if(category == 6)
            removeImage(patientArray, &(*numOfPatients));
        else if(category == 7){
            saveArrayToFile(patientArray, &(*numOfPatients));
            break;
        }else
            printf("Unvalid category\n");
    }
    return;
}

Patient createPatient(long long int ssnr, char name[], char lastName[], int imRef[], int numOfIm){
        Patient p;
        p.ssnr = ssnr;
        strcpy(p.name,name);
        strcpy(p.lastName,lastName);
        p.numOfIm = numOfIm;
        for(int i=0 ; i<numOfIm ; i++)
            p.imRef[i] = imRef[i];
    return p;
}

// SSNR = social security number
int ssnrIsUnique(Patient patientArray[], int *numOfPatients, long long int ssnr) {
    for (int i = 0 ; i < *numOfPatients; i++) {
        if(patientArray[i].ssnr == ssnr) {
            return 0;
        }
    }
    return 1;
}

int registerPatients(Patient patientArray[], int *numOfPatients){
    char repeat[WORDLENGTH] = "yes", name[WORDLENGTH], lastName[WORDLENGTH];
    long long int ssnr;
    int numOfIm;
    int imRef[10];
    while(strcmp(repeat,"yes") == 0){
        if (*numOfPatients != MAX_PATIENTS){
            printf("Enter Social security number: ");
            long long int tmp;
            scanf("%I64u", &tmp);
            if(ssnrIsUnique(patientArray, numOfPatients,tmp) == 1 && ssnrControll(tmp) == 1){
                ssnr = tmp;
                fflush(stdin);
                printf("Enter first name: ");
                gets(name);
                ifHighThenLow(name);
                printf("Enter last name: ");
                gets(lastName);
                ifHighThenLow(lastName);
                numOfIm = 0;
                for(int i = 0 ; i < 10; i++) {
                    printf("Enter image refrence number %d (Or s to skip): ", i+1);
                    if(scanf("%d", &imRef[i]) != 1) 
                        break;
                    numOfIm++;
                }
                fflush(stdin);
                patientArray[*numOfPatients] = createPatient(ssnr,name,lastName,imRef,numOfIm);
                (*numOfPatients)++;
            }else if (ssnrIsUnique(patientArray, numOfPatients,tmp) != 1)
                printf("\n\aThe patient already exists!\n");
            else if(ssnrControll(tmp) != 1)
                printf("\n\aThe enterd social security number is false!\n");
                
            printf("\nDo you want to register another patient? (yes/no): ");
            scanf("%s",repeat);
        }else
            printf("\aThe system is full! Remove some patients first.\n");
    }
    return *numOfPatients;
}

void printPatient(Patient p){
    printf("%-23I64u %-15s %-20s [",p.ssnr,p.name, p.lastName);
    for(int i=0; i < p.numOfIm; i++){
        printf("%d",p.imRef[i]);
        if(i < p.numOfIm-1) 
            printf(","); 
    }
    printf("]\n");
    return;
}

void printPatients(Patient patientArray[], int *numOfPatients){
    printTitel();
    for(int i = 0; i < *numOfPatients; i++){
        printPatient(patientArray[i]);
    }
    return;
}

int searchPatient(Patient patientArray[], int *numOfPatients){
    int category, imRef, position = -1, antal = 0; 
    long long int ssnr;
    char tmp[WORDLENGTH]; 
    searchMenueTitle();
    scanf("%d", &category);
    if (category == 0){
        printf("Enter social security number: ");
        scanf("%I64u", &ssnr);
        printTitel();
        for(int i = 0; i < *numOfPatients; i++){
            if(ssnr == patientArray[i].ssnr){
                printPatient(patientArray[i]);
                position = i;
            }
        }
        if (position == -1){
            printf("\aThe entered number was not found\n");
        }
    }else if(category == 1){
        printf("Enter name: ");
        scanf("%s", tmp);
        ifHighThenLow(tmp);
        char *f, *l;
        printTitel();
        for (int i = 0; i < *numOfPatients; i++){
            f = strstr(patientArray[i].name, tmp);
            l = strstr(patientArray[i].lastName, tmp);
            if( f != 0 || l != 0){
                printPatient(patientArray[i]);
                antal++;
                if(antal == 1)
                    position = i;
                if (antal > 1)
                position = 10000;
            }
        }
        if (position == -1){
            printf("\aNo name includes this string.\n");
        }
    }else if(category == 2){
        printf("Enter bildreferens: ");
        scanf("%d", &imRef);
        printTitel();
        for(int i = 0; i < *numOfPatients; i++){
            for(int j = 0; j < patientArray[i].numOfIm; j++){
                if(imRef == patientArray[i].imRef[j]){
                    printPatient(patientArray[i]);
                    antal++;
                    if(antal == 1)
                        position = i;
                    if (antal > 1)
                        position = 10000;
                }
            }
        }
        if (position == -1){
            printf("\aNo image with this refrence was found.\n");
        }
    }else
        printf("\aInvalid category\n");

    return position;
}

void addImage(Patient patientArray[], int *numOfPatients){
    printf("Find the patient that needs more picture");
    int position = -1;
    while (position == -1 || position == 10000){
    position = searchPatient(patientArray, &(*numOfPatients));
    }
        for(int i = patientArray[position].numOfIm ; i < 10; i++) {
            printf("Enter image refrence number %d (Or s to skip): ", i+1);
            if(scanf("%d", &patientArray[position].imRef[i]) != 1)
                break;
            patientArray[position].numOfIm++;
        }
        fflush(stdin);

    return;
}

int compareName(Patient p1, Patient p2){
    return strcmp(p1.name, p2.name);
}

int compareSsnr(Patient p1, Patient p2){
    return p1.ssnr > p2.ssnr;
}

void sortByName(Patient patientArray[], int *numOfPatients){
    
    Patient tmp;
    for(int i=0; i<*numOfPatients-1; i++)
    {
        for(int j=0; j<*numOfPatients-1-i; j++)
        {
            if(compareName(patientArray[j], patientArray[j+1]) > 0)  // strcmp(v[j],v[j+1]))
            {
                    tmp = patientArray[j];
                    patientArray[j] = patientArray[j+1];
                    patientArray[j+1] = tmp; 
            }
        }
    }
    printf("\nPatients are now sorted by name");
    return;
}

void sortBySsnr(Patient patientArray[], int *numOfPatients){
    
    Patient tmp;
    for(int i=0; i<*numOfPatients-1; i++)
    {
        for(int j=0; j<*numOfPatients-1-i; j++)
        {
            if(compareSsnr(patientArray[j], patientArray[j+1]))  // strcmp(v[j],v[j+1]))
            {
                    tmp = patientArray[j];
                    patientArray[j] = patientArray[j+1];
                    patientArray[j+1] = tmp; 
            }
        }
    }
    printf("\nPatients are now sorted by social security number");
    return;
}

void removePatient(Patient patientArray[], int *numOfPatients){
    char again[WORDLENGTH] = "yes";
    int index;
    while(strcmp(again,"yes")==0){
       index = searchPatient(patientArray, numOfPatients);
       if(index != -1){
            for(int i = index; i< *numOfPatients; i++){
                patientArray[i] = patientArray[i+1]; 
            }
            (*numOfPatients)--;
        }
        printf("The patient is now unregisted from the list\n");
        printf("Enter 'yes' to remove another patient:  ");
        scanf("%s",again);
    }
    return;
}

void removeImage(Patient patientArray[], int *numOfPatients){
    int position = -1;
    char again = 'y';
    while (position == -1 || position == 10000){
        position = searchPatient(patientArray, &(*numOfPatients));
    }
    printf("Enter image refrence number to remove: ");
    while (again == 'y'){
        int imref;
        scanf("%d", &imref);
        for (int i = 0; i < patientArray[position].numOfIm; i++){
            if (imref == patientArray[position].imRef[i]){
                for(int j = i; j < patientArray[position].numOfIm; j++){
                    patientArray[position].imRef[j] = patientArray[position].imRef[j+1];
                }
                patientArray[position].numOfIm--;
            }
        }
        printf("Enter 'y' to remove more images\n");
        scanf(" %c",&again);
        fflush(stdin);
    }
    return;
}

void saveArrayToFile(Patient patientArray[], int *numOfPatients) {
    FILE * fPointer;
    fPointer = fopen("patientFile.txt", "w");
    if(fPointer != NULL){
        for (int i = 0; i <= *numOfPatients; i++){
            fprintf(fPointer,"%I64u %s %s %d ", patientArray[i].ssnr, patientArray[i].name, patientArray[i].lastName, patientArray[i].numOfIm);
            for (int j = 0; j < patientArray[i].numOfIm; j++){
                fprintf(fPointer,"%d ", patientArray[i].imRef[j]);
            }
            fprintf(fPointer,"\n");
        }
        fclose(fPointer);
    }
    printf("File saved!\n");
    return;
}



char *ifHighThenLow(char string[]) 
{
    for (int i = 0; i < strlen(string); i++){
        if (string[i] >= 65 && string[i] <= 90) // if a character is higher case
            string[i] += 32;                   // then make it a lower case (ascii) 
    }
    return string;
}

void theMainMenueTitle(void){
        printf("\n __________________________________________\n");
        printf("|             The main menue               |\n");
        printf("|__________________________________________|\n");
        printf("|Enter 0 to     Register new patients      |\n");
        printf("|Enter 1 to     Print out all patients     |\n");
        printf("|Enter 2 to     Search for patients        |\n");
        printf("|Enter 3 to     Add more images            |\n");
        printf("|Enter 4 to     Sort patients              |\n");
        printf("|Enter 5 to     Remove a patient           |\n");
        printf("|Enter 6 to     Remove images              |\n");
        printf("|Enter 7 to     Save and quit the program  |\n");
        printf("|Enter q to     Quit without saving        |\n");
        printf("|__________________________________________|\n");
        printf("Choose a category: ");
}

int sumOfTwoDigits(int remainer)
{
    int sum = 0;
    for (int i=0; i<2; i++)
    {
        sum += remainer%10;
        remainer = remainer/10;
    }
    return sum;
}

int ssnrControll(long long int ssnr)
{  
    int sumOfDigits = 0, controllDigit = 0, remainer = 0; 
    long long int controllNumber = 0;
                
    controllNumber = (ssnr / 10);
    for(int i=0; i<9;i++) 
    {
        remainer = controllNumber % 10;
            if( (i % 2) == 0 )
            {
                remainer *= 2;
                if ( remainer > 9)
                {
                    remainer = sumOfTwoDigits(remainer);
                }
            }
        sumOfDigits += remainer;
        controllNumber = controllNumber/10;
    }
    if ( (sumOfDigits % 10) == 0 )
        controllDigit = 0;
    else
        controllDigit = 10*((sumOfDigits / 10) + 1) - sumOfDigits;
    printf("Social security number: %I64u, controll digit: %d\n", ssnr, controllDigit);
    
    if (controllDigit == ssnr % 10)
        return 1;
    else 
        return 0;
}

void printTitel(void){
    printf("\nSocial security nr\tFirstname\tLastname\t\tImages\n");
    printf("____________________________________________________________________...\n");
}

void searchMenueTitle(void){
    printf("\n\t __________________________________________\n");
    printf("\t|               Search options             |\n");
    printf("\t|__________________________________________|\n");
    printf("\t|Enter 0 to     Search by Personnumer      |\n");
    printf("\t|Enter 1 to     Search by Name             |\n");
    printf("\t|Enter 2 to     Search by bildreferens     |\n");
    printf("\t|__________________________________________|\n");
}